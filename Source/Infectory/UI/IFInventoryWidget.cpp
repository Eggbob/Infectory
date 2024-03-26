// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/IFInventoryWidget.h"
#include "UI/IFWeaponBox.h"
#include "UI/IFItemBox.h"
#include "Item/IFGunBase.h"
#include "Item/IFInventory.h"
#include "Data/IFItemData.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
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

	TArray<UUserWidget*> ItemBoxWidgetRef;
	UWidgetBlueprintLibrary::GetAllWidgetsOfClass(GetWorld(), ItemBoxWidgetRef, UIFItemBox::StaticClass(), false);

	for (UUserWidget* Widget : ItemBoxWidgetRef)
	{
		ItemBoxes.Add(Cast<UIFItemBox>(Widget));
	}

	CurCreditText = Cast<UTextBlock>(GetWidgetFromName(TEXT("CurCreditText")));
	CurUpgradeGuierText = Cast<UTextBlock>(GetWidgetFromName(TEXT("CurUpgradeText")));

	CurItemImg = Cast<UImage>(GetWidgetFromName(TEXT("MiniItemImage")));
	CurItemNameText = Cast<UTextBlock>(GetWidgetFromName(TEXT("CurItemText")));
	CurItemExplainText = Cast<UTextBlock>(GetWidgetFromName(TEXT("CurItemExplainText")));

	OpenInventory();
}

void UIFInventoryWidget::BindInventory(UIFInventory& PInven)
{
	PlayerInven = &PInven;
	//SetItem();
	//SetWeapon();
}

void UIFInventoryWidget::OpenInventory()
{
	SetVisibility(ESlateVisibility::Visible);
	
	SetItem();
	SetWeapon();
	SetCredit();
	SetUpgradeGier();

	CurItemImg.Get()->SetVisibility(ESlateVisibility::Hidden);
	CurItemNameText.Get()->SetVisibility(ESlateVisibility::Hidden);
	CurItemExplainText.Get()->SetVisibility(ESlateVisibility::Hidden);
}

void UIFInventoryWidget::CloseInventory()
{
	SetVisibility(ESlateVisibility::Hidden);
}

void UIFInventoryWidget::SetWeapon()
{
	if (PlayerInven.IsValid())
	{
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
	for (UIFItemBox* box : ItemBoxes)
	{
		box->InitItemBox();
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

void UIFInventoryWidget::SelectItem(int32 Index)
{
	FIFItemData CurItem = ItemBoxes[Index].Get()->GetItemData();

	CurItemImg.Get()->SetBrushFromTexture(CurItem.GetIconTexture());
	CurItemNameText.Get()->SetText(FText::FromString(CurItem.GetItemName()));
	CurItemExplainText.Get()->SetText(FText::FromString(CurItem.GetToolTip()));
}
