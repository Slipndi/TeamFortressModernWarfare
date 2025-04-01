// UWBP_MainHUD.cpp (optimisé)

#include "UWBP_MainHUD.h"
#include "Components/TextBlock.h"
#include "TFModerneWarfare/Characters/Player/TFC_PlayerBase.h"
#include "TFModerneWarfare/Characters/Components/Health/TFC_HealthComponent.h"
#include "TFModerneWarfare/Characters/Components/Armor/TFC_ArmorComponent.h"
#include "TFModerneWarfare/Core/Game/TFC_GameStatics.h"

void UWBP_MainHUD::SetPlayerRef(ATFC_PlayerBase* Player)
{
	PlayerRef = Player;

	if (PlayerRef)
	{
		HealthComp = PlayerRef->FindComponentByClass<UTFC_HealthComponent>();
		ArmorComp = PlayerRef->FindComponentByClass<UTFC_ArmorComponent>();

		if (const FPlayerClassData* ClassData = UTFC_GameStatics::GetClassData(PlayerRef->GetPlayerClass()))
		{
			CachedClassData = *ClassData;
		}
	}
}

void UWBP_MainHUD::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	if (!PlayerRef || !HealthComp || !ArmorComp) return;

	// 💚 Vie
	const int32 HealthValue = FMath::RoundToInt(HealthComp->GetCurrentHealth());
	if (Text_Health)
	{
		Text_Health->SetText(FText::AsNumber(HealthValue));
	}

	// 🛡️ Armure actuelle / max
	if (Text_Armor)
	{
		const int32 CurrentArmor = FMath::RoundToInt(ArmorComp->GetCurrentArmor());
		Text_Armor->SetText(FText::AsNumber(CurrentArmor));
	}

	// 🛌 Classe
	if (Text_Class)
	{
		const FString ClassStr = UEnum::GetDisplayValueAsText(PlayerRef->GetPlayerClass()).ToString();
		Text_Class->SetText(FText::FromString(ClassStr));
	}
}
