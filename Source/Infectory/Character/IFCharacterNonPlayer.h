// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/IFCharacterBase.h"
#include "Interface/IFGetDefineTypeInterface.h"
#include "Interface/IFCharacterAIInterface.h"
#include "IFCharacterNonPlayer.generated.h"

/**
 * 
 */
UCLASS()
class INFECTORY_API AIFCharacterNonPlayer : public AIFCharacterBase, public IIFCharacterAIInterface, public IIFGetDefineTypeInterface
{
	GENERATED_BODY()
	
public:
	AIFCharacterNonPlayer();
	FORCEINLINE virtual ENPCState GetNPCState() { return CurNpcState; }

	virtual float GetAIPatrolRadius() override;
	virtual float GetAIDetectRange() override;
	virtual float GetAIAttackRange() override;
	virtual float GetAITurnSpeed() override;


protected:
	virtual void BeginPlay() override;
	virtual void SetDead() override;

	virtual void SetAIAttackDelegate(const FAICharacterAttackFinished& InOnAttackFinished) override;
	virtual void AttackByAI() override;

	FAICharacterAttackFinished OnAttackFinished;

	void NotifyAttackActionEnd();

private:
	ENPCState CurNpcState;
	
	UPROPERTY()
	TObjectPtr<class UIFNonPlayerAnimInstance> AnimInstance;
};
