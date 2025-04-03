#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TFC_GameMode.generated.h"

UCLASS()
class TFMODERNEWARFARE_API ATFC_GameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ATFC_GameMode();

protected:
	virtual void BeginPlay() override;
	
	UFUNCTION()
	void RespawnPlayer(AController* Controller);

	UFUNCTION()
	void HandleRespawnReady();
};
