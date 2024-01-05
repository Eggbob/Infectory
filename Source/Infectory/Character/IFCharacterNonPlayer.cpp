// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/IFCharacterNonPlayer.h"
#include "Stat/IFStatComponent.h"
#include "Components/CapsuleComponent.h"
#include "AI/IFAIController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Animation/IFNonPlayerAnimInstance.h"

AIFCharacterNonPlayer::AIFCharacterNonPlayer()
{
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> CharacterMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/Assets/Characters/Test/Parasite_L_Starkie.Parasite_L_Starkie'"));
	if (CharacterMeshRef.Object)
	{
		GetMesh()->SetSkeletalMesh(CharacterMeshRef.Object);
	}

	static ConstructorHelpers::FClassFinder<UAnimInstance> AnimInstanceClassRef(TEXT("/Game/Assets/Animation/Enemy/ABP_TestEnemy.ABP_TestEnemy_C"));
	if (AnimInstanceClassRef.Class)
	{
		GetMesh()->SetAnimInstanceClass(AnimInstanceClassRef.Class);
	}

	AIControllerClass = AIFAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	CurNpcState = ENPCState::Idle;
}

void AIFCharacterNonPlayer::BeginPlay()
{
	Super::BeginPlay();

	AnimInstance = Cast<UIFNonPlayerAnimInstance>(GetMesh()->GetAnimInstance());
	AIController = Cast<AIFAIController>(GetController());

	if (AnimInstance)
	{
		Stat->OnHit.AddUObject(AnimInstance, &UIFNonPlayerAnimInstance::PlayHitAnim);
		AnimInstance->OnAttackEnd.BindUObject(this, &AIFCharacterNonPlayer::NotifyAttackActionEnd);
		AnimInstance->OnBackJump.BindUObject(this, &AIFCharacterNonPlayer::StartBackJump);
		AnimInstance->OnBackJumpEnd.BindUObject(this, &AIFCharacterNonPlayer::NotifyBackJumpActionEnd);

	}
	Stat->OnHpZero.AddUObject(this, &AIFCharacterNonPlayer::SetDead);
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

/// <summary>
/// AI가 Focus할 대상 지정
/// </summary>
/// <param name="TargetActor"></param>
void AIFCharacterNonPlayer::FocusingTarget(TObjectPtr<AActor> TargetActor)
{
	AIController->SetFocus(TargetActor);
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

/// <summary>
/// 공격 시작
/// </summary>
void AIFCharacterNonPlayer::AttackByAI()
{
	CurNpcState = ENPCState::Attacking;
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
	AnimInstance->PlayAttackAnimation();
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

/// <summary>
/// BackJump 실행
/// </summary>
void AIFCharacterNonPlayer::StartBackJump()
{
	FVector JumpDirection = GetActorForwardVector() * -400.0f;
	JumpDirection.Z = 400.f;
	LaunchCharacter(JumpDirection, false, false);
}

void AIFCharacterNonPlayer::NotifyBackJumpActionEnd()
{
	CurNpcState = ENPCState::Idle;
	OnBackJumpFinished.ExecuteIfBound();
}

void AIFCharacterNonPlayer::AttackHitCheck()
{
	FHitResult OutHitResult;
	FCollisionQueryParams Params(SCENE_QUERY_STAT(Attack), false, this);

	const float AttackRange = 100.f;//Stat->GetTotalStat().AttackRange;
	const float AttackRadius = 100.f;//Stat->GetAttackRadius();
	//const float AttackDamage = Stat->GetTotalStat().Attack;
	const FVector Start = GetActorLocation() + GetActorForwardVector() * GetCapsuleComponent()->GetScaledCapsuleRadius();
	const FVector End = Start + GetActorForwardVector() * AttackRange;

	bool HitDetected = GetWorld()->SweepSingleByChannel(OutHitResult, Start, End, FQuat::Identity, ECollisionChannel::ECC_GameTraceChannel1, FCollisionShape::MakeSphere(AttackRadius), Params);
	if (HitDetected)
	{
		//TODO Check Damage

		//FDamageEvent DamageEvent;
		//OutHitResult.GetActor()->TakeDamage(AttackDamage, DamageEvent, GetController(), this);
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
	return 150.0f;
}

float AIFCharacterNonPlayer::GetAITurnSpeed()
{
	return 2.0f;
}


