// Fill out your copyright notice in the Description page of Project Settings.


#include "IFPlayerController.h"

void AIFPlayerController::BeginPlay()
{
	Super::BeginPlay();

	FInputModeGameOnly GameOnlyInputMode;
	SetInputMode(GameOnlyInputMode);
}
