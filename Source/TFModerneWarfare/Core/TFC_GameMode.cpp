#include "TFC_GameMode.h"
#include "TFC_Playercontroller.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "TFModerneWarfare/Characters/TFC_PlayerBase.h"

ATFC_GameMode::ATFC_GameMode()
{
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Blueprints/Characters/BP_PlayerCharacter"));
	if (PlayerPawnBPClass.Class != nullptr)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	PlayerControllerClass = ATFC_PlayerController::StaticClass();

	UE_LOG(LogTemp, Warning, TEXT("[GameMode] Constructor exécuté."));
}

void ATFC_GameMode::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp, Warning, TEXT("[GameMode] BeginPlay appeal."));
}
