
#include "Character/IFCharacterBoss.h"
#include "AI/Boss/IFBossAIController.h"
#include "Stat/IFStatComponent.h"
#include "Components/CapsuleComponent.h"
#include "Item/IFTentalce.h"
#include "Stat/IFStatComponent.h"
#include "Item/IFGunBase.h"
#include "Game/IFGameMode.h"
#include "IFCharacterPlayer.h"
#include "Data/IFGameSingleton.h"
#include "Item/IFTumor.h"
#include "Item/IFSpawnEgg.h"
#include "Data/IFBossPatternData.h"
#include "Game/IFObjectPoolManager.h"
#include "Animation/IFNonPlayerAnimInstance.h"

AIFCharacterBoss::AIFCharacterBoss()
{
	AIControllerClass = AIFBossAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	static ConstructorHelpers::FClassFinder<AIFSpawnEgg> EggClassRef(TEXT("/Game/Assets/Blueprint/BP_SpawnEgg.BP_SpawnEgg_C"));
	if (EggClassRef.Class)
	{
		SpawnEggClass = EggClassRef.Class;
	}

	static ConstructorHelpers::FClassFinder<AIFTumor> TumorClassRef(TEXT("/Game/Assets/Blueprint/ETC/BP_Tumor.BP_Tumor_C"));
	if (TumorClassRef.Class)
	{
		TumorClass = TumorClassRef.Class;
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

	for (AIFTentalce* Tentacle : TentacleArray)
	{
		Tentacle->Destroy();
	}

	AnimInstance.Get()->PlayDeadAnim();
	DeathLogic();
}

/// <summary>
/// �˼� ��� ����
/// </summary>
void AIFCharacterBoss::PerformPierceAttack()
{
	for (AIFTentalce* Tentacle : TentacleArray)
	{
		if (!Tentacle->GetIsDestroy())
		{
			Tentacle->TentacleReadyToPierce();
			
			CurBossState = EBossState::Attack;
			return;
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
		TentacleArray[i].Get()->InitTentacle(TentacleLocArray[i]);
		TentacleArray[i].Get()->OnGiveDamage.BindUObject(this, &AIFCharacterBoss::GiveDamage);
		TentacleArray[i].Get()->OnTentacleDestory.BindUObject(this, &AIFCharacterBoss::CheckTentacle);
		TentacleArray[i].Get()->OnGetTargetLocDelegate.BindUObject(this, &AIFCharacterBoss::ReturnTargetLoc);
		//TentacleArray[i].Get()->OnTentacleActiveFinish.BindUObject(this, &AIFCharacterBoss::PerformCoolDown);
		TentacleArray[i].Get()->OnTentacleActiveFinish.BindLambda([&]() {
			PerformCoolDown();
			});
	}

	for (int i = 1; i < 6; i++)
	{
		FTransform SocketTr = GetMesh()->GetSocketTransform(FName("eggsocket" + FString::FromInt(i)), ERelativeTransformSpace::RTS_World);

		TObjectPtr<AIFTumor> Tumor = GetWorld()->SpawnActor<AIFTumor>(TumorClass, SocketTr);
		Tumor->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, FName("eggsocket" + FString::FromInt(i))); 
		Tumor->InitTumor(FMath::RandRange(200, 320));
		//Tumor->InitTumor(100);
		Tumor->OnTumorFinish.BindUObject(this, &AIFCharacterBoss::CheckAcitveTumor);
		TumorArray.Add(Tumor);
	}

	AnimInstance.Get()->OnAttackEnd.BindUObject(this, &AIFCharacterBoss::PerformCoolDown);

	AnimInstance.Get()->OnHitEnd.BindLambda([&]() {
			CurBossState = EBossState::Idle;
			PerformCoolDown();
		});

	CurTumorCount = TumorArray.Num();
	HideLowerBody();
}

/// <summary>
/// ���� ���� ����
/// </summary>
/// <param name="BossPattern"></param>
void AIFCharacterBoss::CheckPattern(EBossPattern BossPattern)
{
	if (CurBossState != EBossState::Idle)
	{
		PatternArray.Add(BossPattern);
		return;
	}
	else
	{
		if (!PatternArray.IsEmpty()) 
		{
			EBossPattern Pattern = PatternArray.Pop();
			PatternArray.Add(BossPattern);

			ExecutePattern(Pattern);
		}
		else
		{
			ExecutePattern(BossPattern);
			
		}
	}
}

void AIFCharacterBoss::FocusingTarget(TObjectPtr<AActor> InTargetActor)
{
	if (TargetActor == nullptr)
	{
		TargetActor = InTargetActor;

		AnimInstance.Get()->PlayInteractAnimation();
		AnimInstance.Get()->OnInteract.BindLambda([&]() {
			GameMode.Get()->PlayCameraShake(InteractCameraShake);
			});

		FTimerHandle TimerHandle;
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateLambda([&]()
			{
				AIController->SetTarget(TargetActor);
			}), 5.0f, false);
	}
}

void AIFCharacterBoss::ReleaseGrabTentacle()
{
	TentacleArray[CurTentacleIdx]->ReleaseTentacle(); 
}

void AIFCharacterBoss::CheckAcitveTumor()
{
	PerformHitAction();
	
	TumorActive();

	if (PrevTumorCount != CurTumorCount)
	{
		BossAIController.Get()->PerformNextPhase();
	}

	if (CurTumorCount == 0)
	{
		SetDead();
	}

	/*if (TumorActive())
	{
		CurTumorCount--;
	}*/
}

