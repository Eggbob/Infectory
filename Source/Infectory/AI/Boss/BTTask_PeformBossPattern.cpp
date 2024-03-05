// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Boss/BTTask_PeformBossPattern.h"
#include "Interface/IFBossAIInterface.h"
#include "AIController.h"

UBTTask_PeformBossPattern::UBTTask_PeformBossPattern()
{
}

EBTNodeResult::Type UBTTask_PeformBossPattern::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	TObjectPtr<APawn> ControllingPawn = Cast<APawn>(OwnerComp.GetAIOwner()->GetPawn());
	if (nullptr == ControllingPawn)
	{
		return EBTNodeResult::Failed;
	}

	IIFBossAIInterface* AIPawn = Cast<IIFBossAIInterface>(ControllingPawn);
	if (nullptr == AIPawn)
	{
		return EBTNodeResult::Failed;
	}

	//AIPawn->PerformPierceAttack();
	AIPawn->PerformRangeAttack();

	return EBTNodeResult::Succeeded;
}

void UBTTask_PeformBossPattern::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
	
}
