#include "TFC_PlayerController.h"
#include "EnhancedInputComponent.h"
#include "TFModerneWarfare/Characters/Player/TFC_PlayerBase.h"
#include "TFModerneWarfare/Core/Inputs/TFC_InputManagerComponent.h"
#include "TFModerneWarfare/UI/Widgets/WBP_DebugMovementHUD.h"
#include "TFModerneWarfare/UI/UWBP_MainHUD.h"

DEFINE_LOG_CATEGORY_STATIC(PlayerControllerLog, Log, Warning);
ATFC_PlayerController::ATFC_PlayerController()
{
	UE_LOG(LogTemp, Warning, TEXT("[PlayerController] Constructeur exécuté."));
}

void ATFC_PlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (IsLocalController())
	{
		UE_LOG(PlayerControllerLog, Warning, TEXT("[PlayerController] Contrôleur local OK."));

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
			UE_LOG(PlayerControllerLog, Error, TEXT("❌ [HUD] HUDWidgetClass est NULL !"));
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
	UE_LOG(PlayerControllerLog, Warning, TEXT("[PlayerController] SetupInputComponent appelé."));
}

void ATFC_PlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	UE_LOG(LogTemp, Warning, TEXT("🎮 [PlayerController] OnPossess appelé pour : %s"), *InPawn->GetName());

	// Rebind HUDs
	if (ATFC_PlayerBase* PlayerRef = Cast<ATFC_PlayerBase>(InPawn))
	{
		if (IsValid(HUDWidget))
		{
			HUDWidget->SetPlayer(PlayerRef);
		}

		if (IsValid(MainHUDInstance))
		{
			MainHUDInstance->SetPlayerRef(PlayerRef);
		}
	}

	if (ATFC_PlayerBase* TFCPlayer = Cast<ATFC_PlayerBase>(InPawn))
	{
		if (UTFC_InputManagerComponent* InputMgr = TFCPlayer->GetInputManager())
		{
			InputMgr->InitializeInputs(this);
			UE_LOG(LogTemp, Warning, TEXT("🎮 [PlayerController] InputManager relancé après Possess"));
		}
	}

	// ✅ Réactivation des inputs
	SetupInputComponent();

	// ✅ Réactivation de la caméra et de la souris
	bShowMouseCursor = false;
	SetInputMode(FInputModeGameOnly());

	UE_LOG(PlayerControllerLog, Warning, TEXT("✅ [PlayerController] Inputs et souris réactivés après Possess()"));
}

