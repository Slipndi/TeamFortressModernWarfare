#include "TFC_ArmorComponent.h"
#include "TFModerneWarfare/Core/Structs/FPlayerClassData.h"
#include "TFModerneWarfare/Core/Game/TFC_GameStatics.h"


UTFC_ArmorComponent::UTFC_ArmorComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UTFC_ArmorComponent::InitializeArmorFromClass(EPlayerClass PlayerClass)
{
	if (const FPlayerClassData* ClassData = UTFC_GameStatics::GetClassData(PlayerClass))
	{
		if (const FArmorData* Armor = UTFC_GameStatics::GetArmorData(ClassData->ArmorType))
		{
			CurrentArmorData = *Armor;

			UE_LOG(LogTemp, Warning, TEXT("🛡️ Armure appliquée : %s | Absorption: %.0f%%"),
				*UEnum::GetDisplayValueAsText(CurrentArmorData.ArmorType).ToString(),
				CurrentArmorData.DamageAbsorption * 100.f);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("❌ Aucun ArmorData trouvé pour %s"), *UEnum::GetDisplayValueAsText(ClassData->ArmorType).ToString());
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("❌ Aucun ClassData trouvé pour %s"), *UEnum::GetDisplayValueAsText(PlayerClass).ToString());
	}
}
