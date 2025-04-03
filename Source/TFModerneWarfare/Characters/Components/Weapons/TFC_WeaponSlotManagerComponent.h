#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TFModerneWarfare/Core/Enums/EWeaponSlot.h"
#include "TFModerneWarfare/Core/Structs/FWeaponStats.h"
#include "TFModerneWarfare/Weapons/Data/TFC_WeaponDataAsset.h"
#include "TFC_WeaponSlotManagerComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class TFMODERNEWARFARE_API UTFC_WeaponSlotManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UTFC_WeaponSlotManagerComponent();

	/** Charge dynamiquement les armes depuis un DataAsset (défini par la classe du joueur) */
	void LoadWeaponSet(UTFC_WeaponDataAsset* WeaponData);
	
	/** Retourne les stats d’une arme pour un slot donné */
	UFUNCTION(BlueprintCallable)
	UTFC_WeaponStatsDataAsset* GetWeaponStatsForSlot(EWeaponSlot Slot) const;

	/** Retourne tous les slots disponibles (hors null) */
	TArray<EWeaponSlot> GetAvailableSlots() const;

private:
	UPROPERTY()
	TMap<EWeaponSlot, TObjectPtr<UTFC_WeaponStatsDataAsset>> WeaponSlots;
};
