// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "IFCharacterStat.h"
#include "IFGunStat.h"
#include "Data/IFEnumDefine.h"
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
	FIFCharacterStat GetCharacterStat(FName NpcName, FName NPCTier) const;
	FIFGunStat GetGunStat(FName GunName) const;

private:
	UPROPERTY()
	TObjectPtr<class UDataTable> CharacterStatTable;

	TMap<TPair<FName, FName>, FIFCharacterStat> CharacterStatMap;

	UPROPERTY()
	TObjectPtr<class AIFObjectPoolManager> PoolManager;

	UPROPERTY()
	TObjectPtr<class UDataTable> GunStatTable;

	TMap<FName, FIFGunStat> GunStatMap;
};
