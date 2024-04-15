// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/IFItemInfo.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Data/IFEnumDefine.h"
#include "UI/IFItemInfoBox.h"


UIFItemInfo::UIFItemInfo(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	static ConstructorHelpers::FObjectFinder<UTexture2D> AmmoImage(TEXT("/Script/Engine.Texture2D'/Game/Assets/Art/ItemIcon/Bullet.Bullet'"));
	if(AmmoImage.Object != nullptr)
	{
		ItemTypeImageMap.Add(EItemType::Ammo, AmmoImage.Object);
	}
	static ConstructorHelpers::FObjectFinder<UTexture2D> PotionImage(TEXT("/Script/Engine.Texture2D'/Game/Assets/Art/ItemIcon/Potion.Potion'"));
	if (PotionImage.Object != nullptr)
	{
		ItemTypeImageMap.Add(EItemType::Potion, PotionImage.Object);
	}

	static ConstructorHelpers::FObjectFinder<UTexture2D> GoodsImage(TEXT("/Script/Engine.Texture2D'/Game/Assets/Art/ItemIcon/Goods.Goods'"));
	if (GoodsImage.Object != nullptr)
	{
		ItemTypeImageMap.Add(EItemType::Goods, GoodsImage.Object);
	}

}

void UIFItemInfo::ShowItemData(FIFItemData& ItemData)
{
	ItemBox.Get()->InitItemBox(false);

	ItemNameText.Get()->SetText(FText::FromString(ItemData.GetItemName()));
	switch (ItemData.GetItemType())
	{
	case EItemType::Ammo:
		ItemTypeImage.Get()->SetBrushFromTexture(ItemTypeImageMap[EItemType::Ammo]);
		break;

	case EItemType::Potion:
		ItemTypeImage.Get()->SetBrushFromTexture(ItemTypeImageMap[EItemType::Potion]);
		break;

	case EItemType::Goods:
		ItemTypeImage.Get()->SetBrushFromTexture(ItemTypeImageMap[EItemType::Goods]);
		break;
	}
	
	ItemBox.Get()->BindItemData(ItemData);
}

void UIFItemInfo::NativeConstruct()
{
	Super::NativeConstruct();

	ItemNameText = Cast<UTextBlock>(GetWidgetFromName(TEXT("ItemNameText")));
	ItemTypeImage = Cast<UImage>(GetWidgetFromName(TEXT("ItemTypeImage")));
	ItemBox = Cast<UIFItemInfoBox>(GetWidgetFromName(TEXT("BP_ItemInfoBox")));
}
