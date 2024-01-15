// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/IFCharacterBase.h"
#include "Interface/IFGetDefineTypeInterface.h"
#include "Interface/IFCharacterAIInterface.h"
#include "Interface/IFAnimationAttackInterface.h"
#include "IFCharacterNonPlayer.generated.h"

/**
 * 
 */
UCLASS()
class INFECTORY_API AIFCharacterNonPlayer : public AIFCharacterBase, public IIFCharacterAIInterface, public IIFGetDefineTypeInterface, public IIFAnimationAttackInterface
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

	virtual void FocusingTarget(TObjectPtr<AActor> TargetActor) override;
	virtual void SetAIAttackDelegate(const FAICharacterAttackFinished& InOnAttackFinished) override;
	virtual void SetAIBackJumpDelegate(const FAICharacterBackJumpFinished& InOnBackJumpFinished) override;
	virtual void SetAIBeforeMovingDelegate(const FAICharacterBeforeMovingFinished& InOnBeforeMovingFinished) override;
	virtual void AttackByAI() override;
	virtual void PeformBackMoveAI() override;
	virtual void PerformMoving() override;

	void StartBackJump();

	FAICharacterAttackFinished OnAttackFinished;
	FAICharacterBackJumpFinished OnBackJumpFinished;
	FAICharacterBeforeMovingFinished OnBeforeMovingFinished;

	void NotifyAttackActionEnd();
	void NotifyBackJumpActionEnd();
	void NotifyBeforeMovingActionEnd();

private:
	void SetNPCType();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = NPC)
	ENPCType CurNPCType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = NPC)
	ENPCTier CurNPCTier;

private:
	ENPCState CurNpcState;
	
	UPROPERTY()
	TMap<ENPCType, TObjectPtr<class USkeletalMesh>> NPCSkeletalMeshes;
	
	UPROPERTY()
	TMap<ENPCType, TSubclassOf<class UAnimInstance>> NPCAnimInstances;
	
	UPROPERTY()
	TObjectPtr<class UIFNonPlayerAnimInstance> AnimInstance;

	UPROPERTY()
	TObjectPtr<class AIFAIController> AIController;

	void AttackHitCheck() override;
};
