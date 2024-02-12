// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/IFProjectile.h"
#include "Character/IFCharacterNonPlayer.h"
#include "Kismet/GameplayStatics.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"


AIFProjectile::AIFProjectile()
{
	PrimaryActorTick.bCanEverTick = true;
	
	CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CAPSULECOMP"));
	CapsuleComp->SetCapsuleHalfHeight(34.f);
	CapsuleComp->SetCapsuleRadius(10.f);
	CapsuleComp->SetGenerateOverlapEvents(true);
	SetRootComponent(CapsuleComp);

	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComp"));
	StaticMeshComp->SetupAttachment(CapsuleComp);
}

void AIFProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!bIsCollisioned && !bIsExplosive)
	{
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

	if (bIsExplosive)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactEffect, GetActorLocation());
		UGameplayStatics::SpawnSoundAtLocation(GetWorld(), ProjectileSound, GetActorLocation());
		OnShoot.ExecuteIfBound(CameraShake);
		CheckAttackRange();
	}
	else
	{
		ExcuteAttack(OtherActor);
	}

	bIsCollisioned = true;
	GetWorld()->DestroyActor(this);
}

void AIFProjectile::ExcuteAttack(AActor* OtherActor)
{
	FCustomDamageEvent DamageEvent;

	if (OtherActor != nullptr && !bIsCollisioned)
	{
		if (bIsExplosive)
		{
			DamageEvent.DamageType = EDamageType::Explosive;
			LaunchEnemy(OtherActor);
		}

		OnAttack.ExecuteIfBound(OtherActor, DamageEvent);
	}
}

void AIFProjectile::Init(float Speed)
{
	SetActorEnableCollision(true);
	SetActorTickEnabled(true);
	checkTime = 0.f;
	bIsDeInit = false;
	bIsCollisioned = false;
	MoveSpeed = Speed;

	ProjectileMovementComp = GetComponentByClass<UProjectileMovementComponent>();
	if(ProjectileMovementComp)
	{
		ProjectileMovementComp->InitialSpeed = Speed;
		ProjectileMovementComp->MaxSpeed = Speed;
	}
}

void AIFProjectile::DeInit()
{
}

void AIFProjectile::CheckAttackRange()
{
	TArray<FOverlapResult> OverlapResults;

	FCollisionQueryParams CollisionQueryParam(SCENE_QUERY_STAT(Detect), false, this);
	bool bResult = GetWorld()->OverlapMultiByChannel(
		OverlapResults,
		GetActorLocation(),
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel4,
		FCollisionShape::MakeSphere(300.f),
		CollisionQueryParam
	);

	if (bResult)
	{
		for (auto const& OverlapResult : OverlapResults)
		{
			TObjectPtr<AActor> Actor = Cast<AActor>(OverlapResult.GetActor());

			if (Actor->IsA<AIFCharacterNonPlayer>())
			{
				ExcuteAttack(Actor.Get());
				DrawDebugSphere(GetWorld(), GetActorLocation(), 300.f, 16, FColor::Green, false, 0.2f);
				DrawDebugPoint(GetWorld(), Actor.Get()->GetActorLocation(), 10.0f, FColor::Green, false, 0.2f);
				DrawDebugLine(GetWorld(), GetActorLocation(), Actor->GetActorLocation(), FColor::Green, false, 0.27f);
			}
		}
	}

	DrawDebugSphere(GetWorld(), GetActorLocation(), 300.f, 16, FColor::Red, false, 0.2f);
}

