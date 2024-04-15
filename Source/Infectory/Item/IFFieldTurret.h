// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "IFFieldTurret.generated.h"

#define MuzzleSocket "Muzzle"

UCLASS()
class INFECTORY_API AIFFieldTurret : public AActor
{
	GENERATED_BODY()
	
public:	
	AIFFieldTurret();

	UFUNCTION(BlueprintCallable)
	void PerformTurret();

	UFUNCTION(BlueprintCallable)
	void StopTurret();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;


private:
	void SearchTarget();
	void AttackTarget();
	void GiveDamage(FHitResult& Hit);
	void RotateToTarget();
	bool CheckCanFire();

protected:
	UPROPERTY()
	TObjectPtr<class AIFGameMode> GameMode;

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

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess = true))
	TObjectPtr<class UStaticMeshComponent> TurretMesh;

	UPROPERTY()
	TObjectPtr<class AIFCharacterNonPlayer> Target;

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
