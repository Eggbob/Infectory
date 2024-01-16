// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_Wait.h"
#include "BTTask_NPCWait.generated.h"

/**
 * 
 */
UCLASS()
class INFECTORY_API UBTTask_NPCWait : public UBTTask_Wait
{
	GENERATED_BODY()
	
public:
	UBTTask_NPCWait();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess = true))
	bool bIsFirst;
};
