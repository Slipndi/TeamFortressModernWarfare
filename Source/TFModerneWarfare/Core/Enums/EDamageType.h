#pragma once

UENUM(BlueprintType)
enum class EDamageType : uint8
{
	Explosive UMETA(DisplayName = "Explosive"),
	Shells UMETA(DisplayName = "Shells"),
	Nails UMETA(DisplayName = "Nails"),
	Melee UMETA(DisplayName = "Melee"),
	_MAX UMETA(Hidden)	
};
