// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTDecorator_CheckCanActive.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Interface/IFCharacterAIInterface.h"
#include "AIController.h"
#include "AI/IFAI.h"


UBTDecorator_CheckCanActive::UBTDecorator_CheckCanActive()
{
	NodeName = TEXT("CheckCanActive");
}

bool UBTDecorator_CheckCanActive::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	bool bResult = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	TObjectPtr<APawn> ControllingPawn = Cast<APawn>(OwnerComp.GetAIOwner()->GetPawn());
	if (nullptr == ControllingPawn)
	{
		return EBTNodeResult::Failed;
	}

	IIFCharacterAIInterface* AIPawn = Cast<IIFCharacterAIInterface>(ControllingPawn);

	if (nullptr == AIPawn)
	{
		return EBTNodeResult::Failed;
	}


	bool bIsHit;

	bIsHit = OwnerComp.GetBlackboardComponent()->GetValueAsBool(BBKEY_ISHIT);

	return bIsHit && AIPawn->CheckPath();
}
