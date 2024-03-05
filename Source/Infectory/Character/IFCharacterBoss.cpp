
#include "Character/IFCharacterBoss.h"
#include "AI/Boss/IFBossAIController.h"
#include "Stat/IFStatComponent.h"
#include "Components/CapsuleComponent.h"
#include "Item/IFTentalce.h"
#include "Data/IFEnumDefine.h"
#include "Stat/IFStatComponent.h"
#include "Item/IFGunBase.h"
#include "Animation/IFNonPlayerAnimInstance.h"

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

		for (AIFTentalce* Tentacle : TentacleArray)
		{
			if (!Tentacle->GetIsDestroy())
			{
				Tentacle->PierceAttack(BottomLoc);
				return;
			}
		}

		//TentacleArray[0].Get()->PierceAttack(BottomLoc);
	}
}

void AIFCharacterBoss::AttackByAI()
{
	AnimInstance.Get()->SetCurSound(AttackSound);
	AnimInstance->PlayAttackAnimation(StatComp->GetBaseStat().AttackSpeed);
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
		TentacleArray[i].Get()->SetActorLocation(TentacleLocArray[i]);

		TentacleArray[i].Get()->OnGiveDamage.BindUObject(this, &AIFCharacterBoss::GiveDamage);
	}

	//Tentalce.Get()->SetActorLocation(TentacleLocArray[i]);
}

void AIFCharacterBoss::AttackHitCheck()
{
	ProjectileWeapon->StartFire(TargetActor.Get()->GetActorLocation());
}

void AIFCharacterBoss::PerformRangeAttack()
{
	AnimInstance.Get()->SetCurSound(AttackSound);
	AnimInstance->PlayAttackAnimation(StatComp->GetBaseStat().AttackSpeed);

	//ProjectileWeapon->StartFire(TargetActor.Get()->GetActorLocation());
}

void AIFCharacterBoss::GiveDamage(TObjectPtr<AActor> Target)
{
	if (Target)
	{
		FCustomDamageEvent CustomDamageEvent;

		CustomDamageEvent.BoneName = "spine_01";
		CustomDamageEvent.DamageType = EProjectileDamageType::BossAttack;

		Target->TakeDamage(StatComp->GetBaseStat().Attack, CustomDamageEvent, GetController(), this);
	}
}
