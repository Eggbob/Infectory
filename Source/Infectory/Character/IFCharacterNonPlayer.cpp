// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/IFCharacterNonPlayer.h"
#include "Stat/IFStatComponent.h"
#include "Components/CapsuleComponent.h"
#include "AI/IFAIController.h"
#include "Data/IFEnumDefine.h"
#include "Engine/DamageEvents.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AI/IFAI.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Item/IFGunBase.h"
#include "Item/IFShield.h"
#include "Animation/IFNonPlayerAnimInstance.h"
#include "Kismet/GameplayStatics.h"

AIFCharacterNonPlayer::AIFCharacterNonPlayer()
{
#pragma region MeshSet
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> TestMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/Assets/Characters/Test/Parasite_L_Starkie.Parasite_L_Starkie'"));
	if (TestMeshRef.Object)
	{
		GetMesh()->SetSkeletalMesh(TestMeshRef.Object);
		NPCSkeletalMeshes.Add(ENPCType::Test, TestMeshRef.Object);
	}

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> ParasiteMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/ParasiteZombieBundle01/ParasiteZombie02/ParasiteZombie02_Character/SK_ParasiteZombie02.SK_ParasiteZombie02'"));
	if (ParasiteMeshRef.Object)
	{
		//GetMesh()->SetSkeletalMesh(ParasiteMeshRef.Object);
		NPCSkeletalMeshes.Add(ENPCType::Parasite, ParasiteMeshRef.Object);
		NPCSkeletalMeshes.Add(ENPCType::RangedParasite, ParasiteMeshRef.Object);
	}

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> RangedParasiteMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/ParasiteZombieBundle01/ParasiteZombie02/ParasiteZombie02_Character/SK_ParasiteZombie02.SK_ParasiteZombie02'"));
	if (RangedParasiteMeshRef.Object)
	{
		NPCSkeletalMeshes.Add(ENPCType::RangedParasite, RangedParasiteMeshRef.Object);
	}

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> HunterMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/ParasiteZombieBundle01/ParasiteZombie01/ParasiteZombie01_Character/SK_ParasiteZombie01.SK_ParasiteZombie01'"));
	if (HunterMeshRef.Object)
	{
		NPCSkeletalMeshes.Add(ENPCType::Hunter, HunterMeshRef.Object);
		NPCSkeletalMeshes.Add(ENPCType::Boss, HunterMeshRef.Object);
	}

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> HunterMiniMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/ParasiteZombieBundle01/ParasiteZombie01/ParasiteZombie01_Character/SK_HunterMini.SK_HunterMini'"));
	if (HunterMiniMeshRef.Object)
	{
		NPCSkeletalMeshes.Add(ENPCType::MiniHunter, HunterMiniMeshRef.Object);
	}

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> BoomerMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/ParasiteZombieBundle01/ParasiteZombie03/ParasiteZombie03_Character/SK_ParasiteZombie03.SK_ParasiteZombie03'"));
	if (BoomerMeshRef.Object)
	{
		NPCSkeletalMeshes.Add(ENPCType::Boomer, BoomerMeshRef.Object);
		NPCSkeletalMeshes.Add(ENPCType::MiniBoomer, BoomerMeshRef.Object);
	}

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> BigBoomerMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/ParasiteZombieBundle01/ParasiteZombie03/ParasiteZombie03_Character/SM_BoomerShield.SM_BoomerShield'"));
	if (BigBoomerMeshRef.Object)
	{
		NPCSkeletalMeshes.Add(ENPCType::BigBoomer, BigBoomerMeshRef.Object);
	}
	

#pragma endregion

#pragma region AnimationSet

	static ConstructorHelpers::FClassFinder<UAnimInstance> TestAnimInstanceClassRef(TEXT("/Game/Assets/Animation/Enemy/ABP_TestEnemy.ABP_TestEnemy_C"));
	if (TestAnimInstanceClassRef.Class)
	{
		NPCAnimInstances.Add(ENPCType::Test, TestAnimInstanceClassRef.Class);
	}

	static ConstructorHelpers::FClassFinder<UAnimInstance> ParasiteAnimInstanceClassRef(TEXT("/Game/Assets/Animation/Enemy/ABP_Parasite.ABP_Parasite_C"));
	if (ParasiteAnimInstanceClassRef.Class)
	{
		NPCAnimInstances.Add(ENPCType::Parasite, ParasiteAnimInstanceClassRef.Class);
	}

	static ConstructorHelpers::FClassFinder<UAnimInstance> RangedParasiteAnimInstanceClassRef(TEXT("/Game/Assets/Animation/Enemy/ABP_ParasiteRanged.ABP_ParasiteRanged_C"));
	if (RangedParasiteAnimInstanceClassRef.Class)
	{
		NPCAnimInstances.Add(ENPCType::RangedParasite, RangedParasiteAnimInstanceClassRef.Class);
	}

	static ConstructorHelpers::FClassFinder<UAnimInstance> HunterAnimInstanceClassRef(TEXT("/Game/Assets/Animation/Enemy/ABP_Hunter.ABP_Hunter_C"));
	if (HunterAnimInstanceClassRef.Class)
	{
		NPCAnimInstances.Add(ENPCType::Hunter, HunterAnimInstanceClassRef.Class);
	}

	static ConstructorHelpers::FClassFinder<UAnimInstance> MiniHunterAnimInstanceClassRef(TEXT("/Game/Assets/Animation/Enemy/ABP_HunterMini.ABP_HunterMini_C"));
	if (MiniHunterAnimInstanceClassRef.Class)
	{
		NPCAnimInstances.Add(ENPCType::MiniHunter, MiniHunterAnimInstanceClassRef.Class);
	}

	static ConstructorHelpers::FClassFinder<UAnimInstance> BoomerAnimInstanceClassRef(TEXT("/Game/Assets/Animation/Enemy/ABP_Boomer.ABP_Boomer_C"));
	if (BoomerAnimInstanceClassRef.Class)
	{
		NPCAnimInstances.Add(ENPCType::Boomer, BoomerAnimInstanceClassRef.Class);
	}

	static ConstructorHelpers::FClassFinder<UAnimInstance> BigBoomerAnimInstanceClassRef(TEXT("/Game/Assets/Animation/Enemy/ABP_BigBoomer.ABP_BigBoomer_C"));
	if (BigBoomerAnimInstanceClassRef.Class)
	{
		NPCAnimInstances.Add(ENPCType::BigBoomer, BigBoomerAnimInstanceClassRef.Class);
	}

	static ConstructorHelpers::FClassFinder<UAnimInstance> MiniBoomerAnimInstanceClassRef(TEXT("/Game/Assets/Animation/Enemy/ABP_MiniBoomer.ABP_MiniBoomer_C"));
	if (MiniBoomerAnimInstanceClassRef.Class)
	{
		NPCAnimInstances.Add(ENPCType::MiniBoomer, MiniBoomerAnimInstanceClassRef.Class);
	}

	static ConstructorHelpers::FClassFinder<UAnimInstance> BossAnimInstanceClassRef(TEXT("/Game/Assets/Animation/Enemy/ABP_Boss.ABP_Boss_C"));
	if (BossAnimInstanceClassRef.Class)
	{
		NPCAnimInstances.Add(ENPCType::Boss, BossAnimInstanceClassRef.Class);
	}

#pragma endregion

	AIControllerClass = AIFAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	CurNpcState = ENPCState::Idle;
	CurNpcMoveType = ENPCMoveType::Walking;
}

