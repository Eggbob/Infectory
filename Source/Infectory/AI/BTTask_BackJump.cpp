// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_BackJump.h"
#include "AIController.h"
#include "Interface/IFCharacterAIInterface.h"

UBTTask_BackJump::UBTTask_BackJump()
{
	NodeName = ("BackJump");
}

EBTNodeResult::Type UBTTask_BackJump::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
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

	float RandVal = FMath::FRand();
	
	if (RandVal < 0.5f)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		return EBTNodeResult::Succeeded;
	}
	else
	{
		FAICharacterBackJumpFinished OnBackJumpFinished;
		OnBackJumpFinished.BindLambda(
			[&]()
			{
				FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
			}
		);

		AIPawn->SetAIBackJumpDelegate(OnBackJumpFinished);
		AIPawn->PeformBackMoveAI();


		return EBTNodeResult::InProgress;
	}

	
}
