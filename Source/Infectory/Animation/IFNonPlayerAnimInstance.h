// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/IFAnimInstance.h"
#include "IFNonPlayerAnimInstance.generated.h"


DECLARE_DELEGATE(FOnAttackEndDelegate);
DECLARE_DELEGATE(FOnBackJumpEndDelegate);
DECLARE_DELEGATE(FOnBackJumpDelegate);
DECLARE_DELEGATE(FOnBeforeMovingDelegate);

/**
 * 
 */
UCLASS()
class INFECTORY_API UIFNonPlayerAnimInstance : public UIFAnimInstance
{
	GENERATED_BODY()
	
public:
	UIFNonPlayerAnimInstance();

	virtual void PlayHitAnim() override;
	void PlayAttackAnimation(float AttackSpeed);
	void PlayBackJumpAnimation();
	void PlayRandomIdleAnimaiton();

	FOnAttackEndDelegate OnAttackEnd;
	FOnBackJumpEndDelegate OnBackJumpEnd;
	FOnBackJumpDelegate OnBackJump;
	FOnBeforeMovingDelegate OnBeforeMoving;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	ENPCState CurNpcState;

protected:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	UFUNCTION()
	FORCEINLINE void AnimNotify_BackJump() { OnBackJump.ExecuteIfBound(); }

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	TArray<TObjectPtr<class UAnimMontage>> IdleAnimations;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> AttackAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> BackJumpAnimation;

private:
	TObjectPtr<class UIFFootIkComponent> FootIkComponent;

};
