#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TFModerneWarfare/Core/Enums/EWeaponSlot.h"
#include "TFModerneWarfare/Core/Structs/FWeaponStats.h"
#include "TFModerneWarfare/Weapons/Data/TFC_WeaponDataAsset.h"
#include "TFC_WeaponSlotManagerComponent.generated.h"

class AWeaponBase;
class UTFC_WeaponStatsDataAsset;

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
	void SpawnAndAttachWeapon(EWeaponSlot Slot, UTFC_WeaponStatsDataAsset* Stats);

	/** Instances runtime des armes équipées */
	UPROPERTY()
	TMap<EWeaponSlot, TObjectPtr<class AWeaponBase>> WeaponInstances;

	UPROPERTY()
	TMap<EWeaponSlot, UTFC_WeaponStatsDataAsset*> WeaponSlots;

	/** Tire avec l’arme du slot actif */
	UFUNCTION(BlueprintCallable)
	void FireCurrentWeapon();

	/** Slot actuellement actif */
	UPROPERTY()
	EWeaponSlot CurrentSlot;
	
	UFUNCTION(Server, Reliable)
	void Server_FireCurrentWeapon();

	UFUNCTION(NetMulticast, Unreliable)
	void Multicast_PlayFireEffects();

private:
	/** Slots équipés (arme + stats) */
	UPROPERTY()
	TMap<EWeaponSlot, FEquippedWeapon> EquippedWeapons;

	/** Retourne un pointeur vers l’arme équipée */
	FEquippedWeapon* GetEquippedWeapon(EWeaponSlot Slot);
};
