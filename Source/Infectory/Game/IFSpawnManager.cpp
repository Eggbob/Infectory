// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/IFSpawnManager.h"
#include "Components/SceneComponent.h"
#include "Components/BoxComponent.h"
#include "Character/IFCharacterNonPlayer.h"
#include "Components/BillboardComponent.h"
#include "Engine/DataTable.h"
#include "Game/IFGameMode.h"
#include "Game/IFObjectPoolManager.h"
#include "Kismet/KismetSystemLibrary.h"

AIFSpawnManager::AIFSpawnManager()
{
	PrimaryActorTick.bCanEverTick = false;

	SceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComp"));
	RootComponent = SceneComp;
	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComp"));
	BoxComp->SetupAttachment(SceneComp);

	BasicSpawnPoint = CreateDefaultSubobject<UBillboardComponent>(TEXT("SpawnPoint"));
	BasicSpawnPoint->SetupAttachment(RootComponent);

	static ConstructorHelpers::FClassFinder<AIFCharacterNonPlayer> NpcClassRef(TEXT("/Game/Assets/Blueprint/BP_EnemyNPC.BP_EnemyNPC_C"));
	if (NpcClassRef.Class)
	{
		NPCClass = NpcClassRef.Class;
	}

	bIsActivated = false;
}


void AIFSpawnManager::OrderSpawn()
{
	if (bIsActivated) return;

	SpawnNPC();
}

void AIFSpawnManager::BeginPlay()
{
	Super::BeginPlay();
	GetComponents<UBillboardComponent>(SpawnPoints);
}

void AIFSpawnManager::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	if (bIsActivated) return;

	BoxComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	if (SpawnPoints.Num() == NPCSpawnTable.Num())
	{
		SpawnNPC();
	}
	else
	{
		UKismetSystemLibrary::PrintString(this, "SpawnPoints and NPCSpawnTable are not equal");
	}

	

}

void AIFSpawnManager::SpawnNPC()
{
	bIsActivated = true;

	TObjectPtr<AIFGameMode> GameMode = Cast<AIFGameMode>(GetWorld()->GetAuthGameMode());
	
	for(int i = 0; i < NPCSpawnTable.Num(); i++)
	{
		TObjectPtr<AIFCharacterNonPlayer> NPC = Cast<AIFCharacterNonPlayer>(GameMode.Get()->GetPoolManager().Get()->Pop(NPCClass, GetWorld()));
			//GetWorld()->SpawnActor<AIFCharacterNonPlayer>(NPCClass, SpawnPoints[i].Get()->GetComponentLocation(), SpawnPoints[i].Get()->GetComponentRotation());
		
		if (NPC != nullptr)
		{
			NPC->SetNPCType(NPCSpawnTable[i].CurNPCType, NPCSpawnTable[i].CurNPCTier);
			NPC->SetActorLocation(SpawnPoints[i].Get()->GetComponentLocation());
			SpawnedNpcs.Add(NPC);
		}
		else
		{
			ShowLog();
		}
	
	}

	
}



