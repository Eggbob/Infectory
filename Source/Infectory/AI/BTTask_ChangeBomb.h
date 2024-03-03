// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_ChangeBomb.generated.h"

/**
 * 
 */
UCLASS()
class INFECTORY_API UBTTask_ChangeBomb : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTTask_ChangeBomb();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

};
