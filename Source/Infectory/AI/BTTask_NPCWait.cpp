// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_NPCWait.h"
#include "AIController.h"
#include "AI/IFAI.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Interface/IFCharacterAIInterface.h"

UBTTask_NPCWait::UBTTask_NPCWait()
{
	NodeName = ("NPCWait");
}

EBTNodeResult::Type UBTTask_NPCWait::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
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

	FAICharacterWaitingFinished OnWaitingFinished;

	OnWaitingFinished.BindLambda(
		[&]()
		{
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		}
	);

	AIPawn->SetAIWaitingDelegate(OnWaitingFinished);
	AIPawn->PerformWaiting(bIsFirst);

	if (bIsFirst)
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsBool(BBKEY_ISFIRSTCONTACT, false);
	}

	return EBTNodeResult::InProgress;
}
