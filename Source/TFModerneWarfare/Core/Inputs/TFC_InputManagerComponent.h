#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InputActionValue.h"
#include "TFC_InputManagerComponent.generated.h"

class UInputMappingContext;
class UInputAction;
class UTFC_InputConfig;
class UEnhancedInputComponent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TFMODERNEWARFARE_API UTFC_InputManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UTFC_InputManagerComponent();

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY()
	class APlayerController* OwnerController;

	UPROPERTY()
	class APawn* OwnerPawn;

	UPROPERTY()
	class UTFC_MovementComponent* CachedMovementComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UTFC_InputConfig* InputConfig;

	void SetupEnhancedInput();

	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void StartJump(const FInputActionValue& Value);
	void StopJump(const FInputActionValue& Value);
	void StartSprint(const FInputActionValue& Value);
	void StopSprint(const FInputActionValue& Value);
	void HandleCrouchOrSlide(const FInputActionValue& Value);
	void HandleUnCrouch(const FInputActionValue& Value);

};
