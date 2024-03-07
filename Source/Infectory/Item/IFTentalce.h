// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "IFTentalce.generated.h"

DECLARE_DELEGATE_OneParam(OnGiveDamageDelegate, TObjectPtr<AActor>)
DECLARE_DELEGATE(OnTentacleDestroyed)

UCLASS()
class INFECTORY_API AIFTentalce : public APawn
{
	GENERATED_BODY()

public:
	AIFTentalce();
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintImplementableEvent)
	void PlayPierceing();

	UFUNCTION(BlueprintImplementableEvent)
	void TentacleDestroy();

	void PierceAttack(FVector TargetLoc);
	void InitTentacle();

	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	
	FORCEINLINE bool GetIsDestroy() { return bIsDestroyed; }

private:
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

public:
	OnGiveDamageDelegate OnGiveDamage;
	OnTentacleDestroyed OnTentacleDestory;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess = true))
	TObjectPtr<class USkeletalMeshComponent> SkeletalMeshComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess = true))
	TObjectPtr<class UCapsuleComponent> CapsuleComp;

	UPROPERTY(EditAnywhere)
	TObjectPtr<class UStaticMeshComponent> DangerCircle;

	UPROPERTY(EditAnywhere)
	TObjectPtr<USceneComponent> RootComp;

private:
	bool bIsDestroyed = false;
};
