// Fill out your copyright notice in the Description page of Project Settings.

#include "Item/IFGunBase.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Engine/DamageEvents.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/Character.h"
#include "NiagaraComponent.h"
#include "Item/IFProjectile.h"
#include "Game/IFGameMode.h"
#include "Data/IFGameSingleton.h"
#include "Game/IFObjectPoolManager.h"



AIFGunBase::AIFGunBase()
{
	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	NiagaraComp = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Niagara"));

	SetRootComponent(RootComp);
	Mesh->SetupAttachment(RootComp);
}

void AIFGunBase::FireRifle()
{
	FHitResult Hit;
	FVector ShotDirection;

	bool bSuccess = GunTrace(Hit, ShotDirection);

	FVector SpawnLocation = Mesh->GetSocketLocation(MuzzleSocket);;
	FVector Rot = ShotDirection * -1;

	TObjectPtr<AIFProjectile> Tracer = Cast<AIFProjectile>(GameMode.Get()->GetPoolManager().Get()->Pop(TracerEffect, GetWorld()));
	Tracer.Get()->SetActorRotation(Rot.Rotation());
	Tracer.Get()->SetActorLocationAndRotation(SpawnLocation, Rot.Rotation());
	Tracer.Get()->OnFinish.BindLambda([&](AActor* ReturnActor) {

		TObjectPtr<AIFGameMode> GameMode = Cast<AIFGameMode>(GetWorld()->GetAuthGameMode());
		GameMode.Get()->GetPoolManager().Get()->Push(ReturnActor);
	});
	Tracer.Get()->Init(7500);
	Tracer.Get()->LaunchTracer();

	if (bSuccess)
	{
		AActor* HitActor = Hit.GetActor();
		
		if (HitActor != nullptr)
		{
			FCustomDamageEvent CustomDamageEvent;

			CustomDamageEvent.BoneName = Hit.BoneName;
			CustomDamageEvent.HitResult = Hit;

			GiveDamage(HitActor, CustomDamageEvent);

			if (HitActor->IsA(ACharacter::StaticClass()))
			{
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), BloodImpactEffect, Hit.Location, ShotDirection.Rotation());
				UGameplayStatics::SpawnSoundAtLocation(GetWorld(), BodyImpactSound, Hit.Location, ShotDirection.Rotation());
				CrossHairDelegate.ExecuteIfBound(true);
			}
			else
			{
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactEffect, Hit.Location, ShotDirection.Rotation());
				UGameplayStatics::SpawnSoundAtLocation(GetWorld(), ImpactSound, Hit.Location, ShotDirection.Rotation());
				CrossHairDelegate.ExecuteIfBound(false);
			}
		}

		
	}

	if (FireGunDelegate.IsBound())
	{
		FireGunDelegate.Execute(WeaponType);
	}
}

void AIFGunBase::FireShotGun()
{
	FVector ShotDirection;

	ShotGunTrace(ShotDirection);
	bDoOnce = false;

	for (const FHitResult& Hit : HitResults)
	{
		TWeakObjectPtr<AActor> HitActor = Hit.GetActor();

		if (HitActor != nullptr)
		{
			FCustomDamageEvent CustomDamageEvent;

			CustomDamageEvent.BoneName = Hit.BoneName;
			CustomDamageEvent.HitResult = Hit;

			GiveDamage(HitActor.Get(), CustomDamageEvent);
		}

		if (HitActor->IsA(ACharacter::StaticClass()))
		{
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), BloodImpactEffect, Hit.Location, ShotDirection.Rotation());

			if (!bDoOnce)
			{
				bDoOnce = true;
				UGameplayStatics::SpawnSoundAtLocation(GetWorld(), BodyImpactSound, Hit.Location, ShotDirection.Rotation());
			}
		}
		else
		{
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactEffect, Hit.Location, ShotDirection.Rotation());

			if (!bDoOnce)
			{
				bDoOnce = true;
				UGameplayStatics::SpawnSoundAtLocation(GetWorld(), ImpactSound, Hit.Location, ShotDirection.Rotation());
			}
			//CrossHairDelegate.ExecuteIfBound(false);
		}
	}

	if (HitResults.Num() > 0)
	{
		CrossHairDelegate.ExecuteIfBound(true);
	}
	else
	{
		CrossHairDelegate.ExecuteIfBound(false);
		
	}


	if (FireGunDelegate.IsBound())
	{
		FireGunDelegate.Execute(WeaponType);
	}
}

void AIFGunBase::GiveDamage(AActor* HitActor, FCustomDamageEvent& Hit)
{
	HitActor->TakeDamage(Damage, Hit, OwnerController, GetOwner());
}

