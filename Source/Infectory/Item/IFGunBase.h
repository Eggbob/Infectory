// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "IFGunBase.generated.h"

UCLASS()
class INFECTORY_API AIFGunBase : public AActor
{
	GENERATED_BODY()
	
public:	
	AIFGunBase();

	UFUNCTION(Blueprintcallable)
	FVector GetWeaponSocket();

private:
	UPROPERTY(VisibleAnywhere)
	USceneComponent* RootComp;

	UPROPERTY(VisibleAnywhere)
	USkeletalMeshComponent* Mesh;

};