void AIFCharacterNonPlayer::BeginPlay()
{
	Super::BeginPlay();
	AIController = Cast<AIFAIController>(GetController());
	StatComp->OnHpZero.AddUObject(this, &AIFCharacterNonPlayer::SetDead);
	StatComp->bIsNPC = true;

	if (bIsJustSpawn)
	{
		SetNPCType(CurNPCType, CurNPCTier);
	}
}

void AIFCharacterNonPlayer::SetDead()
{
	if (CurNPCType == ENPCType::MiniBoomer && !bIsExplose)
	{
		if (bJustExplose)
		{
			ExploseCharacter();
		}
		else
		{
			ChangeToBomb();
		}

		return;
	}

	Super::SetDead();
	CurNpcState = ENPCState::Dead;

	AnimInstance.Get()->StopAllMontages(0.f);
	GetCapsuleComponent()->SetCollisionProfileName("NoCollision");
	GetMesh()->SetCollisionProfileName("Ragdoll");
	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), DeadSound, GetActorLocation());

	if (AIController)
	{
		AIController.Get()->StopAI();
	}
}

void AIFCharacterNonPlayer::SetNPCType(ENPCType NpcName, FName NpcTier)
{
	CurNPCType = NpcName;
	CurNPCTier = NpcTier;

	GetMesh()->SetSkeletalMesh(NPCSkeletalMeshes[CurNPCType].Get());
	GetMesh()->SetAnimInstanceClass(NPCAnimInstances[CurNPCType].Get());

	StatComp.Get()->SetStat(*UIFEnumDefine::GetEnumName(CurNPCType), CurNPCTier);
	GetCharacterMovement()->MaxWalkSpeed = StatComp.Get()->GetBaseStat().MovementSpeed;
	GetMesh()->SetRelativeLocation(StatComp.Get()->GetBaseStat().MeshLocation);
	GetMesh()->SetRelativeScale3D(StatComp.Get()->GetBaseStat().MeshScale);

	FVector MeshExtent = GetMesh()->Bounds.BoxExtent;
	GetCapsuleComponent()->SetCapsuleHalfHeight(MeshExtent.Z);
	GetCapsuleComponent()->SetCapsuleRadius(FMath::Max(MeshExtent.X, MeshExtent.Y));

	switch (CurNPCType)
	{
	case ENPCType::RangedParasite:
	case ENPCType::Boss:
		ProjectileWeapon = GetWorld()->SpawnActor<AIFGunBase>(GunClass);
		ProjectileWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("weaponsocket"));
		ProjectileWeapon->SetOwner(this);
		ProjectileWeapon->CachingOwner();
		//ProjectileWeapon->SetActorRotation(FRotator(0.f, 0.f, 90.f));
		break;

	case ENPCType::BigBoomer:
		ShieldObject = GetWorld()->SpawnActor<AIFShield>(ShieldClass);
		ShieldObject.Get()->SetOwner(this);
		ShieldObject.Get()->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("shieldsocket"));

		ShieldObject.Get()->SetShieldDestoryedDelegate(FOnShieldDestoryed::CreateLambda([&]()
			{
				StopMoving();
				AIController.Get()->GetBlackboardComponent()->SetValueAsBool(BBKEY_ISHIT, true);
				AnimInstance.Get()->PlaySpecialHitAnimation();
			}));
		break;
		
	case ENPCType::MiniBoomer:
		CurNpcMoveType = ENPCMoveType::Crawling;
	}


	const UEnum* EnumPtr = StaticEnum<ENPCBoneName>();
	BodyDamageCheckMap.Empty();

	for (int i = 0; i < EnumPtr->NumEnums()-1; i++)
	{
		int RandVal = FMath::RandRange(2, 5);

		BodyDamageCheckMap.Add((ENPCBoneName)EnumPtr->GetValueByIndex(i), RandVal);
	}

	AnimInstance = Cast<UIFNonPlayerAnimInstance>(GetMesh()->GetAnimInstance());
	if (AnimInstance)
	{
		StatComp.Get()->OnHit.AddUObject(AnimInstance, &UIFNonPlayerAnimInstance::PlayHitAnim);
		AnimInstance->OnAttackEnd.BindUObject(this, &AIFCharacterNonPlayer::NotifyAttackActionEnd);
		AnimInstance->OnBackJump.BindUObject(this, &AIFCharacterNonPlayer::StartBackJump);
		AnimInstance->OnBeforeMoving.BindUObject(this, &AIFCharacterNonPlayer::NotifyBeforeMovingActionEnd);
		AnimInstance.Get()->SetFootSound(FootStepSound);
		AnimInstance->OnHitEnd.BindLambda([&]() { 
			AIController.Get()->GetBlackboardComponent()->SetValueAsBool(BBKEY_ISHIT, false);
		});
		AnimInstance->OnStandUpFinish.BindLambda([&]() {
			CurNpcState = ENPCState::Idle;
			AIController.Get()->GetBlackboardComponent()->SetValueAsBool(BBKEY_ISHIT, false);
		});
	}
	
	InitPhysicsAnimation();
}

