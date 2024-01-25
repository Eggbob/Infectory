// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/IFNonPlayerAnimInstance.h"
#include "Interface/IFGetDefineTypeInterface.h"
#include "Component/IFFootIkComponent.h"

UIFNonPlayerAnimInstance::UIFNonPlayerAnimInstance()
{

}

void UIFNonPlayerAnimInstance::PlayAttackAnimation(float AttackSpeed)
{
	if (IsValid(AttackAnimation))
	{
		Montage_Play(AttackAnimation, AttackSpeed);

		FTimerHandle TimerHandle;

		GetWorld()->GetTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateLambda(
			[&]()
			{
				OnAttackEnd.ExecuteIfBound();
			}
		), AttackAnimation->GetPlayLength(), false);
	}
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
	), BackJumpAnimation.Get()->GetPlayLength(), false);
}

void UIFNonPlayerAnimInstance::PlayRandomIdleAnimaiton()
{
	int Index = FMath::RandRange(0, IdleAnimations.Num() - 1);
	Montage_Play(IdleAnimations[Index], 1.0f);

	FTimerHandle TimerHandle;

	GetWorld()->GetTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateLambda(
		[&]()
		{
			OnBeforeMoving.ExecuteIfBound();
		}
	), IdleAnimations[Index].Get()->GetPlayLength(), false);
}

void UIFNonPlayerAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	APawn* pOwner = TryGetPawnOwner();
	if (pOwner != nullptr)
	{
		UActorComponent* pActorComp = pOwner->GetComponentByClass(UIFFootIkComponent::StaticClass());
		if (pActorComp != nullptr)
		{
			FootIkComponent = Cast<UIFFootIkComponent>(pActorComp);
			if (FootIkComponent == nullptr)
			{
				UE_LOG(LogTemp, Warning, TEXT("IKFootRef is nullptr"));
			}
		}
	}

}

void UIFNonPlayerAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (Movement)
	{
		CurNpcState = DefineTypePawn.GetInterface()->GetNPCState();
		CurNpcMoveType = DefineTypePawn.GetInterface()->GetNPCMoveType();
	}
}

void UIFNonPlayerAnimInstance::PlayHitAnim()
{
	if (CurNpcMoveType == ENPCMoveType::Crawling) return;

	if (CurNpcState == ENPCState::Idle || CurNpcState == ENPCState::Moving)
	{
		Montage_Play(HitAnimations[FMath::RandRange(0, HitAnimations.Num()-1)], 1.0f);
	}
}


