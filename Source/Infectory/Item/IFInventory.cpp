// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/IFInventory.h"
#include "Item/IFGunBase.h"
#include "Item/IFGadget.h"
#include "Data/IFGameSingleton.h"

UIFInventory::UIFInventory()
{
	static ConstructorHelpers::FClassFinder<AIFGunBase> RifleClassRef(TEXT("/Game/Assets/Blueprint/Weapons/BP_Rifle.BP_Rifle_C"));
	if (RifleClassRef.Class)
	{
		RangedWeaponBP.Add(ERangedWeaponType::Rifle, RifleClassRef.Class);
	}

	static ConstructorHelpers::FClassFinder<AIFGunBase> ShotGunClassRef(TEXT("/Game/Assets/Blueprint/Weapons/BP_ShotGun.BP_ShotGun_C"));
	if (ShotGunClassRef.Class)
	{
		RangedWeaponBP.Add(ERangedWeaponType::ShotGun, ShotGunClassRef.Class);
	}

	static ConstructorHelpers::FClassFinder<AIFGunBase> LauncherClassRef(TEXT("/Game/Assets/Blueprint/Weapons/BP_GrenadeLauncher.BP_GrenadeLauncher_C"));
	if (LauncherClassRef.Class)
	{
		RangedWeaponBP.Add(ERangedWeaponType::Projectile, LauncherClassRef.Class);
	}

	static ConstructorHelpers::FClassFinder<AIFGadget> TurretClassRef(TEXT("/Game/Assets/Blueprint/Weapons/BP_Turret.BP_Turret_C"));
	if (TurretClassRef.Class)
	{
		GadgetBP.Add(EGadgetType::Turret, TurretClassRef.Class);
	}

	static ConstructorHelpers::FClassFinder<AIFGadget> ShieldClassRef(TEXT("/Game/Assets/Blueprint/Weapons/BP_DeployShield.BP_DeployShield_C"));
	if (ShieldClassRef.Class)
	{
		GadgetBP.Add(EGadgetType::Shield, ShieldClassRef.Class);
	}
}

void UIFInventory::InitInventory(UWorld* World)
{
	if (World)
	{
		TObjectPtr<AIFGunBase> Rifle = World->SpawnActor<AIFGunBase>(RangedWeaponBP[ERangedWeaponType::Rifle]);
		Rifle->SetActorHiddenInGame(true);
		Rifle.Get()->ReloadDelegate.BindUObject(this, &UIFInventory::CheckReloadAmmo);
		RangedWeapon.Add(ERangedWeaponType::Rifle, Rifle);

		TObjectPtr<AIFGunBase> ShotGun = World->SpawnActor<AIFGunBase>(RangedWeaponBP[ERangedWeaponType::ShotGun]);
		ShotGun->SetActorHiddenInGame(true);
		ShotGun.Get()->ReloadDelegate.BindUObject(this, &UIFInventory::CheckReloadAmmo);
		RangedWeapon.Add(ERangedWeaponType::ShotGun, ShotGun);

		TObjectPtr<AIFGunBase> Launcher = World->SpawnActor<AIFGunBase>(RangedWeaponBP[ERangedWeaponType::Projectile]);
		Launcher->SetActorHiddenInGame(true);
		Launcher.Get()->ReloadDelegate.BindUObject(this, &UIFInventory::CheckReloadAmmo);
		RangedWeapon.Add(ERangedWeaponType::Projectile, Launcher);

		TObjectPtr<AIFGadget> Turret = World->SpawnActor<AIFGadget>(GadgetBP[EGadgetType::Turret]);
		Turret->SetActorHiddenInGame(true);
		GadgetMap.Add(EGadgetType::Turret, Turret);

		TObjectPtr<AIFGadget> Shield = World->SpawnActor<AIFGadget>(GadgetBP[EGadgetType::Shield]);
		Shield->SetActorHiddenInGame(true);
		GadgetMap.Add(EGadgetType::Shield, Shield);

		CurGadgetType = EGadgetType::Turret;
		CurCredit = 5000;
		CurUpgradeGier = 100;
		
		TestSetItem();
	}
}

void UIFInventory::RecallGadget(EGadgetType GadgetType)
{
	GadgetMap[GadgetType].Get()->SetActorHiddenInGame(true);
	GadgetMap[GadgetType].Get()->DeInitGadget();
	GadgetMap[GadgetType].Get()->SetActorTickEnabled(false);
	GadgetMap[GadgetType].Get()->SetActorEnableCollision(false);
}

void UIFInventory::ChangeGadget(EGadgetType GadgetType)
{
	CurGadgetType = GadgetType;
}

void UIFInventory::CheckTotalAmmo(ERangedWeaponType WeaponType)
{
	int32 TotalAmmo = 0;

	for (FIFItemData Item : ItemList)
	{
		if (Item.GetItemType() == EItemType::Ammo)
		{
			if (Item.GetWeaponType() == WeaponType)
			{
				TotalAmmo += Item.GetAmount();
			}
			
		}
	}

	if (!AmmoMap.Contains(WeaponType))
	{
		AmmoMap.Add(WeaponType, TotalAmmo);
	}
	else
	{
		AmmoMap[WeaponType] = TotalAmmo;
	}

	RangedWeapon[WeaponType].Get()->SetTotalAmmo(TotalAmmo);

}

