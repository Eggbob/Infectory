// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Data/IFItemData.h"
#include "Blueprint/UserWidget.h"
#include "IFShopWidget.generated.h"

/**
 * 
 */
UCLASS()
class INFECTORY_API UIFShopWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	void PerformEnterAction();

	UFUNCTION(BlueprintCallable)
	void PerformTabAction();

	UFUNCTION(BlueprintCallable)
	void OpenWidget(bool InbIsShop);

	UFUNCTION(BlueprintCallable)
	void CloseWidget();

	UFUNCTION(BlueprintCallable)
	void BindInventory(class UIFInventory* PInven);

	UFUNCTION(BlueprintCallable)
	void SelectItem(FVector2D Direction);

	void BindShopItemList(TArray<FIFItemData>& ItemList);

protected:
	virtual void NativeConstruct() override;
	void SetSellingItem();
	void SetCredit();
	void SetUpgradeGier();

	void PerformPay();
	void OpenPayPanel();


private:
	int32 CurItemIndex = 0;
	bool bIsShop = true;

	UPROPERTY()
	TArray<FIFItemData> ShopItemList;

	UPROPERTY()
	TArray<TObjectPtr<class UIFItemInfoBox>> ItemBoxes;

	UPROPERTY()
	TObjectPtr<class UTextBlock> CurCreditText;

	UPROPERTY()
	TObjectPtr<class UTextBlock> CurUpgradeGuierText;

	UPROPERTY()
	TObjectPtr<class UPanelWidget> PayPanel;
	UPROPERTY()
	TObjectPtr<class UTextBlock> PayItemText;
	UPROPERTY()
	TObjectPtr<class UImage> PayItemImage;
	UPROPERTY()
	TObjectPtr<class UTextBlock> PayItemCountText;
	UPROPERTY()
	TObjectPtr<class UTextBlock> PayItemPriceText;

	UPROPERTY()
	TObjectPtr<class UTextBlock> CurItemNameText;
	UPROPERTY()
	TObjectPtr<class UTextBlock> CurItemExplainText;
	UPROPERTY()
	TObjectPtr<class UImage> MiniItemImage;

	UPROPERTY()
	TWeakObjectPtr<class UIFInventory> PlayerInven;


};
