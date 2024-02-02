// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Data/IFEnumDefine.h"
#include "IFGunBase.generated.h"

DECLARE_DELEGATE_OneParam(FOnFireGun, ERangedWeaponType);
DECLARE_DELEGATE_TwoParams(FOnReload, int32, int32); //현재 장탄수, 총 총알 수

UCLASS()
class INFECTORY_API AIFGunBase : public AActor
{
	GENERATED_BODY()
	
public:	
	AIFGunBase();

	void FireRifle();
	void FireShotGun();
	void FireProjectile();
	void GiveDamage(TObjectPtr<AActor> HitActor, FCustomDamageEvent& Hit);
	void CachingOwner();
	void StartFire();
	void StopFire();
	void Reload();

	FORCEINLINE int32 GetCurAmmo() const { return CurrentAmmo; }
	FORCEINLINE int32 GetTotalAmmo() const { return TotalAmmo; }

	UFUNCTION(Blueprintcallable)
	FVector GetWeaponSocket();

protected:
	

private:
	bool GunTrace(FHitResult& Hit, FVector& ShotDirection);
	void ShotGunTrace(FVector& ShotDirection);

public:
	FOnFireGun FireGunDelegate;
	FOnReload AmmoChangedDelegate;

protected:
	UPROPERTY()
	TObjectPtr<AController> OwnerController;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneComponent> RootComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<USkeletalMeshComponent> Mesh;

	UPROPERTY(EditAnyWhere, BlueprintReadOnly)
	TObjectPtr<class UNiagaraComponent> NiagaraComp;

	UPROPERTY(EditAnyWhere, BlueprintReadOnly)
	TSubclassOf<class AIFProjectile> ProjectileoBP;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	ERangedWeaponType WeaponType = ERangedWeaponType::Rifle;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	float MaxRange = 10000;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	float SpreadRange = 1000;

	UPROPERTY(EditAnyWhere)
	float Damage = 10;

	//총 총알 수
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	int32 TotalAmmo;

	//현재 장탄량
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	int32 CurrentAmmo;
	
	//최대 장탄수
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	int32 MagazineCapacity;

	UPROPERTY(EditAnyWhere)
	float FireDelayTime = 0.2f; //발사 딜레이
	
	UPROPERTY(EditAnyWhere)
	TObjectPtr<UParticleSystem> MuzzleFlash;

	UPROPERTY(EditAnyWhere)
	TObjectPtr<UParticleSystem> ImpactEffect;

	UPROPERTY(EditAnyWhere)
	TObjectPtr<UParticleSystem> BloodImpactEffect;

	FTimerHandle FireTimerHandle;


private:
	TArray<FHitResult> HitResults;

	bool IsAuto = true;
};
