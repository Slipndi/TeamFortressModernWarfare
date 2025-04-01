#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "FDamageInfo.generated.h"

USTRUCT(BlueprintType)
struct FDamageInfo
{
	GENERATED_BODY()

	// Dégâts bruts (avant réduction par l'armure)
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float RawDamage = 0.0f;

	// Acteur ayant infligé les dégâts
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	AActor* Instigator = nullptr;

	// Position du coup (utile pour explosion/headshot)
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FVector HitLocation = FVector::ZeroVector;

	// Os touché si disponible (ex: headshot)
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FName BoneHit = NAME_None;

	// Type de dégât (utile pour headshot, explosion, DoT)
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSubclassOf<UDamageType> DamageType = nullptr;
};
