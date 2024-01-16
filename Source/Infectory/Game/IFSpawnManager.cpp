// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/IFSpawnManager.h"
#include "Components/SceneComponent.h"
#include "Components/BoxComponent.h"
#include "Game/IFNPCSpawner.h"

AIFSpawnManager::AIFSpawnManager()
{
	PrimaryActorTick.bCanEverTick = false;

	SceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComp"));
	RootComponent = SceneComp;
	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComp"));
	BoxComp->SetupAttachment(SceneComp);


	//Spawners.Add(NewObject<AIFNPCSpawner>(TEXT("Spawner1")));
}

void AIFSpawnManager::BeginPlay()
{
	Super::BeginPlay();
	
	//GetComponents<AIFNPCSpawner>(Spawners);

}



