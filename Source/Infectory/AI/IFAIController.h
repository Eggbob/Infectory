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
	
	void RunAI();
	void StopAI();
	void SetTarget(TObjectPtr<AActor> Target);
	virtual void UpdateControlRotation(float DeltaTime, bool bUpdatePawn = true) override;

protected:
	virtual void OnPossess(APawn* InPawn) override;


protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SmoothFocusInterpSpeed = 60.0f;

private:
	UPROPERTY()
	TObjectPtr<class UBlackboardData> BBAsset;

	UPROPERTY()
	TObjectPtr<class UBehaviorTree> BTAsset;

	FRotator SmoothTargetRotation;

	bool bHasTarget = false;
};
