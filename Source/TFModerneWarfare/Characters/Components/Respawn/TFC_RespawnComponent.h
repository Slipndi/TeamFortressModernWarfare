#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TFC_RespawnComponent.generated.h"

// Delegate notifiant que le joueur peut être respawn (côté serveur uniquement)
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnRespawnReadySignature, AController*, Controller);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class TFMODERNEWARFARE_API UTFC_RespawnComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UTFC_RespawnComponent();

	/** Démarre le compte à rebours pour respawn (doit être appelé côté serveur) */
	UFUNCTION(BlueprintCallable, Category = "Respawn")
	void StartRespawnCountdown(float Delay);

	/** Annule le timer (ex: si le joueur quitte avant respawn) */
	UFUNCTION(BlueprintCallable, Category = "Respawn")
	void CancelRespawn();

	/** Forcer la fin du timer, utile en debug / cheat / test */
	UFUNCTION(BlueprintCallable, Category = "Respawn")
	void ForceRespawn();

	/** Est-ce qu’un respawn est en attente */
	UFUNCTION(BlueprintPure, Category = "Respawn")
	bool IsPendingRespawn() const { return bIsPendingRespawn; }

	UPROPERTY(BlueprintAssignable)
	FOnRespawnReadySignature OnRespawnReady;

	UFUNCTION(BlueprintPure, Category = "Respawn")
	float GetRemainingTime() const;
	

protected:
	virtual void BeginPlay() override;

private:
	/** Timer interne */
	FTimerHandle RespawnTimerHandle;

	/** Durée du délai actuel */
	UPROPERTY(VisibleAnywhere)
	float RespawnDelay = 0.f;

	/** État du composant */
	UPROPERTY(VisibleAnywhere)
	bool bIsPendingRespawn = false;

	/** Callback quand le timer est terminé */
	void HandleRespawnReady();
};