void AIFGunBase::FireProjectile(FVector& TargetLoc)
{
	FTransform SpawnTransform = WeaponType == ERangedWeaponType::Projectile ? GetProjectileSpawnTransform() : Mesh->GetSocketTransform(MuzzleSocket);
	
	
	TObjectPtr<AIFProjectile> Projectile = Cast<AIFProjectile>(GameMode.Get()->GetPoolManager().Get()->Pop(ProjectileBP, GetWorld()));

	if (Projectile == nullptr)
	{
		Projectile = GetWorld()->SpawnActor<AIFProjectile>(ProjectileBP, SpawnTransform);
	}


	Projectile.Get()->SetActorLocationAndRotation(SpawnTransform.GetLocation(), SpawnTransform.GetRotation());

	Projectile->Init(ProjectileSpeed);
	Projectile->OnAttack.BindLambda([&](TObjectPtr<AActor> HitActor, FCustomDamageEvent CustomDamageEvent) {
		GiveDamage(HitActor, CustomDamageEvent);
	});
	Projectile.Get()->OnFinish.Unbind();
	Projectile.Get()->OnFinish.BindLambda([&](AActor * ReturnActor) {
		TObjectPtr<AIFGameMode> GameMode = Cast<AIFGameMode>(GetWorld()->GetAuthGameMode());
		GameMode.Get()->GetPoolManager().Get()->Push(ReturnActor);
	});

	switch (WeaponType)
	{
	case ERangedWeaponType::Projectile:
		Projectile->LaunchExplosive();
		break;
	case ERangedWeaponType::EnemyProjectile:
		Projectile->LaunchLight(TargetLoc);
		break;
	}


	if (ShootDelegate.IsBound())
	{
		Projectile->OnShoot.BindLambda([&](TSubclassOf<ULegacyCameraShake> CameraShake) {
			ShootDelegate.ExecuteIfBound(CameraShake);
		});
	}

	if (FireGunDelegate.IsBound())
	{
		FireGunDelegate.Execute(WeaponType);
	}
}

void AIFGunBase::FireThrower()
{
	FTransform SpawnTransform = Mesh->GetSocketTransform("BreathSocket");
	FVector ParticleScale = FVector(1.f, 1.f, 1.f);

	BreathEffectComp = UGameplayStatics::SpawnEmitterAttached( BreathEffect,
		Mesh,
		"BreathSocket");

	/*BreathEffectComp = UGameplayStatics::SpawnEmitterAttached(
		BreathEffect,
		Mesh,
		"BreathSocket",
		SpawnTransform.GetLocation(),
		SpawnTransform.GetRotation().Rotator()
	);*/

	//FHitResult Hit;

	//bool bSuccess = BreathTrace(Hit);
	StartBreathTrace();

	FTimerHandle Handler;
	GetWorldTimerManager().SetTimer(Handler, FTimerDelegate::CreateLambda([&]() {
		FinishBreathTrace();
		BreathEffectComp->Deactivate();
	}), 3.f, false);


	//if (bSuccess)
	//{
	//	AActor* HitActor = Hit.GetActor();

	//	if (HitActor != nullptr)
	//	{
	//		FCustomDamageEvent CustomDamageEvent;
	//		
	//		GiveDamage(HitActor, CustomDamageEvent);
	//	}
	//}

	//if (FireGunDelegate.IsBound())
	//{
	//	FireGunDelegate.Execute(WeaponType);
	//}

}

void AIFGunBase::CachingOwner()
{
	APawn* OwnerPawn = Cast<APawn>(GetOwner());
	OwnerController = OwnerPawn->GetController();
	ensure(OwnerController);

	GunStat = UIFGameSingleton::Get().GetGunStat(*UIFEnumDefine::GetEnumName(WeaponType));
	GameMode = Cast<AIFGameMode>(GetWorld()->GetAuthGameMode());
	MaxRange = GunStat.MaxRange;
	Damage = GunStat.Damage;
	TotalAmmo = GunStat.MaxAmmo;
	MagazineCapacity = GunStat.MaxCapacity;
	FireDelayTime = GunStat.FireDelay;
	ProjectileSpeed = GunStat.ProjectileSpeed;
	CurrentAmmo = MagazineCapacity;
}

