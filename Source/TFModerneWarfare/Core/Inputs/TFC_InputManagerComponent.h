#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InputActionValue.h"
#include "TFC_InputManagerComponent.generated.h"

class UInputMappingContext;
class UInputAction;
class UTFC_InputConfig;
class UEnhancedInputComponent;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class TFMODERNEWARFARE_API UTFC_InputManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UTFC_InputManagerComponent();

	UFUNCTION(BlueprintCallable)
	UTFC_InputConfig* GetInputConfig() const { return InputConfig; }

	/** Peut être lu par le MovementComponent */
	bool IsSprintHeld() const { return bWantsToSprint; }

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY()
	APlayerController* OwnerController;

	UPROPERTY()
	APawn* OwnerPawn;

	UPROPERTY()
	class UTFC_MovementComponent* CachedMovementComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UTFC_InputConfig* InputConfig;

	UPROPERTY(EditDefaultsOnly, Category = "Debug")
	bool bDebugInput = true;

	// 🔁 Sprint toggle persistant
	bool bWantsToSprint = false;

	void SetupEnhancedInput();

	// Inputs
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void StartJump(const FInputActionValue& Value);
	void StopJump(const FInputActionValue& Value);
	void StartSprint(const FInputActionValue& Value);
	void StopSprint(const FInputActionValue& Value);
	void HandleCrouchOrSlide(const FInputActionValue& Value);
	void HandleUnCrouch(const FInputActionValue& Value);
};
