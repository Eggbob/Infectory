// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/IFPlayerAnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Character.h"


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

void UIFPlayerAnimInstance::PlayReloadAnim(ERangedWeaponType RangedWeaponType)
{
	if (IsValid(ReloadAnimationMap[RangedWeaponType]))
	{
		Montage_Play(ReloadAnimationMap[RangedWeaponType], 1.0f);

		FTimerHandle TimerHandle;

		GetWorld()->GetTimerManager().SetTimer(TimerHandle,FTimerDelegate::CreateLambda(
			[&]()
			{
				OnReloadFinished.ExecuteIfBound();
			}
		), ReloadAnimationMap[RangedWeaponType].Get()->GetPlayLength() - 0.5f, false);
	}
}

void UIFPlayerAnimInstance::PlayFireAnimation()
{
	Montage_Play(FireAnimation, 1.0f);
}

void UIFPlayerAnimInstance::PlayThrowAnimation()
{
	Montage_Play(ThrowAnimation, 1.0f);
}

void UIFPlayerAnimInstance::PlayRecoverAnimation()
{
	Montage_Play(RecoverAnimation, 1.0f);
}


void UIFPlayerAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (Movement)
	{
		CurVelocity = Movement->Velocity;
		GroundSpeed = FMath::Lerp(GroundSpeed,FVector::DotProduct(CurVelocity, AnimOwner->GetActorForwardVector()), 0.05f);
		RecoilAlpha = FMath::Lerp(RecoilAlpha, 0, 0.04f);
		CurMoveType = DefineTypePawn.GetInterface()->GetPlayerMoveType();
		CurControlType = DefineTypePawn.GetInterface()->GetPlayerControlType();
		CurState = DefineTypePawn.GetInterface()->GetPlayerStateType();
		CurRotation = FMath::Lerp(CurRotation, FVector::DotProduct(CurVelocity, GetOwningActor()->GetActorRightVector()), 0.05f);
	}

	if (OnLeftIKChange.IsBound())
	{
		LeftHandPosition = OnLeftIKChange.Execute();
	}
}


