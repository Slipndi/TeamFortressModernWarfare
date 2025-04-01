#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TFC_DamageTester.generated.h"

UCLASS()
class TFMODERNEWARFARE_API ATFC_DamageTester : public AActor
{
	GENERATED_BODY()

public:
	ATFC_DamageTester();

public:
	virtual void Tick(float DeltaTime) override;

	/** Inflige des dégâts au premier joueur trouvé */
	UFUNCTION(BlueprintCallable)
	void TestApplyDamage();

	UPROPERTY(EditAnywhere, Category = "Test Damage")
	float DamageAmount = 25.0f;

	UPROPERTY(EditAnywhere, Category = "Test Damage")
	FName BoneToHit = "head";

	UPROPERTY(EditAnywhere, Category = "Test Damage")
	TSubclassOf<UDamageType> DamageType;
};
