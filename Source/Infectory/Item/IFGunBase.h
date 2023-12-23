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

	void PullTrigger();
	void CachingOwner();

	UFUNCTION(Blueprintcallable)
	FVector GetWeaponSocket();

private:
	bool GunTrace(FHitResult& Hit, FVector& ShotDirection);

protected:
	UPROPERTY()
	TObjectPtr<AController> OwnerController;

	UPROPERTY(VisibleAnywhere)
	USceneComponent* RootComp;

	UPROPERTY(VisibleAnywhere)
	USkeletalMeshComponent* Mesh;

	UPROPERTY(EditAnyWhere)
	float MaxRange = 1000;

	UPROPERTY(EditAnyWhere)
	float Damage = 10;

	UPROPERTY(EditAnyWhere)
	UParticleSystem* MuzzleFlash;

	UPROPERTY(EditAnyWhere)
	UParticleSystem* ImpactEffect;


};
