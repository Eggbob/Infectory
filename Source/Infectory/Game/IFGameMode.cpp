// Fill out your copyright notice in the Description page of Project Settings.


#include "IFGameMode.h"
#include "Engine/DataTable.h"
#include "Game/IFObjectPoolManager.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Player/IFPlayerController.h"

AIFGameMode::AIFGameMode()
{
	static ConstructorHelpers::FClassFinder<APawn> DefaultPawnClassRef(TEXT("/Script/Engine.Blueprint'/Game/Assets/Blueprint/BP_PlayerCharacter.BP_PlayerCharacter_C'"));
	if (DefaultPawnClassRef.Class)
	{
		DefaultPawnClass = DefaultPawnClassRef.Class;
	}
	static ConstructorHelpers::FClassFinder<AIFPlayerController> PlayerControllerClassRef(TEXT("/Script/Engine.Blueprint'/Game/Assets/Blueprint/BP_IFPlayerController.BP_IFPlayerController_C'"));
	if (PlayerControllerClassRef.Class)
	{
		PlayerControllerClass = PlayerControllerClassRef.Class;
	}
}

void AIFGameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);
	PoolManager.Get()->Clear();
	PoolManager.Get()->Destroy();
}



TObjectPtr<class AIFObjectPoolManager> AIFGameMode::GetPoolManager()
{
	if (PoolManager == nullptr)
	{
		PoolManager = NewObject<AIFObjectPoolManager>();// GetWorld()->SpawnActor<AIFObjectPoolManager>(AIFObjectPoolManager::StaticClass()); 
		UE_LOG(LogTemp, Warning, TEXT("Create New Pool Manager"));
	}

	return PoolManager.Get();
}







