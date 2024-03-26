// Fill out your copyright notice in the Description page of Project Settings.


#include "IFPlayerController.h"
#include "Blueprint/UserWidget.h"

void AIFPlayerController::ChangeControllMode(bool bIsClose)
{
	if (bIsClose)
	{
		SetInputMode(GameOnlyInputMode);
		bShowMouseCursor = false;
	}
	else
	{
		SetInputMode(UIOnlyInputMode);
		bShowMouseCursor = true;
	}
}

void AIFPlayerController::BeginPlay()
{
	Super::BeginPlay();

	SetInputMode(GameOnlyInputMode);
	bIsGameMode = true;

	//HUDWidget = CreateWidget(this, CrossHeadClass);

	//if (HUDWidget)
	//{
	//	HUDWidget->AddToViewport();
	//}
}
