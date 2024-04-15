// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/IFItemBox.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Styling/SlateColor.h"
#include "MaterialShared.h"


UIFItemBox::UIFItemBox(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer)
{
	static ConstructorHelpers::FObjectFinder<UTexture2D> EmptyImgRef(TEXT("/Script/Engine.Texture2D'/Game/Assets/Art/ItemIcon/ItemEmpty.ItemEmpty'"));
	if (EmptyImgRef.Object)
	{
		EmptyBackImg = EmptyImgRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UTexture2D> ItemBackImgRef(TEXT("/Script/Engine.Texture2D'/Game/Assets/Art/ItemIcon/ItemBack.ItemBack'"));
	if (ItemBackImgRef.Object)
	{
		ItemBackImg = ItemBackImgRef.Object;
	}
}

void UIFItemBox::BindItemData(FIFItemData NewItemData)
{
	CurItemData = NewItemData;

	/*FLinearColor Linear = FLinearColor(0.f, 0.f, 0.f);
	FSlateColor Slate = FSlateColor();*/

	switch (CurItemData.GetItemType())
	{
	case EItemType::Potion:
	case EItemType::Goods:
		ItemCountText.Get()->SetVisibility(ESlateVisibility::Hidden);
		//SetCurItemImage(CurItemData.GetIconTexture());
		ItemImage.Get()->SetBrushFromTexture(CurItemData.GetIconTexture());
		ItemImage.Get()->SetVisibility(ESlateVisibility::Visible);
		ItemBackImage.Get()->SetBrushFromTexture(EmptyBackImg);
		ItemPriceText.Get()->SetText(FText::FromString(FString::FromInt(CurItemData.GetItemPrice())));
		//ItemBackImage.Get()->SetBrushTintColor(Slate);
		break;

	case EItemType::Ammo:
		ItemCountText.Get()->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		ItemCountText.Get()->SetText(FText::FromString(FString::FromInt(CurItemData.GetAmount())));
		ItemPriceText.Get()->SetText(FText::FromString(FString::FromInt(CurItemData.GetItemPrice())));
		//SetCurItemImage(CurItemData.GetIconTexture());
		ItemImage.Get()->SetBrushFromTexture(CurItemData.GetIconTexture());
		ItemImage.Get()->SetVisibility(ESlateVisibility::Visible);
		ItemBackImage.Get()->SetBrushFromTexture(EmptyBackImg);
		break;

	case EItemType::None:
		//InitItemBox();
		break;
	}
}

void UIFItemBox::InitItemBox(bool bIsShop = false)
{
	/*ItemCountText.Get()->SetVisibility(ESlateVisibility::Visible);
	ItemImage.Get()->SetVisibility(ESlateVisibility::Visible);*/

	ItemCountText.Get()->SetVisibility(ESlateVisibility::Hidden);
	ItemImage.Get()->SetVisibility(ESlateVisibility::Hidden);
	ItemBackImage.Get()->SetBrushFromTexture(EmptyBackImg);

	if (bIsShop)
	{
		ItemPriceText.Get()->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
	else
	{
		ItemPriceText.Get()->SetVisibility(ESlateVisibility::Hidden);
	}

}

void UIFItemBox::ItemSelected(bool bIsSelected)
{
	if (bIsSelected)
	{
		ItemBackImage.Get()->SetBrushFromTexture(ItemBackImg);
	}
	else
	{
		ItemBackImage.Get()->SetBrushFromTexture(EmptyBackImg);
	}
}

FIFItemData UIFItemBox::GetItemData()
{
	if (CurItemData.GetItemType() != EItemType::None)
	{
		return CurItemData;
	}

	return FIFItemData();
}

void UIFItemBox::NativeConstruct()
{
	Super::NativeConstruct();

	ItemImage = Cast<UImage>(GetWidgetFromName(TEXT("CurItemImage")));
	ItemBackImage = Cast<UImage>(GetWidgetFromName(TEXT("ItemBackImage")));
	ItemCountText = Cast<UTextBlock>(GetWidgetFromName(TEXT("ItemCountText")));
	ItemPriceText = Cast<UTextBlock>(GetWidgetFromName(TEXT("ItemPriceText")));
}
