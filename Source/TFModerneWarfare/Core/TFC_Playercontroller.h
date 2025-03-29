#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TFC_PlayerController.generated.h"

UCLASS()
class TFMODERNEWARFARE_API ATFC_PlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ATFC_PlayerController();

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<class UWBP_DebugMovementHUD> HUDWidgetClass;

private:
	UPROPERTY()
	class UWBP_DebugMovementHUD* HUDWidget;
};
