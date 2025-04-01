#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TFModerneWarfare/Core/Structs/FPlayerClassData.h"
#include "UWBP_MainHUD.generated.h"

class UTextBlock;
class ATFC_PlayerBase;
class UTFC_HealthComponent;
class UTFC_ArmorComponent;

UCLASS()
class TFMODERNEWARFARE_API UWBP_MainHUD : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void SetPlayerRef(ATFC_PlayerBase* Player);

protected:
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* Text_Health;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* Text_Armor;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* Text_Class;

	UPROPERTY()
	ATFC_PlayerBase* PlayerRef;

	UPROPERTY()
	UTFC_HealthComponent* HealthComp;

	UPROPERTY()
	UTFC_ArmorComponent* ArmorComp;

	UPROPERTY()
	FPlayerClassData CachedClassData;
};