/// <summary>
/// AI가 Focus할 대상 지정
/// </summary>
/// <param name="TargetActor"></param>
void AIFCharacterNonPlayer::FocusingTarget(TObjectPtr<AActor> InTargetActor)
{
	TargetActor = InTargetActor;
	AIController->SetTarget(InTargetActor);
	//AIController->SetFocus(TargetActor, EAIFocusPriority::Gameplay);
}

/// <summary>
/// 공격 종료 후 실행할 이벤트 등록
/// </summary>
/// <param name="InOnAttackFinished"></param>
void AIFCharacterNonPlayer::SetAIAttackDelegate(const FAICharacterAttackFinished& InOnAttackFinished)
{
	OnAttackFinished.Unbind();
	OnAttackFinished = InOnAttackFinished;
}

void AIFCharacterNonPlayer::SetAIBackJumpDelegate(const FAICharacterBackJumpFinished& InOnBackJumpFinished)
{
	OnBackJumpFinished.Unbind();
	OnBackJumpFinished = InOnBackJumpFinished;
}

void AIFCharacterNonPlayer::SetAIBeforeMovingDelegate(const FAICharacterBeforeMovingFinished& InOnBeforeMovingFinished)
{
	OnBeforeMovingFinished.Unbind();
	OnBeforeMovingFinished = InOnBeforeMovingFinished;
}

