// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Data/IFEnumDefine.h"
#include "Data/IFItemData.h"
#include "IFInventory.generated.h"

DECLARE_DELEGATE_OneParam(FOnItemUse, int32);

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
	void RecallGadget(EGadgetType GadgetType);
	void ChangeGadget(EGadgetType GadgetType);
	void CheckTotalAmmo(ERangedWeaponType WeaponType);
	int32 CheckReloadAmmo(ERangedWeaponType WeaponType, int32 NeedAmmo);
	bool AddItem(FIFItemData ItemData);
	void UseItem(int32 ItemIndex);
	void UsePotion();

	class AIFGunBase& GetRangedWeapon(ERangedWeaponType WeaponType);
	class AIFGadget& GetGadget();
	
	FORCEINLINE TArray<FIFItemData>& GetItemList() { return ItemList; }
	FORCEINLINE int32& GetCurCredit() { return CurCredit; }
	FORCEINLINE int32& GetCurUpGradeGier() { return CurUpgradeGier; }


private:
	void TestSetItem();
	void RemoveItem(int32 ItemIdx);

public:
	TMap<ERangedWeaponType, int32> AmmoMap;
	FOnItemUse OnItemUse;

private:
	UPROPERTY()
	TObjectPtr<class AIFGameMode> GameMode;

	int32 CurCredit;
	int32 CurUpgradeGier;

	TArray<FIFItemData> ItemList;

	TMap<ERangedWeaponType, TObjectPtr<class AIFGunBase>> RangedWeapon;
	TMap<ERangedWeaponType, TSubclassOf<class AIFGunBase>> RangedWeaponBP;

	TMap<EGadgetType, TSubclassOf<class AIFGadget>> GadgetBP;
	TMap<EGadgetType, TObjectPtr<class AIFGadget>> GadgetMap;
	
	EGadgetType CurGadgetType;

};
