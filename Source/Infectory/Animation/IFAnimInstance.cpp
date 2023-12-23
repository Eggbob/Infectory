// Fill out your copyright notice in the Description page of Project Settings.


#include "IFAnimInstance.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Interface/IFGetDefineTypeInterface.h"
#include "Kismet/KismetMathLibrary.h"

UIFAnimInstance::UIFAnimInstance()
{
	MovingThreshould = 3.0f;
	JumpingThreshould = 100.0f;
}

void UIFAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	AnimOwner = Cast<ACharacter>(GetOwningActor());
	if (AnimOwner)
	{
		Movement = AnimOwner->GetCharacterMovement();
	}

	DefineTypePawn = Cast<IIFGetDefineTypeInterface>(AnimOwner);
}

void UIFAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (Movement)
	{
		CurVelocity = Movement->Velocity;
		GroundSpeed = FMath::Lerp(GroundSpeed,FVector::DotProduct(CurVelocity, AnimOwner->GetActorForwardVector()), 0.05f);
		//GroundSpeed = FMath::Lerp(GroundSpeed, CurVelocity.Size2D(), 0.05f); 
		bIsIdle = GroundSpeed < MovingThreshould;
		bIsFalling = Movement->IsFalling();
		bIsJumping = bIsFalling & (CurVelocity.Z > JumpingThreshould);
		CurRotation = FMath::Lerp(CurRotation,FVector::DotProduct(CurVelocity, AnimOwner->GetActorRightVector()), 0.05f);
		//CurRotation = FMath::Lerp(CurRotation,FVector::DotProduct(Movement->Velocity, AnimOwner->GetActorRightVector()), 0.05f);
		CurMoveType = DefineTypePawn->GetMoveType();
		CurControlType = DefineTypePawn->GetControlType();
	}

	if (OnLeftIKChange.IsBound())
	{
		LeftHandPosition = OnLeftIKChange.Execute();
	}
}


