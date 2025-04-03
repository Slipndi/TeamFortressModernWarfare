#include "TFC_GameMode.h"
#include "../Controller/TFC_Playercontroller.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "TFModerneWarfare/Characters/Components/Respawn/TFC_RespawnComponent.h"
#include "GameFramework/PlayerStart.h"
#include "TFModerneWarfare/Characters/Player/TFC_PlayerBase.h"

ATFC_GameMode::ATFC_GameMode()
{
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Blueprints/Characters/BP_PlayerCharacter"));
	if (PlayerPawnBPClass.Class != nullptr)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("[GameMode] ❌ BP_PlayerCharacter non trouvé !"));
	}

	PlayerControllerClass = ATFC_PlayerController::StaticClass();

	UE_LOG(LogTemp, Warning, TEXT("[GameMode] Constructor exécuté."));
}

void ATFC_GameMode::BeginPlay()
{
	Super::BeginPlay();
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		if (APlayerController* PC = It->Get())
		{
			if (APawn* Pawn = PC->GetPawn())
			{
				if (UTFC_RespawnComponent* RespawnComp = Pawn->FindComponentByClass<UTFC_RespawnComponent>())
				{
					RespawnComp->OnRespawnReady.AddDynamic(this, &ATFC_GameMode::RespawnPlayer);
				}
			}
		}
	}
	UE_LOG(LogTemp, Warning, TEXT("[GameMode] BeginPlay appelé."));
}

void ATFC_GameMode::RespawnPlayer(AController* Controller)
{
	if (!Controller)
	{
		UE_LOG(LogTemp, Error, TEXT("❌ [GameMode] RespawnPlayer: Controller invalide."));
		return;
	}

	// 🧹 1. Détruire le Pawn actuel (s'il existe encore)
	if (APawn* CurrentPawn = Controller->GetPawn())
	{
		UE_LOG(LogTemp, Warning, TEXT("♻️ [GameMode] Destruction de l'ancien pawn : %s"), *CurrentPawn->GetName());
		CurrentPawn->Destroy();
	}

	// 📍 2. Trouver un PlayerStart valide
	AActor* SpawnPoint = nullptr;
	TArray<AActor*> PlayerStarts;
	UGameplayStatics::GetAllActorsOfClass(this, APlayerStart::StaticClass(), PlayerStarts);

	if (PlayerStarts.Num() > 0)
	{
		SpawnPoint = PlayerStarts[FMath::RandRange(0, PlayerStarts.Num() - 1)];
		UE_LOG(LogTemp, Warning, TEXT("📍 [GameMode] SpawnPoint choisi : %s"), *SpawnPoint->GetName());
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("❌ [GameMode] Aucun PlayerStart trouvé."));
		return;
	}

	// 🧱 3. Spawn du nouveau pawn
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = Controller;
	SpawnParams.Instigator = Controller->GetPawn();
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	FVector Location = SpawnPoint->GetActorLocation();
	FRotator Rotation = SpawnPoint->GetActorRotation();

	APawn* NewPawn = GetWorld()->SpawnActor<APawn>(DefaultPawnClass, Location, Rotation, SpawnParams);

	if (!NewPawn)
	{
		UE_LOG(LogTemp, Error, TEXT("❌ [GameMode] Impossible de spawner un nouveau pawn."));
		return;
	}

	// 🎮 4. Possession
	Controller->Possess(NewPawn);
	UE_LOG(LogTemp, Warning, TEXT("✅ [GameMode] Nouveau joueur %s possédé par %s"), *NewPawn->GetName(), *Controller->GetName());
}

void ATFC_GameMode::HandleRespawnReady()
{
	UE_LOG(LogTemp, Warning, TEXT("🎯 [GameMode] Delegate OnRespawnReady reçu → RespawnPlayer() à appeler ici"));

	// Récupère un Controller valide à partir du contexte (à remplacer si tu veux passer une référence spécifique)
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		if (APlayerController* PC = It->Get())
		{
			if (PC->GetPawn() == nullptr)
			{
				RespawnPlayer(PC);
				return;
			}
		}
	}

	UE_LOG(LogTemp, Error, TEXT("❌ [GameMode] Aucun Controller sans pawn trouvé pour respawn."));
}