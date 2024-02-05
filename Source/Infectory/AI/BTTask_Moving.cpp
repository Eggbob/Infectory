// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_Moving.h"
#include "AIController.h"
#include "IFAI.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Interface/IFCharacterAIInterface.h"

UBTTask_Moving::UBTTask_Moving()
{
	NodeName = ("MoveNPC");
}

EBTNodeResult::Type UBTTask_Moving::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	TObjectPtr<APawn> ControllingPawn = Cast<APawn>(OwnerComp.GetAIOwner()->GetPawn());
	if (nullptr == ControllingPawn)
	{
		return EBTNodeResult::Failed;
	}

	IIFCharacterAIInterface* AIPawn = Cast<IIFCharacterAIInterface>(ControllingPawn);
	bool bIsHit = OwnerComp.GetBlackboardComponent()->GetValueAsBool(BBKEY_ISHIT);

	if (nullptr == AIPawn || bIsHit)
	{
		return EBTNodeResult::Failed;
	}

	AIPawn->PerformMoving();

	return EBTNodeResult::Succeeded;
}
