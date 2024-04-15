// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Data/IFItemData.h"
#include "IFItemDropBox.generated.h"

UCLASS()
class INFECTORY_API AIFItemDropBox : public AActor
{
	GENERATED_BODY()
	
public:	
	AIFItemDropBox();

	UFUNCTION(BlueprintCallable)
	void OpenItemBox();

protected:
	virtual void BeginPlay() override;

	void GetItemData();

protected:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (BindWidget))
	int32 ItemNum;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<class UIFItemBox> ItemBox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<class UIFItemInfo> ItemInfo;

	UPROPERTY(BlueprintReadOnly)
	FIFItemData ItemData;

};
