// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/IFNonPlayerAnimInstance.h"
#include "Interface/IFGetDefineTypeInterface.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Component/IFFootIkComponent.h"

UIFNonPlayerAnimInstance::UIFNonPlayerAnimInstance()
{

}

void UIFNonPlayerAnimInstance::PlayAttackAnimation(float AttackSpeed)
{
	if (IsValid(AttackAnimation))
	{
		BlendWeight = 1.f;
		Montage_Play(AttackAnimation, AttackSpeed);

		FTimerHandle TimerHandle;
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateLambda(
			[&]()
			{
				OnAttackEnd.ExecuteIfBound();
			}
		), AttackAnimation->GetPlayLength() - 0.5f, false);
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
	BlendWeight = 1.f; 

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

void UIFNonPlayerAnimInstance::PlaySpecialHitAnimation()
{
	FTimerHandle TimerHandle;

	BlendWeight = 0.f;
	Montage_Play(SpecialHitAnimation, 1.0f);

	/*GetWorld()->GetTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateLambda(
		[&]()
		{
		
		}
	), 0.3f, false);*/
}

void UIFNonPlayerAnimInstance::PlayLyingAnimation()
{
	BlendWeight = 0.f;
	Montage_Play(LyingAnimation, 0.01f);

	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateLambda(
		[&]()
		{
			PlayStandUpAnimation();
		}
	), 1.3f, false);
}

void UIFNonPlayerAnimInstance::PlayStandUpAnimation()
{
	BlendWeight = 0.f;
	Montage_Play(StandUpAnimation, 1.0f);
}

void UIFNonPlayerAnimInstance::PlayTurnAnimation()
{
	BlendWeight = 0.f;
	Montage_Play(TurnAnimation45, 1.0f);
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
		RecoilAlpha = FMath::Lerp(RecoilAlpha, 0, 0.04f);
		CurNpcState = DefineTypePawn.GetInterface()->GetNPCState();
		CurNpcMoveType = DefineTypePawn.GetInterface()->GetNPCMoveType();
		//CurRotation = FMath::Lerp(CurRotation, FVector::DotProduct(CurVelocity, GetOwningActor()->GetActorRightVector()), 0.05f);
	/*	bIsTurnRight = CurRotation > 0.3f ? true : false;
		bIsTurnLeft = CurRotation < -0.3f ? true : false;*/
	}
}

void UIFNonPlayerAnimInstance::PlayHitAnim()
{
	//Montage_Play(HitAnimations[FMath::RandRange(0, HitAnimations.Num() - 1)], 1.0f);
	//PlayAnimation();
	if (CurNpcMoveType == ENPCMoveType::Crawling) return;

	if (CurNpcState == ENPCState::Moving || CurNpcState == ENPCState::Idle)
	{
		RecoilAlpha = 1.0f;
		RecoilRot = FRotator(FMath::RandRange(5.0f, 20.0f), FMath::RandRange(-20.0f, 20.0f), 0.f);
	}
}


