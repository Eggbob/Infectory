// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/IFCharacterBase.h"
#include "IFCharacterNonPlayer.generated.h"

/**
 * 
 */
UCLASS()
class INFECTORY_API AIFCharacterNonPlayer : public AIFCharacterBase
{
	GENERATED_BODY()
	
public:
	AIFCharacterNonPlayer();

protected:
	virtual void PostInitializeComponents() override;

	void SetDead() override;
};
