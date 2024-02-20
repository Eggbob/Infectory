// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Data/IFEnumDefine.h"
#include "IFProjectile.generated.h"

DECLARE_DELEGATE_TwoParams(OnAttackDelegate, TObjectPtr<AActor>, FCustomDamageEvent)
DECLARE_DELEGATE_OneParam(FOnShootDelegate, TSubclassOf<class ULegacyCameraShake>);

UCLASS()
class INFECTORY_API AIFProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	AIFProjectile();

	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
	void ExcuteAttack(AActor* OtherActor);
	void Init(float Speed);
	void DeInit();
	void SetLocation(FVector& TargetLoc);

	UFUNCTION(BlueprintImplementableEvent)
	void LaunchEnemy(AActor* TargetActor);

private:
	void CheckAttackRange();

public:
	OnAttackDelegate OnAttack;
	FOnShootDelegate OnShoot;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Ammo, Meta = (AllowPrivateAccess = true))
	float ReturnTime = 3.f;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = Ammo, Meta = (AllowPrivateAccess = true))
	TObjectPtr<UParticleSystem> ImpactEffect;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<class ULegacyCameraShake> CameraShake;

protected:
	UPROPERTY(EditAnywhere)
	TObjectPtr<USoundBase> ProjectileSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess = true))
	TObjectPtr<class UCapsuleComponent> CapsuleComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess = true))
	TObjectPtr<class UStaticMeshComponent> StaticMeshComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess = true))
	TObjectPtr<class UProjectileMovementComponent> ProjectileMovementComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess = true))
	float MoveSpeed = 600.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess = true))
	bool bIsCollisioned;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess = true))
	bool bIsDeInit;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess = true))
	bool bIsExplosive;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess = true))
	float checkTime = 0.f;



};
