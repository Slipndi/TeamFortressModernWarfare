// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TFModerneWarfare/Core/Enums/EPlayerClass.h"
#include "TFModerneWarfare/Core/Structs/FArmorData.h"
#include "TFC_ArmorComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class TFMODERNEWARFARE_API UTFC_ArmorComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UTFC_ArmorComponent();
	void InitializeArmorFromClass(EPlayerClass PlayerClass);
	
	UFUNCTION(BlueprintCallable, Category = "Armor")
	const FArmorData& GetCurrentArmorData() const { return CurrentArmorData; }

protected:
	// Armure active pour ce joueur
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Armor", DisplayName = "Armure actuelle")
	FArmorData CurrentArmorData;
};
