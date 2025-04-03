#include "TFC_RespawnComponent.h"
#include "TimerManager.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/Controller.h"
#include "Engine/World.h"

DEFINE_LOG_CATEGORY_STATIC(LogRespawn, Log, Warning);
UTFC_RespawnComponent::UTFC_RespawnComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UTFC_RespawnComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UTFC_RespawnComponent::StartRespawnCountdown(float Delay)
{
	if (!GetOwner()->HasAuthority())
	{
		UE_LOG(LogTemp, Warning, TEXT("⛔ [Respawn] Tentative de StartRespawnCountdown côté client."));
		return;
	}

	if (bIsPendingRespawn)
	{
		UE_LOG(LogRespawn, Warning, TEXT("[Respawn] ⚠️ Respawn déjà en attente."));
		return;
	}

	bIsPendingRespawn = true;
	RespawnDelay = Delay;

	GetWorld()->GetTimerManager().SetTimer(RespawnTimerHandle, this, &UTFC_RespawnComponent::HandleRespawnReady, Delay, false);

	UE_LOG(LogRespawn, Warning, TEXT("[Respawn] ⏳ Respawn timer lancé pour %.1f secondes"), Delay);
}

void UTFC_RespawnComponent::CancelRespawn()
{
	if (bIsPendingRespawn)
	{
		GetWorld()->GetTimerManager().ClearTimer(RespawnTimerHandle);
		bIsPendingRespawn = false;
		UE_LOG(LogRespawn, Warning, TEXT("[Respawn] ❌ Respawn annulé."));
	}
}

void UTFC_RespawnComponent::ForceRespawn()
{
	if (!bIsPendingRespawn)
	{
		UE_LOG(LogRespawn, Warning, TEXT("[Respawn] ⚠️ Aucun respawn à forcer."));
		return;
	}

	GetWorld()->GetTimerManager().ClearTimer(RespawnTimerHandle);
	HandleRespawnReady();
}

void UTFC_RespawnComponent::HandleRespawnReady()
{
	bIsPendingRespawn = false;

	APawn* OwnerPawn = Cast<APawn>(GetOwner());
	if (!OwnerPawn)
	{
		UE_LOG(LogRespawn, Error, TEXT("❌ [Respawn] Owner n'est pas un Pawn."));
		return;
	}

	AController* Controller = OwnerPawn->GetController();
	if (!Controller)
	{
		UE_LOG(LogRespawn, Error, TEXT("❌ [Respawn] Pas de Controller trouvé sur le pawn %s."), *OwnerPawn->GetName());
		return;
	}

	UE_LOG(LogRespawn, Warning, TEXT("[Respawn] ✅ Respawn prêt. Delegate émis."));
	OnRespawnReady.Broadcast(Controller);
}

float UTFC_RespawnComponent::GetRemainingTime() const
{
	if (!bIsPendingRespawn) return 0.f;

	return GetWorld()->GetTimerManager().GetTimerRemaining(RespawnTimerHandle);
}
