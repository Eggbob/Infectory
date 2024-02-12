// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Interface/IFGetDefineTypeInterface.h"
#include "Component/IFFootIkComponent.h"
#include "IFAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class INFECTORY_API UIFAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	UIFAnimInstance();

	virtual void PlayHitAnim() {};
	virtual void PlayDeadAnim();
	virtual void SetCurSound(TObjectPtr<USoundBase> InSound);

protected:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	virtual void PlaySound();
	virtual void PlayFootSound();

private:
	void InitIKFootRef();
	void TickIKFoot();
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Character)
	TObjectPtr<class ACharacter> AnimOwner;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Character)
	TObjectPtr<class UCharacterMovementComponent> Movement;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	FVector CurVelocity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Character)
	float CurRotation;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	float GroundSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	uint8 bIsIdle : 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	float MovingThreshould;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	uint8 bIsFalling : 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	uint8 bIsJumping : 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	float JumpingThreshould;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	bool bIsTurnLeft;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	bool bIsTurnRight;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	TArray<TObjectPtr<class UAnimMontage>> HitAnimations;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> DeadAnimaton;

	UPROPERTY()
	TScriptInterface<IIFGetDefineTypeInterface> DefineTypePawn;

	UPROPERTY()
	TObjectPtr<UIFFootIkComponent> IkFootComp;

	UPROPERTY(EditAnywhere)
	TObjectPtr<USoundBase> CurSound;

	UPROPERTY(EditAnywhere)
	TObjectPtr<USoundBase> FootSound;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "IKFoot Value", meta = (AllowPrivateAccess = "true"))
	FIKAnimValue IkAnimValue;
};
