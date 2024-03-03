// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IFCharacterAIInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UIFCharacterAIInterface : public UInterface
{
	GENERATED_BODY()
};


DECLARE_DELEGATE(FAICharacterAttackFinished);
DECLARE_DELEGATE(FAICharacterBackJumpFinished);
DECLARE_DELEGATE(FAICharacterBeforeMovingFinished);
DECLARE_DELEGATE(FAICharacterWaitingFinished);
/**
 * 
 */
class INFECTORY_API IIFCharacterAIInterface
{
	GENERATED_BODY()

public:
	virtual float GetAIPatrolRadius() = 0;
	virtual float GetAIDetectRange() = 0;
	virtual float GetAIAttackRange() = 0;
	virtual float GetAITurnSpeed() = 0;
	
	virtual void FocusingTarget(TObjectPtr<AActor> TargetActor) = 0;
	virtual void SetAIAttackDelegate(const FAICharacterAttackFinished& InOnAttackFinished) = 0;
	virtual void SetAIBackJumpDelegate(const FAICharacterBackJumpFinished& InOnBackJumpFinished) = 0;
	virtual void SetAIBeforeMovingDelegate(const FAICharacterBeforeMovingFinished& InOnBeforeMovingFinished) = 0;
	virtual void SetAIWaitingDelegate(const FAICharacterWaitingFinished& InOnWaitingFinished) = 0;
	virtual void ChangeToBomb() = 0;
	virtual void AttackByAI() = 0;
	virtual void PerformWaiting(bool bIsFirstContact) = 0;
	virtual void PerformMoving() = 0;
	virtual void StopMoving() = 0;
};
