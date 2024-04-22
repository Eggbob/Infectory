// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/IFItemInfoBox.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Data/IFGameSingleton.h"
#include "Styling/SlateColor.h"
#include "MaterialShared.h"

UIFItemInfoBox::UIFItemInfoBox(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
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

void UIFItemInfoBox::BindItemData(FIFItemData NewItemData)
{
	CurItemData = NewItemData;

	/*FLinearColor Linear = FLinearColor(0.f, 0.f, 0.f);
	FSlateColor Slate = FSlateColor();*/

	switch (CurItemData.GetItemType())
	{
	case EItemType::Potion:
	case EItemType::Goods:
		ItemCountText.Get()->SetVisibility(ESlateVisibility::Hidden);
		//ItemImage.Get()->SetBrushFromTexture(CurItemData.GetIconTexture());
		ItemImage.Get()->SetBrushFromTexture(UIFGameSingleton::Get().GetItemIcon(CurItemData.GetID()));
		ItemImage.Get()->SetVisibility(ESlateVisibility::Visible);
		//ChangeItemImage(CurItemData.GetIconTexture());
		ItemBackImage.Get()->SetBrushFromTexture(EmptyBackImg);
		ItemPriceText.Get()->SetText(FText::FromString(FString::FromInt(CurItemData.GetItemPrice())));
		//ItemBackImage.Get()->SetBrushTintColor(Slate);
		break;

	case EItemType::Ammo:
		ItemCountText.Get()->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		ItemCountText.Get()->SetText(FText::FromString(FString::FromInt(CurItemData.GetAmount())));
		ItemPriceText.Get()->SetText(FText::FromString(FString::FromInt(CurItemData.GetItemPrice())));
		ItemImage.Get()->SetVisibility(ESlateVisibility::Visible);
		ItemImage.Get()->SetBrushFromTexture(UIFGameSingleton::Get().GetItemIcon(CurItemData.GetID()));
		//ChangeItemImage(CurItemData.GetIconTexture());
		ItemBackImage.Get()->SetBrushFromTexture(EmptyBackImg);
		break;

	case EItemType::None:
		//InitItemBox();
		break;
	}
}

void UIFItemInfoBox::InitItemBox(bool bIsShop)
{
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

void UIFItemInfoBox::ItemSelected(bool bIsSelected)
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

FIFItemData UIFItemInfoBox::GetItemData()
{
	if (CurItemData.GetItemType() != EItemType::None)
	{
		return CurItemData;
	}

	return FIFItemData();
}

void UIFItemInfoBox::NativeConstruct()
{
	Super::NativeConstruct();

	ItemImage = Cast<UImage>(GetWidgetFromName(TEXT("CurItemImage")));
	ItemBackImage = Cast<UImage>(GetWidgetFromName(TEXT("ItemBackImage")));
	ItemCountText = Cast<UTextBlock>(GetWidgetFromName(TEXT("ItemCountText")));
	ItemPriceText = Cast<UTextBlock>(GetWidgetFromName(TEXT("ItemPriceText")));
}
