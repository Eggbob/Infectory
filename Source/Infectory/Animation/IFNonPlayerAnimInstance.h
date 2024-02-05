// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/IFAnimInstance.h"
#include "IFNonPlayerAnimInstance.generated.h"


DECLARE_DELEGATE(FOnAttackEndDelegate);
DECLARE_DELEGATE(FOnBackJumpEndDelegate);
DECLARE_DELEGATE(FOnBackJumpDelegate);
DECLARE_DELEGATE(FOnBeforeMovingDelegate);
DECLARE_DELEGATE(FOnHitEndDelegate);


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
	void PlaySpecialHitAnimation();

	UFUNCTION(BlueprintImplementableEvent)
	void PlayAnimation();

protected:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	UFUNCTION()
	FORCEINLINE void AnimNotify_BackJump() { OnBackJump.ExecuteIfBound(); }

	UFUNCTION()
	FORCEINLINE void AnimNotify_OnHitEnd() { UE_LOG(LogTemp, Warning, TEXT("AnimNotify_OnHitEnd"));
		OnHitEnd.ExecuteIfBound(); }

public:
	FOnAttackEndDelegate OnAttackEnd;
	FOnBackJumpEndDelegate OnBackJumpEnd;
	FOnBackJumpDelegate OnBackJump;
	FOnBeforeMovingDelegate OnBeforeMoving;
	FOnHitEndDelegate OnHitEnd;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	ENPCState CurNpcState;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	ENPCMoveType CurNpcMoveType;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	float RecoilAlpha;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	FRotator RecoilRot;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	bool bIsTurnLeft;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	bool bIsTurnRight;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	TArray<TObjectPtr<class UAnimMontage>> IdleAnimations;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> AttackAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> BackJumpAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> SpecialHitAnimation;

private:
	TObjectPtr<class UIFFootIkComponent> FootIkComponent;

};