void AIFCharacterNonPlayer::SetAIWaitingDelegate(const FAICharacterWaitingFinished& InOnWaitingFinished)
{
	OnWaitingFinished.Unbind();
	OnWaitingFinished = InOnWaitingFinished;

}

/// <summary>
/// 공격 시작
/// </summary>
void AIFCharacterNonPlayer::AttackByAI()
{
	CurNpcState = ENPCState::Attacking;
	StopMoving();
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
	AnimInstance.Get()->SetCurSound(AttackSound);
	AnimInstance->PlayAttackAnimation(StatComp->GetBaseStat().AttackSpeed);
}

/// <summary>
/// 공격 종료후 delegate 실행
/// </summary>
void AIFCharacterNonPlayer::NotifyAttackActionEnd()
{
	CurNpcState = ENPCState::Idle;
	OnAttackFinished.ExecuteIfBound();
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
}

void AIFCharacterNonPlayer::PerformMoving()
{
	if (CurNpcState != ENPCState::Dead)
	{
		AIController->MoveToTarget(GetAIAttackRange());
	}
}

void AIFCharacterNonPlayer::StopMoving()
{
	AIController->StopMovement();
	//UE_LOG(LogTemp, Warning, TEXT("StopMoving"));
}

void AIFCharacterNonPlayer::PerformWaiting(bool bIsFirstContact)
{
	CurNpcState = ENPCState::Idle;
	
	float WaitTime = bIsFirstContact ? StatComp->GetBaseStat().EncounterTime : StatComp->GetBaseStat().WaitTime;

	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateLambda([&]() 
		{
			OnWaitingFinished.ExecuteIfBound();
		}), WaitTime, false);

	AnimInstance->PlayRandomIdleAnimaiton();

	if (bIsFirstContact)
	{
		AIController.Get()->GetBlackboardComponent()->SetValueAsBool(BBKEY_ISFIRSTCONTACT, false);
	}
}

/// <summary>
/// 플레이어 근처에서 바로 터질때
/// </summary>
void AIFCharacterNonPlayer::ReadyToExplosion()
{
	if(CurNpcState == ENPCState::BeforeDead || CurNpcState == ENPCState::Dead) { return; }
	
	CurNpcState = ENPCState::Dead;
	StopMoving();
	AIController.Get()->StopAI();
	GlowParam = 0;
	bIsExplose = false;
	bIsWaitTime = false;

	AnimInstance.Get()->PlayRandomIdleAnimaiton(2);

	GetWorld()->GetTimerManager().SetTimer(GlowTimerHandle, FTimerDelegate::CreateLambda([&]()
		{
			GlowParam += 3;
			PlayGlowEffect();
		}), 0.5f, true);

	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateLambda([&]()
		{
			GetWorld()->GetTimerManager().ClearTimer(GlowTimerHandle);
			ExploseCharacter();
		}), 2.0f, false);
}

