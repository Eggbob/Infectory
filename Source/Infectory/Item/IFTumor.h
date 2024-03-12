// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "IFTumor.generated.h"

UCLASS()
class INFECTORY_API AIFTumor : public APawn
{
	GENERATED_BODY()

public:
	AIFTumor();
	void InitTumor(int32 InHP);

	UFUNCTION(BlueprintImplementableEvent)
	void ActiveTumor();

	UFUNCTION(BlueprintImplementableEvent)
	void DeActiveTumor();

protected:
	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess = true))
	bool bIsActivate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess = true))
	bool bCanActive;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess = true))
	TObjectPtr<class USphereComponent > SphereComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess = true))
	TObjectPtr<class USkeletalMeshComponent> SkeletalMeshComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float GlowParam = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 DestroyCount = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 TumorHP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 TumorCurHP;
};
