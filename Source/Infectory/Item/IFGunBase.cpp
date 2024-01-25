// Fill out your copyright notice in the Description page of Project Settings.

#include "Item/IFGunBase.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/DamageEvents.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/Character.h"
#include "NiagaraComponent.h"
#include "Item/IFProjectile.h"

AIFGunBase::AIFGunBase()
{
	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	NiagaraComp = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Niagara"));

	SetRootComponent(RootComp);
	Mesh->SetupAttachment(RootComp);
}

void AIFGunBase::FireLineTrace()
{
	UGameplayStatics::SpawnEmitterAttached(MuzzleFlash, Mesh, TEXT("MuzzleFlashSocket"));

	FHitResult Hit;
	FVector ShotDirection;

	bool bSuccess = GunTrace(Hit, ShotDirection);
	if (bSuccess)
	{
		AActor* HitActor = Hit.GetActor();
		
		if (HitActor != nullptr && HitActor->IsA(ACharacter::StaticClass()))
		{
			//UE_LOG(LogTemp, Warning, TEXT("HitActor : %s"), *Hit.BoneName.ToString());

			FCustomDamageEvent CustomDamageEvent;

			CustomDamageEvent.BoneName = Hit.BoneName;
			CustomDamageEvent.HitResult = Hit;

			GiveDamage(HitActor, CustomDamageEvent);
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), BloodImpactEffect, Hit.Location, ShotDirection.Rotation());
		}
		else
		{
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactEffect, Hit.Location, ShotDirection.Rotation());
		}

	}

	BPFire(Hit.ImpactPoint);

	if (FireGunDelegate.IsBound())
	{
		FireGunDelegate.Execute();
	}
}

void AIFGunBase::GiveDamage(TObjectPtr<AActor> HitActor, FCustomDamageEvent& Hit)
{
	HitActor->TakeDamage(Damage, Hit, OwnerController, GetOwner());
}


void AIFGunBase::FireProjectile()
{
	FVector PlusVector = GetActorForwardVector() * 100.f;
	PlusVector.Z = 0.f;
	FVector SpawnLocation = Mesh->GetSocketTransform("MuzzleFlashSocket").GetLocation() + PlusVector;

	TObjectPtr<AIFProjectile> Projectile = GetWorld()->SpawnActor<AIFProjectile>(ProjectileoBP, SpawnLocation, Owner.Get()->GetActorRotation());
	Projectile.Get()->Init();
	Projectile.Get()->OnAttack.BindLambda([&](TObjectPtr<AActor> HitActor){
		FCustomDamageEvent CustomDamageEvent;
		GiveDamage(HitActor, CustomDamageEvent);
	}); 
}

void AIFGunBase::CachingOwner()
{
	APawn* OwnerPawn = Cast<APawn>(GetOwner());
	OwnerController = OwnerPawn->GetController();
	ensure(OwnerController);
}

void AIFGunBase::StartFire()
{
	switch (WeaponType)
	{
	case ERangedWeaponType::LineTrace:
		FireLineTrace();

		if (IsAuto)
		{
			GetWorldTimerManager().SetTimer(FireTimerHandle, this, &AIFGunBase::FireLineTrace, FireDelayTime, true);
		}
		break;
	case ERangedWeaponType::Projectile:
		FireProjectile();
		break;
	}
}

void AIFGunBase::StopFire()
{
	GetWorldTimerManager().ClearTimer(FireTimerHandle);
	FireGunDelegate.Unbind();
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

	FVector End = OwnerLocation + OwnerRotation.Vector() * MaxRange;

	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	Params.AddIgnoredActor(GetOwner());

	//DrawDebugLine(GetWorld(), OwnerLocation, End, FColor::Red, true);

	return GetWorld()->LineTraceSingleByChannel(Hit, OwnerLocation, End, ECollisionChannel::ECC_GameTraceChannel1, Params);
}




