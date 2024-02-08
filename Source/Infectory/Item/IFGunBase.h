// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Data/IFEnumDefine.h"
#include "Data/IFGunStat.h"
#include "IFGunBase.generated.h"


DECLARE_DELEGATE_OneParam(FOnFireGun, ERangedWeaponType);
DECLARE_DELEGATE_OneParam(FOnShootDelegate, TSubclassOf<class ULegacyCameraShake>);
DECLARE_DELEGATE_OneParam(FOnCrossHairDelegate, bool);
DECLARE_DELEGATE_TwoParams(FOnReload, int32, int32); //현재 장탄수, 총 총알 수

#define MuzzleSocket "MuzzleFlashSocket"

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
	FORCEINLINE ERangedWeaponType GetWeaponType() const { return WeaponType; }

	UFUNCTION(Blueprintcallable)
	FVector GetWeaponSocket();

private:
	bool GunTrace(FHitResult& Hit, FVector& ShotDirection);
	void ShotGunTrace(FVector& ShotDirection);

public:
	FOnFireGun FireGunDelegate;
	FOnReload AmmoChangedDelegate;
	FOnShootDelegate ShootDelegate;
	FOnCrossHairDelegate CrossHairDelegate;

protected:
	UPROPERTY()
	TObjectPtr<AController> OwnerController;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneComponent> RootComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TObjectPtr<USkeletalMeshComponent> Mesh;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	TObjectPtr<class UNiagaraComponent> NiagaraComp;

	UPROPERTY(EditAnyWhere)
	TSubclassOf<class AIFProjectile> ProjectileBP;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	TSubclassOf<AActor> TracerEffect;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<class ULegacyCameraShake> CameraShake;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	ERangedWeaponType WeaponType = ERangedWeaponType::Rifle;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	float MaxRange = 10000;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	float SpreadRange = 1000;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	int32 Damage = 10;

	//총 총알 수
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	int32 TotalAmmo;

	//현재 장탄량
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	int32 CurrentAmmo;
	
	//최대 장탄수
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	int32 MagazineCapacity;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	float FireDelayTime = 0.2f; //발사 딜레이
	
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	float ProjectileSpeed = 1000;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	FIFGunStat GunStat;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	TObjectPtr<UParticleSystem> MuzzleFlash;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	TObjectPtr<UParticleSystem> ImpactEffect;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	TObjectPtr<UParticleSystem> BloodImpactEffect;

private:
	bool IsAuto = true;
	
	FTimerHandle FireTimerHandle;
	TArray<FHitResult> HitResults;

};
