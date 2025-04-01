#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "TFModerneWarfare/Core/Enums/EArmorType.h"
#include "TFModerneWarfare/Core/Enums/EPlayerClass.h"
#include "FPlayerClassData.generated.h" 

USTRUCT(BlueprintType)
struct FPlayerClassData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EPlayerClass ClassType = EPlayerClass::Soldier;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float WalkSpeedMultiplier = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SprintSpeedMultiplier = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float JumpZVelocity = 550.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AirControl = 0.6f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float BrakingDeceleration = 1500.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float GroundFriction = 6.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Acceleration = 2048.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxWalkSpeedCrouched = 250.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SlideImpulse = 600.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SlideDuration = 0.75f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EArmorType ArmorType = EArmorType::None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxHealth = 100.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxArmor = 100.0f;
};
