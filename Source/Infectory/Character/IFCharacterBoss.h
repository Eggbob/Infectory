// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/IFCharacterNonPlayer.h"
#include "Data/IFEnumDefine.h"
#include "Containers/Queue.h"
#include "Interface/IFBossAIInterface.h"
#include "IFCharacterBoss.generated.h"

/**
 * 
 */
UCLASS()
class INFECTORY_API AIFCharacterBoss : public AIFCharacterNonPlayer, public IIFBossAIInterface
{
	GENERATED_BODY()
	
public:
	AIFCharacterBoss();

	virtual void BeginPlay() override;
	virtual void SetDead() override;
	virtual void PerformPierceAttack() override;
	virtual void PerformRangeAttack() override;
	virtual void PeformBreathAttack() override;
	virtual void PerformSpawnEnemy() override;
	virtual void PerformGrabAttack() override;
	virtual void SetNPCType(ENPCType NpcName, FName NpcTier) override;
	virtual void CheckPattern(EBossPattern BossPattern) override;

	void ReleaseGrabTentacle();
	void CheckAcitveTumor();
	bool TumorActive();

	UFUNCTION(BlueprintImplementableEvent)
	void SetTentacleActor();

private:
	void AttackHitCheck() override;
	void ExecutePattern(EBossPattern Pattern);
	void CheckTentacle();
	void GiveDamage(TObjectPtr<AActor> Target);
	void PerformCoolDown();
	void PerformHitAction();
	FVector ReturnTargetLoc();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess = true))
	TArray<TObjectPtr<class AIFTentalce>> TentacleArray;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess = true))
	TArray<TObjectPtr<class AIFTumor>> TumorArray;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<class AIFGunBase> BreathGunClass;

private:
	UPROPERTY()
	TSubclassOf<class AIFSpawnEgg> SpawnEggClass;

	UPROPERTY()
	TSubclassOf<class AIFTumor> TumorClass;

	int32 TentacleCount = 0;
	int32 CurTentacleIdx = -1;
	int32 CurTumorCount = 0;

	UPROPERTY()
	TObjectPtr<class AIFGunBase> BossBreathGun;

	UPROPERTY()
	TObjectPtr<class AIFBossAIController> BossAIController;
	
	UPROPERTY()
	TObjectPtr<class AIFGameMode> GameMode;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess = true))
	TArray<TObjectPtr<UMaterial>> BossMaterialArray;

	UPROPERTY()
	TArray<FVector> TentacleLocArray;

	UPROPERTY()
	TMap<EBossPattern, bool> PatternCooldownMap;

	UPROPERTY()
	TArray<EBossPattern> PatternArray;

	EBossPattern CurBossPattern;
	EBossState CurBossState;
};
