// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/IFTurret.h"
#include "Components/StaticMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Character/IFCharacterNonPlayer.h"

AIFTurret::AIFTurret()
{
	PrimaryActorTick.bCanEverTick = true;

	CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CAPSULECOMP"));
	SetRootComponent(CapsuleComp);
	
	TurretParentMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TurretParentMesh"));
	TurretParentMesh->SetupAttachment(CapsuleComp);

	TurretHeadMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TurretHeadMesh"));
	TurretHeadMesh->SetupAttachment(TurretParentMesh);
}



void AIFTurret::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (Target)
	{
		AttackTarget();
	}
	else
	{
		SearchTarget();
	}

}

void AIFTurret::SearchTarget()
{
	TArray<FOverlapResult> OverlapResults;

	//FCollisionQueryParams CollisionQueryParam(SCENE_QUERY_STAT(Detect), false, this);
	//bool bResult = GetWorld()->OverlapMultiByChannel(
	//	OverlapResults,
	//	GetActorLocation(),
	//	FQuat::Identity,
	//	ECollisionChannel::ECC_GameTraceChannel4,
	//	FCollisionShape::MakeSphere(300.f),
	//	CollisionQueryParam
	//);

	FCollisionQueryParams CollisionQueryParam(SCENE_QUERY_STAT(Detect), false, this);
	bool bResult = GetWorld()->OverlapMultiByChannel(
		OverlapResults,
		GetActorLocation(),
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel1,
		FCollisionShape::MakeSphere(300.f),
		CollisionQueryParam
	);


	if (bResult)
	{
		for (auto const& OverlapResult : OverlapResults)
		{
			TObjectPtr<APawn> Actor = Cast<APawn>(OverlapResult.GetActor());

			if (Actor && Actor.Get()->GetController()->IsPlayerController())
			{
				UE_LOG(LogTemp, Warning, TEXT("Turret Find Target"));
				Target = Actor;

				DrawDebugSphere(GetWorld(), GetActorLocation(), 300.f, 16, FColor::Green, false, 0.2f);
				DrawDebugPoint(GetWorld(), Actor.Get()->GetActorLocation(), 10.0f, FColor::Green, false, 0.2f);
				DrawDebugLine(GetWorld(), GetActorLocation(), Actor->GetActorLocation(), FColor::Green, false, 0.27f);
			}
		}
	}
}

void AIFTurret::AttackTarget()
{
	FRotator LookRot = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), Target->GetActorLocation());
	LookRot.Pitch = 0.f;
	LookRot.Yaw -= 60.f;

	TurretHeadMesh.Get()->SetWorldRotation(LookRot);

}


