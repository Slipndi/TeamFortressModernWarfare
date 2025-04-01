#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TFModerneWarfare/Characters/Player/TFC_PlayerBase.h"
#include "TFModerneWarfare/Core/Enums/EMovementState.h"
#include "TFModerneWarfare/Core/Structs/FPlayerClassData.h"
#include "WBP_DebugMovementHUD.generated.h"

class UTextBlock;
class UScrollBox;
class UTFC_HealthComponent;
class UTFC_ArmorComponent;


UCLASS()
class TFMODERNEWARFARE_API UWBP_DebugMovementHUD : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetPlayer(ATFC_PlayerBase* Player);

protected:
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	
	UFUNCTION()
	void AddDebugEntry(const FString& Label, const FString& Value);

	UFUNCTION()
	void ClearDynamicDebugEntries();

private:
	UPROPERTY()
	ATFC_PlayerBase* PlayerRef;

	UPROPERTY()
	UTFC_HealthComponent* CachedHealthComponent;

	UPROPERTY()
	UTFC_ArmorComponent* CachedArmorComponent;

	UPROPERTY()
	UTextBlock* Text_MovementState;

	UPROPERTY(meta = (BindWidget))
	UScrollBox* Scroll_ClassData;

	UPROPERTY(meta = (BindWidget))
	class UScrollBox* Scroll_LiveDebug;

	UPROPERTY()
	class UTFC_MovementComponent* MoveComp;
	
	UPROPERTY()
	TMap<FString, UTextBlock*> DebugTextEntries;
	

	bool bDataBuilt = false;
	EMovementState LastKnownMovementState = EMovementState::Standing;

	void BuildClassDataUI(const FPlayerClassData* ClassData);

};
