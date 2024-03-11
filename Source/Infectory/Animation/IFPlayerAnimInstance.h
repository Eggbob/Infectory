// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/IFAnimInstance.h"
#include "IFPlayerAnimInstance.generated.h"

enum class ECharacterMoveType : uint8;
enum class ECharacterControlType : uint8;
enum class ERangedWeaponType : uint8;

DECLARE_DELEGATE_RetVal(FVector, FOnLeftIKChangeDelegate)
DECLARE_DELEGATE(FOnHitAnimFinishedDelegate)
DECLARE_DELEGATE(FOnReloadFinishedDelegate)
DECLARE_DELEGATE(FOnWeaponChangeFinishedDelegate)
DECLARE_DELEGATE(FOnRecoverDelegate)

/**
 * 
 */
UCLASS()
class INFECTORY_API UIFPlayerAnimInstance : public UIFAnimInstance
{
	GENERATED_BODY()
	
public:
	UIFPlayerAnimInstance();
	
	virtual void PlayHitAnim() override;
	void AddRecoil(ERangedWeaponType RangedWeaponType);
	void PlayReloadAnim(ERangedWeaponType RangedWeaponType);
	void PlayWeaponChangeAnim();
	void PlayFireAnimation();
	void PlayThrowAnimation();
	void PlayRecoverAnimation();
	void PlayStunAnimation();

	UFUNCTION()
	FORCEINLINE void AnimNotify_OnWeaponChange() { OnWeaponChangeFinished.ExecuteIfBound(); }
	UFUNCTION()
	FORCEINLINE void AnimNotify_Hit() { OnHitAnimFinished.ExecuteIfBound(); }
	UFUNCTION()
	FORCEINLINE void AnimNotify_PlaySound() { PlaySound(); }
	UFUNCTION()
	FORCEINLINE void AnimNotify_PlayFootStep() { PlayFootSound(); }
	UFUNCTION()
	FORCEINLINE void AnimNotify_Recover() { OnRecover.ExecuteIfBound(); }

protected:
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;


public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	ECharacterMoveType CurMoveType;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	ECharacterControlType CurControlType;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	ECharacterState CurState;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	FVector LeftHandPosition;

	FOnLeftIKChangeDelegate OnLeftIKChange;
	FOnHitAnimFinishedDelegate OnHitAnimFinished;
	FOnReloadFinishedDelegate OnReloadFinished;
	FOnWeaponChangeFinishedDelegate OnWeaponChangeFinished;
	FOnRecoverDelegate OnRecover;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Character)
	float AimPitch;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Character)
	float AimYaw;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Character)
	float RecoilAlpha;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Character)
	float RightHandAlpha;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Character)
	FRotator RecoilRotation = FRotator(-5.0f, 0.0f, 0.0f);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	TMap<ERangedWeaponType, FRotator> WeaponRecoilVectorMap;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	TMap<ERangedWeaponType, TObjectPtr<class UAnimMontage>> ReloadAnimationMap;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> WeaponChangeAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> FireAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> ThrowAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> StunAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> RecoverAnimation;
};
