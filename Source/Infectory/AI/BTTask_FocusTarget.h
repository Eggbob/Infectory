// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_FocusTarget.generated.h"

/**
 * 
 */
UCLASS()
class INFECTORY_API UBTTask_FocusTarget : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTTask_FocusTarget();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
