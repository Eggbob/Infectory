// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Data/IFEnumDefine.h"
#include "Data/IFItemData.h"
#include "Game/IFSpawnManager.h"
#include "IFEventManager.generated.h"

USTRUCT(BlueprintType)
struct FSpawnData
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FNPCTable> NPCTable;
};

UCLASS()
class INFECTORY_API AIFEventManager : public AActor
{
	GENERATED_BODY()
	
public:	
	AIFEventManager();

	UFUNCTION(BlueprintCallable)
	void StartTimer();

	UFUNCTION(BlueprintImplementableEvent)
	void StartFlash();

	UFUNCTION(BlueprintImplementableEvent)
	void FinishEvent();

private:
	void CountSecondsTimer();
	void StopCounting();
	void SpawnEnemy();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FSpawnData> NPCSpawnTable1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FSpawnData> NPCSpawnTable2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<class AIFCharacterPlayer> TargetActor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 ItemID;

	UPROPERTY(BlueprintReadOnly)
	FIFItemData ItemData;

private:
	UPROPERTY()
	TArray<TObjectPtr<class UBillboardComponent>> SpawnPoints;

	UPROPERTY()
	TSubclassOf<class AIFCharacterNonPlayer> NPCClass;

	UPROPERTY()
	TObjectPtr<class AIFGameMode> GameMode;

	FTimerHandle CountSecondsTimerHandle;
	FTimerHandle StopCountingTimerHandle;
	FTimerHandle SpawnEnemyTimerHandle;
	FTimerHandle GlowTimerHandle;

	int32 CurrentCount = 0;
	const int32 MaxSeconds = 180;
	int32 SpawnSecond = 35;
	int32 WaveCount = 0;
	int32 SpawnDelay = 30;
};
