// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Interface/IFCharacterAIInterface.h"
#include "IFAIController.generated.h"

/**
 * 
 */
UCLASS()
class INFECTORY_API AIFAIController : public AAIController
{
	GENERATED_BODY()
	
public:
	AIFAIController();
	
	virtual void RunAI();
	virtual void StopAI();
	void SetTarget(TObjectPtr<AActor> Target);
	void MoveToTarget(float Range);
	bool CheckPath();

	virtual void UpdateControlRotation(float DeltaTime, bool bUpdatePawn = true) override;
	virtual void OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result) override;


protected:
	virtual void OnPossess(APawn* InPawn) override;


protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SmoothFocusInterpSpeed = 60.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<class UBlackboardData> BBAsset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<class UBehaviorTree> BTAsset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<class UBehaviorTree> BombBTAsset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsBomb = false;

	FRotator SmoothTargetRotation;
	bool bHasTarget = false;
};
