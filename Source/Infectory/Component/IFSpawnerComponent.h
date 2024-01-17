// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BillboardComponent.h"
#include "Data/IFEnumDefine.h"
#include "IFSpawnerComponent.generated.h"

/**
 * 
 */
UCLASS()
class INFECTORY_API UIFSpawnerComponent : public UBillboardComponent
{
	GENERATED_BODY()

public:
	UIFSpawnerComponent();

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
	TObjectPtr<class AIFCharacterNonPlayer> SpawnedNPC;

};
