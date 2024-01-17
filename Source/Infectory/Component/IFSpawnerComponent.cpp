// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/IFSpawnerComponent.h"
#include "Components/BillboardComponent.h"
#include "Character/IFCharacterNonPlayer.h"


UIFSpawnerComponent::UIFSpawnerComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	static ConstructorHelpers::FClassFinder<AIFCharacterNonPlayer> NpcClassRef(TEXT("/Game/Assets/Blueprint/BP_EnemyNPC.BP_EnemyNPC_C"));
	if (NpcClassRef.Class)
	{
		NPCClass = NpcClassRef.Class;
	}
}

void UIFSpawnerComponent::SpawnNPC()
{
	SpawnedNPC = GetWorld()->SpawnActor<AIFCharacterNonPlayer>(NPCClass, GetComponentLocation(), GetComponentRotation());
	if (SpawnedNPC)
	{
		SpawnedNPC->SetNPCType(CurNPCType, CurNPCTier);
	}
}
