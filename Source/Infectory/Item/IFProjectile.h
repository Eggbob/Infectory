// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "IFProjectile.generated.h"

DECLARE_DELEGATE_OneParam(OnAttackDelegate, TObjectPtr<AActor>)

UCLASS()
class INFECTORY_API AIFProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	AIFProjectile();

	virtual void Tick(float DeltaTime) override;
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
	void ExcuteAttack(AActor* OtherActor);
	void Init(float Speed);
	void DeInit();

private:
	void CheckAttackRange();

public:
	OnAttackDelegate OnAttack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Ammo, Meta = (AllowPrivateAccess = true))
	float ReturnTime = 3.f;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = Ammo, Meta = (AllowPrivateAccess = true))
	TObjectPtr<UParticleSystem> ImpactEffect;

protected:
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
