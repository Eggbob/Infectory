// Fill out your copyright notice in the Description page of Project Settings.


#include "Data/IFGameSingleton.h"
#include "Game/IFObjectPoolManager.h"

UIFGameSingleton::UIFGameSingleton()
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
				CharacterStatMap.Add(TPair<FName, FName>(CharacterStat->Name, CharacterStat->NPCTier),*CharacterStat);
			}
		}
	}


	static ConstructorHelpers::FObjectFinder<UDataTable> GunDataTableRef(TEXT("/Script/Engine.DataTable'/Game/Assets/GameData/IFGunStatTable.IFGunStatTable'"));
	if(GunDataTableRef.Object)
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


UIFGameSingleton& UIFGameSingleton::Get()
{
	UIFGameSingleton* Singleton = CastChecked<UIFGameSingleton>(GEngine->GameSingleton);
	if (Singleton)
	{
		return *Singleton;
	}

	UE_LOG(LogTemp, Error, TEXT("Invalid Game Singleton"));
	return *NewObject<UIFGameSingleton>();
}



FIFCharacterStat UIFGameSingleton::GetCharacterStat(FName NpcName, FName NPCTier) const
{
	if(CharacterStatMap.Contains(TPair<FName, FName>(NpcName, NPCTier)))
	{
		return CharacterStatMap[TPair<FName, FName>(NpcName, NPCTier)];
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Invalid Character Stat"));
		return FIFCharacterStat();
	}
}

FIFGunStat UIFGameSingleton::GetGunStat(FName GunName) const
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




