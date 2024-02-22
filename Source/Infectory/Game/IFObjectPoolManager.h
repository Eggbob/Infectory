// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "IFObjectPoolManager.generated.h"

UCLASS()
class INFECTORY_API AIFObjectPoolManager : public AActor
{
	GENERATED_BODY()
	
public:	
	AIFObjectPoolManager();
	~AIFObjectPoolManager();

	void Push(TObjectPtr<AActor> PoolObj);
	void CreatePool(TSubclassOf<AActor> _Original, UWorld* World, int count = 60);

	TObjectPtr<AActor> Pop(TSubclassOf<AActor> original, UWorld* World, AActor* Parent = nullptr);

	void Clear();

protected:
	virtual void PostInitializeComponents() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;


private:
	TObjectPtr<AActor> RootActor;
	TObjectPtr<UWorld> WorldObject;

	UPROPERTY()
	TMap<FString, TObjectPtr<class AIFObjPool>> PoolMap;

	UPROPERTY()
	TSubclassOf<AActor> BP_Pool;


};
