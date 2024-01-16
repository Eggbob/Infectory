// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "IFSpawnManager.generated.h"

UCLASS()
class INFECTORY_API AIFSpawnManager : public AActor
{
	GENERATED_BODY()
	
public:	
	AIFSpawnManager();

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY()
	TArray<class AIFNPCSpawner*> Spawners;

	UPROPERTY()
	TObjectPtr<class USceneComponent> SceneComp;

	UPROPERTY()
	TObjectPtr<class UBoxComponent> BoxComp;
};
