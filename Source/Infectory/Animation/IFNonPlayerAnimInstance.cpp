// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/IFNonPlayerAnimInstance.h"
#include "Interface/IFGetDefineTypeInterface.h"

UIFNonPlayerAnimInstance::UIFNonPlayerAnimInstance()
{
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
	
	}
}