/// <summary>
/// BackJump 실행
/// </summary>
void AIFCharacterNonPlayer::StartBackJump()
{
	FVector JumpDirection = GetActorForwardVector() * -1.0f * StatComp->GetBaseStat().JumpVelocity;// -300.0f;
	JumpDirection.Z = 400.f;
	LaunchCharacter(JumpDirection, false, false);
}

void AIFCharacterNonPlayer::SetHitWalkSpeed()
{
	FTimerHandle TimerHandle;

	float CurMaxSpeed = GetCharacterMovement()->MaxWalkSpeed;
	GetCharacterMovement()->MaxWalkSpeed = StatComp.Get()->GetBaseStat().MovementSpeed / 2.0f;

	GetWorld()->GetTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateLambda(
		[&]()
		{
			GetCharacterMovement()->MaxWalkSpeed = CurNpcMoveType == ENPCMoveType::Walking ?
				StatComp.Get()->GetBaseStat().MovementSpeed : StatComp.Get()->GetBaseStat().CrawlMovementSpeed;
		}
	), 0.3f, false);
}

/// <summary>
/// 공격으로 인해 사망 후 폭발하게 변경
/// </summary>
void AIFCharacterNonPlayer::ChangeToBomb()
{
	if (CurNpcState != ENPCState::BeforeDead && CurNpcState != ENPCState::Dead)
	{
		bIsWaitTime = true;
		CurNpcState = ENPCState::BeforeDead;
		StopMoving();
		AIController.Get()->StopAI();
		GlowParam = 0;

		FTimerHandle TimerHandle;
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateLambda([&]()
			{
				bIsWaitTime = false;
			}), 2.0f, false);

		return;
	}
	else
	{
		
		ExploseCharacter();
	}

}

void AIFCharacterNonPlayer::NotifyBeforeMovingActionEnd()
{
	CurNpcState = ENPCState::Moving;
	OnBeforeMovingFinished.ExecuteIfBound();
}

float AIFCharacterNonPlayer::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (CurNpcState == ENPCState::Dead) { return Damage; }
	
	SetHitWalkSpeed();
	FocusingTarget(DamageCauser);

	if (const FCustomDamageEvent* CustomDamageEvent = static_cast<const FCustomDamageEvent*>(&DamageEvent))
	{
		switch (CustomDamageEvent->DamageType)
		{
			case EProjectileDamageType::Explosive:
				AIController.Get()->GetBlackboardComponent()->SetValueAsBool(BBKEY_ISHIT, true);
				StopMoving();
				CurNpcState = ENPCState::Lying;
				AnimInstance.Get()->PlayLyingAnimation();
				break;

			case EProjectileDamageType::BossAttack:
				bJustExplose = true;
				break;
		}

		//TODO Fix Enum
		FString BoneNameStr = CustomDamageEvent->BoneName.ToString();
		ENPCBoneName BoneEnum = UIFEnumDefine::GetBoneEum(BoneNameStr);
		BodyDamageCheckMap[BoneEnum] -= 1;

		if (BodyDamageCheckMap[BoneEnum] == 0)
		{
			PlayHitReaction(CustomDamageEvent->BoneName, CustomDamageEvent->HitResult);

			if (CurNpcMoveType != ENPCMoveType::Crawling && CurNpcState != ENPCState::Lying)
			{
				AIController.Get()->GetBlackboardComponent()->SetValueAsBool(BBKEY_ISHIT, true);
				AnimInstance.Get()->PlaySpecialHitAnimation();
			}
		}
	}

	Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);

	return Damage;
}

void AIFCharacterNonPlayer::AttackHitCheck()
{
	if (CurNPCType == ENPCType::RangedParasite)
	{
		ProjectileWeapon->StartFire(TargetActor.Get()->GetActorLocation());
		return;
	}
	else
	{
		MeleeAttackCheck();
		return;
	}
}