void AIFGunBase::StartFire(FVector TargetLoc)
{
	if (!bIsCanFire) return;

	if (CurrentAmmo <= 0) 
	{ 
		UGameplayStatics::SpawnSoundAttached(EmptySound, Mesh, MuzzleSocket);
		return; 
	}
	
	CurrentAmmo--;
	AmmoChangedDelegate.ExecuteIfBound(CurrentAmmo, TotalAmmo);

	ShootDelegate.ExecuteIfBound(CameraShake);
	UGameplayStatics::SpawnEmitterAttached(MuzzleFlash, Mesh, MuzzleSocket);
	UGameplayStatics::SpawnSoundAttached(FireSound, Mesh, MuzzleSocket);

	GetWorldTimerManager().SetTimer(FireTimerHandle, FTimerDelegate::CreateLambda([&]() {
		bIsCanFire = true;
		if (bIsAuto)
		{
			StartFire(TargetLoc);
		}
	}), FireDelayTime, false);

	switch (WeaponType)
	{
		case ERangedWeaponType::Rifle:
			FireRifle();
			break;

		case ERangedWeaponType::Projectile:
		case ERangedWeaponType::EnemyProjectile:
			FireProjectile(TargetLoc);
			break;

		case ERangedWeaponType::ShotGun:
			FireShotGun();
			break;

		case ERangedWeaponType::Thrower:
			FireThrower();
			break;
	}

	bIsCanFire = false;
}

void AIFGunBase::StopFire()
{
	GetWorldTimerManager().ClearTimer(FireTimerHandle);
	bIsCanFire = true;
	FireGunDelegate.Unbind();
}

void AIFGunBase::Reload()
{
	TotalAmmo += CurrentAmmo;

	if (TotalAmmo >= MagazineCapacity)
	{
		CurrentAmmo = MagazineCapacity;
		TotalAmmo -= MagazineCapacity;
	}
	else
	{
		CurrentAmmo = TotalAmmo;
		TotalAmmo = 0;
	}

	AmmoChangedDelegate.ExecuteIfBound(CurrentAmmo, TotalAmmo);

}

bool AIFGunBase::CanReload()
{
	if (TotalAmmo == 0 || CurrentAmmo == MagazineCapacity)
	{
		return false;
	}
	else
	{
		return true;
	}
}

FVector AIFGunBase::GetWeaponSocket()
{
	return Mesh->GetSocketTransform(LeftHandSocket).GetLocation();
}

bool AIFGunBase::GunTrace(FHitResult& Hit, FVector& ShotDirection)
{
	if (OwnerController == nullptr) return false;

	FVector OwnerLocation;
	FRotator OwnerRotation;

	OwnerController.Get()->GetPlayerViewPoint(OwnerLocation, OwnerRotation);
	ShotDirection = -OwnerRotation.Vector();

	FVector End = OwnerLocation + OwnerRotation.Vector() * MaxRange;

	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	Params.AddIgnoredActor(GetOwner());

	//DrawDebugLine(GetWorld(), OwnerLocation, End, FColor::Red, true);
	return GetWorld()->LineTraceSingleByChannel(Hit, OwnerLocation, End, ECollisionChannel::ECC_GameTraceChannel1, Params);
}

void AIFGunBase::ShotGunTrace(FVector& ShotDirection)
{
	if (OwnerController == nullptr) return;

	HitResults.Empty();
	FVector OwnerLocation;
	FRotator OwnerRotation;
	OwnerController.Get()->GetPlayerViewPoint(OwnerLocation, OwnerRotation);
	ShotDirection = -OwnerRotation.Vector();
	FVector End = OwnerLocation + OwnerRotation.Vector() * MaxRange;
	FVector SpawnLocation = Mesh->GetSocketLocation(MuzzleSocket);

	for (int i = 0; i < 10; i++)
	{
		FHitResult Hit;
		
		FVector RandomVector = FVector(FMath::RandRange(-1 * SpreadRange, SpreadRange), FMath::RandRange(-1 * SpreadRange, SpreadRange), FMath::RandRange(-1 * SpreadRange, SpreadRange));
		End += RandomVector;
	
		FCollisionQueryParams Params;
		Params.AddIgnoredActor(this);
		Params.AddIgnoredActor(GetOwner());

		//DrawDebugLine(GetWorld(), OwnerLocation, End, FColor::Red, true, 0.5f);

		if (GetWorld()->LineTraceSingleByChannel(Hit, OwnerLocation, End, ECollisionChannel::ECC_GameTraceChannel1, Params))
		{
			HitResults.Add(Hit);
		}
		
		FRotator Rotation = UKismetMathLibrary::FindLookAtRotation(OwnerLocation, End);
		TObjectPtr<AIFProjectile> Tracer = Cast<AIFProjectile>(GameMode.Get()->GetPoolManager().Get()->Pop(TracerEffect, GetWorld()));
		Tracer.Get()->SetActorLocationAndRotation(SpawnLocation, Rotation);
		Tracer.Get()->OnFinish.BindLambda([&](AActor* ReturnActor) {
			TObjectPtr<AIFGameMode> GameMode = Cast<AIFGameMode>(GetWorld()->GetAuthGameMode());
			GameMode.Get()->GetPoolManager().Get()->Push(ReturnActor);
			});
		Tracer.Get()->Init(7500);
		Tracer.Get()->LaunchTracer();
	}
}




