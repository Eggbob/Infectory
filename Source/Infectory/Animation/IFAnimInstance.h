// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "IFAnimInstance.generated.h"

enum class ECharacterMoveType : uint8;

DECLARE_DELEGATE_OneParam(FOnLeftIKChangeDelegate ,FVector)

/**
 * 
 */
UCLASS()
class INFECTORY_API UIFAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	UIFAnimInstance();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	ECharacterMoveType CurMoveType;

	UPROPERTY()
	FVector LeftHandPosition;
	
	FOnLeftIKChangeDelegate OnLeftIKChange;

protected:
	virtual void NativeInitializeAnimation() override;

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Character)
	TObjectPtr<class ACharacter> AnimOwner;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Character)
	TObjectPtr<class UCharacterMovementComponent> Movement;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	FVector CurVelocity;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
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

private:
	UPROPERTY()
	TObjectPtr<class IIFGetDefineTypeInterface> DefineTypePawn;


};
