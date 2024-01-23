// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Interface/IFCharacterAIInterface.h"
#include "IFAIController.generated.h"

/**
 * 
 */
UCLASS()
class INFECTORY_API AIFAIController : public AAIController
{
	GENERATED_BODY()
	
public:
	AIFAIController();
	
	void RunAI();
	void StopAI();
	void SetTarget(TObjectPtr<AActor> Target);

protected:
	virtual void OnPossess(APawn* InPawn) override;

private:
	UPROPERTY()
	TObjectPtr<class UBlackboardData> BBAsset;

	UPROPERTY()
	TObjectPtr<class UBehaviorTree> BTAsset;

	bool bHasTarget = false;
};
