// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "IFAmmoWidget.generated.h"

/**
 * 
 */
UCLASS()
class INFECTORY_API UIFAmmoWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void SetAmmoText(int32 CurAmmo, int32 TotalAmmo);
	
protected:
	virtual void NativeConstruct() override;

private:
	UPROPERTY()
	TObjectPtr<class UTextBlock> CurAmmoText;
	
	UPROPERTY()
	TObjectPtr<class UTextBlock> TotalAmmoText;
};
