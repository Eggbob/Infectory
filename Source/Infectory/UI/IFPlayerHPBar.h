// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "IFPlayerHPBar.generated.h"

/**
 * 
 */
UCLASS()
class INFECTORY_API UIFPlayerHPBar : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void SetHPBar(float HP);
	void InitHPBar(int32 InTotalHP);

protected:
	virtual void NativeConstruct() override;

	UFUNCTION(BlueprintImplementableEvent)
	void SetHPColor(int32 index, int32 ColorIndex);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<TObjectPtr<class UImage>> HPBarArray;

private:
	int32 TotalHP = 0;

	
};
