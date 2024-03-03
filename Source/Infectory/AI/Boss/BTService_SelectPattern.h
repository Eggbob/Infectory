// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_SelectPattern.generated.h"

/**
 * 
 */
UCLASS()
class INFECTORY_API UBTService_SelectPattern : public UBTService
{
	GENERATED_BODY()
	
public:
	UBTService_SelectPattern();

protected:
	virtual void OnSearchStart(FBehaviorTreeSearchData& SearchData) override;
};