int32 UIFInventory::CheckReloadAmmo(ERangedWeaponType WeaponType, int32 NeedAmmo)
{
	for (FIFItemData& Item : ItemList)
	{
		if (Item.GetItemType() == EItemType::Ammo)
		{
			if (Item.GetWeaponType() == WeaponType)
			{
				int amount = Item.GetAmount();

				if (amount >= NeedAmmo)
				{
					Item.DcreaseAmount(NeedAmmo);
					//NeedAmmo = 0;
					break;
				}
				else
				{
					Item.SetAmount(0);
					NeedAmmo -= amount;
				}

			}

		}
	}

	return NeedAmmo;
}

bool UIFInventory::AddItem(FIFItemData ItemData)
{
	int32 ItemIndex = ItemList.Num();
	if (ItemIndex < 25)
	{
		ItemData.SetItemIndex(ItemIndex);
		ItemData.OnItemEmpty.BindUObject(this, &UIFInventory::RemoveItem);

		ItemList.Add(ItemData);

		return true;
	}
	else
	{
		return false;
	}

	return false;
}

void UIFInventory::UseItem(int32 ItemIdx)
{
	int32 Value = ItemList[ItemIdx].GetItemEffectValue();
	ItemList[ItemIdx].DcreaseAmount(1);
	OnItemUse.ExecuteIfBound(Value);
}

void UIFInventory::UsePotion()
{
	for (int i = 0; i < ItemList.Num(); i++)
	{
		if (ItemList[i].GetItemType() == EItemType::Potion)
		{
			UseItem(i);
			break;
		}
	}
}

void UIFInventory::SetCredit(int32 Credit)
{
	CurCredit = FMath::Clamp(Credit , 0, Credit);
}

bool UIFInventory::CheckItem(int32 ItemID)
{
	for (FIFItemData Item : ItemList)
	{
		if (Item.GetID() == ItemID)
		{
			return true;
		}
	}

	return false;
}

AIFGunBase& UIFInventory::GetRangedWeapon(ERangedWeaponType WeaponType)
{
	if (RangedWeapon.Contains(WeaponType))
	{
		return *RangedWeapon[WeaponType];
	}

	return *GetWorld()->SpawnActor<AIFGunBase>(RangedWeaponBP[WeaponType]);
}

AIFGadget& UIFInventory::GetGadget()
{
	if (GadgetMap.Contains(CurGadgetType))
	{
		GadgetMap[CurGadgetType].Get()->SetActorHiddenInGame(false);
		GadgetMap[CurGadgetType].Get()->DeInitGadget();
		GadgetMap[CurGadgetType].Get()->SetActorEnableCollision(true);

		return *GadgetMap[CurGadgetType];
	}
	else
	{
		return *GetWorld()->SpawnActor<AIFGadget>(GadgetBP[CurGadgetType]);
	}
	/*if (SpawnedTurret)
	{
		SpawnedTurret.Get()->SetActorHiddenInGame(false);
		SpawnedTurret.Get()->DeInitGadget();
		SpawnedTurret.Get()->SetActorEnableCollision(true);
		return SpawnedTurret;
	}
	else
		return GetWorld()->SpawnActor<AIFGadget>(TurretBP);*/

}

void UIFInventory::TestSetItem()
{
	AddItem(UIFGameSingleton::Get().GetItemData(1));
	AddItem(UIFGameSingleton::Get().GetItemData(2));
	AddItem(UIFGameSingleton::Get().GetItemData(3));
	AddItem(UIFGameSingleton::Get().GetItemData(4));
	AddItem(UIFGameSingleton::Get().GetItemData(4));
	AddItem(UIFGameSingleton::Get().GetItemData(4));
	AddItem(UIFGameSingleton::Get().GetItemData(4));
	AddItem(UIFGameSingleton::Get().GetItemData(5));
	AddItem(UIFGameSingleton::Get().GetItemData(5));
	AddItem(UIFGameSingleton::Get().GetItemData(5));
	AddItem(UIFGameSingleton::Get().GetItemData(6));
	AddItem(UIFGameSingleton::Get().GetItemData(6));
	AddItem(UIFGameSingleton::Get().GetItemData(6));
	AddItem(UIFGameSingleton::Get().GetItemData(8));

	/*ItemList.Add(UIFGameSingleton::Get().GetItemData(1));
	ItemList.Add(UIFGameSingleton::Get().GetItemData(2));
	ItemList.Add(UIFGameSingleton::Get().GetItemData(3));
	ItemList.Add(UIFGameSingleton::Get().GetItemData(4));
	ItemList.Add(UIFGameSingleton::Get().GetItemData(4));
	ItemList.Add(UIFGameSingleton::Get().GetItemData(4));
	ItemList.Add(UIFGameSingleton::Get().GetItemData(4));
	ItemList.Add(UIFGameSingleton::Get().GetItemData(5));
	ItemList.Add(UIFGameSingleton::Get().GetItemData(5));
	ItemList.Add(UIFGameSingleton::Get().GetItemData(5));
	ItemList.Add(UIFGameSingleton::Get().GetItemData(6));
	ItemList.Add(UIFGameSingleton::Get().GetItemData(6));
	ItemList.Add(UIFGameSingleton::Get().GetItemData(6));
	ItemList.Add(UIFGameSingleton::Get().GetItemData(8));*/

}

void UIFInventory::RemoveItem(int32 ItemIdx)
{
	if (ItemList.IsValidIndex(ItemIdx))
	{
		ItemList.RemoveAt(ItemIdx);
	}

}
