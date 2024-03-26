// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/IFItemBox.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
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

	switch (CurItemData.GetItemType())
	{
	case EItemType::Potion:
	case EItemType::Goods:
		ItemCountText.Get()->SetVisibility(ESlateVisibility::Hidden);
		ItemImage.Get()->SetBrushFromTexture(CurItemData.GetIconTexture());
		ItemImage.Get()->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		ItemBackImage.Get()->SetBrushFromTexture(EmptyBackImg);
		break;

	case EItemType::Ammo:
		ItemCountText.Get()->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		ItemCountText.Get()->SetText(FText::FromString(FString::FromInt(CurItemData.GetAmount())));
		ItemImage.Get()->SetBrushFromTexture(CurItemData.GetIconTexture());
		ItemImage.Get()->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		ItemBackImage.Get()->SetBrushFromTexture(EmptyBackImg);
		break;

	case EItemType::None:
		InitItemBox();
		break;
	}
}

void UIFItemBox::InitItemBox()
{
	ItemCountText.Get()->SetVisibility(ESlateVisibility::Hidden);
	ItemImage.Get()->SetVisibility(ESlateVisibility::Hidden);
	ItemBackImage.Get()->SetBrushFromTexture(EmptyBackImg);
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

	ItemImage = Cast<UImage>(GetWidgetFromName(TEXT("ItemImage")));
	ItemBackImage = Cast<UImage>(GetWidgetFromName(TEXT("ItemBackImage")));
	ItemCountText = Cast<UTextBlock>(GetWidgetFromName(TEXT("ItemCountText")));
}
