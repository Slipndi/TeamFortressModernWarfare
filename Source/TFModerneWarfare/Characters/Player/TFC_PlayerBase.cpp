#include "TFC_PlayerBase.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "TFModerneWarfare/Core/Inputs/TFC_InputManagerComponent.h"
#include "TFModerneWarfare/Characters/Components/Movement/TFC_MovementComponent.h"
#include "TFModerneWarfare/Characters/Components/Armor/TFC_ArmorComponent.h"
#include "Engine/DataTable.h"

ATFC_PlayerBase::ATFC_PlayerBase()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;
	bAlwaysRelevant = true;

	ACharacter::SetReplicateMovement(true);

	if (GetCharacterMovement())
	{
		GetCharacterMovement()->SetIsReplicated(true);
	}

	UE_LOG(LogTemp, Warning, TEXT("[PlayerBase] Constructeur exécuté."));

	// 🎥 Caméra
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(GetCapsuleComponent());
	CameraComponent->bUsePawnControlRotation = true;
	CameraComponent->SetRelativeLocation(FVector(0.f, 0.f, 64.f));

	// 🧠 Composants custom
	MovementComponent = CreateDefaultSubobject<UTFC_MovementComponent>(TEXT("MovementComponent"));
	ArmorComponent = CreateDefaultSubobject<UTFC_ArmorComponent>(TEXT("ArmorComponent"));
	InputComponentManager = CreateDefaultSubobject<UTFC_InputManagerComponent>(TEXT("InputManagerComponent"));
}

void ATFC_PlayerBase::BeginPlay()
{
	Super::BeginPlay();
	
	if (ArmorComponent)
	{
		ArmorComponent->InitializeArmorFromClass(PlayerClassType);
	}
}

void ATFC_PlayerBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

const FPlayerClassData* ATFC_PlayerBase::GetClassData() const
{
	if (!ClassDataTable) return nullptr;

	const FString Context = TEXT("GetClassData Context");
	const FName RowName = FName(*UEnum::GetDisplayValueAsText(PlayerClassType).ToString());

	return ClassDataTable->FindRow<FPlayerClassData>(RowName, Context);
}
