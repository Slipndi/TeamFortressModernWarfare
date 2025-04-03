#pragma once

#include "CoreMinimal.h"
#include "TFModerneWarfare/Weapons/Base/AWeaponBase.h"
#include "AWeapon_Hitscan.generated.h"

UCLASS()
class TFMODERNEWARFARE_API AWeapon_Hitscan : public AWeaponBase
{
	GENERATED_BODY()

public:
	virtual void Fire_Implementation() override;
	void ProcessHit(const FHitResult& Hit);
};