#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TFModerneWarfare/Core/Interfaces/IDamageable.h"
#include "TFModerneWarfare/Core/Structs/FDamageInfo.h"
#include "TFC_HealthComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHealthChangedSignature, float, NewHealth);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDeathSignature);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class TFMODERNEWARFARE_API UTFC_HealthComponent : public UActorComponent, public IDamageable
{
	GENERATED_BODY()

public:
	UTFC_HealthComponent();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void BeginPlay() override;

	/** Interface Damageable */
	virtual void ApplyDamage(const FDamageInfo& DamageInfo) override;

	/** Initialise les PV en fonction de la classe */
	void InitializeHealthFromClass();

	/** Réinitialise la santé à la valeur max */
	void ResetHealth();

	/** Retourne le pourcentage de santé restante */
	UFUNCTION(BlueprintCallable)
	float GetHealthRatio() const;

	/** Retourne les PV actuels */
	float GetCurrentHealth() const { return Health; }

	/** Retourne les PV max */
	float GetMaxHealth() const { return MaxHealth; }

	/** Applique une quantité de soin */
	UFUNCTION(BlueprintCallable, Category = "Health")
	void ApplyHealing(float HealAmount);

	UFUNCTION(BlueprintCallable)
	bool IsDead() const { return bIsDead; }
	
	UFUNCTION(BlueprintCallable)
	void Kill();
	
protected:
	/** Santé actuelle */
	UPROPERTY(ReplicatedUsing = OnRep_Health)
	float Health = 0.0f;

	/** Santé maximale (set au début depuis la DataTable de classe) */
	float MaxHealth = 100.0f;

	/** Delegate appelé quand la santé change */
	UPROPERTY(BlueprintAssignable)
	FOnHealthChangedSignature OnHealthChanged;

	/** Delegate appelé quand la mort est déclenchée */
	UPROPERTY(BlueprintAssignable)
	FOnDeathSignature OnDeath;

	/** Hook de réplication */
	UFUNCTION()
	void OnRep_Health();

	/** Etat de mort, répliqué */
	UPROPERTY(ReplicatedUsing = OnRep_Death)
	bool bIsDead = false;

	UFUNCTION()
	void OnRep_Death();

	void HandleDeath(const FDamageInfo& DamageInfo);
};
