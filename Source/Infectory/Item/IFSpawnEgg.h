// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Data/IFEnumDefine.h"
#include "IFSpawnEgg.generated.h"

DECLARE_DELEGATE_OneParam(FOnFinishDelegate, TObjectPtr<AActor>);

UCLASS()
class INFECTORY_API AIFSpawnEgg : public AActor
{
	GENERATED_BODY()
	
public:	
	AIFSpawnEgg();
	void LaunchEgg(AActor * InTarget, ENPCType InNpcType, FName InNPCTier);

protected:
	virtual void BeginPlay() override;

private:
	void SpawnNPC();

public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = Ammo, Meta = (AllowPrivateAccess = true))
	TObjectPtr<UParticleSystem> SpawnEffect;

	FOnFinishDelegate OnFinish;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Spawner)
	TObjectPtr<class UBoxComponent> BoxComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Spawner)
	TObjectPtr<class UStaticMeshComponent> EggMesh;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess = true))
	TObjectPtr<class UProjectileMovementComponent> ProjectileMovementComp;

	UPROPERTY()
	TSubclassOf<class AIFCharacterNonPlayer> NPCClass;

	UPROPERTY()
	TSubclassOf<class AIFCharacterNonPlayer> BoomerClass;

	UPROPERTY()
	TObjectPtr<AActor> TargetActor;

	ENPCType NpcType;
	FName NPCTier;
};
