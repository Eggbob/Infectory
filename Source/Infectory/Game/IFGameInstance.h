// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Stat/IFStatComponent.h"
#include "Data/IFCharacterStat.h"
#include "Data/IFGunStat.h"
#include "Data/IFEnumDefine.h"
#include "IFGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class INFECTORY_API UIFGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	UIFGameInstance();

	static UIFGameInstance& Get();

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
