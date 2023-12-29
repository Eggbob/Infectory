// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/IFAnimInstance.h"
#include "IFNonPlayerAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class INFECTORY_API UIFNonPlayerAnimInstance : public UIFAnimInstance
{
	GENERATED_BODY()
	
public:
	UIFNonPlayerAnimInstance();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	ENPCState CurNpcState;



protected:
	virtual void NativeInitializeAnimation() override;

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
};
