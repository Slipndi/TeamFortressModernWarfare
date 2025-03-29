#include "TFC_PlayerController.h"
#include "EnhancedInputComponent.h"
#include "TFModerneWarfare/Characters/TFC_PlayerBase.h"
#include "TFModerneWarfare/UI/WBP_DebugMovementHUD.h"

ATFC_PlayerController::ATFC_PlayerController()
{
	bReplicates = true;
	UE_LOG(LogTemp, Warning, TEXT("[PlayerController] Constructeur exécuté."));
}

void ATFC_PlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (IsLocalController())
	{
		UE_LOG(LogTemp, Warning, TEXT("[PlayerController] Contrôleur local OK."));

		if (HUDWidgetClass)
		{
			HUDWidget = CreateWidget<UWBP_DebugMovementHUD>(this, HUDWidgetClass);
			if (HUDWidget)
			{
				HUDWidget->AddToViewport();
				HUDWidget->SetPlayer(Cast<ATFC_PlayerBase>(GetPawn()));
			}
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("❌ [HUD] HUDWidgetClass est NULL !"));
		}
	}
}


void ATFC_PlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	UE_LOG(LogTemp, Warning, TEXT("[PlayerController] SetupInputComponent appelé."));
}
