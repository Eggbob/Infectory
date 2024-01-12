// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "IFCharacterStat.h"
#include "IFGameSingleton.generated.h"

/**
 * 
 */
UCLASS()
class INFECTORY_API UIFGameSingleton : public UObject
{
	GENERATED_BODY()
	
public:
	UIFGameSingleton();
	static UIFGameSingleton& Get();

public:
	FIFCharacterStat GetCharacterStat(FName NpcName) const;

private:
	TMap<FName, FIFCharacterStat> CharacterStatMap;
	TObjectPtr<class UDataTable> CharacterStatTable;
};
