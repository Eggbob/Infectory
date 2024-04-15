// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/IFFieldTurret.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Game/IFGameMode.h"
#include "Game/IFObjectPoolManager.h"
#include "Item/IFProjectile.h"
#include "Kismet/GameplayStatics.h"
#include "Character/IFCharacterNonPlayer.h"
#include "Components/StaticMeshComponent.h"

AIFFieldTurret::AIFFieldTurret()
{
	PrimaryActorTick.bCanEverTick = true;

}

void AIFFieldTurret::BeginPlay()
{
	Super::BeginPlay();

	bDoOnce = false;
	bCanFire = true;
	bRealoading = false;
	bDoReload = false;
	bStartFire = false;

	TurretMesh = GetComponentByClass<UStaticMeshComponent>();
}

void AIFFieldTurret::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (!bIsStart) return;

	if (CheckCanFire())
	{
		AttackTarget();
	}
	if (Target == nullptr)
	{
		SearchTarget();
	}
}

void AIFFieldTurret::PerformTurret()
{
	bIsStart = true;
}

void AIFFieldTurret::StopTurret()
{
	bIsStart = false;
}

void AIFFieldTurret::SearchTarget()
{
	TArray<FOverlapResult> OverlapResults;

	FCollisionQueryParams CollisionQueryParam(SCENE_QUERY_STAT(Detect), false, this);
	bool bResult = GetWorld()->OverlapMultiByChannel(
		OverlapResults,
		GetActorLocation(),
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel4,
		FCollisionShape::MakeSphere(2000.f),
		CollisionQueryParam
	);


	if (bResult)
	{
		for (auto const& OverlapResult : OverlapResults)
		{
			if (OverlapResult.GetActor()->IsA(AIFCharacterNonPlayer::StaticClass()))
			{
				Target = Cast<AIFCharacterNonPlayer>(OverlapResult.GetActor());
				if (Target.Get()->GetNPCState() == ENPCState::Dead || Target.Get()->GetNPCState() == ENPCState::BeforeDead)
				{
					Target = nullptr;
				}
				else
				{
					return;
				}
				/*DrawDebugSphere(GetWorld(), GetActorLocation(), 1000.f, 16, FColor::Green, false, 0.2f);
				DrawDebugPoint(GetWorld(), OverlapResult.GetActor()->GetActorLocation(), 10.0f, FColor::Green, false, 0.2f);
				DrawDebugLine(GetWorld(), GetActorLocation(), OverlapResult.GetActor()->GetActorLocation(), FColor::Green, false, 0.27f);*/
			}
		}
	}
}

void AIFFieldTurret::AttackTarget()
{
	if (bCanFire)
	{
		FTransform SpawnTransform = TurretMesh->GetSocketTransform(MuzzleSocket);
		FHitResult Hit;
		FVector End = SpawnTransform.GetLocation() + SpawnTransform.GetRotation().Vector() * MaxRange;

		if (GetWorld()->LineTraceSingleByChannel(Hit, SpawnTransform.GetLocation(), End, ECollisionChannel::ECC_GameTraceChannel1, Params))
		{
			GiveDamage(Hit);
		}

		UGameplayStatics::SpawnEmitterAttached(MuzzleFlash, TurretMesh, MuzzleSocket);

		//SpawnTarcerEffect(SpawnTransform);
		//DrawDebugLine(GetWorld(), SpawnTransform.GetLocation(), End, FColor::Red, true, 1.f);

		bCanFire = false;
		UGameplayStatics::SpawnSoundAttached(FireSound, TurretMesh, MuzzleSocket);
		GetWorldTimerManager().SetTimer(FireTimerHandle, FTimerDelegate::CreateLambda([&]() {
			bCanFire = true;
			}), FireDelayTime, false);
	}
}

void AIFFieldTurret::GiveDamage(FHitResult& Hit)
{
	TWeakObjectPtr<AActor> HitActor = Hit.GetActor();

	if (HitActor != nullptr && HitActor->IsA<AIFCharacterNonPlayer>())
	{
		FCustomDamageEvent CustomDamageEvent;

		CustomDamageEvent.BoneName = Hit.BoneName;
		CustomDamageEvent.HitResult = Hit;

		HitActor->TakeDamage(Damage, CustomDamageEvent, nullptr, this);
	}

	if (HitActor->IsA(AIFCharacterNonPlayer::StaticClass()))
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), BloodImpactEffect, Hit.Location);

		if (!bDoOnce)
		{
			bDoOnce = true;
			UGameplayStatics::SpawnSoundAtLocation(GetWorld(), BodyImpactSound, Hit.Location);
		}
	}
	else
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactEffect, Hit.Location);

		if (!bDoOnce)
		{
			bDoOnce = true;
			UGameplayStatics::SpawnSoundAtLocation(GetWorld(), ImpactSound, Hit.Location);
		}
	}
}

void AIFFieldTurret::RotateToTarget()
{
	FRotator LookRot = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), Target->GetActorLocation());

	//TurretMesh.Get()->SetWorldRotation(LookRot);
	SetActorRotation(LookRot);
}

bool AIFFieldTurret::CheckCanFire()
{
	if (Target)
	{
		if (Target.Get()->GetNPCState() == ENPCState::Dead || Target.Get()->GetNPCState() == ENPCState::BeforeDead)
		{
			Target = nullptr;
			return false;
		}

		RotateToTarget();

		if (!bRealoading)
		{
			if (!bStartFire)
			{
				bStartFire = true;
				FTimerHandle TimerHandle;

				GetWorldTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateLambda([&]() {
					bRealoading = true;
					bDoReload = false;
					}), FireTime, false);
			}

			return true;
		}
		else
		{
			if (!bDoReload)
			{
				bDoReload = true;
				FTimerHandle TimerHandle;

				GetWorldTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateLambda([&]() {
					bRealoading = false;
					bStartFire = false;
					}), ReloadTime, false);
			}

			return false;
		}
	}
	else
	{
		return false;
	}
}

