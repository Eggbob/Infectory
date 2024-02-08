// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Math/Color.h"
#include "IFCrossHairWidget.generated.h"

/**
 * 
 */
UCLASS()
class INFECTORY_API UIFCrossHairWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void PlayCrossHair(bool bIsHit);
	
protected:
	virtual void NativeConstruct() override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CrosshairSpreadVal;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FLinearColor CrossHairColor = FLinearColor::White;

	UPROPERTY()
	TObjectPtr<class UImage> CrossHairImg;
};
