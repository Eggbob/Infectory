// Fill out your copyright notice in the Description page of Project Settings.


#include "IFPlayerController.h"
#include "Blueprint/UserWidget.h"

void AIFPlayerController::BeginPlay()
{
	Super::BeginPlay();

	FInputModeGameOnly GameOnlyInputMode;
	SetInputMode(GameOnlyInputMode);

	HUDWidget = CreateWidget(this, CrossHeadClass);

	if (HUDWidget)
	{
		HUDWidget->AddToViewport();
	}
}
