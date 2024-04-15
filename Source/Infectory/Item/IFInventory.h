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
	void UseItem(int32 ItemIndex);
	void UsePotion();
	void SetCredit(int32 Credit);
	void RemoveItem(int32 ItemIdx);

	UFUNCTION(BlueprintCallable)
	bool CheckItem(int32 ItemID);

	UFUNCTION(BlueprintCallable)
	bool AddItem(FIFItemData ItemData);

	class AIFGunBase& GetRangedWeapon(ERangedWeaponType WeaponType);
	class AIFGadget& GetGadget();
	
	FORCEINLINE TArray<FIFItemData>& GetItemList() { return ItemList; }
	FORCEINLINE int32& GetCurCredit() { return CurCredit; }
	FORCEINLINE int32& GetCurUpGradeGier() { return CurUpgradeGier; }


private:
	void TestSetItem();


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
