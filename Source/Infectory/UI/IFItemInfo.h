// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Data/IFItemData.h"
#include "IFItemInfo.generated.h"

/**
 * 
 */
UCLASS()
class INFECTORY_API UIFItemInfo : public UUserWidget
{
	GENERATED_BODY()
	

public:
	UIFItemInfo(const FObjectInitializer& ObjectInitializer);
	
	UFUNCTION(BlueprintCallable)
	void ShowItemData(FIFItemData& ItemData);

protected:
	virtual void NativeConstruct() override;

protected:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (BindWidget))
	TObjectPtr<class UTextBlock> ItemNameText;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (BindWidget))
	TObjectPtr<class UImage> ItemTypeImage;

	UPROPERTY()
	TObjectPtr<class UIFItemInfoBox> ItemBox;

private:
	TMap<EItemType,class UTexture2D*> ItemTypeImageMap;
};
