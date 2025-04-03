#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "TFModerneWarfare/Core/Structs/FWeaponStats.h"
#include "TFC_WeaponStatsDataAsset.generated.h"

class AWeaponBase;

UCLASS(BlueprintType)
class TFMODERNEWARFARE_API UTFC_WeaponStatsDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	FWeaponStats Stats;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<AWeaponBase> WeaponActorClass;


	/** Getters utiles pour simplifier l’accès */
	FName GetWeaponName() const { return Stats.WeaponName; }
	EWeaponType GetWeaponType() const { return Stats.WeaponType; }
	int32 GetMaxAmmo() const { return Stats.MaxAmmo; }
	int32 GetClipSize() const { return Stats.ClipSize; }
	float GetReloadTime() const { return Stats.ReloadTime; }
	bool IsHitscan() const { return Stats.bIsHitscan; }
	TSubclassOf<AWeaponBase> GetWeaponActorClass() const { return WeaponActorClass; }
	float GetFireInterval() const { return Stats.FireInterval; }
	float GetSpreadAngle() const { return Stats.SpreadAngle; }
	int32 GetProjectilesPerShot() const { return Stats.ProjectilesPerShot; }
	float GetBaseDamage() const { return Stats.BaseDamage; }
};
