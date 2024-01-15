// Fill out your copyright notice in the Description page of Project Settings.


#include "Data/IFGameSingleton.h"

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
				CharacterStatMap.Add(TPair<FName, ENPCTier>(CharacterStat->Name, CharacterStat->NPCTier),*CharacterStat);
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

FIFCharacterStat UIFGameSingleton::GetCharacterStat(FName NpcName, ENPCTier NPCTier) const
{
	if(CharacterStatMap.Contains(TPair<FName, ENPCTier>(NpcName, NPCTier)))
	{
		return CharacterStatMap[TPair<FName, ENPCTier>(NpcName, NPCTier)];
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Invalid Character Stat"));
		return FIFCharacterStat();
	}

	//return CharacterStatMap.Contains(TPair<FName, ENPCTier>(NpcName, NPCTier)) ? CharacterStatMap[TPair<FName, ENPCTier>(NpcName, NPCTier)] : FIFCharacterStat();
}
