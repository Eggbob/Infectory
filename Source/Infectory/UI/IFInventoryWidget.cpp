// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/IFInventoryWidget.h"
#include "UI/IFItemInfoBox.h"
#include "UI/IFWeaponBox.h"
#include "UI/IFItemBox.h"
#include "Item/IFGunBase.h"
#include "Item/IFInventory.h"
#include "Data/IFItemData.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/ScrollBox.h"
#include "Data/IFGameSingleton.h"
#include "Blueprint/WidgetBlueprintLibrary.h"

void UIFInventoryWidget::NativeConstruct()
{
	Super::NativeConstruct();

	TArray<UUserWidget*> AmmoWidgetRef;
	UWidgetBlueprintLibrary::GetAllWidgetsOfClass(GetWorld(), AmmoWidgetRef, UIFWeaponBox::StaticClass(), false);

	for (UUserWidget* Widget : AmmoWidgetRef)
	{
		WeaponWidgets.Add(Cast<UIFWeaponBox>(Widget));
	}

	//TArray<UUserWidget*> ItemBoxWidgetRef;
	//UWidgetBlueprintLibrary::GetAllWidgetsOfClass(GetWorld(), ItemBoxWidgetRef, UIFItemInfoBox::StaticClass(), false);

	//for (UUserWidget* Widget : ItemBoxWidgetRef)
	//{
	//	//ItemBoxes.Add(Cast<UIFItemInfoBox>(Widget));
	//	ItemBoxes.Add(Cast<UIFItemInfoBox>(Widget));
	//}

	for (int32 i = 1; i < 26; i++)
	{
		FString ItemBoxName = FString::Printf(TEXT("ItemInfoBox_%d"), i);
		UIFItemInfoBox* ItemBox = Cast<UIFItemInfoBox>(GetWidgetFromName(*ItemBoxName));
		ItemBoxes.Add(ItemBox);
	}


	CurCreditText = Cast<UTextBlock>(GetWidgetFromName(TEXT("CurCreditText")));
	CurUpgradeGuierText = Cast<UTextBlock>(GetWidgetFromName(TEXT("CurUpgradeText")));

	CurItemImg = Cast<UImage>(GetWidgetFromName(TEXT("MiniItemImage")));
	CurItemNameText = Cast<UTextBlock>(GetWidgetFromName(TEXT("CurItemText")));
	CurItemExplainText = Cast<UTextBlock>(GetWidgetFromName(TEXT("CurItemExplainText")));
	ScrollBox = Cast<UScrollBox>(GetWidgetFromName(TEXT("ScrollBox")));

	SetVisibility(ESlateVisibility::Hidden);
}

void UIFInventoryWidget::BindInventory(UIFInventory& PInven)
{
	PlayerInven = &PInven;
}

void UIFInventoryWidget::OpenInventory()
{
	SetVisibility(ESlateVisibility::Visible);
	
	SetItem();
	SetWeapon();
	SetCredit();
	SetUpgradeGier();

	FVector2D Vect = FVector2D(0, 0);
	CurItemIndex = 0;
	SelectItem(Vect);
}

void UIFInventoryWidget::CloseInventory()
{
	SetVisibility(ESlateVisibility::Hidden);
}

void UIFInventoryWidget::SetWeapon()
{
	if (PlayerInven.IsValid())
	{
		PlayerInven.Get()->CheckTotalAmmo(ERangedWeaponType::Rifle);
		PlayerInven.Get()->CheckTotalAmmo(ERangedWeaponType::ShotGun);
		PlayerInven.Get()->CheckTotalAmmo(ERangedWeaponType::Projectile);

		AIFGunBase& WeaponRifle = PlayerInven.Get()->GetRangedWeapon(ERangedWeaponType::Rifle);
		AIFGunBase& WeaponShotGun = PlayerInven.Get()->GetRangedWeapon(ERangedWeaponType::ShotGun);
		AIFGunBase& WeaponProjectile = PlayerInven.Get()->GetRangedWeapon(ERangedWeaponType::Projectile);

		WeaponWidgets[0]->SetWeapon(ERangedWeaponType::Rifle, WeaponRifle.GetCurAmmo(), WeaponRifle.GetTotalAmmo());
		WeaponWidgets[1]->SetWeapon(ERangedWeaponType::ShotGun, WeaponShotGun.GetCurAmmo(), WeaponShotGun.GetTotalAmmo());
		WeaponWidgets[2]->SetWeapon(ERangedWeaponType::Projectile, WeaponProjectile.GetCurAmmo(), WeaponProjectile.GetTotalAmmo());
	}
}

void UIFInventoryWidget::SetItem()
{
	for (UIFItemInfoBox* box : ItemBoxes)
	{
		box->InitItemBox(false);
	}
	
	TArray<FIFItemData> Items = PlayerInven.Get()->GetItemList();

	for (int32 i = 0; i < Items.Num(); i++)
	{
		ItemBoxes[i].Get()->BindItemData(Items[i]);
	}
}

void UIFInventoryWidget::SetCredit()
{
	CurCreditText.Get()->SetText(FText::FromString(FString::Printf(TEXT("%d"), PlayerInven.Get()->GetCurCredit())));
}

void UIFInventoryWidget::SetUpgradeGier()
{
	CurUpgradeGuierText.Get()->SetText(FText::FromString(FString::Printf(TEXT("%d"), PlayerInven.Get()->GetCurUpGradeGier())));
}

void UIFInventoryWidget::SelectItem(FVector2D Direction)
{
	ItemBoxes[CurItemIndex].Get()->ItemSelected(false);

	CurItemIndex = FMath::Clamp(CurItemIndex + (Direction.X * -5), 0, 24);
	CurItemIndex = FMath::Clamp(CurItemIndex + Direction.Y, 0, 24);

	ItemBoxes[CurItemIndex].Get()->ItemSelected(true);

	FIFItemData CurItem = ItemBoxes[CurItemIndex].Get()->GetItemData();

	if (CurItem.GetItemType() != EItemType::None)
	{
		//CurItemImg.Get()->SetBrushFromTexture(CurItem.GetIconTexture());
		CurItemImg.Get()->SetBrushFromTexture(UIFGameSingleton::Get().GetItemIcon(CurItem.GetID()));
		CurItemNameText.Get()->SetText(FText::FromString(CurItem.GetItemName()));
		CurItemExplainText.Get()->SetText(FText::FromString(CurItem.GetToolTip()));
	}

	if (CurItemIndex > 15)
	{
		ScrollBox.Get()->ScrollToEnd();
	}
	else
	{
		ScrollBox.Get()->ScrollToStart();
	}

}

void UIFInventoryWidget::UseItem()
{
	if (ItemBoxes[CurItemIndex].Get()->GetItemData().GetItemType() == EItemType::Potion)
	{
		PlayerInven.Get()->UseItem(CurItemIndex);
		SetItem();
	}

}
