// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/IFUserWidget.h"
#include "UI/IFCrossHairWidget.h"
#include "UI/IFAmmoWidget.h"
#include "Components/TextBlock.h"
#include "Interface/IFUserWidgetInterface.h"

void UIFUserWidget::UpdateAmmoState(int32 CurAmmo, int32 TotalAmmo)
{
	if (AmmoWidget)
	{
		AmmoWidget->SetAmmoText(CurAmmo, TotalAmmo);
	}
}

void UIFUserWidget::UpdateCrossHair(bool bIsHit)
{
	if (CrossHairWidget)
	{
		CrossHairWidget->PlayCrossHair(bIsHit);
	}
}

void UIFUserWidget::ActiveCrossHair(bool bIsActive)
{
	if (bIsActive)
	{
		CrossHairWidget->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		CrossHairWidget->SetVisibility(ESlateVisibility::Hidden);
	}
}


void UIFUserWidget::UpdateNotifyText(const FText& InText)
{
	if (NotifyText)
	{
		NotifyText.Get()->SetText(InText);
		NotifyText.Get()->SetVisibility(ESlateVisibility::Visible);

		DisableNotifyText();
	}
}

void UIFUserWidget::NativeConstruct()
{
	Super::NativeConstruct();

	CrossHairWidget = Cast<UIFCrossHairWidget>(GetWidgetFromName(TEXT("BP_CrossHair")));
	AmmoWidget = Cast<UIFAmmoWidget>(GetWidgetFromName(TEXT("BP_AmmoInterface")));
	NotifyText = Cast<UTextBlock>(GetWidgetFromName(TEXT("NotifyText")));

	IIFUserWidgetInterface* UserWidgetInterface = Cast<IIFUserWidgetInterface>(GetOwningPlayerPawn());
	if (UserWidgetInterface)
	{
		UserWidgetInterface->SetupUserWidget(this);
	}
}
