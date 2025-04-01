#include "TFC_MovementComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "TFModerneWarfare/Characters/Player/TFC_PlayerBase.h"
#include "TimerManager.h"
#include "Net/UnrealNetwork.h"
#include "TFModerneWarfare/Core/Inputs/TFC_InputManagerComponent.h"

UTFC_MovementComponent::UTFC_MovementComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.TickGroup = TG_PostPhysics;
	PrimaryComponentTick.bStartWithTickEnabled = false;
	SetIsReplicatedByDefault(true);
	PrimaryComponentTick.TickInterval = 1.f / 140.f;
}

void UTFC_MovementComponent::BeginPlay()
{
	Super::BeginPlay();
	CachePlayer();
	CachedInputManager = Player->FindComponentByClass<UTFC_InputManagerComponent>();


	if (!Player) return;

	InitializeMovementFromClass();

	ClientSimPosition = Player->GetActorLocation();
	ClientSimVelocity = FVector::ZeroVector;

	if (!GetOwner()->HasAuthority())
	{
		UE_LOG(LogTemp, Warning, TEXT("🎮 [MovementComp] Activation du Tick client pour simulation locale"));
		SetComponentTickEnabled(true);
	}
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

			// ✅ Appliquer le SlideImpulse en local aussi (client autonome)
			if (Player->GetLocalRole() == ROLE_AutonomousProxy)
			{
				ClientSimVelocity += Player->GetActorForwardVector() * ClassData->SlideImpulse;
			}
			else
			{
				MoveComp->Velocity += Player->GetActorForwardVector() * ClassData->SlideImpulse;
			}

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

void UTFC_MovementComponent::OnRep_ServerValidatedFrame()
{
	UE_LOG(LogTemp, Warning, TEXT("📥 [RepFrame] Reçu frame #%d | Pos=%s | Vel=%s"),
		ReplicatedServerFrame.FrameIndex,
		*ReplicatedServerFrame.Position.ToCompactString(),
		*ReplicatedServerFrame.Velocity.ToCompactString());

	const FMovementFrame* MatchingFrame = MovementBuffer.FindByPredicate(
		[this](const FMovementFrame& Frame)
		{
			return Frame.FrameIndex == ReplicatedServerFrame.FrameIndex;
		});

	if (!MatchingFrame)
	{
		UE_LOG(LogTemp, Error, TEXT("[ROLLBACK] ❌ Frame #%d non trouvée dans le buffer client"), ReplicatedServerFrame.FrameIndex);
		return;
	}

	const float Error = FVector::Dist(MatchingFrame->Position, ReplicatedServerFrame.Position);
	if (Error < KINDA_SMALL_NUMBER)
	{
		UE_LOG(LogTemp, Warning, TEXT("[ROLLBACK] ✅ Sync OK avec frame #%d (Erreur = %.2f)"), ReplicatedServerFrame.FrameIndex, Error);
		return;
	}

	// Correction XY toujours
	ClientSimPosition.X = ReplicatedServerFrame.Position.X;
	ClientSimPosition.Y = ReplicatedServerFrame.Position.Y;

	// Correction Z uniquement si trop d'écart
	const float ZError = FMath::Abs(ReplicatedServerFrame.Position.Z - ClientSimPosition.Z);
	if (ZError > MaxZCorrectionThreshold)
	{
		UE_LOG(LogTemp, Warning, TEXT("[ROLLBACK] ⚠️ Correction Z forcée (%.1fcm)"), ZError);
		ClientSimPosition.Z = ReplicatedServerFrame.Position.Z;
	}

	// Appliquer la nouvelle vitesse validée
	ClientSimVelocity = ReplicatedServerFrame.Velocity;

	const int32 StartIndex = MovementBuffer.IndexOfByPredicate(
		[this](const FMovementFrame& Frame) { return Frame.FrameIndex == ReplicatedServerFrame.FrameIndex; });

	for (int32 i = StartIndex + 1; i < MovementBuffer.Num(); ++i)
	{
		const FMovementFrame& Frame = MovementBuffer[i];
		const float Speed = (Frame.MovementState == EMovementState::Sprinting) ? FinalSprintSpeed : FinalWalkSpeed;
		const FVector Direction = Frame.InputVector.GetSafeNormal();
		ClientSimVelocity = Direction * Speed;
		ClientSimPosition += ClientSimVelocity * (1.f / 60.f);
	}

	UE_LOG(LogTemp, Warning, TEXT("[ROLLBACK] 🔁 Resimulation terminée. Position finale : %s"), *ClientSimPosition.ToCompactString());
}

void UTFC_MovementComponent::OnRep_ServerState()
{
	UE_LOG(LogTemp, Warning, TEXT("📦 [OnRep_ServerState] Position=%s | Vitesse=%s | State=%s"),
		*ServerPosition.ToCompactString(),
		*ServerVelocity.ToCompactString(),
		*UEnum::GetValueAsString(ServerMovementState));

	if (!Player || GetOwnerRole() != ROLE_SimulatedProxy) return;

	const float PositionError = FVector::Dist(Player->GetActorLocation(), ServerPosition);

	if (PositionError > BigGapThreshold)
	{
		Player->SetActorLocation(ServerPosition);
	}
	else
	{
		FVector InterpPos = FMath::VInterpTo(Player->GetActorLocation(), ServerPosition, GetWorld()->GetDeltaSeconds(), InterpSpeed);
		Player->SetActorLocation(InterpPos);
	}
}

void UTFC_MovementComponent::OnRep_ReplicatedSpeed()
{
	UE_LOG(LogTemp, Warning, TEXT("📶 [OnRep_ReplicatedSpeed] Speed=%.2f"), ReplicatedCurrentSpeed);
}

void UTFC_MovementComponent::OnRep_ReplicatedState()
{
	UE_LOG(LogTemp, Warning, TEXT("📦 [OnRep_ReplicatedState] State=%s"), *UEnum::GetValueAsString(ReplicatedMovementState));

	if (!Player || GetOwnerRole() != ROLE_SimulatedProxy) return;

	MovementState = ReplicatedMovementState;
	const float Speed = (MovementState == EMovementState::Sprinting) ? FinalSprintSpeed : FinalWalkSpeed;
	ReplicatedCurrentSpeed = Speed;
	ClientSimVelocity = Player->GetActorForwardVector() * Speed;
}

void UTFC_MovementComponent::ServerUpdateMovementState_Implementation(EMovementState NewState)
{
	if (!Player) return;

	// 🔐 Vérification simple côté serveur
	if (NewState == EMovementState::Sprinting && !Player->CanSprint())
	{
		UE_LOG(LogTemp, Warning, TEXT("[SECURITY] Sprint refusé pour %s"), *Player->GetName());
		return;
	}

	// 🔁 Tu peux étendre ici avec d'autres règles (ex : empêcher sliding sans sprint)
	MovementState = NewState;
}

void UTFC_MovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!Player) return;

	if (GetOwner()->HasAuthority())
	{
		TickServerMovement();
		return;
	}

	// ✳️ Simulation autonome ou interpolation (selon le rôle)
	if (GetOwnerRole() == ROLE_AutonomousProxy)
	{
		const FVector InputVector = Player->GetLastMovementInputVector();

		if (MovementState == EMovementState::Sliding)
		{
			// ✅ En slide, on conserve la vélocité actuelle (déjà boostée par HandleCrouchOrSlide)
			ClientSimPosition += ClientSimVelocity * DeltaTime;
		}
		else
		{
			const float Speed = (MovementState == EMovementState::Sprinting) ? FinalSprintSpeed : FinalWalkSpeed;
			ClientSimVelocity = InputVector.GetSafeNormal() * Speed;
			ClientSimPosition += ClientSimVelocity * DeltaTime;
		}

		FMovementFrame NewFrame;
		NewFrame.FrameIndex = ClientFrameIndex++;
		NewFrame.TimeStamp = GetWorld()->GetTimeSeconds();
		NewFrame.Position = ClientSimPosition;
		NewFrame.Velocity = ClientSimVelocity;
		NewFrame.InputVector = InputVector;
		NewFrame.MovementState = MovementState;

		if (MovementBuffer.Num() >= MaxBufferSize)
		{
			MovementBuffer.RemoveAt(0);
		}
		MovementBuffer.Add(NewFrame);

		const bool bIsAirborne = Player->GetCharacterMovement()->IsFalling();
		const bool bClientThinksAirborne = (MovementState != EMovementState::Standing && MovementState != EMovementState::Crouching);

		if (!bIsAirborne && !bClientThinksAirborne)
		{
			Player->SetActorLocation(ClientSimPosition);
		}
		else
		{
			// Correction partielle uniquement en XY, laisse le Z local
			FVector CurrentLocation = Player->GetActorLocation();
			FVector NewLocation = FVector(ClientSimPosition.X, ClientSimPosition.Y, CurrentLocation.Z);
			Player->SetActorLocation(NewLocation);
		}


		//UE_LOG(LogTemp, Warning, TEXT("🟢 [CLIENT] Frame #%d | Pos=%s | Vel=%s | Input=%s | State=%s"),
		//	NewFrame.FrameIndex,
		//	*NewFrame.Position.ToCompactString(),
		//	*NewFrame.Velocity.ToCompactString(),
		//	*NewFrame.InputVector.ToCompactString(),
		//	*UEnum::GetValueAsString(NewFrame.MovementState));

		// ✅ Vérifie si on vient d'atterrir
		const bool bIsCurrentlyInAir = Player->GetCharacterMovement()->IsFalling();

		if (bWasInAir && !bIsCurrentlyInAir)
		{
			if (CachedInputManager && CachedInputManager->IsSprintHeld())
			{
				StartSprint();
				UE_LOG(LogTemp, Warning, TEXT("⏩ [Movement] Reprise automatique du sprint à l'atterrissage"));
			}
		}

		bWasInAir = bIsCurrentlyInAir;
	}
	else if (GetOwnerRole() == ROLE_SimulatedProxy)
	{
		FVector InterpVel = FMath::VInterpTo(Player->GetVelocity(), ClientSimVelocity, DeltaTime, InterpSpeed);
		Player->GetCharacterMovement()->Velocity = InterpVel;
	}
}

void UTFC_MovementComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(UTFC_MovementComponent, ReplicatedServerFrame, COND_SkipOwner);
	DOREPLIFETIME(UTFC_MovementComponent, ServerPosition);
	DOREPLIFETIME(UTFC_MovementComponent, ServerVelocity);
	DOREPLIFETIME(UTFC_MovementComponent, ServerMovementState);
	DOREPLIFETIME(UTFC_MovementComponent, ReplicatedCurrentSpeed);
	DOREPLIFETIME(UTFC_MovementComponent, ReplicatedMovementState);
}

void UTFC_MovementComponent::TickServerMovement()
{
	if (!Player) return;

	FMovementFrame NewFrame;
	NewFrame.FrameIndex = ServerFrameIndex++;
	NewFrame.TimeStamp = GetWorld()->GetTimeSeconds();
	NewFrame.Position = Player->GetActorLocation();
	NewFrame.Velocity = Player->GetVelocity();
	NewFrame.InputVector = Player->GetLastMovementInputVector();
	NewFrame.MovementState = MovementState;

	ReplicatedServerFrame = NewFrame;

	ServerPosition = NewFrame.Position;
	ServerVelocity = NewFrame.Velocity;
	ServerMovementState = NewFrame.MovementState;
}
