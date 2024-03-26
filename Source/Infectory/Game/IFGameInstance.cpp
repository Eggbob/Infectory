// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/IFGameInstance.h"
#include "Engine/DataTable.h"
#include "Game/IFObjectPoolManager.h"
#include "Kismet/KismetSystemLibrary.h"

UIFGameInstance::UIFGameInstance()
{
	static ConstructorHelpers::FObjectFinder<UDataTable> DataTableRef(TEXT("/Script/Engine.DataTable'/Game/Assets/GameData/IFCharacterStatTable.IFCharacterStatTable'"));
	if (nullptr != DataTableRef.Object)
	{
		CharacterStatTable = DataTableRef.Object;

		TArray<FName> RowNames = CharacterStatTable->GetRowNames();

		for (FName& RName : RowNames)
		{
			FIFCharacterStat* CharacterStat = CharacterStatTable->FindRow<FIFCharacterStat>(RName, TEXT(""));

			if (nullptr != CharacterStat)
			{
				CharacterStatMap.Add(TPair<FName, FName>(CharacterStat->Name, CharacterStat->NPCTier), *CharacterStat);
			}
		}
	}

	static ConstructorHelpers::FObjectFinder<UDataTable> GunDataTableRef(TEXT("/Script/Engine.DataTable'/Game/Assets/GameData/IFGunStatTable.IFGunStatTable'"));
	if (GunDataTableRef.Object)
	{
		GunStatTable = GunDataTableRef.Object;

		TArray<FName> RowNames = GunStatTable->GetRowNames();

		for (FName& RName : RowNames)
		{
			FIFGunStat* GunStat = GunStatTable->FindRow<FIFGunStat>(RName, TEXT(""));

			if (nullptr != GunStat)
			{
				GunStatMap.Add((FName)GunStat->Name, *GunStat);
			}
		}
	}

}


UIFGameInstance& UIFGameInstance::Get()
{
	UIFGameInstance* Singleton = CastChecked<UIFGameInstance>(GEngine->GameSingleton);
	if (Singleton)
	{
		return *Singleton;
	}

	return *NewObject<UIFGameInstance>();
}

FIFCharacterStat UIFGameInstance::GetCharacterStat(FName NpcName, FName NPCTier) const
{
	
	if (CharacterStatMap.Contains(TPair<FName, FName>(NpcName, NPCTier)))
	{
		return CharacterStatMap[TPair<FName, FName>(NpcName, NPCTier)];
	}
	else
	{
		UKismetSystemLibrary::PrintString(this, "Invalid Character Stat");
		return FIFCharacterStat();
	}
}

FIFGunStat UIFGameInstance::GetGunStat(FName GunName) const
{
	if (GunStatMap.Contains(GunName))
	{
		return GunStatMap[GunName];
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Invalid Gun Stat"));
		return FIFGunStat();
	}
}

