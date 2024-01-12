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
				CharacterStatMap.Add(RName, *CharacterStat);
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

FIFCharacterStat UIFGameSingleton::GetCharacterStat(FName NpcName) const
{
	return CharacterStatMap.Contains(NpcName) ? CharacterStatMap[NpcName] : FIFCharacterStat();
}
