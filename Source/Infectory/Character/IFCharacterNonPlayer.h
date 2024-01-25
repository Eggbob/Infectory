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
	FORCEINLINE virtual ENPCMoveType GetNPCMoveType() { return CurNpcMoveType; }

	void SetNPCType(ENPCType NpcName, FName NpcTier);
	virtual float GetAIPatrolRadius() override;
	virtual float GetAIDetectRange() override;
	virtual float GetAIAttackRange() override;
	virtual float GetAITurnSpeed() override;

	UFUNCTION(BlueprintImplementableEvent)
	void InitPhysicsAnimation();

	UFUNCTION(BlueprintImplementableEvent)
	void PlayHitReaction(FName BoneName, FHitResult HitResult);

protected:
	virtual void BeginPlay() override;
	virtual void SetDead() override;

	virtual void FocusingTarget(TObjectPtr<AActor> TargetActor) override;
	virtual void SetAIAttackDelegate(const FAICharacterAttackFinished& InOnAttackFinished) override;
	virtual void SetAIBackJumpDelegate(const FAICharacterBackJumpFinished& InOnBackJumpFinished) override;
	virtual void SetAIBeforeMovingDelegate(const FAICharacterBeforeMovingFinished& InOnBeforeMovingFinished) override;
	virtual void SetAIWaitingDelegate(const FAICharacterWaitingFinished& InOnWaitingFinished) override;
	virtual void AttackByAI() override;
	virtual void PeformBackMoveAI() override;
	virtual void PerformMoving() override;
	virtual void PerformWaiting(bool bIsFirstContact) override;

	void StartBackJump();
	void SetHitWalkSpeed();

	FAICharacterAttackFinished OnAttackFinished;
	FAICharacterBackJumpFinished OnBackJumpFinished;
	FAICharacterBeforeMovingFinished OnBeforeMovingFinished;
	FAICharacterWaitingFinished OnWaitingFinished;

	void NotifyAttackActionEnd();
	void NotifyBackJumpActionEnd();
	void NotifyBeforeMovingActionEnd();
	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

private:
	void AttackHitCheck() override;
	void MeleeAttackCheck();

	UFUNCTION(BlueprintCallable)
	void ChangeNPCMoveMode();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = NPC)
	ENPCType CurNPCType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = NPC)
	FName CurNPCTier;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = NPC)
	bool bIsJustSpawn;

private:
	ENPCState CurNpcState;

	ENPCMoveType CurNpcMoveType;

	UPROPERTY()
	TObjectPtr<class AIFGunBase> ProjectileWeapon;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class AIFGunBase> GunClass;

	UPROPERTY()
	TMap<ENPCType, TObjectPtr<class USkeletalMesh>> NPCSkeletalMeshes;
	
	UPROPERTY()
	TMap<ENPCType, TSubclassOf<class UAnimInstance>> NPCAnimInstances;
	
	UPROPERTY()
	TObjectPtr<class UIFNonPlayerAnimInstance> AnimInstance;

	UPROPERTY()
	TObjectPtr<class AIFAIController> AIController;

	UPROPERTY()
	TMap<ENPCBoneName, int32> BodyDamageCheckMap;
};
