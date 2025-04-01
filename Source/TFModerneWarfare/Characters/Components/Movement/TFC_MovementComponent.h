#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TFModerneWarfare/Core/Enums/EMovementState.h"
#include "Net/UnrealNetwork.h"
#include "TFModerneWarfare/Core/Structs/FMovementFrame.h"

#include "TFC_MovementComponent.generated.h"

class ATFC_PlayerBase;
struct FInputActionValue;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class TFMODERNEWARFARE_API UTFC_MovementComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UTFC_MovementComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	// 🔹 Init à partir de la DataTable selon PlayerClass
	void InitializeMovementFromClass();

	// 🔹 Fonctions de mouvement appelées via InputManager
	void StartSprint();
	void StopSprint();
	void HandleCrouchOrSlide();
	void HandleUnCrouch();
	void EndSlide();


	UFUNCTION(BlueprintCallable, Category = "Movement")
	FORCEINLINE EMovementState GetMovementState() const { return MovementState; }

	FORCEINLINE float GetFinalWalkSpeed() const { return FinalWalkSpeed; }
	FORCEINLINE float GetFinalSprintSpeed() const { return FinalSprintSpeed; }

	
	FTimerHandle SlideTimerHandle;
	
protected:
	virtual void BeginPlay() override;

	// 🎯 Statistiques mouvement calculées depuis la classe
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Speed")
	float FinalWalkSpeed = 500.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Speed")
	float FinalSprintSpeed = 800.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Speed")
	float WalkSpeed = 500.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Speed")
	float SprintSpeed = 800.f;

	// 🔄 État de mouvement courant
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	EMovementState MovementState = EMovementState::Standing;

	UFUNCTION(Server, Reliable)
	void ServerUpdateMovementState(EMovementState NewState);


private:
	// 🔒 Référence au pawn propriétaire
	UPROPERTY()
	ATFC_PlayerBase* Player = nullptr;

	// 🔧 Initialisation joueur interne
	void CachePlayer();
	
	void TickServerMovement();
	void TickClientMovement(float DeltaTime);

	TArray<FMovementFrame> MovementBuffer;
	const int32 MaxBufferSize = 50;

	bool bWasInAir = false;

	UPROPERTY()
	class UTFC_InputManagerComponent* CachedInputManager = nullptr;

protected:
	// SERVER-AUTHORITATIVE //
	UPROPERTY(ReplicatedUsing = OnRep_ServerState) FVector ServerPosition;

	UPROPERTY(ReplicatedUsing = OnRep_ServerState)
	FVector ServerVelocity;

	UPROPERTY(ReplicatedUsing = OnRep_ServerState)
    EMovementState ServerMovementState;

	UPROPERTY(ReplicatedUsing = OnRep_ServerValidatedFrame)
	FMovementFrame ReplicatedServerFrame;
	
	UFUNCTION()
	void OnRep_ServerValidatedFrame();
	
	UFUNCTION()
	void OnRep_ServerState();

	UPROPERTY(ReplicatedUsing = OnRep_ReplicatedSpeed)
	float ReplicatedCurrentSpeed = 0.f;

	UFUNCTION()
	void OnRep_ReplicatedSpeed();

	// 🔁 Réplication propre de l'état de mouvement pour simulation client
	UPROPERTY(ReplicatedUsing = OnRep_ReplicatedState)
	EMovementState ReplicatedMovementState;

	UFUNCTION()
	void OnRep_ReplicatedState();

	// ==========================
	//  CLIENT-SIDE
	// ==========================

	// Position / Velocity du client (simulation locale)
	FVector ClientSimPosition = FVector::ZeroVector;
	FVector ClientSimVelocity = FVector::ZeroVector;
	
	// Petite variable pour gérer la vitesse d'interpolation
	UPROPERTY(EditDefaultsOnly, Category="Movement|Replication")
	float InterpSpeed = 30.f;

	UPROPERTY(EditDefaultsOnly, Category="Movement|Replication")
	float SmallGapThreshold = 5.f;    // En centimètres, par exemple

	UPROPERTY(EditDefaultsOnly, Category="Movement|Replication")
	float BigGapThreshold   = 200.f;
	
	UPROPERTY(EditDefaultsOnly, Category = "Movement|Replication")
	float VisualSpeedMultiplier = 1.1f;

	UPROPERTY(EditDefaultsOnly, Category="Movement|Replication")
	float MaxZCorrectionThreshold = 200.f;

public:
	// Override obligatoire pour que nos variables soient répliquées
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	/** Frame serveur validée (servira pour rollback client) */
	FMovementFrame LastValidatedServerFrame;

	/** Index de validation pour la synchronisation */
	int32 ServerFrameIndex = 0;

	/** Pour debug visuel dans les logs */
	FString FormatMovementFrame(const FMovementFrame& Frame) const;

	int32 ClientFrameIndex = 0;

};
