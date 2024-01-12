// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_Moving.h"
#include "AIController.h"
#include "Interface/IFCharacterAIInterface.h"

UBTTask_Moving::UBTTask_Moving()
{
	NodeName = ("BeforeMoving");
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
	if (nullptr == AIPawn)
	{
		return EBTNodeResult::Failed;
	}

	FAICharacterBeforeMovingFinished OnBeforeMovingFinished;

	OnBeforeMovingFinished.BindLambda(
		[&]()
		{
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		}
	);

	AIPawn->SetAIBeforeMovingDelegate(OnBeforeMovingFinished);
	AIPawn->PerformMoving();

	return EBTNodeResult::InProgress;
}
