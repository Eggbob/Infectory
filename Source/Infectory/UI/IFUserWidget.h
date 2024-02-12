// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "IFUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class INFECTORY_API UIFUserWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void UpdateAmmoState(int32 CurAmmo, int32 TotalAmmo);
	void UpdateCrossHair(bool bIsHit);

protected:
	virtual void NativeConstruct() override;
	
protected:
	UPROPERTY()
	TObjectPtr<class UIFCrossHairWidget> CrossHairWidget;

	UPROPERTY()
	TObjectPtr<class UIFAmmoWidget> AmmoWidget;
};
