
#include "Character/IFCharacterBoss.h"
#include "AI/Boss/IFBossAIController.h"
#include "Stat/IFStatComponent.h"
#include "Components/CapsuleComponent.h"
#include "Item/IFTentalce.h"
#include "Stat/IFStatComponent.h"
#include "Item/IFGunBase.h"
#include "Game/IFGameMode.h"
#include "IFCharacterPlayer.h"
#include "Animation/IFNonPlayerAnimInstance.h"

AIFCharacterBoss::AIFCharacterBoss()
{
	AIControllerClass = AIFBossAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	static ConstructorHelpers::FClassFinder<AIFCharacterNonPlayer> BoomerClassRef(TEXT("/Game/Assets/Blueprint/BP_MiniBoomer.BP_MiniBoomer_C"));
	if (BoomerClassRef.Class)
	{
		BoomerClass = BoomerClassRef.Class;
	}
}

void AIFCharacterBoss::BeginPlay()
{
	Super::BeginPlay();

	BossAIController = Cast<AIFBossAIController>(GetController());
	StatComp->OnHpZero.AddUObject(this, &AIFCharacterBoss::SetDead);
	StatComp->bIsNPC = true;

	GameMode = Cast<AIFGameMode>(GetWorld()->GetAuthGameMode());
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
	}
}


void AIFCharacterBoss::SetNPCType(ENPCType NpcName, FName NpcTier)
{
	Super::SetNPCType(NpcName, NpcTier);

	SetTentacleActor();

	BossBreathGun = GetWorld()->SpawnActor<AIFGunBase>(BreathGunClass);
	BossBreathGun->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("breathsocket"));
	BossBreathGun->SetOwner(this);
	BossBreathGun->CachingOwner();
	BossBreathGun.Get()->ShootDelegate.Unbind();
	BossBreathGun.Get()->ShootDelegate.BindLambda([&](TSubclassOf<ULegacyCameraShake> CameraShake) {
		GameMode.Get()->PlayCameraShake(CameraShake);
		});


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
		TentacleArray[i].Get()->OnTentacleDestory.BindUObject(this, &AIFCharacterBoss::CheckTentacle);
	}
}

void AIFCharacterBoss::ReleaseGrabTentacle()
{
	TentacleArray[CurTentacleIdx]->ReleaseTentacle(); 
} 


void AIFCharacterBoss::AttackHitCheck()
{
	switch (CurBossPattern)
	{
	case EBossPattern::Range:
		ProjectileWeapon.Get()->ShootDelegate.Unbind();
		ProjectileWeapon.Get()->ShootDelegate.BindLambda([&](TSubclassOf<ULegacyCameraShake> CameraShake) {
			GameMode.Get()->PlayCameraShake(CameraShake);
		});

		ProjectileWeapon->StartFire(TargetActor.Get()->GetActorLocation());
		break;

	case EBossPattern::Breath:
		BossBreathGun.Get()->StartFire(TargetActor.Get()->GetActorLocation());
		break;
	}
}

void AIFCharacterBoss::CheckTentacle()
{
	TentacleCount++;

	if (TentacleCount >= 2)
	{
		BossAIController.Get()->PerformNextPhase();
	}
}

void AIFCharacterBoss::PerformRangeAttack()
{
	AnimInstance.Get()->SetCurSound(AttackSound);
	AnimInstance->PlayAttackAnimation(StatComp->GetBaseStat().AttackSpeed);
	CurBossPattern = EBossPattern::Range;
}

void AIFCharacterBoss::PeformBreathAttack()
{
	AnimInstance.Get()->SetCurSound(AttackSound);
	AnimInstance.Get()->PlayBreathAttackAnimation();
	CurBossPattern = EBossPattern::Breath;
}

void AIFCharacterBoss::PerformSpawnBoomer()
{
	FVector SpawnVector = GetActorLocation();
	SpawnVector.Z += 100.f;

	GetWorld()->SpawnActor<AIFCharacterNonPlayer>(BoomerClass, SpawnVector, GetActorRotation());
}

void AIFCharacterBoss::PerformGrabAttack()
{
	Cast<AIFCharacterPlayer>(TargetActor)->RegistGrabDelegate.BindUObject(this, &AIFCharacterBoss::ReleaseGrabTentacle);

	for (int i = 0; i < TentacleArray.Num(); i++)
	{
		if (!TentacleArray[i]->GetIsDestroy())
		{
			TentacleArray[i]->ActiveTentacle(Cast<ACharacter>(TargetActor));
			CurTentacleIdx = i;
			return;
		}
	}
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
