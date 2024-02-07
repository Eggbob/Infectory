// Fill out your copyright notice in the Description page of Project Settings.


#include "IFAnimInstance.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

UIFAnimInstance::UIFAnimInstance()
{
	MovingThreshould = 3.0f;
	JumpingThreshould = 100.0f;
}

void UIFAnimInstance::PlayDeadAnim()
{
	StopAllMontages(0.f);
	Montage_Play(DeadAnimaton, 1.0f);
}

void UIFAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	AnimOwner = Cast<ACharacter>(GetOwningActor());
	if (AnimOwner)
	{
		Movement = AnimOwner->GetCharacterMovement();
	}

	DefineTypePawn = TScriptInterface<IIFGetDefineTypeInterface>(AnimOwner);

	InitIKFootRef();
}

void UIFAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (Movement)
	{
		CurVelocity = Movement->Velocity;
		GroundSpeed = FMath::Lerp(GroundSpeed,FVector::DotProduct(CurVelocity, AnimOwner->GetActorForwardVector()), 0.05f);
		bIsIdle = GroundSpeed < MovingThreshould;
		bIsFalling = Movement->IsFalling();
		bIsJumping = bIsFalling & (CurVelocity.Z > JumpingThreshould);
		CurRotation = FMath::Lerp(CurRotation,FVector::DotProduct(CurVelocity, AnimOwner->GetActorRightVector()), 0.05f);
		bIsTurnRight = CurRotation > 0.3f ? true : false;
		bIsTurnLeft = CurRotation < -0.3f ? true : false;
	}

	TickIKFoot();
}

void UIFAnimInstance::InitIKFootRef()
{
	APawn* OwnerPawn = TryGetPawnOwner();
	if (OwnerPawn)
	{
		UActorComponent* ActorComp = OwnerPawn->GetComponentByClass(UIFFootIkComponent::StaticClass());
		if (ActorComp)
		{
			IkFootComp = Cast<UIFFootIkComponent>(ActorComp);
			if (IkFootComp == nullptr)
			{
				UE_LOG(LogTemp, Warning, TEXT("IKFootComp is NULL"));
			}
		}
	}

}

void UIFAnimInstance::TickIKFoot()
{
	if (IkFootComp == nullptr) return;

	IkAnimValue = IkFootComp->GetIKAnimValue();
}


