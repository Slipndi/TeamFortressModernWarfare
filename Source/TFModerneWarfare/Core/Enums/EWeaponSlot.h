#pragma once

#include "EWeaponSlot.generated.h"

UENUM(BlueprintType)
enum class EWeaponSlot : uint8
{
	Slot_None      UMETA(DisplayName = "None"),
	Slot_Primary UMETA(DisplayName = "Primary"),
	Slot_Secondary UMETA(DisplayName = "Secondary"),
	Slot_Melee UMETA(DisplayName = "Melee"),
	Slot_GrenadeA UMETA(DisplayName = "Grenade A"),
	Slot_GrenadeB UMETA(DisplayName = "Grenade B")
};