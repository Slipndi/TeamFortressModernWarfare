#include "TFC_PlayerController.h"
#include "EnhancedInputComponent.h"
#include "TFModerneWarfare/Characters/Player/TFC_PlayerBase.h"
#include "TFModerneWarfare/UI/Widgets/WBP_DebugMovementHUD.h"
#include "TFModerneWarfare/UI/UWBP_MainHUD.h"

ATFC_PlayerController::ATFC_PlayerController()
{
	UE_LOG(LogTemp, Warning, TEXT("[PlayerController] Constructeur exécuté."));
}

void ATFC_PlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (IsLocalController())
	{
		UE_LOG(LogTemp, Warning, TEXT("[PlayerController] Contrôleur local OK."));

		// 🔧 HUD Debug
		if (HUDWidgetClass)
		{
			HUDWidget = CreateWidget<UWBP_DebugMovementHUD>(this, HUDWidgetClass);
			if (IsValid(HUDWidget) && !HUDWidget->IsInViewport())
			{
				HUDWidget->AddToViewport();

				if (ATFC_PlayerBase* PlayerRef = Cast<ATFC_PlayerBase>(GetPawn()))
				{
					HUDWidget->SetPlayer(PlayerRef);
				}
			}
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("❌ [HUD] HUDWidgetClass est NULL !"));
		}

		// 🔧 HUD Principal
		if (MainHUDClass)
		{
			MainHUDInstance = CreateWidget<UWBP_MainHUD>(this, MainHUDClass);
			if (IsValid(MainHUDInstance))
			{
				MainHUDInstance->AddToViewport();

				if (ATFC_PlayerBase* PlayerRef = Cast<ATFC_PlayerBase>(GetPawn()))
				{
					MainHUDInstance->SetPlayerRef(PlayerRef);
				}
			}
		}
	}
}

void ATFC_PlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	UE_LOG(LogTemp, Warning, TEXT("[PlayerController] SetupInputComponent appelé."));
}
