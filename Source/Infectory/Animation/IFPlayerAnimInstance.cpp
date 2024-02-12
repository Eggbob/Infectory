// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/IFPlayerAnimInstance.h"


UIFPlayerAnimInstance::UIFPlayerAnimInstance()
{

}

void UIFPlayerAnimInstance::AddRecoil(ERangedWeaponType RangedWeaponType)
{
	RecoilAlpha = 1.0f;
	RecoilRotation = WeaponRecoilVectorMap[RangedWeaponType];
}

void UIFPlayerAnimInstance::PlayHitAnim()
{
	Montage_Play(HitAnimations[FMath::RandRange(0, HitAnimations.Num() - 1)], 1.0f);
}

void UIFPlayerAnimInstance::PlayWeaponChangeAnim()
{
	if (IsValid(WeaponChangeAnimation))
	{
		Montage_Play(WeaponChangeAnimation, 1.0f);
	}
}

void UIFPlayerAnimInstance::PlayReloadAnim()
{
	if (IsValid(ReloadAnimation))
	{
		Montage_Play(ReloadAnimation, 1.0f);

		FTimerHandle TimerHandle;

		GetWorld()->GetTimerManager().SetTimer(TimerHandle,FTimerDelegate::CreateLambda(
			[&]()
			{
				OnReloadFinished.ExecuteIfBound();
			}
		), ReloadAnimation.Get()->GetPlayLength(), false);
	}
}



void UIFPlayerAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (Movement)
	{
		RecoilAlpha = FMath::Lerp(RecoilAlpha, 0, 0.04f);
		CurMoveType = DefineTypePawn.GetInterface()->GetPlayerMoveType();
		CurControlType = DefineTypePawn.GetInterface()->GetPlayerControlType();
		CurRotation = FMath::Lerp(CurRotation, FVector::DotProduct(CurVelocity, GetOwningActor()->GetActorRightVector()), 0.05f);
	}

	if (OnLeftIKChange.IsBound())
	{
		LeftHandPosition = OnLeftIKChange.Execute();
	}
}