bool AIFCharacterBoss::TumorActive()
{
	TArray<AIFTumor *> TumorActiveArray;

	PrevTumorCount = CurTumorCount;
	CurTumorCount = 0;

	for (int i = 0; i < TumorArray.Num(); i++)
	{
		if (TumorArray[i]->bCanActive)
		{
			CurTumorCount++;
			TumorActiveArray.Add(TumorArray[i]);
		}
	}

	

	if (TumorActiveArray.Num() == 0)
	{
		return false;
	}

	int32 interation = 0;
	while (interation < 10)
	{
		int32 Num = FMath::RandRange(0, TumorActiveArray.Num()-1);

		if (!TumorActiveArray[Num]->bIsActivate)
		{
			TumorActiveArray[Num]->ActiveTumor(true);
			break;
		}

		interation++;
	}

	if (interation < 10)
	{
		return true;
	}
	else if(interation == 10)
	{
		for (int i = 0; i < TumorActiveArray.Num(); i++)
		{
			if (!TumorActiveArray[i]->bIsActivate)
			{
				TumorActiveArray[i]->ActiveTumor(true);
				return true;
			}
		}
	}

	return false;

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

/// <summary>
/// ���Ͻ���
/// </summary>
/// <param name="Pattern"></param>
void AIFCharacterBoss::ExecutePattern(EBossPattern Pattern)
{
	CurBossState = EBossState::Attack;

	CurBossPattern = Pattern;
	switch (Pattern)
	{
	case EBossPattern::Pierce:
		PerformPierceAttack();
		break;

	case EBossPattern::SpawnBomb:
	case EBossPattern::SpawnEnemy:
		PerformSpawnEnemy();
		break;

	case EBossPattern::Range:
		PerformRangeAttack();
		break;

	case EBossPattern::Breath:
		PeformBreathAttack();
		break;

	case EBossPattern::Grab:
		PerformGrabAttack();
		break;
	}
}

void AIFCharacterBoss::CheckTentacle()
{
	TentacleCount++;

	if (TentacleCount >= 2)
	{
		BossAIController.Get()->PerformNextPhase();

		for (AIFTumor* Tumor : TumorArray)
		{
			Tumor->ReadyTumor();
		}

		PerformHitAction();
		TumorActive();
	}
}

/// <summary>
/// ���Ÿ� ���ݽ���
/// </summary>
void AIFCharacterBoss::PerformRangeAttack()
{
	AnimInstance.Get()->SetCurSound(AttackSound);
	AnimInstance->PlayAttackAnimation(StatComp->GetBaseStat().AttackSpeed);
}

/// <summary>
/// �극�� ���ݽ���
/// </summary>
void AIFCharacterBoss::PeformBreathAttack()
{
	AnimInstance.Get()->SetCurSound(AttackSound);
	AnimInstance.Get()->PlayBreathAttackAnimation();
}

/// <summary>
/// Enemy ��ȯ
/// </summary>
void AIFCharacterBoss::PerformSpawnEnemy()
{
	AnimInstance.Get()->PlaySpawnEnemyAnimation();

	FVector Loc = GetMesh()->GetSocketLocation(FName("breathsocket"));

	TObjectPtr<AIFSpawnEgg> Egg = Cast<AIFSpawnEgg>(GameMode.Get()->GetPoolManager().Get()->Pop(SpawnEggClass, GetWorld()));

	Egg.Get()->OnFinish.BindLambda([&](AActor* ReturnActor) {
		if (ReturnActor)
		{
			GameMode.Get()->GetPoolManager().Get()->Push(ReturnActor);
		}

	});

	Egg.Get()->SetActorLocation(Loc);

	Egg->LaunchEgg(TargetActor.Get(), 
		UIFGameSingleton::Get().GetBossPatternData(CurBossPattern, CurTumorCount).SpawnEnemyType,
		UIFGameSingleton::Get().GetBossPatternData(CurBossPattern, CurTumorCount).SpawnEnemyTier);

	PerformCoolDown();
}

void AIFCharacterBoss::PerformGrabAttack()
{
	Cast<AIFCharacterPlayer>(TargetActor)->RegistGrabDelegate.BindUObject(this, &AIFCharacterBoss::ReleaseGrabTentacle);

	for (int i = 0; i < TentacleArray.Num(); i++)
	{
		if (!TentacleArray[i]->GetIsDestroy() && TentacleArray[i].Get()->GetCurPattern() == ETentaclePattern::None)
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

/// <summary>
/// ��Ÿ�� ����
/// </summary>
/// <param name="CoolDownPattern"></param>
void AIFCharacterBoss::PerformCoolDown()
{
	float CoolTime = UIFGameSingleton::Get().GetBossPatternData(CurBossPattern, CurTumorCount).PatternDelay;
	BossAIController.Get()->SetCoolTime(CoolTime);

	UE_LOG(LogTemp, Warning, TEXT("CoolDownPattern : %d"), (int32)CurBossPattern);

	CurBossState = EBossState::Idle;
	CurBossPattern = EBossPattern::None;

	BossAIController.Get()->SetFinishAttack(true);
}

void AIFCharacterBoss::PerformHitAction()
{
	CurBossState = EBossState::Hit;
	CurBossPattern = EBossPattern::None;
	AnimInstance.Get()->PlayRandomHitAnimation();
}

/// <summary>
/// Ÿ�� ��ġ ��ȯ
/// </summary>
/// <returns></returns>
FVector AIFCharacterBoss::ReturnTargetLoc()
{
	TWeakObjectPtr<UCapsuleComponent> CapsuleComp = TargetActor.Get()->GetComponentByClass<UCapsuleComponent>();

	if (CapsuleComp.Get())
	{
		FVector BottomLoc = CapsuleComp.Get()->GetComponentLocation();
		float HalfHeight = CapsuleComp.Get()->GetScaledCapsuleHalfHeight();
		BottomLoc.Z -= HalfHeight;
		return BottomLoc;
	}

	return FVector();
}
