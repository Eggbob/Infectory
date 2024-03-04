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

	virtual void SetNPCType(ENPCType NpcName, FName NpcTier);
	virtual float GetAIPatrolRadius() override;
	virtual float GetAIDetectRange() override;
	virtual float GetAIAttackRange() override;
	virtual float GetAITurnSpeed() override;

	UFUNCTION(BlueprintImplementableEvent)
	void InitPhysicsAnimation();

	UFUNCTION(BlueprintImplementableEvent)
	void PlayGlowEffect();

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
	virtual void PerformMoving() override;
	virtual void StopMoving() override;
	virtual void PerformWaiting(bool bIsFirstContact) override;
	virtual void ReadyToExplosion() override;

	void StartBackJump();
	void SetHitWalkSpeed();
	void ChangeToBomb();

	FAICharacterAttackFinished OnAttackFinished;
	FAICharacterBackJumpFinished OnBackJumpFinished;
	FAICharacterBeforeMovingFinished OnBeforeMovingFinished;
	FAICharacterWaitingFinished OnWaitingFinished;

	void NotifyAttackActionEnd();
	void NotifyBeforeMovingActionEnd();
	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

private:
	void AttackHitCheck() override;
	void MeleeAttackCheck();
	void ExploseCharacter();

	UFUNCTION(BlueprintCallable)
	void ChangeNPCMoveMode();


public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = NPC)
	ENPCType CurNPCType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = NPC)
	FName CurNPCTier;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = NPC)
	bool bIsJustSpawn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = NPC)
	float GlowParam = 0;

protected:

	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	TObjectPtr<UParticleSystem> ImpactEffect;

	UPROPERTY(EditAnywhere)
	TObjectPtr<USoundBase> ProjectileSound;

	UPROPERTY(EditAnywhere)
	TObjectPtr<USoundBase> AttackSound;

	UPROPERTY(EditAnywhere)
	TObjectPtr<USoundBase> CrawlSound;

	UPROPERTY(EditAnywhere)
	TObjectPtr<USoundBase> DeadSound;

	UPROPERTY()
	TObjectPtr<class AIFGunBase> ProjectileWeapon;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class AIFGunBase> GunClass;

	UPROPERTY()
	TObjectPtr<class AIFShield> ShieldObject;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class AIFShield> ShieldClass;

	UPROPERTY()
	TMap<ENPCType, TObjectPtr<class USkeletalMesh>> NPCSkeletalMeshes;

	UPROPERTY()
	TMap<ENPCType, TSubclassOf<class UAnimInstance>> NPCAnimInstances;

	UPROPERTY()
	TObjectPtr<class UIFNonPlayerAnimInstance> AnimInstance;

	UPROPERTY()
	TObjectPtr<class AActor> TargetActor;

	UPROPERTY()
	TMap<ENPCBoneName, int32> BodyDamageCheckMap;

private:
	bool bIsExplose = false;
	bool bJustExplose = false;

	UPROPERTY()
	TObjectPtr<class AIFAIController> AIController;

	FTimerHandle GlowTimerHandle;

	ENPCState CurNpcState;

	ENPCMoveType CurNpcMoveType;
};
