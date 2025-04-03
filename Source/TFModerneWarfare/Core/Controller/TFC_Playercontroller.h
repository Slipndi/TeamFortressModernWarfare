#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TFModerneWarfare/UI/UWBP_MainHUD.h"
#include "TFModerneWarfare/UI/Widgets/WBP_DebugMovementHUD.h"
#include "TFC_PlayerController.generated.h"

UCLASS(Blueprintable)
class TFMODERNEWARFARE_API ATFC_PlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ATFC_PlayerController();

	FORCEINLINE UWBP_DebugMovementHUD* GetHUDWidget() const { return HUDWidget; }
	FORCEINLINE UWBP_MainHUD* GetMainHUD() const { return MainHUDInstance; }

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
	virtual void OnPossess(APawn* InPawn) override;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<class UWBP_DebugMovementHUD> HUDWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UWBP_MainHUD> MainHUDClass;

private:
	UPROPERTY()
	UWBP_DebugMovementHUD* HUDWidget;

	UPROPERTY()
	UWBP_MainHUD* MainHUDInstance;
};
