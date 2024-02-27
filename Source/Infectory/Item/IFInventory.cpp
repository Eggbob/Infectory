// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/IFInventory.h"
#include "Item/IFGunBase.h"
#include "Item/IFTurret.h"

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

	static ConstructorHelpers::FClassFinder<AIFTurret> TurretClassRef(TEXT("/Game/Assets/Blueprint/Weapons/BP_Turret.BP_Turret_C"));
	if (TurretClassRef.Class)
	{
		TurretBP = TurretClassRef.Class;
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

		SpawnedTurret = World->SpawnActor<AIFTurret>(TurretBP);
		//SpawnedTurret->SetActorHiddenInGame(false);
	}
}

TObjectPtr<AIFGunBase> UIFInventory::GetRangedWeapon(ERangedWeaponType WeaponType)
{
	if (RangedWeapon.Contains(WeaponType))
	{
		return RangedWeapon[WeaponType];
	}

	return nullptr;
}

TObjectPtr<class AIFTurret> UIFInventory::GetTurret()
{
	if (SpawnedTurret)
	{
		return SpawnedTurret;
	}
	else
		return GetWorld()->SpawnActor<AIFTurret>(TurretBP);

}
