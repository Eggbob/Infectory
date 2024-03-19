// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "Data/IFEnumDefine.h"
#include "BTService_SelectPattern.generated.h"


USTRUCT(BlueprintType)
struct FBossPahsePattern
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<EBossPattern> BossPatterns;
};


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

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pattern")
	TMap<int32, FBossPahsePattern> PatternMap;
};
