// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Data/IFItemData.h"
#include "IFItemBox.generated.h"

/**
 * 
 */
UCLASS()
class INFECTORY_API UIFItemBox : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UIFItemBox(const FObjectInitializer& ObjectInitializer);
	void BindItemData(FIFItemData NewItemData);
	void InitItemBox();
	void ItemSelected(bool bIsSelected);

	FIFItemData GetItemData();

protected:
	virtual void NativeConstruct() override;
	
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (BindWidget))
	TObjectPtr<class UImage> ItemImage;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (BindWidget))
	TObjectPtr<class UImage> ItemBackImage;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (BindWidget))
	TObjectPtr<class UTextBlock> ItemCountText;

private:

	FIFItemData CurItemData;
	
	UPROPERTY()
	TObjectPtr<class UTexture2D> EmptyBackImg;

	UPROPERTY()
	TObjectPtr<class UTexture2D> ItemBackImg;

};
