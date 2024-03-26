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
		RangedWeapon.Add(ERangedWeaponType::Rifle, Rifle);

		TObjectPtr<AIFGunBase> ShotGun = World->SpawnActor<AIFGunBase>(RangedWeaponBP[ERangedWeaponType::ShotGun]);
		ShotGun->SetActorHiddenInGame(true);
		RangedWeapon.Add(ERangedWeaponType::ShotGun, ShotGun);

		TObjectPtr<AIFGunBase> Launcher = World->SpawnActor<AIFGunBase>(RangedWeaponBP[ERangedWeaponType::Projectile]);
		Launcher->SetActorHiddenInGame(true);
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
	ItemList.Add(UIFGameSingleton::Get().GetItemData(1));
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
	ItemList.Add(UIFGameSingleton::Get().GetItemData(8));

}
