#include "TFModerneWarfare/Characters/Components/Weapons/TFC_WeaponSlotManagerComponent.h"
#include "TFModerneWarfare/Weapons/Data/TFC_WeaponDataAsset.h"

UTFC_WeaponSlotManagerComponent::UTFC_WeaponSlotManagerComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	SetIsReplicatedByDefault(false); // Ce component est local pour le moment (pas d'état répliqué)
}

void UTFC_WeaponSlotManagerComponent::LoadWeaponSet(UTFC_WeaponDataAsset* WeaponData)
{
	if (!WeaponData) return;
	WeaponSlots.Empty();

	for (const FEquippedWeapon& Entry : WeaponData->EquippedWeapons)
	{
		if (Entry.Slot != EWeaponSlot::Slot_None && Entry.WeaponStats)
		{
			WeaponSlots.Add(Entry.Slot, Entry.WeaponStats);

			UE_LOG(LogTemp, Warning, TEXT("🔫 Slot %s → %s"),
				*UEnum::GetValueAsString(Entry.Slot),
				*Entry.WeaponStats->GetWeaponName().ToString());
		}
	}
}

UTFC_WeaponStatsDataAsset* UTFC_WeaponSlotManagerComponent::GetWeaponStatsForSlot(EWeaponSlot Slot) const
{
	const TObjectPtr<UTFC_WeaponStatsDataAsset>* Found = WeaponSlots.Find(Slot);
	return Found ? *Found : nullptr;
}
TArray<EWeaponSlot> UTFC_WeaponSlotManagerComponent::GetAvailableSlots() const
{
	TArray<EWeaponSlot> OutKeys;
	WeaponSlots.GetKeys(OutKeys);
	return OutKeys;
}
