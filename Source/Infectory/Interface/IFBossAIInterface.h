// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IFBossAIInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UIFBossAIInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class INFECTORY_API IIFBossAIInterface
{
	GENERATED_BODY()

public:
	virtual void PerformPierceAttack() = 0;
	virtual void PerformRangeAttack() = 0;
	virtual void PeformBreathAttack() = 0;
	virtual void PerformSpawnBoomer() = 0;
};