void AIFCharacterNonPlayer::MeleeAttackCheck()
{
	FHitResult OutHitResult;
	FCollisionQueryParams Params(SCENE_QUERY_STAT(Attack), false, this);

	const float AttackRange = StatComp->GetBaseStat().AttackRange;
	const float AttackRadius = 50.f;//StatComp->GetRadi;
	const FVector Start = GetActorLocation() + GetActorForwardVector() * GetCapsuleComponent()->GetScaledCapsuleRadius();
	const FVector End = Start + GetActorForwardVector() * AttackRange;

	bool HitDetected = GetWorld()->SweepSingleByChannel(OutHitResult, Start, End, FQuat::Identity, ECollisionChannel::ECC_GameTraceChannel3, FCollisionShape::MakeSphere(AttackRadius), Params);
	if (HitDetected)
	{
		FDamageEvent DamageEvent;
		OutHitResult.GetActor()->TakeDamage(StatComp->GetBaseStat().Attack, DamageEvent, GetController(), this);
	}

#if ENABLE_DRAW_DEBUG

	/*FVector CapsuleOrigin = Start + (End - Start) * 0.5f;
	float CapsuleHalfHeight = AttackRange * 0.5f;
	FColor DrawColor = HitDetected ? FColor::Green : FColor::Red;

	DrawDebugCapsule(GetWorld(), CapsuleOrigin, CapsuleHalfHeight, AttackRadius, FRotationMatrix::MakeFromZ(GetActorForwardVector()).ToQuat(), DrawColor, false, 5.0f);*/

#endif
}

void AIFCharacterNonPlayer::ChangeNPCMoveMode()
{
	CurNpcMoveType = ENPCMoveType::Crawling;
	GetCharacterMovement()->MaxWalkSpeed = StatComp.Get()->GetBaseStat().CrawlMovementSpeed;
	AnimInstance.Get()->StopAllMontages(0.0f);
	AnimInstance.Get()->SetFootSound(CrawlSound);
	
}

void AIFCharacterNonPlayer::ExploseCharacter()
{
	UE_LOG(LogTemp, Warning, TEXT("ExploseCharacter"));
	if(bIsWaitTime) { return; }

	GlowParam = 0;
	bIsExplose = true;
	CurNpcState = ENPCState::Dead;

	TArray<FOverlapResult> OverlapResults;

	FCollisionQueryParams CollisionQueryParam(SCENE_QUERY_STAT(Detect), false, this);
	bool bResult = GetWorld()->OverlapMultiByChannel(
		OverlapResults,
		GetActorLocation(),
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel8,
		FCollisionShape::MakeSphere(400.f),
		CollisionQueryParam
	);

	if (bResult)
	{
		for (auto const& OverlapResult : OverlapResults)
		{
			TObjectPtr<AActor> Actor = Cast<AActor>(OverlapResult.GetActor());

			if (!Actor->IsA<AIFCharacterNonPlayer>())
			{
				FCustomDamageEvent DamageEvent;
				DamageEvent.DamageType = EProjectileDamageType::Explosive;

				Actor.Get()->TakeDamage(StatComp->GetBaseStat().Attack, DamageEvent, GetController(), this);

				//ExcuteAttack(Actor.Get());
				DrawDebugSphere(GetWorld(), GetActorLocation(), 300.f, 16, FColor::Green, false, 0.2f);
				DrawDebugPoint(GetWorld(), Actor.Get()->GetActorLocation(), 10.0f, FColor::Green, false, 0.2f);
				DrawDebugLine(GetWorld(), GetActorLocation(), Actor->GetActorLocation(), FColor::Green, false, 0.27f);
			}
		}
	}

	if (ImpactEffect && ProjectileSound)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactEffect, GetActorLocation());
		UGameplayStatics::SpawnSoundAtLocation(GetWorld(), ProjectileSound, GetActorLocation());
	}

	PlayGlowEffect();
	SetDead();

	DrawDebugSphere(GetWorld(), GetActorLocation(), 300.f, 16, FColor::Red, false, 0.2f);
}

float AIFCharacterNonPlayer::GetAIPatrolRadius()
{
	return 800.0f;
}

float AIFCharacterNonPlayer::GetAIDetectRange()
{
	return StatComp->GetBaseStat().DetectRange; // 400.0f;
}

float AIFCharacterNonPlayer::GetAIAttackRange()
{
	return StatComp->GetBaseStat().AttackRange; //150.0f;
}

float AIFCharacterNonPlayer::GetAITurnSpeed()
{
	return 2.0f;
}


