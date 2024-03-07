

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
	SkeletalMeshComp->SetupAttachment(CapsuleComp);

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

void AIFTentalce::PierceAttack(FVector TargetLoc)
{
	SetActorLocation(TargetLoc);
	DangerCircle.Get()->SetWorldLocation(TargetLoc);

	TargetLoc.Z -= CapsuleComp.Get()->GetScaledCapsuleHalfHeight() * 2;
	CapsuleComp.Get()->SetWorldLocation(TargetLoc);
	PlayPierceing();
}

void AIFTentalce::InitTentacle()
{

}

float AIFTentalce::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);

	if (const FCustomDamageEvent* CustomDamageEvent = static_cast<const FCustomDamageEvent*>(&DamageEvent))
	{
		if (CustomDamageEvent->DamageType == EProjectileDamageType::Explosive)
		{
			UE_LOG(LogTemp, Warning, TEXT("Tentacle TakeDamage"));
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

void AIFTentalce::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	OnGiveDamage.ExecuteIfBound(OtherActor);
}



