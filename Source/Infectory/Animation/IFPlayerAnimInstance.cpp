// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/IFPlayerAnimInstance.h"

UIFPlayerAnimInstance::UIFPlayerAnimInstance()
{

}

void UIFPlayerAnimInstance::AddRecoil()
{
	RecoilAlpha = 1.0f;
}

void UIFPlayerAnimInstance::PlayHitAnim()
{
	UE_LOG(LogTemp, Warning, TEXT("PlayHitAnim"));
	Montage_Play(HitAnimation, 1.0f);
}

void UIFPlayerAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
}

void UIFPlayerAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (Movement)
	{
		RecoilAlpha = FMath::Lerp(RecoilAlpha, 0, 0.04f);
		CurMoveType = DefineTypePawn.GetInterface()->GetPlayerMoveType();
		CurControlType = DefineTypePawn.GetInterface()->GetPlayerControlType();
	}

	if (OnLeftIKChange.IsBound())
	{
		LeftHandPosition = OnLeftIKChange.Execute();
	}
}

