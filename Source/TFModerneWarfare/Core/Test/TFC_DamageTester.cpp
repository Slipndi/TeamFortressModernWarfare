#include "TFC_DamageTester.h"
#include "Kismet/GameplayStatics.h"
#include "TFModerneWarfare/Core/Structs/FDamageInfo.h"
#include "TFModerneWarfare/Core/Interfaces/IDamageable.h"
#include "TFModerneWarfare/Characters/Player/TFC_PlayerBase.h"

ATFC_DamageTester::ATFC_DamageTester()
{
	PrimaryActorTick.bCanEverTick = true;
}

bool bTKeyWasDownLastFrame = false;

void ATFC_DamageTester::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Anti-spam : ne déclenche qu’une seule fois par pression
	const bool bTKeyNowDown = GEngine->GameViewport->Viewport->KeyState(EKeys::T);

	if (bTKeyNowDown && !bTKeyWasDownLastFrame)
	{
		TestApplyDamage();
	}

	bTKeyWasDownLastFrame = bTKeyNowDown;
}

void ATFC_DamageTester::TestApplyDamage()
{
	TArray<AActor*> Players;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATFC_PlayerBase::StaticClass(), Players);

	if (Players.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("[DAMAGE TEST] Aucun joueur ATFC_PlayerBase trouvé."));
		return;
	}

	AActor* Target = Players[0];

	if (UTFC_HealthComponent* HealthComp = Target->FindComponentByClass<UTFC_HealthComponent>())
	{
		FDamageInfo Info;
		Info.RawDamage = DamageAmount;
		Info.BoneHit = BoneToHit;
		Info.HitLocation = Target->GetActorLocation();
		Info.Instigator = this;
		Info.DamageType = DamageType;

		HealthComp->ApplyDamage(Info);

		UE_LOG(LogTemp, Warning, TEXT("[DAMAGE TEST] Dégâts appliqués à %s via HealthComponent"), *Target->GetName());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("[DAMAGE TEST] Pas de HealthComponent trouvé sur %s."), *Target->GetName());
	}
}
