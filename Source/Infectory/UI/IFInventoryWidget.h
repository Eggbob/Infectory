// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "IFInventoryWidget.generated.h"

/**
 * 
 */
UCLASS()
class INFECTORY_API UIFInventoryWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void BindInventory(class UIFInventory& PInven);
	void OpenInventory();
	void CloseInventory();
	void SelectItem(FVector2D Direction);
	void UseItem();

protected:
	virtual void NativeConstruct() override;
	void SetWeapon();
	void SetItem();
	void SetCredit();
	void SetUpgradeGier();

private:
	int32 CurItemIndex = 0;

	UPROPERTY()
	TArray<TObjectPtr<class UIFWeaponBox>> WeaponWidgets;
	UPROPERTY()
	TArray<TObjectPtr<class UIFItemBox>> ItemBoxes;

	//UPROPERTY()
	//TArray<TObjectPtr<class UIFItemInfoBox>> ItemBoxes;

	UPROPERTY()
	TObjectPtr<class UImage> CurItemImg;
	UPROPERTY()
	TObjectPtr<class UTextBlock> CurItemNameText;
	UPROPERTY()
	TObjectPtr<class UTextBlock> CurItemExplainText;

	UPROPERTY()
	TObjectPtr<class UTextBlock> CurCreditText;
	UPROPERTY()
	TObjectPtr<class UTextBlock> CurUpgradeGuierText;

	UPROPERTY()
	TObjectPtr<class UScrollBox> ScrollBox;
	UPROPERTY()
	TWeakObjectPtr<class UIFInventory> PlayerInven;
};
