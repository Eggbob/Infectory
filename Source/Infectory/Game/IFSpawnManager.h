// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Data/IFEnumDefine.h"
#include "IFSpawnManager.generated.h"

USTRUCT(BlueprintType)
struct FNPCTable
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = NPC)
	ENPCType CurNPCType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = NPC)
	FName CurNPCTier;
};


UCLASS()
class INFECTORY_API AIFSpawnManager : public AActor
{
	GENERATED_BODY()
	
public:	
	AIFSpawnManager();

	UFUNCTION(BlueprintCallable)
	void OrderSpawn();

protected:
	virtual void BeginPlay() override;

private:
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
	void SpawnNPC();

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Spawner)
	 TArray<FNPCTable> NPCSpawnTable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Spawner)
	TObjectPtr<class UBillboardComponent> BasicSpawnPoint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Spawner)
	TObjectPtr<class UBoxComponent> BoxComp;

	UPROPERTY()
	TArray<TObjectPtr<class UBillboardComponent>> SpawnPoints;

private:
	bool bIsActivated;

	UPROPERTY()
	TObjectPtr<class USceneComponent> SceneComp;

	UPROPERTY()
	TSubclassOf<class AIFCharacterNonPlayer> NPCClass;

	UPROPERTY()
	TArray<TObjectPtr<class AIFCharacterNonPlayer>> SpawnedNpcs; 
};
