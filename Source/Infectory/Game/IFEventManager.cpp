#include "Game/IFEventManager.h"
#include "Character/IFCharacterNonPlayer.h"
#include "Components/BillboardComponent.h"
#include "Engine/DataTable.h"
#include "Game/IFGameMode.h"
#include "Game/IFObjectPoolManager.h"
#include "UI/IFUserWidget.h"
#include "Character/IFCharacterPlayer.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Data/IFGameSingleton.h"
#include "Components/SceneComponent.h"

AIFEventManager::AIFEventManager()
{
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FClassFinder<AIFCharacterNonPlayer> NpcClassRef(TEXT("/Game/Assets/Blueprint/BP_EnemyNPC.BP_EnemyNPC_C"));
	if (NpcClassRef.Class)
	{
		NPCClass = NpcClassRef.Class;
	}
}

void AIFEventManager::StartTimer()
{
	GetComponents<UBillboardComponent>(SpawnPoints);
	GameMode = Cast<AIFGameMode>(GetWorld()->GetAuthGameMode());
	ItemData = UIFGameSingleton::Get().GetItemData(ItemID);
	GetWorldTimerManager().SetTimer(CountSecondsTimerHandle, this, &AIFEventManager::CountSecondsTimer, MaxSeconds / 100.f, true);
	GetWorldTimerManager().SetTimer(StopCountingTimerHandle, this, &AIFEventManager::StopCounting, MaxSeconds, false);
	GetWorldTimerManager().SetTimer(SpawnEnemyTimerHandle, this, &AIFEventManager::SpawnEnemy, SpawnDelay, true);
	GetWorldTimerManager().SetTimer(GlowTimerHandle, this, &AIFEventManager::StartFlash, 1.0f, true);

	SpawnEnemy();
}



void AIFEventManager::CountSecondsTimer()
{
	CurrentCount++;

	TargetActor.Get()->GetUserWidget()->UpdateNotifyText(FText::FromString(FString::Printf(TEXT("키 코드 다운로드 %d%%"), CurrentCount)));

	if (CurrentCount >= 100)
	{
		StopCounting();
	}
}

void AIFEventManager::StopCounting()
{
	GetWorldTimerManager().ClearTimer(CountSecondsTimerHandle);
	GetWorldTimerManager().ClearTimer(SpawnEnemyTimerHandle);
	GetWorldTimerManager().ClearTimer(GlowTimerHandle);

	TargetActor.Get()->GetUserWidget()->UpdateNotifyText(FText::FromString(FString::Printf(TEXT("다운로드 완료"))));
	FinishEvent();
}

void AIFEventManager::SpawnEnemy()
{
	if(WaveCount >= NPCSpawnTable1.Num())
	{
		return;
	}

	for (int i = 0; i < NPCSpawnTable1[WaveCount].NPCTable.Num(); i++)
	{
		TObjectPtr<AIFCharacterNonPlayer> NPC = GetWorld()->SpawnActor<AIFCharacterNonPlayer>(NPCClass, SpawnPoints[0].Get()->GetComponentLocation(), SpawnPoints[0].Get()->GetComponentRotation());
	
		if (NPC != nullptr)
		{
			NPC->SetNPCType(NPCSpawnTable1[WaveCount].NPCTable[i].CurNPCType, NPCSpawnTable1[WaveCount].NPCTable[i].CurNPCTier);
			NPC->FocusingTarget(TargetActor);
		}
	}

	for (int i = 0; i < NPCSpawnTable2[WaveCount].NPCTable.Num(); i++)
	{
		TObjectPtr<AIFCharacterNonPlayer> NPC = GetWorld()->SpawnActor<AIFCharacterNonPlayer>(NPCClass, SpawnPoints[1].Get()->GetComponentLocation(), SpawnPoints[1].Get()->GetComponentRotation());

		if (NPC != nullptr)
		{
			NPC->SetNPCType(NPCSpawnTable2[WaveCount].NPCTable[i].CurNPCType, NPCSpawnTable2[WaveCount].NPCTable[i].CurNPCTier);
			NPC->FocusingTarget(TargetActor);
		}
	}

	WaveCount++;
}


