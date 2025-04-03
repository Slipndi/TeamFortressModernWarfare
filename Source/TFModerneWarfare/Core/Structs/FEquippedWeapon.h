#pragma once

#include "CoreMinimal.h"
#include "TFModerneWarfare/Core/Enums/EWeaponSlot.h"
#include "TFModerneWarfare/Weapons/Data/TFC_WeaponStatsDataAsset.h"
#include "FEquippedWeapon.generated.h"

USTRUCT(BlueprintType)
struct FEquippedWeapon
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	EWeaponSlot Slot = EWeaponSlot::Slot_None;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UTFC_WeaponStatsDataAsset> WeaponStats = nullptr;
};
