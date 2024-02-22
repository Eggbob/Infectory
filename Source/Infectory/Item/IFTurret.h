// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "IFTurret.generated.h"

UCLASS()
class INFECTORY_API AIFTurret : public AActor
{
	GENERATED_BODY()
	
public:	
	AIFTurret();

protected:
	virtual void Tick(float DeltaTime) override;

private:
	void SearchTarget();
	void AttackTarget();

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess = true))
	TObjectPtr<class UCapsuleComponent> CapsuleComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess = true))
	TObjectPtr<class UStaticMeshComponent> TurretHeadMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess = true))
	TObjectPtr<class UStaticMeshComponent> TurretParentMesh;


	UPROPERTY()
	TObjectPtr<class AActor> Target;
};
