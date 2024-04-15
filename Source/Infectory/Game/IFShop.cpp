// Fill out your copyright notice in the Description page of Project Settings.

#include "Game/IFShop.h"
#include "Components/WidgetComponent.h"
#include "UI/IFShopWidget.h"

AIFShop::AIFShop()
{
	static ConstructorHelpers::FObjectFinder<UDataTable> ItemDataTableRef(TEXT("/Script/Engine.DataTable'/Game/Assets/GameData/IFShopSellingData.IFShopSellingData'"));
	if (nullptr != ItemDataTableRef.Object)
	{
		SaleDataTable = ItemDataTableRef.Object;

		TArray<FName> RowNames = SaleDataTable->GetRowNames();

		for (FName& RName : RowNames)
		{
			FIFItemData* ItemStat = SaleDataTable->FindRow<FIFItemData>(RName, TEXT(""));

			if (nullptr != ItemStat)
			{
				SaleItemList.Add(*ItemStat);
			}
		}
	}
}

void AIFShop::OpenShopWidget()
{
	if (ShopWidget == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("ShopWidget is null"));
	}
	else
	{
		ShopWidget.Get()->SetVisibility(ESlateVisibility::Visible);
		ShopWidget.Get()->OpenWidget(true);
	}
}

void AIFShop::BeginPlay()
{
	Super::BeginPlay();
	
	TArray<UWidgetComponent*> WidgetComponents;
	this->GetComponents<UWidgetComponent>(WidgetComponents);

	for (UWidgetComponent* WidgetComponent : WidgetComponents)
	{
		if (WidgetComponent->GetWidget()->IsA<UIFShopWidget>())
		{
			ShopWidget = Cast<UIFShopWidget>(WidgetComponent->GetUserWidgetObject());
		}
	
	}

	if (ShopWidget == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("ShopWidget is null"));
	}
	else
	{
		ShopWidget.Get()->BindShopItemList(SaleItemList);
	}
}




