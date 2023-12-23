// Fill out your copyright notice in the Description page of Project Settings.

#include "Item/IFGunBase.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"

AIFGunBase::AIFGunBase()
{
	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));

	SetRootComponent(RootComp);
	Mesh->SetupAttachment(RootComp);
}

void AIFGunBase::PullTrigger()
{
	UGameplayStatics::SpawnEmitterAttached(MuzzleFlash, Mesh, TEXT("MuzzleFlashSocket"));

	FHitResult Hit;
	FVector ShotDirection;

	bool bSuccess = GunTrace(Hit, ShotDirection);
	if (bSuccess)
	{
		DrawDebugPoint(GetWorld(), Hit.Location, 20, FColor::Red, true);

		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactEffect, Hit.Location, ShotDirection.Rotation());


		AActor* HitActor = Hit.GetActor();
		if (HitActor != nullptr)
		{
	/*		FPointDamageEvent DmgeEvent(Damage, Hit, ShotDirection, nullptr);
			AController* OwnerController = GetOwnerController();
			HitActor->TakeDamage(Damage, DmgeEvent, OwnerController, this);*/
		}
	}
}

void AIFGunBase::CachingOwner()
{
	APawn* OwnerPawn = Cast<APawn>(GetOwner());
	OwnerController = OwnerPawn->GetController();
	ensure(OwnerController);
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




