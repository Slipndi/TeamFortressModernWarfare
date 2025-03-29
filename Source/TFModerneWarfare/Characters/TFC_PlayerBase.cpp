#include "TFC_PlayerBase.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "TFModerneWarfare/Core/Structs/FPlayerClassData.h"
#include "Engine/DataTable.h"
#include "TFModerneWarfare/Core/TFC_InputManagerComponent.h"
#include "TFModerneWarfare/Core/Structs/FPlayerClassData.h"


ATFC_PlayerBase::ATFC_PlayerBase()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	UE_LOG(LogTemp, Warning, TEXT("[PlayerBase] Constructeur exécuté."));

	// Création de la caméra
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(GetCapsuleComponent());
	CameraComponent->bUsePawnControlRotation = true;
	CameraComponent->SetRelativeLocation(FVector(0.f, 0.f, 64.f));

	InputComponentManager = CreateDefaultSubobject<UTFC_InputManagerComponent>(TEXT("InputManagerComponent"));
	GetCharacterMovement()->MaxWalkSpeed = FinalWalkSpeed;
	GetCharacterMovement()->MaxWalkSpeedCrouched = 250.f;
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.6f;
	GetCharacterMovement()->BrakingDecelerationWalking = 1500.f;
	GetCharacterMovement()->GroundFriction = 6.f;
	GetCharacterMovement()->MaxAcceleration = 2048.f;
}

void ATFC_PlayerBase::BeginPlay()
{
	Super::BeginPlay();

	if (ClassDataTable)
	{
		static const FString Context(TEXT("Class Data Context"));
		FPlayerClassData* ClassData = ClassDataTable->FindRow<FPlayerClassData>(
				FName(*UEnum::GetDisplayValueAsText(PlayerClassType).ToString()),
				Context
			);

		if (ClassData)
		{
			FinalWalkSpeed = WalkSpeed * ClassData->WalkSpeedMultiplier;
			FinalSprintSpeed = SprintSpeed * ClassData->SprintSpeedMultiplier;
			GetCharacterMovement()->MaxWalkSpeed = FinalWalkSpeed;
			UE_LOG(LogTemp, Warning, TEXT("[PlayerBase] Classe %s chargée. Walk: %.0f / Sprint: %.0f"),
				*UEnum::GetDisplayValueAsText(PlayerClassType).ToString(),
				FinalWalkSpeed,
				FinalSprintSpeed
			);
			GetCharacterMovement()->MaxWalkSpeed = FinalWalkSpeed;
			GetCharacterMovement()->MaxWalkSpeedCrouched = ClassData->MaxWalkSpeedCrouched;
			GetCharacterMovement()->JumpZVelocity = ClassData->JumpZVelocity;
			GetCharacterMovement()->AirControl = ClassData->AirControl;
			GetCharacterMovement()->BrakingDecelerationWalking = ClassData->BrakingDeceleration;
			GetCharacterMovement()->GroundFriction = ClassData->GroundFriction;
			GetCharacterMovement()->MaxAcceleration = ClassData->Acceleration;
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("[PlayerBase] Classe %s introuvable dans la DataTable !"),
				*UEnum::GetDisplayValueAsText(PlayerClassType).ToString()
			);
		}
	}
}


void ATFC_PlayerBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ATFC_PlayerBase::Move(const FInputActionValue& Value)
{
	if (!Controller || Value.GetMagnitude() == 0.f) return;

	const FVector2D MovementVector = Value.Get<FVector2D>();

	// Déplacement selon la rotation de la caméra
	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	const FVector Forward = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector Right = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	AddMovementInput(Forward, MovementVector.Y);
	AddMovementInput(Right, MovementVector.X);
}

void ATFC_PlayerBase::Look(const FInputActionValue& Value)
{
	if (!Controller) return;

	const FVector2D LookAxisVector = Value.Get<FVector2D>();
	AddControllerYawInput(LookAxisVector.X);
	AddControllerPitchInput(LookAxisVector.Y);
}

void ATFC_PlayerBase::EndSlide()
{
	if (MovementState == EMovementState::Sliding)
	{
		UnCrouch();
		MovementState = EMovementState::Standing;
		GetCharacterMovement()->BrakingFrictionFactor = 2.f;

		UE_LOG(LogTemp, Warning, TEXT("⏹️ [Slide] terminé automatiquement"));
	}
}

const FPlayerClassData* ATFC_PlayerBase::GetClassData() const
{
	if (!ClassDataTable) return nullptr;

	const FString Context = TEXT("GetClassData Context");
	const FName RowName = FName(*UEnum::GetDisplayValueAsText(PlayerClassType).ToString());

	return ClassDataTable->FindRow<FPlayerClassData>(RowName, Context);
}