#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "TFModerneWarfare/Core/Structs/FEquippedWeapon.h"
#include "TFC_WeaponDataAsset.generated.h"

UCLASS(BlueprintType)
class TFMODERNEWARFARE_API UTFC_WeaponDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FEquippedWeapon> EquippedWeapons;
};
