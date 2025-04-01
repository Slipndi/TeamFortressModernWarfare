#include "TFC_ArmorComponent.h"
#include "Net/UnrealNetwork.h"
#include "TFModerneWarfare/Core/Game/TFC_GameStatics.h"
#include "TFModerneWarfare/Core/Structs/FPlayerClassData.h"

UTFC_ArmorComponent::UTFC_ArmorComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	SetIsReplicatedByDefault(true);
}

void UTFC_ArmorComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME_CONDITION(UTFC_ArmorComponent, CurrentArmorAmount, COND_SkipOwner);
}

void UTFC_ArmorComponent::InitializeArmorFromClass(EPlayerClass PlayerClass)
{
	FArmorData LoadedArmor;
	float MaxAmount;

	if (TryLoadArmorData(PlayerClass, LoadedArmor, MaxAmount))
	{
		CurrentArmorData = LoadedArmor;
		MaxArmorAmount = MaxAmount;
		CurrentArmorAmount = MaxArmorAmount;
		OnRep_ArmorAmount();

		UE_LOG(LogTemp, Warning, TEXT("🛡️ Armure appliquée : %s | Absorption: %.0f%% | Amount: %.0f"),
			*UEnum::GetDisplayValueAsText(CurrentArmorData.ArmorType).ToString(),
			CurrentArmorData.DamageAbsorption * 100.f,
			CurrentArmorAmount);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("❌ Impossible d'initialiser l'armure pour la classe %s"), *UEnum::GetValueAsString(PlayerClass));
	}
}

bool UTFC_ArmorComponent::TryLoadArmorData(EPlayerClass PlayerClass, FArmorData& OutArmor, float& OutMaxAmount) const
{
	if (const FPlayerClassData* ClassData = UTFC_GameStatics::GetClassData(PlayerClass))
	{
		if (const FArmorData* Armor = UTFC_GameStatics::GetArmorData(ClassData->ArmorType))
		{
			OutArmor = *Armor;
			OutMaxAmount = ClassData->MaxArmor;
			return true;
		}
	}
	return false;
}

void UTFC_ArmorComponent::SetArmorAmount(float NewAmount)
{
	if (!GetOwner() || !GetOwner()->HasAuthority()) return;

	const float Clamped = FMath::Clamp(NewAmount, 0.f, MaxArmorAmount);
	if (FMath::IsNearlyEqual(Clamped, CurrentArmorAmount, KINDA_SMALL_NUMBER)) return;

	CurrentArmorAmount = Clamped;
	OnRep_ArmorAmount(); 
}

void UTFC_ArmorComponent::ApplyArmorDamage(float DamageAmount)
{
	if (!GetOwner() || !GetOwner()->HasAuthority() || DamageAmount <= 0.f) return;

	const float Clamped = FMath::Min(CurrentArmorAmount, DamageAmount);
	SetArmorAmount(CurrentArmorAmount - Clamped);
}

void UTFC_ArmorComponent::OnRep_ArmorAmount()
{
	UE_LOG(LogTemp, Log, TEXT("[Armor] Updated Armor: %.1f / %.1f"), CurrentArmorAmount, MaxArmorAmount);
	OnArmorChanged.Broadcast(CurrentArmorAmount);
}

float UTFC_ArmorComponent::TryAbsorbDamage(float IncomingDamage)
{
	if (!GetOwner() || !GetOwner()->HasAuthority() || IncomingDamage <= 0.f)
		return 0.f;

	const float AbsorptionRatio = CurrentArmorData.DamageAbsorption;
	const float MaxAbsorbable = IncomingDamage * AbsorptionRatio;

	const float Absorbed = FMath::Min(CurrentArmorAmount, MaxAbsorbable);
	ApplyArmorDamage(Absorbed);

	UE_LOG(LogTemp, Log, TEXT("[ARMOR] %s absorbed %.1f damage → Remaining Armor: %.1f"),
		*GetOwner()->GetName(), Absorbed, CurrentArmorAmount);

	return Absorbed;
}

