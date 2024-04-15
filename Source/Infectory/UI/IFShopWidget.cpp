// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/IFShopWidget.h"
#include "UI/IFItemBox.h"
#include "UI/IFItemInfoBox.h"
#include "Item/IFInventory.h"
#include "Components/Image.h"
#include "Components/PanelWidget.h"
#include "Components/TextBlock.h"
#include "Blueprint/WidgetBlueprintLibrary.h"


void UIFShopWidget::BindInventory(UIFInventory* PInven)
{
	PlayerInven = PInven;
}

void UIFShopWidget::SelectItem(FVector2D Direction)
{
	TArray<FIFItemData> ItemList;

	if (bIsShop)
	{
		ItemList = ShopItemList;
	}
	else
	{
		ItemList = PlayerInven.Get()->GetItemList();
	}
	
	ItemBoxes[CurItemIndex].Get()->ItemSelected(false);

	CurItemIndex = FMath::Clamp(CurItemIndex + (Direction.X * -5), 0, ItemList.Num()-1);
	CurItemIndex = FMath::Clamp(CurItemIndex + Direction.Y, 0, ItemList.Num()-1);

	if (CurItemIndex >= ItemList.Num())
	{
		CurItemIndex = ItemList.Num()-1;
	}

	ItemBoxes[CurItemIndex].Get()->ItemSelected(true);

	FIFItemData CurItem = ItemBoxes[CurItemIndex].Get()->GetItemData();

	if (CurItem.GetItemType() != EItemType::None)
	{
		MiniItemImage.Get()->SetBrushFromTexture(CurItem.GetIconTexture());
		CurItemNameText.Get()->SetText(FText::FromString(CurItem.GetItemName()));
		CurItemExplainText.Get()->SetText(FText::FromString(CurItem.GetToolTip()));
		//ScrollBox.Get()->ScrollToEnd();
	}
}

void UIFShopWidget::BindShopItemList(TArray<FIFItemData>& InItemList)
{
	ShopItemList = InItemList;
}


void UIFShopWidget::PerformEnterAction()
{
	if (PayPanel.Get()->IsVisible())
	{
		PerformPay();
	}
	else
	{
		OpenPayPanel();
	}
}

void UIFShopWidget::PerformTabAction()
{
	if (PayPanel.Get()->IsVisible())
	{
		PayPanel.Get()->SetVisibility(ESlateVisibility::Hidden);
	}
	else
	{
		CloseWidget();
	}
}

void UIFShopWidget::PerformPay()
{
	FIFItemData CurItem = ItemBoxes[CurItemIndex].Get()->GetItemData();

	if (bIsShop)
	{
		if (PlayerInven.Get()->GetCurCredit() >= CurItem.GetItemPrice())
		{
			PlayerInven.Get()->AddItem(CurItem);
			PlayerInven.Get()->SetCredit(PlayerInven.Get()->GetCurCredit() - CurItem.GetItemPrice());
			
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Not enough credit"));
		}
	}
	else
	{

		PlayerInven.Get()->SetCredit(PlayerInven.Get()->GetCurCredit() + CurItem.GetItemPrice());
		PlayerInven.Get()->RemoveItem(CurItemIndex);
	}
	
	SetCredit();
	SetSellingItem();
	PayPanel.Get()->SetVisibility(ESlateVisibility::Hidden);
}

void UIFShopWidget::OpenPayPanel()
{
	FIFItemData CurItem = ItemBoxes[CurItemIndex].Get()->GetItemData();

	PayPanel.Get()->SetVisibility(ESlateVisibility::Visible);
	PayItemText.Get()->SetText(FText::FromString(CurItem.GetItemName()));
	PayItemImage.Get()->SetBrushFromTexture(CurItem.GetIconTexture());
	PayItemCountText.Get()->SetText(FText::FromString(FString::Printf(TEXT("%d"), CurItem.GetAmount())));
	PayItemPriceText.Get()->SetText(FText::FromString(FString::Printf(TEXT("%d"), CurItem.GetItemPrice())));
}

void UIFShopWidget::OpenWidget(bool InbIsShop)
{
	bIsShop = InbIsShop;

	SetSellingItem();
	SetCredit();
	SetUpgradeGier();

	CurItemIndex = 0;
	SelectItem(FVector2D(0, 0));
}

void UIFShopWidget::CloseWidget()
{
	SetVisibility(ESlateVisibility::Hidden);
}


void UIFShopWidget::NativeConstruct()
{
	Super::NativeConstruct();

	TArray<UUserWidget*> ItemBoxWidgetRef;
	UWidgetBlueprintLibrary::GetAllWidgetsOfClass(GetWorld(), ItemBoxWidgetRef, UIFItemBox::StaticClass(), false);

	for (UUserWidget* Widget : ItemBoxWidgetRef)
	{
		ItemBoxes.Add(Cast<UIFItemBox>(Widget));
	}

	CurCreditText = Cast<UTextBlock>(GetWidgetFromName(TEXT("CurCreditText")));
	CurUpgradeGuierText = Cast<UTextBlock>(GetWidgetFromName(TEXT("CurUpgradeText")));

	CurItemNameText = Cast<UTextBlock>(GetWidgetFromName(TEXT("CurItemText")));
	CurItemExplainText = Cast<UTextBlock>(GetWidgetFromName(TEXT("CurItemExplainText")));
	MiniItemImage = Cast<UImage>(GetWidgetFromName(TEXT("MiniItemImage")));
	
	PayPanel = Cast<UPanelWidget>(GetWidgetFromName(TEXT("PayPanel")));
	PayItemText = Cast<UTextBlock>(GetWidgetFromName(TEXT("PayItemText")));
	PayItemImage = Cast<UImage>(GetWidgetFromName(TEXT("PayItemImage")));
	PayItemCountText = Cast<UTextBlock>(GetWidgetFromName(TEXT("PayItemCountText")));
	PayItemPriceText = Cast<UTextBlock>(GetWidgetFromName(TEXT("PayItemPriceText")));
	
}

void UIFShopWidget::SetSellingItem()
{
	TArray<FIFItemData> ItemList;

	if (bIsShop)
	{
		ItemList = ShopItemList;
	}
	else
	{
		ItemList = PlayerInven.Get()->GetItemList();
	}

	for (UIFItemBox* box : ItemBoxes)
	{
		box->InitItemBox(true);
		//box->SetVisibility(ESlateVisibility::Hidden);
	}

	for (int32 i = 0; i < ItemList.Num(); i++)
	{
		ItemBoxes[i].Get()->BindItemData(ItemList[i]);
		ItemBoxes[i].Get()->SetVisibility(ESlateVisibility::Visible);
	}
}

void UIFShopWidget::SetCredit()
{
	CurCreditText.Get()->SetText(FText::FromString(FString::Printf(TEXT("%d"), PlayerInven.Get()->GetCurCredit())));
}

void UIFShopWidget::SetUpgradeGier()
{
	CurUpgradeGuierText.Get()->SetText(FText::FromString(FString::Printf(TEXT("%d"), PlayerInven.Get()->GetCurUpGradeGier())));
}
