#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WBP_DebugMovementHUD.generated.h"

class UTextBlock;

UCLASS()
class TFMODERNEWARFARE_API UWBP_DebugMovementHUD : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetPlayer(class ATFC_PlayerBase* Player);

protected:
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* Text_ClassName;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* Text_FinalWalkSpeed;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* Text_FinalSprintSpeed;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* Text_SlideImpulse;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* Text_MovementState;

private:
	UPROPERTY()
	ATFC_PlayerBase* PlayerRef;
};
