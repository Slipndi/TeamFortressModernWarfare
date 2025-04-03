#include "TFModerneWarfare/Characters/Components/Weapons/TFC_WeaponSlotManagerComponent.h"
#include "TFModerneWarfare/Weapons/Base/AWeaponBase.h"
#include "TFModerneWarfare/Weapons/Data/TFC_WeaponStatsDataAsset.h"
#include "TFModerneWarfare/Weapons/Data/TFC_WeaponDataAsset.h"
#include "GameFramework/Actor.h"
#include "GameFramework/Character.h"
#include "UObject/UnrealType.h"
#include "Misc/AssertionMacros.h"

UTFC_WeaponSlotManagerComponent::UTFC_WeaponSlotManagerComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	SetIsReplicatedByDefault(false); // Ce component est local pour le moment (pas d'état répliqué)
}

void UTFC_WeaponSlotManagerComponent::LoadWeaponSet(UTFC_WeaponDataAsset* WeaponData)
{
	if (!WeaponData) return;

	const bool bHasAuthority = GetOwner() && GetOwner()->HasAuthority();
	UE_LOG(LogTemp, Warning, TEXT("🎯 LoadWeaponSet() appelé dans BeginPlay [HasAuthority=%s]"),
		bHasAuthority ? TEXT("true") : TEXT("false"));

	// Reset des maps
	WeaponSlots.Empty();
	WeaponInstances.Empty();

	// Pour chaque arme équipée dans le DataAsset
	for (const FEquippedWeapon& Equipped : WeaponData->EquippedWeapons)
	{
		if (!Equipped.WeaponStats)
		{
			UE_LOG(LogTemp, Error, TEXT("❌ Slot %s : WeaponStats est NULL"), *UEnum::GetValueAsString(Equipped.Slot));
			continue;
		}

		WeaponSlots.Add(Equipped.Slot, Equipped.WeaponStats);
		SpawnAndAttachWeapon(Equipped.Slot, Equipped.WeaponStats);
	}

	// Debug slots disponibles
	UE_LOG(LogTemp, Warning, TEXT("🎯 WeaponInstances disponibles :"));
	for (const auto& Elem : WeaponInstances)
	{
		UE_LOG(LogTemp, Warning, TEXT("   🔸 Slot: %s → %s"),
			*UEnum::GetValueAsString(Elem.Key),
			*Elem.Value->GetName());
	}

	// Set du slot courant
	CurrentSlot = EWeaponSlot::Slot_Primary;
	UE_LOG(LogTemp, Warning, TEXT("🎯 CurrentSlot défini à %s"), *UEnum::GetValueAsString(CurrentSlot));
}

void UTFC_WeaponSlotManagerComponent::FireCurrentWeapon()
{
	UE_LOG(LogTemp, Warning, TEXT("📍 CurrentSlot = %s | WeaponInstances contient: %s"),
		*UEnum::GetValueAsString(CurrentSlot),
		WeaponInstances.Contains(CurrentSlot) ? TEXT("✅ OUI") : TEXT("❌ NON"));
	
	if (!WeaponInstances.Contains(CurrentSlot))
	{
		UE_LOG(LogTemp, Error, TEXT("❌ Aucun acteur d’arme dans WeaponInstances pour le slot %d"), (int32)CurrentSlot);
		return;
	}

	AWeaponBase* Weapon = WeaponInstances[CurrentSlot];

	if (!IsValid(Weapon))
	{
		UE_LOG(LogTemp, Error, TEXT("❌ L'arme pour le slot %d est invalide."), (int32)CurrentSlot);
		return;
	}
	UE_LOG(LogTemp, Warning, TEXT("📌 Weapon class: %s"), *Weapon->GetClass()->GetName());
	// ✅ Le feu est lancé ici
	Weapon->Fire();
}

UTFC_WeaponStatsDataAsset* UTFC_WeaponSlotManagerComponent::GetWeaponStatsForSlot(EWeaponSlot Slot) const
{
	if (WeaponSlots.Contains(Slot))
	{
		return WeaponSlots[Slot];
	}
	return nullptr;
}

TArray<EWeaponSlot> UTFC_WeaponSlotManagerComponent::GetAvailableSlots() const
{
	TArray<EWeaponSlot> Keys;
	WeaponSlots.GetKeys(Keys);
	return Keys;
}

void UTFC_WeaponSlotManagerComponent::Server_FireCurrentWeapon_Implementation()
{
	FireCurrentWeapon(); 
	Multicast_PlayFireEffects();
}

void UTFC_WeaponSlotManagerComponent::Multicast_PlayFireEffects_Implementation()
{
	// 🔊 TODO: Ajouter les VFX/SFX de tir ici
	// Exemple :
	// if (AWeaponBase* Weapon = WeaponInstances.FindRef(CurrentSlot))
	// {
	//     Weapon->PlayFireEffects(); // si tu fais une fonction dédiée
	// }

	UE_LOG(LogTemp, Warning, TEXT("✨ [Multicast] Effets de tir lancés sur tous les clients"));
}

void UTFC_WeaponSlotManagerComponent::SpawnAndAttachWeapon(EWeaponSlot Slot, UTFC_WeaponStatsDataAsset* Stats)
{
	if (!Stats || !GetOwner() || !GetWorld()) return;

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = GetOwner();
	SpawnParams.Instigator = Cast<APawn>(GetOwner());
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	AWeaponBase* Weapon = GetWorld()->SpawnActor<AWeaponBase>(
		Stats->WeaponActorClass,
		FVector::ZeroVector,
		FRotator::ZeroRotator,
		SpawnParams);
	
	if (!Weapon) return;

	Weapon->AttachToComponent(Cast<ACharacter>(GetOwner())->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("WeaponSocket")); // Assumes socket exists
	Weapon->InitializeWeapon(Stats);

	WeaponInstances.Add(Slot, Weapon);

	UE_LOG(LogTemp, Warning, TEXT("✅ [SpawnWeapon] Arme '%s' spawnée et attachée pour le slot %d"), *Weapon->GetName(), (int32)Slot);
}
