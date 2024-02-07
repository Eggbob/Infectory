// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTDecorator_CheckCanActive.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AI/IFAI.h"


UBTDecorator_CheckCanActive::UBTDecorator_CheckCanActive()
{
	NodeName = TEXT("CheckCanActive");
}

bool UBTDecorator_CheckCanActive::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	bool bResult = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	bool bIsHit;

	bIsHit = OwnerComp.GetBlackboardComponent()->GetValueAsBool(BBKEY_ISHIT);

	return bIsHit;
}
