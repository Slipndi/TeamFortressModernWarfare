#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TFModerneWarfare/Characters/Player/TFC_PlayerBase.h"
#include "TFModerneWarfare/Core/Enums/EMovementState.h"
#include "TFModerneWarfare/Core/Structs/FPlayerClassData.h"
#include "WBP_DebugMovementHUD.generated.h"

class UTextBlock;
class UScrollBox;

UCLASS()
class TFMODERNEWARFARE_API UWBP_DebugMovementHUD : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetPlayer(ATFC_PlayerBase* Player);

protected:
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

private:
	UPROPERTY()
	ATFC_PlayerBase* PlayerRef;

	UPROPERTY()
	UTextBlock* Text_MovementState;

	UPROPERTY(meta = (BindWidget))
	UScrollBox* Scroll_ClassData;

	UPROPERTY()
	class UTFC_MovementComponent* MoveComp;

	bool bDataBuilt = false;
	EMovementState LastKnownMovementState = EMovementState::Standing;

	void BuildClassDataUI(const FPlayerClassData* ClassData);

};
