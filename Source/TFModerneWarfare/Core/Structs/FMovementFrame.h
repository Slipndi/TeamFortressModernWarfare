#pragma once

#include "CoreMinimal.h"
#include "TFModerneWarfare/Core/Enums/EMovementState.h"
#include "FMovementFrame.generated.h"

USTRUCT(BlueprintType)
struct FMovementFrame
{
	GENERATED_BODY()

	UPROPERTY()
	float TimeStamp = 0.f;

	UPROPERTY()
	FVector InputVector = FVector::ZeroVector;

	UPROPERTY()
	FVector Position = FVector::ZeroVector;

	UPROPERTY()
	FVector Velocity = FVector::ZeroVector;

	UPROPERTY()
	EMovementState MovementState = EMovementState::Standing;

	UPROPERTY()
	int32 FrameIndex = -1; // 🆕 Pour identifiant de rollback
};
