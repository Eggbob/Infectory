// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "IFPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class INFECTORY_API AIFPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	void ChangeControllMode(bool bIsClose);
	
protected:
	virtual void BeginPlay() override;

private:
	bool bIsGameMode = true;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UUserWidget> CrossHeadClass;
	
	UPROPERTY()
	TObjectPtr<class UUserWidget> HUDWidget;

	FInputModeGameOnly GameOnlyInputMode;
	FInputModeUIOnly UIOnlyInputMode;
};
