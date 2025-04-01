#include "TFC_HealthComponent.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/Actor.h"
#include "TFModerneWarfare/Characters/Components/Armor/TFC_ArmorComponent.h"
#include "TFModerneWarfare/Core/Game/TFC_GameStatics.h"
#include "TFModerneWarfare/Characters/Player/TFC_PlayerBase.h"

DEFINE_LOG_CATEGORY_STATIC(LogHealth, Log, All);

UTFC_HealthComponent::UTFC_HealthComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	SetIsReplicatedByDefault(true);
}

void UTFC_HealthComponent::BeginPlay()
{
	Super::BeginPlay();
	InitializeHealthFromClass();
}

void UTFC_HealthComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME_CONDITION(UTFC_HealthComponent, Health, COND_SkipOwner);
	DOREPLIFETIME_CONDITION(UTFC_HealthComponent, bIsDead, COND_SkipOwner);

}

void UTFC_HealthComponent::InitializeHealthFromClass()
{
	if (const AActor* Owner = GetOwner())
	{
		if (const ATFC_PlayerBase* Player = Cast<ATFC_PlayerBase>(Owner))
		{
			const FPlayerClassData* ClassData = UTFC_GameStatics::GetClassData(Player->PlayerClassType);
			if (ClassData)
			{
				MaxHealth = ClassData->MaxHealth;
				Health = MaxHealth;
				OnHealthChanged.Broadcast(Health);
			}
		}
	}
}

void UTFC_HealthComponent::ResetHealth()
{
	if (MaxHealth <= 0.0f) return;

	bIsDead = false;
	Health = MaxHealth;
	OnHealthChanged.Broadcast(Health);
}

float UTFC_HealthComponent::GetHealthRatio() const
{
	return (MaxHealth > 0.0f) ? Health / MaxHealth : 0.0f;
}

void UTFC_HealthComponent::ApplyDamage(const FDamageInfo& DamageInfo)
{
	if (!GetOwner() || !GetOwner()->HasAuthority() || Health <= 0.0f)
		return;

	if (MaxHealth <= 0.0f) return;

	float FinalDamage = DamageInfo.RawDamage;
	float AbsorbedDamage = 0.0f;

	const bool bIsHeadshot = DamageInfo.BoneHit.ToString().Equals(TEXT("head"), ESearchCase::IgnoreCase);
	if (bIsHeadshot)
	{
		FinalDamage *= 2.0f;
	}

	// 🛡️ Réduction via armure
	if (AActor* Owner = GetOwner())
	{
		if (UTFC_ArmorComponent* ArmorComp = Owner->FindComponentByClass<UTFC_ArmorComponent>())
		{
			AbsorbedDamage = ArmorComp->TryAbsorbDamage(FinalDamage);
			FinalDamage -= AbsorbedDamage;
		}
	}

	// 💥 Application des dégâts à la vie
	Health = FMath::Clamp(Health - FinalDamage, 0.0f, MaxHealth);
	OnHealthChanged.Broadcast(Health);

	// 🪵 DEBUG LOG
	UE_LOG(LogHealth, Warning,
		TEXT("[DAMAGE] %s took %.1f damage (Raw: %.1f | Absorbed: %.1f | Crit: %s | Bone: %s | Instigator: %s) → HP: %.1f"),
		*GetOwner()->GetName(),
		FinalDamage,
		DamageInfo.RawDamage,
		AbsorbedDamage,
		bIsHeadshot ? TEXT("YES") : TEXT("no"),
		*DamageInfo.BoneHit.ToString(),
		DamageInfo.Instigator ? *DamageInfo.Instigator->GetName() : TEXT("UNKNOWN"),
		Health
	);

	// 💀 Mort
	if (Health <= 0.0f && !bIsDead)
	{
		HandleDeath(DamageInfo);
	}
}

void UTFC_HealthComponent::HandleDeath(const FDamageInfo& DamageInfo)
{
	bIsDead = true;
	OnHealthChanged.Broadcast(Health);
	OnRep_Death();

	UE_LOG(LogHealth, Warning, TEXT("[DEATH] %s was killed by %s | BoneHit: %s"),
		*GetOwner()->GetName(),
		DamageInfo.Instigator ? *DamageInfo.Instigator->GetName() : TEXT("UNKNOWN"),
		*DamageInfo.BoneHit.ToString()
	);
}

void UTFC_HealthComponent::OnRep_Health()
{
	OnHealthChanged.Broadcast(Health);
}

void UTFC_HealthComponent::ApplyHealing(float HealAmount)
{
	if (!GetOwner() || bIsDead || HealAmount <= 0.0f)
		return;

	const float OldHealth = Health;
	Health = FMath::Clamp(Health + HealAmount, 0.0f, MaxHealth);

	if (Health != OldHealth)
	{
		OnHealthChanged.Broadcast(Health);
		UE_LOG(LogHealth, Log, TEXT("[HEAL] %s healed %.1f HP → New HP: %.1f"), *GetOwner()->GetName(), HealAmount, Health);
	}
}

void UTFC_HealthComponent::OnRep_Death()
{
	if (bIsDead)
	{
		OnDeath.Broadcast();
		UE_LOG(LogHealth, Warning, TEXT("[REPLICATED DEATH] %s is DEAD (client)."), *GetOwner()->GetName());
	}
}

void UTFC_HealthComponent::Kill()
{
	if (!GetOwner() || !GetOwner()->HasAuthority() || bIsDead) return;

	Health = 0.0f;
	bIsDead = true;

	OnHealthChanged.Broadcast(Health);
	OnRep_Death();

	UE_LOG(LogHealth, Warning, TEXT("[KILL] %s has been force-killed."), *GetOwner()->GetName());
}

