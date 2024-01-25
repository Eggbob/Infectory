// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_FocusTarget.h"
#include "AIController.h"
#include "IFAI.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Interface/IFCharacterAIInterface.h"

UBTTask_FocusTarget::UBTTask_FocusTarget()
{
	NodeName = TEXT("FocusTarget");
}

EBTNodeResult::Type UBTTask_FocusTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
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

	APawn* Target = Cast<APawn>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(BBKEY_TARGET));
	if (nullptr == Target)
	{
		return EBTNodeResult::Failed;
	}

	FVector LookAtDirection = UKismetMathLibrary::GetDirectionUnitVector(ControllingPawn->GetActorLocation(), Target->GetActorLocation());

	FRotator LookAtRotation = LookAtDirection.Rotation();
	LookAtRotation.Pitch = 0.0f;
	ControllingPawn->SetActorRotation(LookAtRotation);
	//AIPawn->FocusingTarget(Target);


	return EBTNodeResult::Succeeded;
}
