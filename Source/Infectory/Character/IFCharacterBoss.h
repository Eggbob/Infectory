// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/IFCharacterNonPlayer.h"
#include "Data/IFEnumDefine.h"
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
	virtual void PerformSpawnBoomer() override;
	virtual void PerformGrabAttack() override;
	virtual void SetNPCType(ENPCType NpcName, FName NpcTier) override;
	void ReleaseGrabTentacle();

	UFUNCTION(BlueprintImplementableEvent)
	void SetTentacleActor();

	UFUNCTION(BlueprintImplementableEvent)
	void TestTentacleActive();

private:
	void AttackHitCheck() override;
	void CheckTentacle();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess = true))
	TArray<TObjectPtr<class AIFTentalce>> TentacleArray;

private:
	void GiveDamage(TObjectPtr<AActor> Target);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<class AIFGunBase> BreathGunClass;

private:

	UPROPERTY()
	TSubclassOf<class AIFCharacterNonPlayer> BoomerClass;

	int32 TentacleCount = 0;
	int32 CurTentacleIdx = -1;

	UPROPERTY()
	TObjectPtr<class AIFGunBase> BossBreathGun;

	UPROPERTY()
	TObjectPtr<class AIFBossAIController> BossAIController;
	
	UPROPERTY()
	TObjectPtr<class AIFGameMode> GameMode;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess = true))
	TArray<TObjectPtr<UMaterial>> BossMaterialArray;

	TArray<FVector> TentacleLocArray;

	EBossPattern CurBossPattern;
};
