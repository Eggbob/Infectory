// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/IFAnimInstance.h"
#include "IFNonPlayerAnimInstance.generated.h"


DECLARE_DELEGATE(FOnAttackEndDelegate);

/**
 * 
 */
UCLASS()
class INFECTORY_API UIFNonPlayerAnimInstance : public UIFAnimInstance
{
	GENERATED_BODY()
	
public:
	UIFNonPlayerAnimInstance();

	void PlayAttackAnimation();

	FOnAttackEndDelegate OnAttackEnd;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	ENPCState CurNpcState;

protected:
	virtual void NativeInitializeAnimation() override;

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	
	void AttckEnd(class UAnimMontage* TargetMontage, bool IsProperlyEnded);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> AttackAnimation;

};
