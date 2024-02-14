// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_FinishMove.h"
#include "AI/IFAI.h"
#include "AIController.h"
#include "Interface/IFCharacterAIInterface.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_FinishMove::UBTTask_FinishMove()
{
	bNotifyTick = true;
	NodeName = TEXT("FinishMove");
}

EBTNodeResult::Type UBTTask_FinishMove::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	bIsReady = false;

	TObjectPtr<APawn> ControllingPawn = Cast<APawn>(OwnerComp.GetAIOwner()->GetPawn());
	if (nullptr == ControllingPawn)
	{
		return EBTNodeResult::Failed;
	}
	
	AIPawn = Cast<IIFCharacterAIInterface>(ControllingPawn);
	if (nullptr == AIPawn)
	{
		return EBTNodeResult::Failed;
	}

	FTimerHandle WaitHandle;
	GetWorld()->GetTimerManager().SetTimer(WaitHandle, FTimerDelegate::CreateLambda([&]()
		{
			bIsReady = true;
		}), 0.5f, false);

	return EBTNodeResult::InProgress;
}


void UBTTask_FinishMove::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	bIsHit = OwnerComp.GetBlackboardComponent()->GetValueAsBool(BBKEY_ISHIT);
	
	if (bIsReady && (bIsHit || MinSpeed >= OwnerComp.GetAIOwner()->GetPawn()->GetVelocity().Size()))
	{
		//OwnerComp.GetBlackboardComponent()->SetValueAsBool(BBKEY_ISHIT, false);
		AIPawn.Get()->StopMoving();
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}
