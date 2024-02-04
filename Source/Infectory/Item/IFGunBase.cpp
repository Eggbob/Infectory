// Fill out your copyright notice in the Description page of Project Settings.

#include "Item/IFGunBase.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Engine/DamageEvents.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/Character.h"
#include "NiagaraComponent.h"
#include "Data/IFGameSingleton.h"
#include "Item/IFProjectile.h"

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
	UGameplayStatics::SpawnEmitterAttached(MuzzleFlash, Mesh, MuzzleSocket);

	FHitResult Hit;
	FVector ShotDirection;

	bool bSuccess = GunTrace(Hit, ShotDirection);

	FVector SpawnLocation = Mesh->GetSocketLocation(MuzzleSocket);;
	FVector Rot = ShotDirection * -1;
	GetWorld()->SpawnActor<AActor>(TracerEffect, SpawnLocation, Rot.Rotation());

	if (bSuccess)
	{
		AActor* HitActor = Hit.GetActor();
		
		if (HitActor != nullptr)
		{
			FCustomDamageEvent CustomDamageEvent;

			CustomDamageEvent.BoneName = Hit.BoneName;
			CustomDamageEvent.HitResult = Hit;

			GiveDamage(HitActor, CustomDamageEvent);
		}

		if (HitActor->IsA(ACharacter::StaticClass()))
		{
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), BloodImpactEffect, Hit.Location, ShotDirection.Rotation());
		}
		else
		{
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactEffect, Hit.Location, ShotDirection.Rotation());
		}
	}

	if (FireGunDelegate.IsBound())
	{
		FireGunDelegate.Execute(WeaponType);
	}
}

void AIFGunBase::FireShotGun()
{
	UGameplayStatics::SpawnEmitterAttached(MuzzleFlash, Mesh, MuzzleSocket);

	FVector ShotDirection;

	ShotGunTrace(ShotDirection);

	for (FHitResult Hit : HitResults)
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
		}
		else
		{
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactEffect, Hit.Location, ShotDirection.Rotation());
		}
	}

	if (FireGunDelegate.IsBound())
	{
		FireGunDelegate.Execute(WeaponType);
	}
}

void AIFGunBase::GiveDamage(TObjectPtr<AActor> HitActor, FCustomDamageEvent& Hit)
{
	HitActor->TakeDamage(Damage, Hit, OwnerController, GetOwner());
}

void AIFGunBase::FireProjectile()
{
	UGameplayStatics::SpawnEmitterAttached(MuzzleFlash, Mesh, MuzzleSocket);

	FVector SpawnLocation = Mesh->GetSocketTransform(MuzzleSocket).GetLocation();

	TObjectPtr<AIFProjectile> Projectile = GetWorld()->SpawnActor<AIFProjectile>(ProjectileBP, SpawnLocation, Owner.Get()->GetActorRotation());
	Projectile.Get()->Init(ProjectileSpeed);
	Projectile.Get()->OnAttack.BindLambda([&](TObjectPtr<AActor> HitActor){
		FCustomDamageEvent CustomDamageEvent;
		GiveDamage(HitActor, CustomDamageEvent);
	}); 

	if (FireGunDelegate.IsBound())
	{
		FireGunDelegate.Execute(WeaponType);
	}
}

void AIFGunBase::CachingOwner()
{
	APawn* OwnerPawn = Cast<APawn>(GetOwner());
	OwnerController = OwnerPawn->GetController();
	ensure(OwnerController);

	GunStat = UIFGameSingleton::Get().GetGunStat(*UIFEnumDefine::GetEnumName(WeaponType));
	MaxRange = GunStat.MaxRange;
	Damage = GunStat.Damage;
	TotalAmmo = GunStat.MaxAmmo;
	MagazineCapacity = GunStat.MaxCapacity;
	FireDelayTime = GunStat.FireDelay;
	ProjectileSpeed = GunStat.ProjectileSpeed;

	CurrentAmmo = MagazineCapacity;
}

void AIFGunBase::StartFire()
{
	if (CurrentAmmo <= 0) { return; }

	CurrentAmmo--;
	AmmoChangedDelegate.ExecuteIfBound(CurrentAmmo, TotalAmmo);

	switch (WeaponType)
	{
	case ERangedWeaponType::Rifle:
		FireRifle();

		if (IsAuto)
		{
			GetWorldTimerManager().SetTimer(FireTimerHandle, this, &AIFGunBase::StartFire, FireDelayTime, false);
		}
		break;
	case ERangedWeaponType::Projectile:
	case ERangedWeaponType::EnemyProjectile:
		FireProjectile();
		break;

	case ERangedWeaponType::ShotGun:
		FireShotGun();
		break;
	}
}

void AIFGunBase::StopFire()
{
	GetWorldTimerManager().ClearTimer(FireTimerHandle);
	FireGunDelegate.Unbind();
}

void AIFGunBase::Reload()
{
	UE_LOG(LogTemp, Warning, TEXT("Reload"));

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

FVector AIFGunBase::GetWeaponSocket()
{
	return Mesh->GetSocketTransform("LeftHandSocket").GetLocation();
}

bool AIFGunBase::GunTrace(FHitResult& Hit, FVector& ShotDirection)
{
	if (OwnerController == nullptr) return false;

	FVector OwnerLocation;
	FRotator OwnerRotation;

	OwnerController->GetPlayerViewPoint(OwnerLocation, OwnerRotation);
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
	OwnerController->GetPlayerViewPoint(OwnerLocation, OwnerRotation);
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
		GetWorld()->SpawnActor<AActor>(TracerEffect, SpawnLocation, Rotation);
	}

}




