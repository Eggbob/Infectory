#include "Item/IFTumor.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/SphereComponent.h"

AIFTumor::AIFTumor()
{
	SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("SPHERECOMP"));
	SetRootComponent(SphereComp);

	SkeletalMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("TentacleMesh"));
	SkeletalMeshComp->SetupAttachment(SphereComp);
}

void AIFTumor::InitTumor(int32 InHP)
{
	TumorHP = InHP;
	TumorCurHP = TumorHP;
	GlowParam = 0;
	DestroyCount = 0;
	bCanActive = true;
	bIsActivate = false;
}


float AIFTumor::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (!bIsActivate)
	{
		return 0;
	}

	Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);

	TumorCurHP -= Damage;

	if (TumorCurHP <= 0)
	{
		OnTumorFinish.ExecuteIfBound();
		DeActiveTumor();
	}

	return Damage;
}




