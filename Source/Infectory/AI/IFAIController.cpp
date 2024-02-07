// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/IFAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/KismetMathLibrary.h"
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
		Blackboard.Get()->SetValueAsBool(BBKEY_ISFIRSTCONTACT, true);
		Blackboard.Get()->SetValueAsBool(BBKEY_ISHIT, false);

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

void AIFAIController::MoveToTarget(float Range)
{
	float ExceptRange = FMath::RandRange(50.f, Range - 150.f);

	TWeakObjectPtr<AActor> Target = Cast<AActor>(Blackboard.Get()->GetValueAsObject(BBKEY_TARGET));
	MoveToActor(Target.Get(), ExceptRange, true, true, true, 0, true);
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

void AIFAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	RunAI();
}


