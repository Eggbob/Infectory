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

	static ConstructorHelpers::FObjectFinder<UDataTable> BossTableRef(TEXT("/Script/Engine.DataTable'/Game/Assets/GameData/IFBossPatternTable.IFBossPatternTable'"));
	if (BossTableRef.Object)
	{
		BossPatternTable = BossTableRef.Object;

		TArray<FName> RowNames = BossPatternTable->GetRowNames();

		for (FName& RName : RowNames)
		{
			FIFBossPatternData* BossPatternData = BossPatternTable->FindRow<FIFBossPatternData>(RName, TEXT(""));

			if (nullptr != BossPatternData)
			{
				BossPatternMap.Add(TPair<EBossPattern, int32>(BossPatternData->Pattern, BossPatternData->BeadCount), *BossPatternData);
			}
		}
	}

	static ConstructorHelpers::FObjectFinder<UDataTable> ItemDataTableRef(TEXT("/Script/Engine.DataTable'/Game/Assets/GameData/IFItemData.IFItemData'"));
	if (nullptr != ItemDataTableRef.Object)
	{
		ItemDataTable = ItemDataTableRef.Object;

		TArray<FName> RowNames = ItemDataTable->GetRowNames();

		for (FName& RName : RowNames)
		{
			FIFItemData* ItemStat = ItemDataTable->FindRow<FIFItemData>(RName, TEXT(""));

			if (nullptr != ItemStat)
			{
				ItemMap.Add(ItemStat->GetID(), *ItemStat);
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

FIFBossPatternData UIFGameSingleton::GetBossPatternData(EBossPattern BossPattern, int32 BeadCnt) const
{
	if (BossPatternMap.Contains(TPair<EBossPattern, int32>(BossPattern, BeadCnt)))
	{
		return BossPatternMap[TPair<EBossPattern, int32>(BossPattern, BeadCnt)];
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Invalid Boss Stat"));
		return FIFBossPatternData();
	}
}

FIFItemData UIFGameSingleton::GetItemData(int32 ItemID) const
{
	if (ItemMap.Contains(ItemID))
	{
		return ItemMap[ItemID];
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Invalid ItemData"));
		return FIFItemData();
	}
}




