// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IFUserWidgetInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UIFUserWidgetInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class INFECTORY_API IIFUserWidgetInterface
{
	GENERATED_BODY()

public:
	virtual void SetupUserWidget(TObjectPtr<class UIFUserWidget> InUserWidget) = 0;

};
