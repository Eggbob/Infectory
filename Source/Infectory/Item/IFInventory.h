// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Data/IFEnumDefine.h"
#include "IFInventory.generated.h"

/**
 * 
 */
UCLASS()
class INFECTORY_API UIFInventory : public UObject
{
	GENERATED_BODY()

public:
	UIFInventory();

	void InitInventory(UWorld * World);
	void RecallTurret();

	TObjectPtr<class AIFGunBase> GetRangedWeapon(ERangedWeaponType WeaponType);
	TObjectPtr<class AIFTurret> GetTurret();
	

private:
	TMap<ERangedWeaponType, TObjectPtr<class AIFGunBase>> RangedWeapon;
	TMap<ERangedWeaponType, TSubclassOf<class AIFGunBase>> RangedWeaponBP;

	TSubclassOf<class AIFTurret> TurretBP;
	TObjectPtr<class AIFTurret> SpawnedTurret;
};
