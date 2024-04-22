// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Boss/IFBossAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AI/IFAI.h"

AIFBossAIController::AIFBossAIController()
{
	static ConstructorHelpers::FObjectFinder<UBlackboardData> NpcBBAssetRef(TEXT("/Script/AIModule.BlackboardData'/Game/Assets/AI/BB_IFNPC.BB_IFNPC'"));
	if (nullptr != NpcBBAssetRef.Object)
	{
		BBAsset = NpcBBAssetRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UBehaviorTree> NPCBTAssetRef(TEXT("/Script/AIModule.BehaviorTree'/Game/Assets/AI/BT_IFBoss.BT_IFBoss'"));
	if (nullptr != NPCBTAssetRef.Object)
	{
		BTAsset = NPCBTAssetRef.Object;
	}
}

void AIFBossAIController::RunAI()
{
	Super::RunAI();
	CurrentPhase = 1;
	Blackboard.Get()->SetValueAsInt(BBKEY_BOSSPHASE, CurrentPhase);
}

void AIFBossAIController::StopAI()
{
	Super::StopAI();
}

void AIFBossAIController::PerformNextPhase()
{
	CurrentPhase++;
	Blackboard.Get()->SetValueAsInt(BBKEY_BOSSPHASE, CurrentPhase);
}

void AIFBossAIController::SetCoolTime(float DelayTime)
{
	Blackboard.Get()->SetValueAsFloat(BBKEY_DELAYTIME, DelayTime);
}

void AIFBossAIController::SetFinishAttack(bool IsFinish)
{
	Blackboard.Get()->SetValueAsBool(BBEKY_BOSSFINISHATTACK, IsFinish);
}

void AIFBossAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	bIsBomb = false;
	RunAI();
}
