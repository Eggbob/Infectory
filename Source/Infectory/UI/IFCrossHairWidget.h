// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "IFCrossHairWidget.generated.h"

/**
 * 
 */
UCLASS()
class INFECTORY_API UIFCrossHairWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;

	UPROPERTY()
	TObjectPtr<class UImage> CrossHairImg;
};
