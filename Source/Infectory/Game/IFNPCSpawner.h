// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Data/IFEnumDefine.h"
#include "IFNPCSpawner.generated.h"

UCLASS()
class INFECTORY_API AIFNPCSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	AIFNPCSpawner();

	void SpawnNPC();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = NPC)
	ENPCType CurNPCType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = NPC)
	FName CurNPCTier;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = NPC)
	TSubclassOf<class AIFCharacterNonPlayer> NPCClass;

private:
	UPROPERTY()
	TObjectPtr<class UBillboardComponent> SpawnPoint;

	UPROPERTY()
	TObjectPtr<class AIFCharacterNonPlayer> SpawnedNPC;
};
