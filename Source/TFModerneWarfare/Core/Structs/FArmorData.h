#pragma once

#include "CoreMinimal.h"
#include "TFModerneWarfare/Core/Enums/EArmorType.h"
#include "FArmorData.generated.h"

USTRUCT(BlueprintType)
struct TFMODERNEWARFARE_API FArmorData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Armor", DisplayName = "Type d'Armure")
	EArmorType ArmorType = EArmorType::None;
	
	// Pourcentage de dégâts absorbés (ex: 0.3 pour 30%)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Armor", DisplayName = "Absorption des dommages")
	float DamageAbsorption = 0.f;

	// Couleur associée (utile UI ou fx)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Armor", DisplayName = "Couleur de l'armure")
	FLinearColor ArmorColor = FLinearColor::White;
};
