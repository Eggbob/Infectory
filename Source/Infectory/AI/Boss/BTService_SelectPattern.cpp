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

	int32 BossPhase = SearchData.OwnerComp.GetBlackboardComponent()->GetValueAsInt(BBKEY_BOSSPHASE);

	if (!PatternMap.Contains(BossPhase))
	{
		UE_LOG(LogTemp, Warning, TEXT("PatternMap does not contain BossPhase %d"), BossPhase)
			return;
	}
	
	int32 BossPattern = FMath::RandRange(0, PatternMap[BossPhase].BossPatterns.Num() - 1);

	UE_LOG(LogTemp, Warning, TEXT("BossPhase %d, BossPattern %d"), BossPhase, BossPattern);

	SearchData.OwnerComp.GetBlackboardComponent()->SetValueAsInt(BBKEY_BOSSPATTERN, 
		(int32)PatternMap[BossPhase].BossPatterns[BossPattern]);

}
