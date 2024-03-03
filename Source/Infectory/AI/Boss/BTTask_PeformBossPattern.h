// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_PeformBossPattern.generated.h"

/**
 * 
 */
UCLASS()
class INFECTORY_API UBTTask_PeformBossPattern : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTTask_PeformBossPattern();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	
};
