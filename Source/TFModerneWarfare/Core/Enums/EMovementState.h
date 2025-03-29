#pragma once

#include "EMovementState.generated.h"

UENUM(BlueprintType)
enum class EMovementState : uint8
{
	Standing	UMETA(DisplayName = "Standing"),
	Sprinting	UMETA(DisplayName = "Sprinting"),
	Crouching	UMETA(DisplayName = "Crouching"),
	Sliding		UMETA(DisplayName = "Sliding")
};
