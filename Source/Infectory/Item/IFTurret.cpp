// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/IFTurret.h"
#include "Components/StaticMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Character/IFCharacterNonPlayer.h"
#include "Game/IFGameMode.h"
#include "Game/IFObjectPoolManager.h"
#include "Item/IFProjectile.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "Character/IFCharacterNonPlayer.h"
#include "GameFramework/ProjectileMovementComponent.h"

AIFTurret::AIFTurret()
{
	PrimaryActorTick.bCanEverTick = false;

	CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CAPSULECOMP"));
	SetRootComponent(CapsuleComp);
	
	TurretParentMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TurretParentMesh"));
	TurretParentMesh->SetupAttachment(CapsuleComp);

	TurretHeadMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TurretHeadMesh"));
	TurretHeadMesh->SetupAttachment(TurretParentMesh);

	TurretBarrelMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TurretBrrelMesh"));
	TurretBarrelMesh->SetupAttachment(TurretHeadMesh);
}

void AIFTurret::InitTurret(TObjectPtr<AController> Controller)
{
	OwnerController = Controller;
	ProjectileMovementComp = GetComponentByClass<UProjectileMovementComponent>();

	bDoOnce = false;
	bCanFire = true;
	bRealoading = false;
    bDoReload = false;
	bStartFire = false;

	FTimerHandle TimerHandle;
	GetWorldTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateLambda([&]() {
		PrimaryActorTick.bCanEverTick = true;
		}), 2.f, false);
}

void AIFTurret::LaunchTurret(FVector& TargetLoc)
{
	FVector LaunchDirec = TargetLoc - GetActorLocation();
	LaunchDirec.Z = 0.f;

	float ProjectileSpeed = 1000.f;
	float Gravity = 980.f;
	float TimeToTarget = LaunchDirec.Size() / ProjectileSpeed;
	FVector LaunchVelocity = LaunchDirec / TimeToTarget;
	LaunchVelocity.Z += 0.5f * Gravity * TimeToTarget;

	ProjectileMovementComp->Velocity = LaunchVelocity;
	ProjectileMovementComp->InitialSpeed = LaunchVelocity.Size();
	ProjectileMovementComp->MaxSpeed = LaunchVelocity.Size();
}


void AIFTurret::BeginPlay()
{
	Super::BeginPlay();

	for (int i = 1; i <= 4; i++)
	{
		FName MuzzleSocketName = FName(FString::Printf(TEXT("Muzzle%d"), i));

		MuzzleSockets.Add(MuzzleSocketName);
	}
	
	GameMode = Cast<AIFGameMode>(GetWorld()->GetAuthGameMode());
	Params.AddIgnoredActor(this);
}

void AIFTurret::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (CheckCanFire())
	{
		AttackTarget();
	}
	
	if(Target == nullptr)
	{
		SearchTarget();
	}
}

void AIFTurret::SearchTarget()
{
	TArray<FOverlapResult> OverlapResults;

	FCollisionQueryParams CollisionQueryParam(SCENE_QUERY_STAT(Detect), false, this);
	bool bResult = GetWorld()->OverlapMultiByChannel(
		OverlapResults,
		GetActorLocation(),
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel4,
		FCollisionShape::MakeSphere(1000.f),
		CollisionQueryParam
	);

	//FCollisionQueryParams CollisionQueryParam(SCENE_QUERY_STAT(Detect), false, this);
	//bool bResult = GetWorld()->OverlapMultiByChannel(
	//	OverlapResults,
	//	GetActorLocation(),
	//	FQuat::Identity,
	//	ECollisionChannel::ECC_GameTraceChannel1,
	//	FCollisionShape::MakeSphere(1000.f),
	//	CollisionQueryParam
	//);

	if (bResult)
	{
		for (auto const& OverlapResult : OverlapResults)
		{
			TObjectPtr<APawn> Actor = Cast<APawn>(OverlapResult.GetActor());

			if (Actor && !Actor.Get()->GetController()->IsPlayerController())
			{
				Target = Actor;
				/*DrawDebugSphere(GetWorld(), GetActorLocation(), 1000.f, 16, FColor::Green, false, 0.2f);
				DrawDebugPoint(GetWorld(), Actor.Get()->GetActorLocation(), 10.0f, FColor::Green, false, 0.2f);
				DrawDebugLine(GetWorld(), GetActorLocation(), Actor->GetActorLocation(), FColor::Green, false, 0.27f);*/
			}
		}
	}
}

void AIFTurret::AttackTarget()
{
	if (bCanFire)
	{ 
		for (FName & Name : MuzzleSockets)
		{
			FTransform SpawnTransform = TurretBarrelMesh->GetSocketTransform(Name);
			FHitResult Hit;
			FVector End = SpawnTransform.GetLocation() + SpawnTransform.GetRotation().Vector() * MaxRange;
			
			if (GetWorld()->LineTraceSingleByChannel(Hit, SpawnTransform.GetLocation(), End, ECollisionChannel::ECC_GameTraceChannel1, Params))
			{
				GiveDamage(Hit);
			}

			UGameplayStatics::SpawnEmitterAttached(MuzzleFlash, TurretBarrelMesh, Name);
			//SpawnTarcerEffect(SpawnTransform);
			//DrawDebugLine(GetWorld(), SpawnTransform.GetLocation(), End, FColor::Red, true, 1.f);
		}

		bCanFire = false;
		GetWorldTimerManager().SetTimer(FireTimerHandle, FTimerDelegate::CreateLambda([&]() {
			bCanFire = true;
			}), FireDelayTime, false);
	}
}

void AIFTurret::GiveDamage(FHitResult& Hit)
{
	TWeakObjectPtr<AActor> HitActor = Hit.GetActor();

	if (HitActor != nullptr)
	{
		FCustomDamageEvent CustomDamageEvent;

		CustomDamageEvent.BoneName = Hit.BoneName;
		CustomDamageEvent.HitResult = Hit;

		//HitActor->TakeDamage(Damage, CustomDamageEvent, OwnerController, GetOwner());
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

void AIFTurret::RotateToTarget()
{
	FRotator LookRot = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), Target->GetActorLocation());
	LookRot.Pitch = 0.f;
	LookRot.Yaw -= 60.f;

	TurretHeadMesh.Get()->SetWorldRotation(LookRot);
}

bool AIFTurret::CheckCanFire()
{
	if (Target)
	{
		RotateToTarget();

		if (!bRealoading)
		{
			if(!bStartFire)
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

void AIFTurret::SpawnTarcerEffect(FTransform& SpawnTransform)
{
	TObjectPtr<AIFProjectile> Tracer = Cast<AIFProjectile>(GameMode.Get()->GetPoolManager().Get()->Pop(TracerEffect, GetWorld()));
	Tracer.Get()->SetActorLocationAndRotation(SpawnTransform.GetLocation(), SpawnTransform.GetRotation());
	Tracer.Get()->OnFinish.Unbind();
	Tracer.Get()->OnFinish.BindLambda([&](AActor* ReturnActor) {
		TObjectPtr<AIFGameMode> GameMode = Cast<AIFGameMode>(GetWorld()->GetAuthGameMode());
		GameMode.Get()->GetPoolManager().Get()->Push(ReturnActor);
		});
	Tracer.Get()->Init(7500);
	Tracer.Get()->LaunchTracer();
}


