// Fill out your copyright notice in the Description page of Project Settings.

#include "Item/IFGunBase.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/DamageEvents.h"
#include "DrawDebugHelpers.h"
#include "NiagaraComponent.h"

AIFGunBase::AIFGunBase()
{
	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	NiagaraComp = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Niagara"));

	SetRootComponent(RootComp);
	Mesh->SetupAttachment(RootComp);
}

void AIFGunBase::Fire()
{
	UGameplayStatics::SpawnEmitterAttached(MuzzleFlash, Mesh, TEXT("MuzzleFlashSocket"));

	FHitResult Hit;
	FVector ShotDirection;

	bool bSuccess = GunTrace(Hit, ShotDirection);
	if (bSuccess)
	{
		DrawDebugPoint(GetWorld(), Hit.Location, 20, FColor::Red, true);
		AActor* HitActor = Hit.GetActor();

		if (HitActor != nullptr)
		{
			FDamageEvent DamageEvent;
			HitActor->TakeDamage(10.f, DamageEvent, OwnerController, GetOwner());
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), BloodImpactEffect, Hit.Location, ShotDirection.Rotation());
			/*FPointDamageEvent DmgeEvent(Damage, Hit, ShotDirection, nullptr);
			AController* OwnerController = GetOwnerController();
			HitActor->TakeDamage(Damage, DmgeEvent, OwnerController, this);*/
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

void AIFGunBase::CachingOwner()
{
	APawn* OwnerPawn = Cast<APawn>(GetOwner());
	OwnerController = OwnerPawn->GetController();
	ensure(OwnerController);
}

void AIFGunBase::StartFire()
{
	Fire();

	if (IsAuto)
	{
		GetWorldTimerManager().SetTimer(FireTimerHandle, this, &AIFGunBase::Fire, FireDelayTime, true);
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

	DrawDebugLine(GetWorld(), OwnerLocation, End, FColor::Red, true);

	return GetWorld()->LineTraceSingleByChannel(Hit, OwnerLocation, End, ECollisionChannel::ECC_GameTraceChannel1, Params);;
}




