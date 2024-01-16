// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/IFCharacterNonPlayer.h"
#include "Stat/IFStatComponent.h"
#include "Components/CapsuleComponent.h"
#include "AI/IFAIController.h"
#include "Data/IFEnumDefine.h"
#include "Engine/DamageEvents.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Animation/IFNonPlayerAnimInstance.h"


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
	}

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> HunterMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/ParasiteZombieBundle01/ParasiteZombie01/ParasiteZombie01_Character/SK_ParasiteZombie01.SK_ParasiteZombie01'"));
	if (HunterMeshRef.Object)
	{
		NPCSkeletalMeshes.Add(ENPCType::Hunter, HunterMeshRef.Object);
	}

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> BoomerMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/ParasiteZombieBundle01/ParasiteZombie03/ParasiteZombie03_Character/SK_ParasiteZombie03.SK_ParasiteZombie03'"));
	if (BoomerMeshRef.Object)
	{
		NPCSkeletalMeshes.Add(ENPCType::Boomer, BoomerMeshRef.Object);
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

	static ConstructorHelpers::FClassFinder<UAnimInstance> HunterAnimInstanceClassRef(TEXT("/Game/Assets/Animation/Enemy/ABP_Hunter.ABP_Hunter_C"));
	if (HunterAnimInstanceClassRef.Class)
	{
		NPCAnimInstances.Add(ENPCType::Hunter, HunterAnimInstanceClassRef.Class);
	}

	static ConstructorHelpers::FClassFinder<UAnimInstance> BoomerAnimInstanceClassRef(TEXT("/Game/Assets/Animation/Enemy/ABP_Boomer.ABP_Boomer_C"));
	if (BoomerAnimInstanceClassRef.Class)
	{
		NPCAnimInstances.Add(ENPCType::Boomer, BoomerAnimInstanceClassRef.Class);
	}


#pragma endregion
	AIControllerClass = AIFAIController::StaticClass();
	
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	CurNpcState = ENPCState::Idle;
}

void AIFCharacterNonPlayer::BeginPlay()
{
	Super::BeginPlay();
	AIController = Cast<AIFAIController>(GetController());
	StatComp->OnHpZero.AddUObject(this, &AIFCharacterNonPlayer::SetDead);

	SetNPCType(CurNPCType, CurNPCTier);
}

void AIFCharacterNonPlayer::SetDead()
{
	Super::SetDead();
	CurNpcState = ENPCState::Dead;

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

	//UE_LOG(LogTemp, Warning, TEXT("MaxWalkSpeed : %f"), GetCharacterMovement()->MaxWalkSpeed);
	//UE_LOG(LogTemp, Warning, TEXT("MovementSpeed : %f"), StatComp->GetBaseStat().MovementSpeed);
	//UE_LOG(LogTemp, Warning, TEXT("MaxWalkSpeed2 : %f"), GetCharacterMovement()->MaxWalkSpeed);

	AnimInstance = Cast<UIFNonPlayerAnimInstance>(GetMesh()->GetAnimInstance());
	if (AnimInstance)
	{
		StatComp.Get()->OnHit.AddUObject(AnimInstance, &UIFNonPlayerAnimInstance::PlayHitAnim);
		AnimInstance->OnAttackEnd.BindUObject(this, &AIFCharacterNonPlayer::NotifyAttackActionEnd);
		AnimInstance->OnBackJump.BindUObject(this, &AIFCharacterNonPlayer::StartBackJump);
		AnimInstance->OnBackJumpEnd.BindUObject(this, &AIFCharacterNonPlayer::NotifyBackJumpActionEnd);
		AnimInstance->OnBeforeMoving.BindUObject(this, &AIFCharacterNonPlayer::NotifyBeforeMovingActionEnd);

	}
}

/// <summary>
/// AI가 Focus할 대상 지정
/// </summary>
/// <param name="TargetActor"></param>
void AIFCharacterNonPlayer::FocusingTarget(TObjectPtr<AActor> TargetActor)
{
	AIController->SetFocus(TargetActor, EAIFocusPriority::Gameplay);
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
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
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

/// <summary>
/// 백점프 애니메이션 재생
/// </summary>
void AIFCharacterNonPlayer::PeformBackMoveAI()
{
	CurNpcState = ENPCState::Jumping;
	AnimInstance->PlayBackJumpAnimation();
}

void AIFCharacterNonPlayer::PerformMoving()
{
	AnimInstance->PlayRandomIdleAnimaiton();
}

void AIFCharacterNonPlayer::PerformWaiting(bool bIsFirstContact)
{
	CurNpcState = ENPCState::Idle;
	float WaitTime = bIsFirstContact ? StatComp->GetBaseStat().EncounterTime : StatComp->GetBaseStat().WaitTime;
	
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateLambda([&]() 
		{
			OnWaitingFinished.ExecuteIfBound();
		}), bIsFirstContact, false);
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

void AIFCharacterNonPlayer::NotifyBackJumpActionEnd()
{
	CurNpcState = ENPCState::Idle;
	OnBackJumpFinished.ExecuteIfBound();
}

void AIFCharacterNonPlayer::NotifyBeforeMovingActionEnd()
{
	CurNpcState = ENPCState::Moving;
	OnBeforeMovingFinished.ExecuteIfBound();
}

void AIFCharacterNonPlayer::AttackHitCheck()
{
	FHitResult OutHitResult;
	FCollisionQueryParams Params(SCENE_QUERY_STAT(Attack), false, this);

	const float AttackRange = StatComp->GetBaseStat().AttackRange;
	const float AttackRadius = 100.f;//StatComp->GetRadi;
	//const float AttackDamage = Stat->GetTotalStat().Attack;
	const FVector Start = GetActorLocation() + GetActorForwardVector() * GetCapsuleComponent()->GetScaledCapsuleRadius();
	const FVector End = Start + GetActorForwardVector() * AttackRange;

	bool HitDetected = GetWorld()->SweepSingleByChannel(OutHitResult, Start, End, FQuat::Identity, ECollisionChannel::ECC_GameTraceChannel1, FCollisionShape::MakeSphere(AttackRadius), Params);
	if (HitDetected)
	{
		//TODO Check Damage
		FDamageEvent DamageEvent;
		OutHitResult.GetActor()->TakeDamage(StatComp->GetBaseStat().Attack, DamageEvent, GetController(), this);
	}

#if ENABLE_DRAW_DEBUG

	FVector CapsuleOrigin = Start + (End - Start) * 0.5f;
	float CapsuleHalfHeight = AttackRange * 0.5f;
	FColor DrawColor = HitDetected ? FColor::Green : FColor::Red;

	DrawDebugCapsule(GetWorld(), CapsuleOrigin, CapsuleHalfHeight, AttackRadius, FRotationMatrix::MakeFromZ(GetActorForwardVector()).ToQuat(), DrawColor, false, 5.0f);

#endif
}

float AIFCharacterNonPlayer::GetAIPatrolRadius()
{
	return 800.0f;
}

float AIFCharacterNonPlayer::GetAIDetectRange()
{
	return 400.0f;
}

float AIFCharacterNonPlayer::GetAIAttackRange()
{
	return StatComp->GetBaseStat().AttackRange; //150.0f;
}

float AIFCharacterNonPlayer::GetAITurnSpeed()
{
	return 2.0f;
}


