// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Data/IFEnumDefine.h"
#include "Data/IFItemData.h"
#include "IFShop.generated.h"

UCLASS()
class INFECTORY_API AIFShop : public AActor
{
	GENERATED_BODY()
	
public:	
	AIFShop();

	UFUNCTION(BlueprintCallable)
	void OpenShopWidget();

protected:
	virtual void BeginPlay() override;

protected:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TObjectPtr<class UIFShopWidget> ShopWidget;

private:

	UPROPERTY()
	TArray<FIFItemData> ItemList;

	UPROPERTY()
	TObjectPtr<class UDataTable> SaleDataTable;

	UPROPERTY()
	TArray<FIFItemData> SaleItemList;
};
