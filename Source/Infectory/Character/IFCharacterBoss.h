// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/IFCharacterNonPlayer.h"
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
	virtual void SetNPCType(ENPCType NpcName, FName NpcTier) override;

	UFUNCTION(BlueprintImplementableEvent)
	void SetTentacleActor();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess = true))
	TArray<TObjectPtr<class AIFTentalce>> TentacleArray;

private:
	void GiveDamage(TObjectPtr<AActor> Target);

private:
	TObjectPtr<class AIFBossAIController> AIController;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess = true))
	TArray<TObjectPtr<UMaterial>> BossMaterialArray;

	TArray<FVector> TentacleLocArray;
};
