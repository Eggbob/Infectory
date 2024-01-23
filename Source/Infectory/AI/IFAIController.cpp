// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/IFAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "IFAI.h"

AIFAIController::AIFAIController()
{
	static ConstructorHelpers::FObjectFinder<UBlackboardData> BBAssetRef(TEXT("/Script/AIModule.BlackboardData'/Game/Assets/AI/BB_IFNPC.BB_IFNPC'"));
	if (nullptr != BBAssetRef.Object)
	{
		BBAsset = BBAssetRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BTAssetRef(TEXT("/Script/AIModule.BehaviorTree'/Game/Assets/AI/BT_IFNPC.BT_IFNPC'"));
	if (nullptr != BTAssetRef.Object)
	{
		BTAsset = BTAssetRef.Object;
	}
}

void AIFAIController::RunAI()
{
	UBlackboardComponent* BlackboardPtr = Blackboard.Get();
	if (UseBlackboard(BBAsset, BlackboardPtr))
	{
		Blackboard.Get()->SetValueAsVector("HomePos", GetPawn()->GetActorLocation());
		Blackboard.Get()->SetValueAsBool("bIsFirstContact", true);

		bHasTarget = false;
		bool RunResult = RunBehaviorTree(BTAsset);
		ensure(RunResult);
	}
}

void AIFAIController::StopAI()
{
	UBehaviorTreeComponent* BTComponent = Cast<UBehaviorTreeComponent>(BrainComponent);
	if (BTComponent)
	{
		BTComponent->StopTree();
	}
}

void AIFAIController::SetTarget(TObjectPtr<AActor> Target)
{
	if (bHasTarget) return;

	Blackboard.Get()->SetValueAsObject(BBKEY_TARGET, Target);
	bHasTarget = true;
}

void AIFAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	RunAI();
}


