// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/IFInventory.h"
#include "Item/IFGunBase.h"
#include "Item/IFGadget.h"

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
		TurretBP = TurretClassRef.Class;
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

	/*	SpawnedTurret = World->SpawnActor<AIFGadget>(TurretBP);
		SpawnedTurret->SetActorHiddenInGame(true);*/
	}
}

void UIFInventory::RecallGadget(EGadgetType GadgetType)
{
	GadgetMap[GadgetType].Get()->SetActorHiddenInGame(true);
	GadgetMap[GadgetType].Get()->DeInitGadget();
	GadgetMap[GadgetType].Get()->SetActorTickEnabled(false);
	GadgetMap[GadgetType].Get()->SetActorEnableCollision(false);
}

TObjectPtr<AIFGunBase> UIFInventory::GetRangedWeapon(ERangedWeaponType WeaponType)
{
	if (RangedWeapon.Contains(WeaponType))
	{
		return RangedWeapon[WeaponType];
	}

	return nullptr;
}

TObjectPtr<class AIFGadget> UIFInventory::GetGadget(EGadgetType GadgetType)
{
	if (GadgetMap.Contains(GadgetType))
	{
		GadgetMap[GadgetType].Get()->SetActorHiddenInGame(false);
		GadgetMap[GadgetType].Get()->DeInitGadget();
		GadgetMap[GadgetType].Get()->SetActorEnableCollision(true);

		return GadgetMap[GadgetType];
	}
	else
	{
		return GetWorld()->SpawnActor<AIFGadget>(GadgetBP[GadgetType]);
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
