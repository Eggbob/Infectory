// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/IFAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "NavigationSystem.h"
#include "NavigationPath.h"
#include "IFAI.h"

AIFAIController::AIFAIController()
{
	static ConstructorHelpers::FObjectFinder<UBlackboardData> NpcBBAssetRef(TEXT("/Script/AIModule.BlackboardData'/Game/Assets/AI/BB_IFNPC.BB_IFNPC'"));
	if (nullptr != NpcBBAssetRef.Object)
	{
		BBAsset = NpcBBAssetRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UBehaviorTree> NPCBTAssetRef(TEXT("/Script/AIModule.BehaviorTree'/Game/Assets/AI/BT_IFNPC.BT_IFNPC'"));
	if (nullptr != NPCBTAssetRef.Object)
	{
		BTAsset = NPCBTAssetRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BombBTAssetRef(TEXT("/Script/AIModule.BehaviorTree'/Game/Assets/AI/BT_IFBoomer.BT_IFBoomer'"));
	if (nullptr != BombBTAssetRef.Object)
	{
		BombBTAsset = BombBTAssetRef.Object;
	}
}

void AIFAIController::RunAI()
{
	UBlackboardComponent* BlackboardPtr = Blackboard.Get();
	
	if (UseBlackboard(BBAsset, BlackboardPtr))
	{
		Blackboard.Get()->SetValueAsBool(BBKEY_ISFIRSTCONTACT, true);
		Blackboard.Get()->SetValueAsBool(BBKEY_ISMOVING, false);
		Blackboard.Get()->SetValueAsBool(BBKEY_ISHIT, false);
		Blackboard->SetValueAsVector(BBKEY_HOMEPOS, GetPawn()->GetActorLocation());

		bHasTarget = false;

		UBehaviorTree* TreeAsset = bIsBomb ? BombBTAsset : BTAsset;
		bool RunResult = RunBehaviorTree(TreeAsset);
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

void AIFAIController::MoveToTarget(float Range)
{
	float ExceptRange = -10.f;// FMath::RandRange(100.f, 190.f);

	TWeakObjectPtr<AActor> Target = Cast<AActor>(Blackboard.Get()->GetValueAsObject(BBKEY_TARGET));
	MoveToActor(Target.Get(), ExceptRange, true, true, true, 0, true);

	Blackboard.Get()->SetValueAsBool(BBKEY_ISMOVING, true);
}

bool AIFAIController::CheckPath()
{
	FVector PathStart = GetPawn()->GetActorLocation();
	TWeakObjectPtr<AActor> Target = Cast<AActor>(Blackboard.Get()->GetValueAsObject(BBKEY_TARGET));


	UNavigationPath* NavPath = UNavigationSystemV1::FindPathToLocationSynchronously(this, PathStart, Target->GetActorLocation(), NULL);
	
	if (!NavPath) return false;

	return !NavPath->IsPartial();
}


void AIFAIController::UpdateControlRotation(float DeltaTime, bool bUpdatePawn)
{
	Super::UpdateControlRotation(DeltaTime, bUpdatePawn);

	/*if (bUpdatePawn)
	{
		const FRotator CurrentPawnRotation = GetPawn()->GetActorRotation();

		SmoothTargetRotation = UKismetMathLibrary::RInterpTo_Constant(GetPawn()->GetActorRotation(), ControlRotation, DeltaTime, SmoothFocusInterpSpeed);

		if (CurrentPawnRotation.Equals(SmoothTargetRotation, 1e-3f) == false)
		{
			GetPawn()->FaceRotation(SmoothTargetRotation, DeltaTime);
		}
	}*/
}

void AIFAIController::OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result)
{
	Blackboard.Get()->SetValueAsBool(BBKEY_ISMOVING, false);
}

void AIFAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	RunAI();

}


