// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/IFNonPlayerAnimInstance.h"
#include "Interface/IFGetDefineTypeInterface.h"

UIFNonPlayerAnimInstance::UIFNonPlayerAnimInstance()
{

}

void UIFNonPlayerAnimInstance::PlayAttackAnimation()
{
	Montage_Play(AttackAnimation, 1.0f);
}

void UIFNonPlayerAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	FOnMontageEnded EndDelegate;
	EndDelegate.BindUObject(this, &UIFNonPlayerAnimInstance::AttckEnd);
	Montage_SetEndDelegate(EndDelegate, AttackAnimation);

}

void UIFNonPlayerAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (Movement)
	{
		CurNpcState = DefineTypePawn.GetInterface()->GetNPCState();
	}
}

void UIFNonPlayerAnimInstance::AttckEnd(UAnimMontage* TargetMontage, bool IsProperlyEnded)
{
	OnAttackEnd.ExecuteIfBound();
}


