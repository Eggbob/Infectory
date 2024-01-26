// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/IFAnimInstance.h"
#include "IFPlayerAnimInstance.generated.h"

enum class ECharacterMoveType : uint8;
enum class ECharacterControlType : uint8;

DECLARE_DELEGATE_RetVal(FVector, FOnLeftIKChangeDelegate)
DECLARE_DELEGATE(FOnHitAnimFinishedDelegate)
/**
 * 
 */
UCLASS()
class INFECTORY_API UIFPlayerAnimInstance : public UIFAnimInstance
{
	GENERATED_BODY()
	
public:
	UIFPlayerAnimInstance();
	void AddRecoil();
	
	virtual void PlayHitAnim() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	ECharacterMoveType CurMoveType;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	ECharacterControlType CurControlType;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	FVector LeftHandPosition;

	FOnLeftIKChangeDelegate OnLeftIKChange;
	FOnHitAnimFinishedDelegate OnHitAnimFinished;

protected:
	virtual void NativeInitializeAnimation() override;

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	UFUNCTION()
	FORCEINLINE void AnimNotify_Hit() { OnHitAnimFinished.ExecuteIfBound(); }

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Character)
	float AimPitch;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Character)
	float AimYaw;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	float RecoilAlpha;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Character)
	FRotator RecoilRotation = FRotator(-5.0f, 0.0f, 0.0f);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> HitAnimation;
};
