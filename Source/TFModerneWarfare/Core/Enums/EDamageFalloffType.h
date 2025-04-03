#pragma once

UENUM(BlueprintType)
enum class EDamageFalloffType : uint8
{
	None        UMETA(DisplayName = "None"),
	Linear      UMETA(DisplayName = "Linear"),
	Exponential UMETA(DisplayName = "Exponential"),
};
