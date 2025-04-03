#pragma once

#include "CoreMinimal.h"
#include "TFModerneWarfare/Core/Enums/EDamageType.h"
#include "TFModerneWarfare/Core/Enums/EWeaponSlot.h"
#include "TFModerneWarfare/Core/Enums/EWeaponType.h"
#include "FWeaponStats.generated.h"

USTRUCT(BlueprintType)
struct FWeaponStats
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName WeaponName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	EWeaponType WeaponType = EWeaponType::None;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 MaxAmmo = 100;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 ClipSize = 30;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float ReloadTime = 1.5f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool bIsHitscan = true;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float FireInterval = 0.7f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float SpreadAngle = 0.0f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 ProjectilesPerShot = 1;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float BaseDamage = 1.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	EDamageType DamageType = EDamageType::Shells;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<AActor> WeaponActorClass;
};
