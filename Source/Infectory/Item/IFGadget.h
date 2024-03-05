// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Data/IFEnumDefine.h"
#include "IFGadget.generated.h"

#define MuzzleSocket "Muzzle"

DECLARE_DELEGATE(FOnGadgetDeInit);

UCLASS()
class INFECTORY_API AIFGadget : public AActor
{
	GENERATED_BODY()
	
public:	
	AIFGadget();
	void InitGadget(TObjectPtr<AController> Controller);
	void InitTurret();
	void DeInitGadget();
	void LaunchGadget(FVector& TargetLoc);

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

private:
	void SearchTarget();
	void AttackTarget();
	void GiveDamage(FHitResult& Hit);
	void RotateToTarget();
	bool CheckCanFire();

	void SpawnTarcerEffect(FTransform& SpawnTransform);

public:
	FOnGadgetDeInit GadgetDeInitDelegate;

protected:
	UPROPERTY()
	TObjectPtr<class AIFGameMode> GameMode;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess = true))
	TObjectPtr<class UProjectileMovementComponent> ProjectileMovementComp;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	TSubclassOf<class AIFProjectile> TracerEffect;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	TObjectPtr<UParticleSystem> MuzzleFlash;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	TObjectPtr<UParticleSystem> ImpactEffect;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	TObjectPtr<UParticleSystem> BloodImpactEffect;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	float FireDelayTime = 0.2f; //발사 딜레이

	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	float FireTime = 5.0f; //발사 시간

	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	float ReloadTime = 5.0f; //발사 대기 시간

	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	int32 Damage = 10;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	int32 MaxRange = 10000;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	bool bIsStart = false;

	UPROPERTY(EditAnywhere)
	TObjectPtr<USoundBase> FireSound;

	UPROPERTY(EditAnywhere)
	TObjectPtr<USoundBase> ImpactSound;

	UPROPERTY(EditAnywhere)
	TObjectPtr<USoundBase> BodyImpactSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<AController> OwnerController;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EGadgetType GadgetType;
	
private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess = true))
	TObjectPtr<class UCapsuleComponent> CapsuleComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess = true))
	TObjectPtr<class UStaticMeshComponent> TurretHeadMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess = true))
	TObjectPtr<class UStaticMeshComponent> TurretParentMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess = true))
	TObjectPtr<class UStaticMeshComponent> TurretBarrelMesh;

	UPROPERTY()
	TObjectPtr<class AIFCharacterNonPlayer> Target;

	UPROPERTY()
	TArray<FName> MuzzleSockets;
	
	UPROPERTY()
	TArray<FHitResult> HitResults;

	FTimerHandle FireTimerHandle;
	FCollisionQueryParams Params;

	bool bDoOnce = false;
	bool bCanFire = true;
	bool bRealoading = false;
	bool bDoReload = false;
	bool bStartFire = false;
	

};
