#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TFModerneWarfare/Core/Structs/FArmorData.h"
#include "TFModerneWarfare/Core/Enums/EPlayerClass.h"
#include "TFC_ArmorComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnArmorChangedSignature, float, NewArmorValue);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class TFMODERNEWARFARE_API UTFC_ArmorComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UTFC_ArmorComponent();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	/** Initialise l'armure à partir de la classe */
	void InitializeArmorFromClass(EPlayerClass PlayerClass);

	/** Modifie la quantité d’armure de façon sécurisée */
	void SetArmorAmount(float NewAmount);

	/** Applique des dégâts à l’armure côté serveur uniquement */
	void ApplyArmorDamage(float DamageAmount);

	/** Renvoie la Data d’armure */
	UFUNCTION(BlueprintCallable, Category = "Armor")
	const FArmorData& GetCurrentArmorData() const { return CurrentArmorData; }

	/** Renvoie le pourcentage d'absorption de dégâts (ex: 0.6) */
	UFUNCTION(BlueprintCallable, Category = "Armor")
	float GetDamageAbsorption() const { return CurrentArmorData.DamageAbsorption; }

	/** Accès quantités */
	float GetCurrentArmor() const { return CurrentArmorAmount; }
	float GetMaxArmor() const { return MaxArmorAmount; }

	UFUNCTION(BlueprintCallable)
	EArmorType GetArmorType() const { return CurrentArmorData.ArmorType; }

	/** Tente d'absorber une partie des dégâts. Retourne combien a été absorbé */
	float TryAbsorbDamage(float IncomingDamage);

protected:
	/** Data de type d’armure (non-répliquée) */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Armor")
	FArmorData CurrentArmorData;

	/** Quantité d'armure max */
	float MaxArmorAmount = 0.f;

	/** Quantité d'armure actuelle */
	UPROPERTY(ReplicatedUsing = OnRep_ArmorAmount)
	float CurrentArmorAmount = 0.f;

	/** Hook de réplication */
	UFUNCTION()
	void OnRep_ArmorAmount();

	/** Delegate appelé quand la quantité change */
	UPROPERTY(BlueprintAssignable)
	FOnArmorChangedSignature OnArmorChanged;

	/** Récupère les données d’armure et maxArmor depuis les DataTables */
	bool TryLoadArmorData(EPlayerClass PlayerClass, FArmorData& OutArmor, float& OutMaxAmount) const;
};
