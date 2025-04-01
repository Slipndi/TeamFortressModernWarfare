#pragma once

#include "EArmorType.generated.h"

UENUM(BlueprintType)
enum class EArmorType : uint8
{
	Light 	UMETA(DisplayName = "Light"),
	Medium 	UMETA(DisplayName = "Medium"),
	Heavy 	UMETA(DisplayName = "Heavy"),
	None	UMETA(DisplayName = "None"),
	_MAX UMETA(Hidden)
};