// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Data/IFItemData.h"
#include "IFItemInfoBox.generated.h"

/**
 * 
 */
UCLASS()
class INFECTORY_API UIFItemInfoBox : public UUserWidget
{
	GENERATED_BODY()
	

public:

	UIFItemInfoBox(const FObjectInitializer& ObjectInitializer);
	void BindItemData(FIFItemData NewItemData);
	void InitItemBox(bool bIsShop);
	void ItemSelected(bool bIsSelected);

	UFUNCTION(BlueprintImplementableEvent)
	void ChangeItemImage(UTexture2D* Texture);

	FIFItemData GetItemData();

protected:
	virtual void NativeConstruct() override;

public:
	//UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (BindWidget))
	TObjectPtr<class UImage> ItemImage;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (BindWidget))
	TObjectPtr<class UImage> ItemBackImage;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (BindWidget))
	TObjectPtr<class UTextBlock> ItemCountText;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (BindWidget))
	TObjectPtr<class UTextBlock> ItemPriceText;

private:

	FIFItemData CurItemData;

	UPROPERTY()
	TObjectPtr<class UTexture2D> EmptyBackImg;

	UPROPERTY()
	TObjectPtr<class UTexture2D> ItemBackImg;
};
