#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "GameFramework/Character.h"
#include "TFModerneWarfare/Characters/Components/Armor/TFC_ArmorComponent.h"
#include "TFModerneWarfare/Characters/Components/Health/TFC_HealthComponent.h"
#include "TFModerneWarfare/Core/Enums/EPlayerClass.h"
#include "TFModerneWarfare/Core/Structs/FPlayerClassData.h"
#include "TFC_PlayerBase.generated.h"

class UTFC_WeaponSlotManagerComponent;
class UCameraComponent;
class UTFC_InputManagerComponent;
class UTFC_MovementComponent;
class UTFC_RespawnComponent;
class UDataTable;

UCLASS()
class TFMODERNEWARFARE_API ATFC_PlayerBase : public ACharacter
{
	GENERATED_BODY()

public:
	ATFC_PlayerBase();

	virtual void BeginPlay() override;
	
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	const FPlayerClassData* GetClassData() const;
	
	UFUNCTION(BlueprintCallable)
	UTFC_InputManagerComponent* GetInputManager() const { return InputComponentManager; }

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Class")
	EPlayerClass PlayerClassType = EPlayerClass::Soldier;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Class")
	UDataTable* ClassDataTable;

	UFUNCTION()
	EPlayerClass GetPlayerClass() const { return PlayerClassType; }
	
	UFUNCTION(BlueprintCallable)
	bool CanSprint() const;

	UFUNCTION(BlueprintCallable)
	UTFC_MovementComponent* GetMovementComponentCustom() const { return MovementComponent; }

	/** Applique localement les effets visuels de la mort (désactivation, hide mesh...) */
	UFUNCTION()
	void HandleLocalDeath();

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	UCameraComponent* CameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UTFC_InputManagerComponent* InputComponentManager;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UTFC_MovementComponent* MovementComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UTFC_ArmorComponent* ArmorComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UTFC_HealthComponent* HealthComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UTFC_RespawnComponent* RespawnComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapons", meta = (AllowPrivateAccess = "true"))
	UTFC_WeaponSlotManagerComponent* WeaponSlotManager;
	
};
