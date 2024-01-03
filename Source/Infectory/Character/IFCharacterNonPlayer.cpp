// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/IFCharacterNonPlayer.h"
#include "Stat/IFStatComponent.h"
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

	if (AnimInstance)
	{
		Stat->OnHit.AddUObject(AnimInstance, &UIFNonPlayerAnimInstance::PlayHitAnim);
		AnimInstance->OnAttackEnd.BindUObject(this, &AIFCharacterNonPlayer::NotifyAttackActionEnd);

	}
	Stat->OnHpZero.AddUObject(this, &AIFCharacterNonPlayer::SetDead);
}

void AIFCharacterNonPlayer::SetDead()
{
	Super::SetDead();
	CurNpcState = ENPCState::Dead;

	TObjectPtr<AIFAIController> IFController = Cast<AIFAIController>(GetController());
	if (IFController)
	{
		IFController.Get()->StopAI();
	}
}

void AIFCharacterNonPlayer::SetAIAttackDelegate(const FAICharacterAttackFinished& InOnAttackFinished)
{
	OnAttackFinished.Unbind();
	OnAttackFinished = InOnAttackFinished;

	FTimerHandle TimerHandle;

	GetWorld()->GetTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateLambda(
		[&]()
		{
			NotifyAttackActionEnd();
		}
	), 1.3f, false);
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

void AIFCharacterNonPlayer::NotifyAttackActionEnd()
{
	CurNpcState = ENPCState::Idle;
	OnAttackFinished.ExecuteIfBound();
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
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
	return 200.0f;
}

float AIFCharacterNonPlayer::GetAITurnSpeed()
{
	return 2.0f;
}


