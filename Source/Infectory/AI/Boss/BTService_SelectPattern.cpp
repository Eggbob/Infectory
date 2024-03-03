// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Boss/BTService_SelectPattern.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTService_SelectPattern::UBTService_SelectPattern()
{
	Interval = 1.f;
	NodeName = TEXT("SelectPattern");
}

void UBTService_SelectPattern::OnSearchStart(FBehaviorTreeSearchData& SearchData)
{

	Super::OnSearchStart(SearchData);
}
