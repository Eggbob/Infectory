// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "IFShield.generated.h"

DECLARE_DELEGATE(FOnShieldDestoryed);

UCLASS()
class INFECTORY_API AIFShield : public AActor
{
	GENERATED_BODY()
	
public:	
	AIFShield();
	void SetShieldDestoryedDelegate(const FOnShieldDestoryed& InOnShieldDestoryed);

protected:
	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneComponent> RootComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<class UCapsuleComponent> CapsuleComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<class UStaticMeshComponent> MeshComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 ShieldHP = 100;

private:
	FOnShieldDestoryed OnShieldDestoryed;
};
