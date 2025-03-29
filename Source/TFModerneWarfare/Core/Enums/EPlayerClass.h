#pragma once

#include "EPlayerClass.generated.h"

UENUM(BlueprintType)
enum class EPlayerClass : uint8
{
	Soldier UMETA(DisplayName = "Soldier"),
	Medic UMETA(DisplayName = "Medic"),
	Scout UMETA(DisplayName = "Scout")
};
