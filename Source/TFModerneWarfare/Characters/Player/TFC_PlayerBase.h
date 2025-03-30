#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "GameFramework/Character.h"
#include "TFModerneWarfare/Characters/Components/Armor/TFC_ArmorComponent.h"
#include "TFModerneWarfare/Core/Enums/EPlayerClass.h"
#include "TFModerneWarfare/Core/Structs/FPlayerClassData.h"
#include "TFC_PlayerBase.generated.h"

class UCameraComponent;
class UTFC_InputManagerComponent;
class UTFC_MovementComponent;
class UDataTable;

UCLASS()
class TFMODERNEWARFARE_API ATFC_PlayerBase : public ACharacter
{
	GENERATED_BODY()

public:
	ATFC_PlayerBase();

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	const FPlayerClassData* GetClassData() const;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Class")
	EPlayerClass PlayerClassType = EPlayerClass::Soldier;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Class")
	UDataTable* ClassDataTable;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	UCameraComponent* CameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UTFC_InputManagerComponent* InputComponentManager;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UTFC_MovementComponent* MovementComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UTFC_ArmorComponent* ArmorComponent;
};
