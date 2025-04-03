#include "AWeapon_Hitscan.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Actor.h"
#include "DrawDebugHelpers.h"
#include "TFModerneWarfare/Core/Enums/EDamageType.h"

void AWeapon_Hitscan::Fire_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("🔥 AWeapon_Hitscan::Fire_Implementation appelé"));

	if (!GetOwner()) return;

	APawn* OwnerPawn = Cast<APawn>(GetOwner());
	if (!OwnerPawn) return;

	APlayerController* PC = Cast<APlayerController>(OwnerPawn->GetController());
	if (!PC) return;

	FVector CameraLocation;
	FRotator CameraRotation;
	PC->GetPlayerViewPoint(CameraLocation, CameraRotation);

	FVector TraceStart = CameraLocation;
	FVector TraceEnd = TraceStart + CameraRotation.Vector() * 10000.f;

	FHitResult HitResult;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);
	QueryParams.AddIgnoredActor(OwnerPawn);

	bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECC_Visibility, QueryParams);

	// Debug trace
	DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::Red, false, 2.f, 0, 1.f);

	if (bHit)
	{
		UE_LOG(LogTemp, Warning, TEXT("[Hitscan] Hit actor: %s at %s"), *HitResult.GetActor()->GetName(), *HitResult.ImpactPoint.ToString());
		ProcessHit(HitResult);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("[Hitscan] Missed shot."));
	}

}

void AWeapon_Hitscan::ProcessHit(const FHitResult& Hit)
{
	if (Hit.GetActor())
	{
		UGameplayStatics::ApplyPointDamage(
			Hit.GetActor(),
			WeaponStats->Stats.BaseDamage,
			Hit.TraceStart - Hit.TraceEnd,
			Hit,
			OwningController,
			this,
			WeaponStats->Stats.DamageTypeClass
		);

		UE_LOG(LogTemp, Warning, TEXT("[Hitscan] Hit %s for %.1f damage."), *Hit.GetActor()->GetName(), WeaponStats->Stats.BaseDamage);
	}
}
