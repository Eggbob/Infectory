// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Containers/Queue.h"
#include "IFObjPool.generated.h"

UCLASS()
class INFECTORY_API AIFObjPool : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AIFObjPool();

	void Clear();
	void Init(TSubclassOf<AActor> Original, UWorld* _World, int size = 10);

	TObjectPtr<AActor> Pop(AActor* Parent);
	void Push(TObjectPtr<AActor> Object);

	TObjectPtr<AActor> CreateObjPool();


protected:
	virtual void BeginPlay() override;

public:
	UPROPERTY()
	class USceneComponent* RootComp;

private:
	TQueue<TObjectPtr<AActor>> DisableQueue;

	TObjectPtr<UWorld> WorldObject;

	UPROPERTY()
	TSubclassOf<AActor> OriginalObject;

	UPROPERTY()
	TMap<FString, TObjectPtr<AActor>> EnableMap;

	UPROPERTY(VisibleAnywhere)
	TArray<TObjectPtr<AActor>> PoolArray;

	int CreateCount = 0;;
	int MaxSize = 0;
};
