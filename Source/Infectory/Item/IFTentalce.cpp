

#include "Item/IFTentalce.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Data/IFEnumDefine.h"

AIFTentalce::AIFTentalce()
{
	PrimaryActorTick.bCanEverTick = false;

	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(RootComp);

	CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CAPSULECOMP"));
	CapsuleComp->SetupAttachment(RootComp);

	SkeletalMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("TentacleMesh"));
	SkeletalMeshComp->SetupAttachment(RootComp);

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> TentacleMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/Assets/Art/Tentacle/SM_Tentacle.SM_Tentacle'"));
	if (TentacleMeshRef.Succeeded())
	{
		SkeletalMeshComp->SetSkeletalMesh(TentacleMeshRef.Object);
	}
}

void AIFTentalce::BeginPlay()
{
	Super::BeginPlay();
	
	DangerCircle = GetComponentByClass<UStaticMeshComponent>();
}

void AIFTentalce::ResetTentacle()
{
	CurPattern = ETentaclePattern::None;
	SetActorLocationAndRotation(TentacleBasicLoc, FRotator::ZeroRotator);
	OnTentacleActiveFinish.ExecuteIfBound();
}

void AIFTentalce::GiveDamage(AActor* Target)
{
	if (CurPattern == ETentaclePattern::Pierce)
	{
		OnGiveDamage.ExecuteIfBound(Target);
	}
}

/// <summary>
/// 찌르기 공격실행
/// </summary>
void AIFTentalce::PierceAttack()
{
	FVector TargetLocRef = OnGetTargetLocDelegate.Execute();
	
	CurPattern = ETentaclePattern::Pierce;
	SetActorLocation(TargetLocRef - FVector(0, 0, CapsuleComp.Get()->GetUnscaledCapsuleHalfHeight() * 3));

	DangerCircle.Get()->SetWorldLocation(TargetLocRef);
	PlayPierceing();
}

void AIFTentalce::InitTentacle(FVector TentacleLoc)
{
	TentacleBasicLoc = TentacleLoc;
}

float AIFTentalce::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);

	if (const FCustomDamageEvent* CustomDamageEvent = static_cast<const FCustomDamageEvent*>(&DamageEvent))
	{
		if (CustomDamageEvent->DamageType == EProjectileDamageType::Explosive)
		{
			TentacleDestroy();
			bIsDestroyed = true;
			OnTentacleDestory.ExecuteIfBound();
		}
		else
		{
			return Damage;
		}
	}

	return Damage;
}


