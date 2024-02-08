// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/IFCrossHairWidget.h"
#include "Components/Image.h"

void UIFCrossHairWidget::PlayCrossHair(bool bIsHit)
{
	CrosshairSpreadVal = 60.f;

	if (bIsHit)
	{
		CrossHairColor = FLinearColor::Red;
	}
	else
	{
		CrossHairColor = FLinearColor::White;
	}
}

void UIFCrossHairWidget::NativeConstruct()
{
	Super::NativeConstruct();

//	CrossHairImg = Cast<UImage>(GetWidgetFromName("ImgCrossHair"));
	//ensure(CrossHairImg);
}
