// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "IFGunBase.generated.h"

DECLARE_DELEGATE(FOnFireGun);

UCLASS()
class INFECTORY_API AIFGunBase : public AActor
{
	GENERATED_BODY()
	
public:	
	AIFGunBase();

	void Fire();
	void CachingOwner();
	void StartFire();
	void StopFire();

	UFUNCTION(Blueprintcallable)
	FVector GetWeaponSocket();

	UFUNCTION(BlueprintImplementableEvent)
	void BPFire(FVector HitLocation);

	FOnFireGun FireGunDelegate;

private:
	bool GunTrace(FHitResult& Hit, FVector& ShotDirection);

protected:
	UPROPERTY()
	TObjectPtr<AController> OwnerController;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneComponent> RootComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<USkeletalMeshComponent> Mesh;

	UPROPERTY(EditAnyWhere, BlueprintReadOnly)
	TObjectPtr<class UNiagaraComponent> NiagaraComp;

	UPROPERTY(EditAnyWhere)
	float MaxRange = 1000;

	UPROPERTY(EditAnyWhere)
	float Damage = 10;

	UPROPERTY(EditAnyWhere)
	float FireDelayTime = 0.2f; //발사 딜레이
	
	UPROPERTY(EditAnyWhere)
	TObjectPtr<UParticleSystem> MuzzleFlash;

	UPROPERTY(EditAnyWhere)
	TObjectPtr<UParticleSystem> ImpactEffect;

	FTimerHandle FireTimerHandle;

	bool IsAuto = true;
};
