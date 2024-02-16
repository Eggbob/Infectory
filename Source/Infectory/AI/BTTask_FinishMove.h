// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_FinishMove.generated.h"

/**
 * 
 */
UCLASS()
class INFECTORY_API UBTTask_FinishMove : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTTask_FinishMove();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	float MinSpeed = 50.0f;

private:
	TObjectPtr<class IIFCharacterAIInterface> AIPawn;
	TObjectPtr<APawn> TargetPawn;

	bool bIsHit = false;
	bool bIsReady = false;
	bool bIsMoving = true;
};
