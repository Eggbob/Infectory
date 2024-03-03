// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_ChangeBomb.h"
#include "AIController.h"
#include "Interface/IFCharacterAIInterface.h"

UBTTask_ChangeBomb::UBTTask_ChangeBomb()
{
	NodeName = ("ChangeBomb");
}

EBTNodeResult::Type UBTTask_ChangeBomb::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

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

	AIPawn->ChangeToBomb();

	return EBTNodeResult::Succeeded;
}
