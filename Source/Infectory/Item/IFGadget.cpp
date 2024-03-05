// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/IFGadget.h"
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

AIFGadget::AIFGadget()
{
	PrimaryActorTick.bCanEverTick = true;

	CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CAPSULECOMP"));
	SetRootComponent(CapsuleComp);
	
	TurretParentMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TurretParentMesh"));
	TurretParentMesh->SetupAttachment(CapsuleComp);

	TurretHeadMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TurretHeadMesh"));
	TurretHeadMesh->SetupAttachment(TurretParentMesh);

	TurretBarrelMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TurretBrrelMesh"));
	TurretBarrelMesh->SetupAttachment(TurretHeadMesh);
}

void AIFGadget::InitGadget(TObjectPtr<AController> Controller)
{
	OwnerController = Controller;

	ProjectileMovementComp.Get()->Activate(true);

	bDoOnce = false;
	bCanFire = true;
	bRealoading = false;
	bDoReload = false;
	bStartFire = false;

	switch (GadgetType)
	{
	case EGadgetType::Turret:
		InitTurret();
		break;

	case EGadgetType::Shield:
		FTimerHandle TimerHandle;
		GetWorldTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateLambda([&]() {
			GadgetDeInitDelegate.ExecuteIfBound();
			}), 10.f, false);

		break;
	}
}

void AIFGadget::InitTurret()
{
	for (int i = 1; i <= 4; i++)
	{
		FName MuzzleSocketName = FName(FString::Printf(TEXT("Muzzle%d"), i));

		MuzzleSockets.Add(MuzzleSocketName);
	}

	SetActorTickEnabled(true);

	FTimerHandle TimerHandle;
	GetWorldTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateLambda([&]() {
		bIsStart = true;
		}), 0.1f, false);
}

void AIFGadget::DeInitGadget()
{
	ProjectileMovementComp->Velocity = FVector::ZeroVector;
	ProjectileMovementComp->InitialSpeed = 0.f;
	ProjectileMovementComp->MaxSpeed = 0.f;
	bIsStart = false;
	ProjectileMovementComp.Get()->Deactivate();

	//SetActorTickEnabled(false);
	/*SetActorHiddenInGame(true);
	SetActorEnableCollision(false);*/
}

void AIFGadget::LaunchGadget(FVector& TargetLoc)
{
	FVector LaunchDirec = TargetLoc - GetActorLocation();

	float ProjectileSpeed = 1000.f;
	float Gravity = 980.f;
	float TimeToTarget = LaunchDirec.Size() / ProjectileSpeed;
	FVector LaunchVelocity = LaunchDirec / TimeToTarget;
	LaunchVelocity.Z += 0.5f * Gravity * TimeToTarget;

	FRotator LookRot = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), TargetLoc);
	LookRot.Pitch = 0.f;

	SetActorRotation(LookRot);

	ProjectileMovementComp->Velocity = LaunchVelocity;
	ProjectileMovementComp->InitialSpeed = LaunchVelocity.Size();
	ProjectileMovementComp->MaxSpeed = LaunchVelocity.Size();
}

void AIFGadget::BeginPlay()
{
	Super::BeginPlay();

	GameMode = Cast<AIFGameMode>(GetWorld()->GetAuthGameMode());
	ProjectileMovementComp = GetComponentByClass<UProjectileMovementComponent>();
	Params.AddIgnoredActor(this);
}

void AIFGadget::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!bIsStart) return;

	if (CheckCanFire())
	{
		AttackTarget();
	}
	
	if(Target == nullptr)
	{
		SearchTarget();
	}
}

void AIFGadget::SearchTarget()
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


	if (bResult)
	{
		for (auto const& OverlapResult : OverlapResults)
		{
			if (OverlapResult.GetActor()->IsA(AIFCharacterNonPlayer::StaticClass()))
			{
				Target = Cast<AIFCharacterNonPlayer>(OverlapResult.GetActor());
				/*DrawDebugSphere(GetWorld(), GetActorLocation(), 1000.f, 16, FColor::Green, false, 0.2f);
				DrawDebugPoint(GetWorld(), Actor.Get()->GetActorLocation(), 10.0f, FColor::Green, false, 0.2f);
				DrawDebugLine(GetWorld(), GetActorLocation(), Actor->GetActorLocation(), FColor::Green, false, 0.27f);*/
			}
		}
	}
}

void AIFGadget::AttackTarget()
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
		UGameplayStatics::SpawnSoundAttached(FireSound, TurretBarrelMesh, MuzzleSockets[0]);
		GetWorldTimerManager().SetTimer(FireTimerHandle, FTimerDelegate::CreateLambda([&]() {
			bCanFire = true;
			}), FireDelayTime, false);
	}
}

void AIFGadget::GiveDamage(FHitResult& Hit)
{
	TWeakObjectPtr<AActor> HitActor = Hit.GetActor();

	if (HitActor != nullptr)
	{
		FCustomDamageEvent CustomDamageEvent;

		CustomDamageEvent.BoneName = Hit.BoneName;
		CustomDamageEvent.HitResult = Hit;

		HitActor->TakeDamage(Damage, CustomDamageEvent, OwnerController, GetOwner());
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

void AIFGadget::RotateToTarget()
{
	FRotator LookRot = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), Target->GetActorLocation());
	LookRot.Pitch = 0.f;
	LookRot.Yaw -= 60.f;

	TurretHeadMesh.Get()->SetWorldRotation(LookRot);
}

bool AIFGadget::CheckCanFire()
{
	if (Target)
	{
		if (Target.Get()->GetNPCState() == ENPCState::Dead)
		{
			Target = nullptr;
			return false;
		}

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

void AIFGadget::SpawnTarcerEffect(FTransform& SpawnTransform)
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


