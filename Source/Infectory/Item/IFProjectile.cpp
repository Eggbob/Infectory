// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/IFProjectile.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"

AIFProjectile::AIFProjectile()
{
	PrimaryActorTick.bCanEverTick = true;
	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComp"));
	SetRootComponent(StaticMeshComp);

	CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CAPSULECOMP"));
	CapsuleComp->SetupAttachment(StaticMeshComp);
	CapsuleComp->SetCapsuleHalfHeight(34.f);
	CapsuleComp->SetCapsuleRadius(10.f);
	CapsuleComp->SetGenerateOverlapEvents(true);
}

void AIFProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!bIsCollisioned)
	{
		Super::Tick(DeltaTime);
		SetActorLocationAndRotation((GetActorLocation() + GetActorForwardVector() * (MoveSpeed * DeltaTime)), GetActorRotation());
		checkTime += DeltaTime;
	}

	if (checkTime >= ReturnTime || bIsCollisioned)
	{
		DeInit();
	}
}

void AIFProjectile::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	UE_LOG(LogTemp, Warning, TEXT("NotifyActorBeginOverlap"));

	if (OnAttack.IsBound() && OtherActor != nullptr && !bIsCollisioned)
	{
		OnAttack.Execute(OtherActor);
		bIsCollisioned = true;
	}

	GetWorld()->DestroyActor(this);
}

void AIFProjectile::Init()
{
	SetActorEnableCollision(true);
	SetActorTickEnabled(true);
	checkTime = 0.f;
	bIsDeInit = false;
	bIsCollisioned = false;

	if (CapsuleComp == nullptr)
	{
		CapsuleComp = GetComponentByClass<UCapsuleComponent>();
	}
}

void AIFProjectile::DeInit()
{
}

