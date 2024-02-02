// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/IFAmmoWidget.h"
#include "Components/TextBlock.h"

void UIFAmmoWidget::SetAmmoText(int32 CurAmmo, int32 TotalAmmo)
{
	if (CurAmmoText)
	{
		CurAmmoText->SetText(FText::FromString(FString::FromInt(CurAmmo)));
	}

	if (TotalAmmoText)
	{
		TotalAmmoText->SetText(FText::FromString(FString::FromInt(TotalAmmo)));
	}
}

void UIFAmmoWidget::NativeConstruct()
{
	Super::NativeConstruct();

	CurAmmoText = Cast<UTextBlock>(GetWidgetFromName(TEXT("TxtCurrentAmmo")));
	TotalAmmoText = Cast<UTextBlock>(GetWidgetFromName(TEXT("TxtTotalAmmo")));
}
