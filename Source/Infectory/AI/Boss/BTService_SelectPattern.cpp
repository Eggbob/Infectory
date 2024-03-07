// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Boss/BTService_SelectPattern.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Math/UnrealMathUtility.h"
#include "AI/IFAI.h"

UBTService_SelectPattern::UBTService_SelectPattern()
{
	Interval = 1.f;
	NodeName = TEXT("SelectPattern");
}

void UBTService_SelectPattern::OnSearchStart(FBehaviorTreeSearchData& SearchData)
{
	Super::OnSearchStart(SearchData);

	int32 BossPase = SearchData.OwnerComp.GetBlackboardComponent()->GetValueAsInt(BBKEY_BOSSPASE);

	int32 BossPattern;

	if (BossPase == 1)
	{
		BossPattern = FMath::RandRange(1, 2);
	}
	else
	{
		BossPattern = FMath::RandRange(1, 4);
	}

	SearchData.OwnerComp.GetBlackboardComponent()->SetValueAsInt(BBKEY_BOSSPATTERN, BossPattern);

	//FMath::RandRange()

}
