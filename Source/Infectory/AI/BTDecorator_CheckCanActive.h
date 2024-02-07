// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTDecorator_CheckCanActive.generated.h"

/**
 * 
 */
UCLASS()
class INFECTORY_API UBTDecorator_CheckCanActive : public UBTDecorator
{
	GENERATED_BODY()
	
public:
	UBTDecorator_CheckCanActive();

protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
	
};
