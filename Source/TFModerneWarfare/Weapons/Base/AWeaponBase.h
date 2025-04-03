#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TFModerneWarfare/Weapons/Data/TFC_WeaponStatsDataAsset.h"
#include "AWeaponBase.generated.h"

class AController;

UCLASS(Abstract)
class TFMODERNEWARFARE_API AWeaponBase : public AActor
{
	GENERATED_BODY()

public:
	AWeaponBase();

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintNativeEvent, Category = "Weapon")
	void Fire();
	virtual void Fire_Implementation();
	
	/** Rechargement (à spécialiser) */
	virtual void Reload();

	/** Affecte les stats depuis un DataAsset */
	virtual void InitializeWeapon(UTFC_WeaponStatsDataAsset* InStats);

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	USkeletalMeshComponent* WeaponMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
	UTFC_WeaponStatsDataAsset* WeaponStats;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
	AActor* OwningCharacter;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
	AController* OwningController;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
	int32 CurrentAmmo;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
	int32 AmmoInReserve;

	// Utilitaire
	FVector GetMuzzleLocation() const;
	FRotator GetFireDirection() const;
	void PlayFireEffects();
};
