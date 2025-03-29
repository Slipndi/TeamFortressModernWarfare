#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "GameFramework/Character.h"
#include "TFModerneWarfare/Core/Enums/EMovementState.h"
#include "TFModerneWarfare/Core/Enums/EPlayerClass.h"
#include "TFModerneWarfare/Core/Structs/FPlayerClassData.h"
#include "TFC_PlayerBase.generated.h"


UCLASS()
class TFMODERNEWARFARE_API ATFC_PlayerBase : public ACharacter
{
	GENERATED_BODY()

public:
	ATFC_PlayerBase();
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Class")
	float FinalWalkSpeed = 500.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Class")
	float FinalSprintSpeed = 800.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement")
	float WalkSpeed = 500.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement")
	float SprintSpeed = 800.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	EMovementState MovementState = EMovementState::Standing;
	UFUNCTION(BlueprintCallable, Category = "Movement")
	FORCEINLINE EMovementState GetMovementState() const { return MovementState; }
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	FRotator SlideCameraTilt = FRotator(-10.f, 0.f, 0.f);
	void EndSlide();
	FTimerHandle SlideTimerHandle;
	const FPlayerClassData* GetClassData() const;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Class")
	EPlayerClass PlayerClassType = EPlayerClass::Soldier ;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Class")
	UDataTable* ClassDataTable;

protected:
	virtual void BeginPlay() override;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	class UCameraComponent* CameraComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	class UCharacterMovementComponent* MovementComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UTFC_InputManagerComponent* InputComponentManager;

};

