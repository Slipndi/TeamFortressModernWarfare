#include "TFC_PlayerBase.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "TFModerneWarfare/Core/Inputs/TFC_InputManagerComponent.h"
#include "TFModerneWarfare/Characters/Components/Movement/TFC_MovementComponent.h"
#include "TFModerneWarfare/Characters/Components/Armor/TFC_ArmorComponent.h"
#include "Engine/DataTable.h"
#include "TFModerneWarfare/Characters/Components/Health/TFC_HealthComponent.h"
#include "TFModerneWarfare/Characters/Components/Respawn/TFC_RespawnComponent.h"
#include "TFModerneWarfare/Characters/Components/Weapons/TFC_WeaponSlotManagerComponent.h"
#include "TFModerneWarfare/Core/Game/TFC_GameStatics.h"

DEFINE_LOG_CATEGORY_STATIC(TFCPlayerBase, Log, Warning);
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
	HealthComponent = CreateDefaultSubobject<UTFC_HealthComponent>(TEXT("HealthComponent"));
	RespawnComponent = CreateDefaultSubobject<UTFC_RespawnComponent>(TEXT("RespawnComponent"));
	WeaponSlotManager = CreateDefaultSubobject<UTFC_WeaponSlotManagerComponent>(TEXT("WeaponSlotManager"));
}

void ATFC_PlayerBase::BeginPlay()
{
	Super::BeginPlay();

	if (ArmorComponent)
	{
		ArmorComponent->InitializeArmorFromClass(PlayerClassType);
	}
	if (HealthComponent)
	{
		UE_LOG(LogTemp, Log, TEXT("✅ HealthComponent initialisé pour %s, PV = %.1f"), *GetName(), HealthComponent->GetCurrentHealth());
	}

	// 📌 Chargement des armes pour le serveur ET le client
	const FPlayerClassData* ClassData = UTFC_GameStatics::GetClassData(PlayerClassType);
	if (ClassData && ClassData->WeaponSet && WeaponSlotManager)
	{
		WeaponSlotManager->LoadWeaponSet(ClassData->WeaponSet);

		UE_LOG(LogTemp, Warning, TEXT("🎯 LoadWeaponSet() appelé dans BeginPlay [HasAuthority=%s]"),
			HasAuthority() ? TEXT("true") : TEXT("false"));

		for (EWeaponSlot Slot : WeaponSlotManager->GetAvailableSlots())
		{
			if (UTFC_WeaponStatsDataAsset* Stats = WeaponSlotManager->GetWeaponStatsForSlot(Slot))
			{
				UE_LOG(LogTemp, Warning, TEXT("🧪 [WeaponTest] Slot: %s → %s"),
					*UEnum::GetValueAsString(Slot),
					*Stats->GetWeaponName().ToString());
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("❌ [WeaponTest] Aucun data asset trouvé pour le slot %s"),
					*UEnum::GetValueAsString(Slot));
			}
		}
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

bool ATFC_PlayerBase::CanSprint() const
{
	if (!HealthComponent || HealthComponent->IsDead()) return false;
	return true;
}

void ATFC_PlayerBase::HandleLocalDeath()
{
	// 🔇 Désactiver input
	if (AController* PC = GetController())
	{
		DisableInput(Cast<APlayerController>(PC));
	}

	// 🚫 Désactiver collision capsule
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// 🙈 Cacher le mesh (en attendant un ragdoll futur)
	if (GetMesh())
	{
		GetMesh()->SetVisibility(false, true);
	}

	UE_LOG(LogTemp, Warning, TEXT("☠️ [PlayerBase] %s: local death handled (input/collision hidden)"), *GetName());
}

