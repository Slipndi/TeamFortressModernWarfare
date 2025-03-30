#include "TFC_InputManagerComponent.h"
#include "TFC_InputConfig.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/PlayerController.h"
#include "InputMappingContext.h"
#include "InputAction.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "../Structs/FPlayerClassData.h"
#include "TFModerneWarfare/Characters/Player/TFC_PlayerBase.h"
#include "TFModerneWarfare/Characters/Components/Movement/TFC_MovementComponent.h"

UTFC_InputManagerComponent::UTFC_InputManagerComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UTFC_InputManagerComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerPawn = Cast<APawn>(GetOwner());
	if (!OwnerPawn) return;
	CachedMovementComponent = OwnerPawn->FindComponentByClass<UTFC_MovementComponent>();

	OwnerController = Cast<APlayerController>(OwnerPawn->GetController());
	if (!OwnerController || !InputConfig) return;

	SetupEnhancedInput();
}

void UTFC_InputManagerComponent::SetupEnhancedInput()
{
	if (ULocalPlayer* LocalPlayer = OwnerController->GetLocalPlayer())
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer))
		{
			Subsystem->ClearAllMappings();
			Subsystem->AddMappingContext(InputConfig->PlayerContext, 0);
		}
	}

	if (UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(OwnerController->InputComponent))
	{
		EIC->BindAction(InputConfig->IA_Move, ETriggerEvent::Triggered, this, &UTFC_InputManagerComponent::Move);
		EIC->BindAction(InputConfig->IA_Look, ETriggerEvent::Triggered, this, &UTFC_InputManagerComponent::Look);
		EIC->BindAction(InputConfig->IA_Jump, ETriggerEvent::Started, this, &UTFC_InputManagerComponent::StartJump);
        EIC->BindAction(InputConfig->IA_Jump, ETriggerEvent::Completed, this, &UTFC_InputManagerComponent::StopJump);
		EIC->BindAction(InputConfig->IA_Sprint, ETriggerEvent::Started, this, &UTFC_InputManagerComponent::StartSprint);
		EIC->BindAction(InputConfig->IA_Sprint, ETriggerEvent::Completed, this, &UTFC_InputManagerComponent::StopSprint);
		EIC->BindAction(InputConfig->IA_CrouchSlide, ETriggerEvent::Started, this, &UTFC_InputManagerComponent::HandleCrouchOrSlide);
		EIC->BindAction(InputConfig->IA_CrouchSlide, ETriggerEvent::Completed, this, &UTFC_InputManagerComponent::HandleUnCrouch);

	}
}

void UTFC_InputManagerComponent::Move(const FInputActionValue& Value)
{
	if (!OwnerPawn) return;

	const FVector2D Input = Value.Get<FVector2D>();
	const FRotator YawRotation(0.f, OwnerController->GetControlRotation().Yaw, 0.f);
	const FVector Forward = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector Right = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	OwnerPawn->AddMovementInput(Forward, Input.Y);
	OwnerPawn->AddMovementInput(Right, Input.X);
}

void UTFC_InputManagerComponent::Look(const FInputActionValue& Value)
{
	if (!OwnerController) return;

	const FVector2D LookAxis = Value.Get<FVector2D>();
	OwnerController->AddYawInput(LookAxis.X);
	OwnerController->AddPitchInput(LookAxis.Y);
}

void UTFC_InputManagerComponent::StartJump(const FInputActionValue& Value)
{
	ACharacter* Character = Cast<ACharacter>(OwnerPawn);
	if (Character)
	{
		Character->Jump();
	}
}

void UTFC_InputManagerComponent::StopJump(const FInputActionValue& Value)
{
	ACharacter* Character = Cast<ACharacter>(OwnerPawn);
	if (Character)
	{
		Character->StopJumping();
	}
}

void UTFC_InputManagerComponent::StartSprint(const FInputActionValue& Value)
{
	if (CachedMovementComponent)
	{
		CachedMovementComponent->StartSprint();
	}
}

void UTFC_InputManagerComponent::StopSprint(const FInputActionValue& Value)
{
	if (CachedMovementComponent)
	{
		CachedMovementComponent->StopSprint();
	}
}

void UTFC_InputManagerComponent::HandleCrouchOrSlide(const FInputActionValue& /*Value*/)
{
	UE_LOG(LogTemp, Warning, TEXT("⏺ [Input] CrouchOrSlide Triggered"));
	if (CachedMovementComponent)
	{
		CachedMovementComponent->HandleCrouchOrSlide();
	}
}


void UTFC_InputManagerComponent::HandleUnCrouch(const FInputActionValue& /*Value*/)
{
	if (CachedMovementComponent)
	{
		CachedMovementComponent->HandleUnCrouch();
	}
}

