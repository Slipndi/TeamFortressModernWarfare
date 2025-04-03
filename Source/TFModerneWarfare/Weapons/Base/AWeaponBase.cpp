#include "AWeaponBase.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "TFModerneWarfare/Weapons/Data/TFC_WeaponStatsDataAsset.h"

AWeaponBase::AWeaponBase()
{
	PrimaryActorTick.bCanEverTick = false;

	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	SetRootComponent(WeaponMesh);
	WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	CurrentAmmo = 0;
	AmmoInReserve = 0;
}

void AWeaponBase::BeginPlay()
{
	Super::BeginPlay();
	OwningCharacter = GetOwner();
	OwningController = Cast<AController>(OwningCharacter ? Cast<APawn>(OwningCharacter)->GetController() : nullptr);
}

void AWeaponBase::InitializeWeapon(UTFC_WeaponStatsDataAsset* InStats)
{
	if (InStats)
	{
		WeaponStats = InStats;
		CurrentAmmo = WeaponStats->Stats.ClipSize;
		AmmoInReserve = WeaponStats->Stats.MaxAmmo;
	}
}

void AWeaponBase::Fire_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("🔫 AWeaponBase::Fire_Implementation() [Default Base Call]"));
}

void AWeaponBase::Reload()
{
	// Reload basique, à spécialiser
	const int32 AmmoToReload = WeaponStats ? WeaponStats->Stats.ClipSize - CurrentAmmo : 0;
	const int32 AmmoAvailable = FMath::Min(AmmoToReload, AmmoInReserve);

	CurrentAmmo += AmmoAvailable;
	AmmoInReserve -= AmmoAvailable;

	UE_LOG(LogTemp, Warning, TEXT("[WeaponBase] Reloaded: %d bullets. Remaining: %d"), CurrentAmmo, AmmoInReserve);
}

FVector AWeaponBase::GetMuzzleLocation() const
{
	return WeaponMesh->GetSocketLocation(TEXT("Muzzle")); // Assumes a "Muzzle" socket
}

FRotator AWeaponBase::GetFireDirection() const
{
	return WeaponMesh->GetSocketRotation(TEXT("Muzzle"));
}

void AWeaponBase::PlayFireEffects()
{
	// TODO: Particules de bouche, flash, bruit, secousses caméra, etc.
}