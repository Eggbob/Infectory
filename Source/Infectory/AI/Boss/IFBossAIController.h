// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/IFAIController.h"
#include "IFBossAIController.generated.h"

/**
 * 
 */
UCLASS()
class INFECTORY_API AIFBossAIController : public AIFAIController
{
	GENERATED_BODY()
	
public:
	AIFBossAIController();
	
	virtual void RunAI();
	virtual void StopAI();

protected:
	virtual void OnPossess(APawn* InPawn) override;
};
