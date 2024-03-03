
#include "Character/IFCharacterBoss.h"
#include "AI/Boss/IFBossAIController.h"
#include "Stat/IFStatComponent.h"
#include "Components/CapsuleComponent.h"
#include "Item/IFTentalce.h"
#include "Data/IFEnumDefine.h"
#include "Stat/IFStatComponent.h"

AIFCharacterBoss::AIFCharacterBoss()
{
	AIControllerClass = AIFBossAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

void AIFCharacterBoss::BeginPlay()
{
	Super::BeginPlay();

	AIController = Cast<AIFBossAIController>(GetController());
	StatComp->OnHpZero.AddUObject(this, &AIFCharacterBoss::SetDead);
	StatComp->bIsNPC = true;
}

void AIFCharacterBoss::SetDead()
{
	Super::SetDead();
}

void AIFCharacterBoss::PerformPierceAttack()
{
	TWeakObjectPtr<UCapsuleComponent> CapsuleComp = TargetActor.Get()->GetComponentByClass<UCapsuleComponent>();
	
	if (CapsuleComp.Get())
	{
		FVector BottomLoc = CapsuleComp.Get()->GetComponentLocation();
		float HalfHeight = CapsuleComp.Get()->GetScaledCapsuleHalfHeight();
		BottomLoc.Z -= HalfHeight;

		TentalceArray[0].Get()->PierceAttack(BottomLoc);
	}
}

void AIFCharacterBoss::SetNPCType(ENPCType NpcName, FName NpcTier)
{
	Super::SetNPCType(NpcName, NpcTier);

	SetTentacleActor();

	for (int i = 0; i < BossMaterialArray.Num(); i++)
	{
		GetMesh()->SetMaterial(i, BossMaterialArray[i]);
	}

	for (int i = 0; i < 5; i++)
	{
		TentacleLocArray.Add(GetMesh()->GetSocketLocation(FName("Tentacle" + FString::FromInt(i))));
	}

	for (int i = 0; i < 5; i++)
	{
		TentalceArray[i].Get()->SetActorLocation(TentacleLocArray[i]);

		TentalceArray[i].Get()->OnGiveDamage.BindUObject(this, &AIFCharacterBoss::GiveDamage);
	}

	//Tentalce.Get()->SetActorLocation(TentacleLocArray[i]);
}

void AIFCharacterBoss::GiveDamage(TObjectPtr<AActor> Target)
{
	if (Target)
	{
		FDamageEvent DamageEvent;
		Target->TakeDamage(StatComp->GetBaseStat().Attack, DamageEvent, GetController(), this);
	}
}
