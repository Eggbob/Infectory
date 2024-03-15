// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Data/IFEnumDefine.h"
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

	UFUNCTION(BlueprintImplementableEvent)
	void ActiveTentacle(ACharacter * Target);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void ReleaseTentacle();

	UFUNCTION(BlueprintCallable)
	void ResetTentacle();

	UFUNCTION(BlueprintCallable)
	void GiveDamage(AActor* Target);
	
	void PierceAttack(FVector TargetLoc);

	void InitTentacle(FVector TentacleLoc);

	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	
	FORCEINLINE bool GetIsDestroy() {
		return bIsDestroyed; 
	}


public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess = true))
	TObjectPtr<class USkeletalMeshComponent> SkeletalMeshComp;

	OnGiveDamageDelegate OnGiveDamage;
	OnTentacleDestroyed OnTentacleDestory;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess = true))
	ETentaclePattern CurPattern = ETentaclePattern::None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess = true))
	TObjectPtr<class UCapsuleComponent> CapsuleComp;

	UPROPERTY(EditAnywhere)
	TObjectPtr<class UStaticMeshComponent> DangerCircle;

	UPROPERTY(EditAnywhere)
	TObjectPtr<USceneComponent> RootComp;

private:
	bool bIsDestroyed = false;
	FVector TentacleBasicLoc;
};
