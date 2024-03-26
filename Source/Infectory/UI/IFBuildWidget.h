// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "IFBuildWidget.generated.h"

/**
 * 
 */
UCLASS()
class INFECTORY_API UIFBuildWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable ,Category = "Build")
	void CanBuild(bool bCanBuild);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetGadgetName(const FText& GadgetName);

	UFUNCTION(BlueprintImplementableEvent)
	void ChangeGadget(EGadgetType GadgetType);

};
