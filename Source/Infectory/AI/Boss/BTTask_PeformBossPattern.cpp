// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Boss/BTTask_PeformBossPattern.h"
#include "Interface/IFBossAIInterface.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "AI/IFAI.h"
#include "Data/IFEnumDefine.h"

UBTTask_PeformBossPattern::UBTTask_PeformBossPattern()
{
	bNotifyTick = true;
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

	int32 BossPatternNum = OwnerComp.GetBlackboardComponent()->GetValueAsInt(BBKEY_BOSSPATTERN);

	OwnerComp.GetBlackboardComponent()->SetValueAsBool(BBEKY_BOSSFINISHATTACK, false);

	EBossPattern BossPattern = static_cast<EBossPattern>(BossPatternNum);
	AIPawn->CheckPattern(BossPattern);

	//AIPawn->PerformGrabAttack();


	//AIPawn->PerformPierceAttack();
	//AIPawn->PerformRangeAttack();
	//AIPawn->PeformBreathAttack();
	//AIPawn->PerformSpawnEnemy();


	return EBTNodeResult::InProgress;
}

void UBTTask_PeformBossPattern::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	bIsFinishAttack = OwnerComp.GetBlackboardComponent()->GetValueAsBool(BBEKY_BOSSFINISHATTACK);

	if (bIsFinishAttack)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}

}


