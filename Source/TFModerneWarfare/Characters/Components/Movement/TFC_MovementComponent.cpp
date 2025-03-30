#include "TFC_MovementComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "TFModerneWarfare/Characters/Player/TFC_PlayerBase.h"
#include "TimerManager.h"

UTFC_MovementComponent::UTFC_MovementComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.TickGroup = TG_PostPhysics;
	PrimaryComponentTick.bStartWithTickEnabled = false;
	SetIsReplicatedByDefault(true);
	PrimaryComponentTick.TickInterval = 0.f;
}

void UTFC_MovementComponent::BeginPlay()
{
	Super::BeginPlay();
	CachePlayer();

	if (!Player) return;

	InitializeMovementFromClass();

	ClientSimPosition = Player->GetActorLocation();
	ClientSimVelocity = FVector::ZeroVector;
}

void UTFC_MovementComponent::CachePlayer()
{
	Player = Cast<ATFC_PlayerBase>(GetOwner());
}

void UTFC_MovementComponent::InitializeMovementFromClass()
{
	if (!Player) return;

	if (const FPlayerClassData* ClassData = Player->GetClassData())
	{
		FinalWalkSpeed = WalkSpeed * ClassData->WalkSpeedMultiplier;
		FinalSprintSpeed = SprintSpeed * ClassData->SprintSpeedMultiplier;

		UCharacterMovementComponent* MoveComp = Player->GetCharacterMovement();
		MoveComp->MaxWalkSpeed = FinalWalkSpeed;
		MoveComp->MaxWalkSpeedCrouched = ClassData->MaxWalkSpeedCrouched;
		MoveComp->JumpZVelocity = ClassData->JumpZVelocity;
		MoveComp->AirControl = ClassData->AirControl;
		MoveComp->BrakingDecelerationWalking = ClassData->BrakingDeceleration;
		MoveComp->GroundFriction = ClassData->GroundFriction;
		MoveComp->MaxAcceleration = ClassData->Acceleration;

		UE_LOG(LogTemp, Warning, TEXT("✅ [MovementComp] Classe %s chargée. Walk: %.0f / Sprint: %.0f"),
			*UEnum::GetDisplayValueAsText(Player->PlayerClassType).ToString(),
			FinalWalkSpeed,
			FinalSprintSpeed
		);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("❌ [MovementComp] Classe %s introuvable dans la DataTable !"),
			*UEnum::GetDisplayValueAsText(Player->PlayerClassType).ToString());
	}
}

void UTFC_MovementComponent::StartSprint()
{
	if (!Player) return;
	MovementState = EMovementState::Sprinting;
	Player->GetCharacterMovement()->MaxWalkSpeed = FinalSprintSpeed;
}

void UTFC_MovementComponent::StopSprint()
{
	if (!Player) return;
	MovementState = EMovementState::Standing;
	Player->GetCharacterMovement()->MaxWalkSpeed = FinalWalkSpeed;
}

void UTFC_MovementComponent::HandleCrouchOrSlide()
{
	if (!Player) return;

	switch (MovementState)
	{
	case EMovementState::Sprinting:
		MovementState = EMovementState::Sliding;

		if (const FPlayerClassData* ClassData = Player->GetClassData())
		{
			UCharacterMovementComponent* MoveComp = Player->GetCharacterMovement();
			MoveComp->BrakingFrictionFactor = 0.f;
			MoveComp->Velocity += Player->GetActorForwardVector() * ClassData->SlideImpulse;
			Player->GetWorldTimerManager().SetTimer(SlideTimerHandle, this, &UTFC_MovementComponent::EndSlide, ClassData->SlideDuration, false);
		}
		break;

	case EMovementState::Standing:
		MovementState = EMovementState::Crouching;
		Player->Crouch();
		break;

	default:
		break;
	}
}

void UTFC_MovementComponent::HandleUnCrouch()
{
	if (!Player) return;

	if (MovementState == EMovementState::Crouching || MovementState == EMovementState::Sliding)
	{
		Player->UnCrouch();
		MovementState = EMovementState::Standing;
		Player->GetCharacterMovement()->BrakingFrictionFactor = 2.f;
	}
}

void UTFC_MovementComponent::EndSlide()
{
	if (!Player) return;

	Player->UnCrouch();
	MovementState = EMovementState::Standing;
	Player->GetCharacterMovement()->BrakingFrictionFactor = 2.f;
}

