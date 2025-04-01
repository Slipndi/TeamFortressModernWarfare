#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IDamageable.generated.h"

// Forward declaration
struct FDamageInfo;

UINTERFACE(MinimalAPI, BlueprintType)
class UDamageable : public UInterface
{
	GENERATED_BODY()
};

class TFMODERNEWARFARE_API IDamageable
{
	GENERATED_BODY()

public:
	// Fonction à implémenter dans les composants/acteurs recevant des dégâts
	virtual void ApplyDamage(const FDamageInfo& DamageInfo) = 0;
	virtual void ApplyHealing(float Amount) {}
};
