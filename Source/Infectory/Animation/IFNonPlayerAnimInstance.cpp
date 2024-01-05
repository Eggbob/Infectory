// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/IFNonPlayerAnimInstance.h"
#include "Interface/IFGetDefineTypeInterface.h"

UIFNonPlayerAnimInstance::UIFNonPlayerAnimInstance()
{

}

void UIFNonPlayerAnimInstance::PlayAttackAnimation()
{
	Montage_Play(AttackAnimation, 1.0f);

	FTimerHandle TimerHandle;

	GetWorld()->GetTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateLambda(
		[&]()
		{
			OnAttackEnd.ExecuteIfBound();
		}
	), AttackAnimation.Get()->GetPlayLength(), false);
}

void UIFNonPlayerAnimInstance::PlayBackJumpAnimation()
{
	Montage_Play(BackJumpAnimation, 1.0f);

	FTimerHandle TimerHandle;

	GetWorld()->GetTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateLambda(
		[&]()
		{
			OnBackJumpEnd.ExecuteIfBound();
		}
	), BackJumpAnimation.Get()->GetPlayLength() - 1.0f, false);
}

void UIFNonPlayerAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
}

void UIFNonPlayerAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (Movement)
	{
		CurNpcState = DefineTypePawn.GetInterface()->GetNPCState();
	}
}

void UIFNonPlayerAnimInstance::PlayHitAnim()
{
	if (CurNpcState == ENPCState::Idle || CurNpcState == ENPCState::Moving)
	{
		Montage_Play(HitAnimation, 1.0f);
	}
}