void UTFC_MovementComponent::OnRep_ServerState()
{
	MovementState = ServerMovementState;

	const float DeltaTime = GetWorld()->GetDeltaSeconds();

	ClientSimPosition = FMath::VInterpTo(
		ClientSimPosition,
		ServerPosition,
		DeltaTime,
		InterpSpeed);
}

void UTFC_MovementComponent::OnRep_ReplicatedSpeed()
{
	UE_LOG(LogTemp, Warning, TEXT("📶 Client vitesse répliquée : %.1f"), ReplicatedCurrentSpeed);
}

void UTFC_MovementComponent::OnRep_ReplicatedState()
{
	if (!Player) return;

	const FVector InputVector = Player->GetLastMovementInputVector();

	switch (ReplicatedMovementState)
	{
	case EMovementState::Sprinting:
		ClientSimVelocity = InputVector.GetSafeNormal() * FinalSprintSpeed;
		break;
	case EMovementState::Sliding:
		if (const FPlayerClassData* ClassData = Player->GetClassData())
		{
			ClientSimVelocity = InputVector.GetSafeNormal() * (FinalSprintSpeed + ClassData->SlideImpulse);
		}
		break;
	default:
		ClientSimVelocity = InputVector.GetSafeNormal() * FinalWalkSpeed;
		break;
	}
}

void UTFC_MovementComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(UTFC_MovementComponent, ServerPosition, COND_SkipOwner);
	DOREPLIFETIME_CONDITION(UTFC_MovementComponent, ServerVelocity, COND_SkipOwner);
	DOREPLIFETIME_CONDITION(UTFC_MovementComponent, ServerMovementState, COND_SkipOwner);
	DOREPLIFETIME_CONDITION(UTFC_MovementComponent, ReplicatedCurrentSpeed, COND_SkipOwner);
	DOREPLIFETIME_CONDITION(UTFC_MovementComponent, ReplicatedMovementState, COND_SkipOwner);
}

void UTFC_MovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!Player || (Player->HasAuthority() && GetOwner()->GetNetMode() != NM_ListenServer))
		return;

	if (GetOwner()->HasAuthority())
	{
		TickServerMovement();
	}
	else
	{
		TickClientMovement(DeltaTime);
	}
}

void UTFC_MovementComponent::TickServerMovement()
{
	if (!Player) return;

	ServerPosition = Player->GetActorLocation();
	ReplicatedMovementState = MovementState;

	if (UCharacterMovementComponent* CharMove = Player->GetCharacterMovement())
	{
		ClientSimVelocity = CharMove->Velocity;
	}
	else
	{
		ServerVelocity = FVector::ZeroVector;
	}

	ServerMovementState = MovementState;
}

void UTFC_MovementComponent::TickClientMovement(float DeltaTime)
{
	if (!Player) return;

	const FVector InputVector = Player->GetLastMovementInputVector();
	const FVector Forward = Player->GetActorForwardVector();

	const FVector TargetVelocity = InputVector.GetSafeNormal() * ReplicatedCurrentSpeed * VisualSpeedMultiplier;
	ClientSimVelocity = FMath::VInterpTo(ClientSimVelocity, TargetVelocity, DeltaTime, 6.f);

	if (ReplicatedMovementState == EMovementState::Sprinting)
	{
		ClientSimVelocity += Forward * 75.f;
	}
	else if (ReplicatedMovementState == EMovementState::Sliding)
	{
		ClientSimVelocity += Forward * 150.f;
	}

	ClientSimPosition += ClientSimVelocity * DeltaTime;

	const float DistToServerPos = FVector::Dist(ClientSimPosition, ServerPosition);

	if (DistToServerPos > BigGapThreshold)
	{
		ClientSimPosition = FMath::Lerp(ClientSimPosition, ServerPosition, 0.5f);
	}
	else if (DistToServerPos > SmallGapThreshold)
	{
		ClientSimPosition = FMath::VInterpTo(ClientSimPosition, ServerPosition, DeltaTime, InterpSpeed);
	}

	UE_LOG(LogTemp, Warning, TEXT("🚀 Client Tick | InputVector: %s | Velocity: %s | Pos: %s"),
		*InputVector.ToString(),
		*ClientSimVelocity.ToString(),
		*ClientSimPosition.ToString());

	Player->SetActorLocation(ClientSimPosition, false, nullptr, ETeleportType::TeleportPhysics);
}
