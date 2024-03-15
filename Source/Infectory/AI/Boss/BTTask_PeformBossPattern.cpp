// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Boss/BTTask_PeformBossPattern.h"
#include "Interface/IFBossAIInterface.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "AI/IFAI.h"
#include "Data/IFEnumDefine.h"

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

	int32 BossPatternNum = OwnerComp.GetBlackboardComponent()->GetValueAsInt(BBKEY_BOSSPATTERN);

	EBossPattern BossPattern = static_cast<EBossPattern>(BossPatternNum);

	/*switch (BossPattern)
	{
		case EBossPattern::Pierce:
			AIPawn->PerformPierceAttack();
			break;

		case EBossPattern::SpawnBomb:
			AIPawn->PerformSpawnBoomer();
			break;

		case EBossPattern::Range:
			AIPawn->PerformRangeAttack();
			break;

		case EBossPattern::Breath:
			AIPawn->PeformBreathAttack();
			break;
	}*/

	//AIPawn->PerformGrabAttack();

	//AIPawn->PerformPierceAttack();
	//AIPawn->PerformRangeAttack();
	//AIPawn->PeformBreathAttack();
	//AIPawn->PerformSpawnBoomer();
	return EBTNodeResult::Succeeded;
}


