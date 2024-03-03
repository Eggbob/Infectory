// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "IFTentalce.generated.h"

DECLARE_DELEGATE_OneParam(OnGiveDamageDelegate, TObjectPtr<AActor>)

UCLASS()
class INFECTORY_API AIFTentalce : public APawn
{
	GENERATED_BODY()

public:
	AIFTentalce();
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintImplementableEvent)
	void PlayPierceing();

	void PierceAttack(FVector TargetLoc);
	void InitTentacle(float InDamage);

	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
private:
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess = true))
	TObjectPtr<class USkeletalMeshComponent> SkeletalMeshComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess = true))
	TObjectPtr<class UCapsuleComponent> CapsuleComp;

	UPROPERTY(EditAnywhere)
	TObjectPtr<class UStaticMeshComponent> DangerCircle;

	UPROPERTY(EditAnywhere)
	TObjectPtr<USceneComponent> RootComp;

public:
	OnGiveDamageDelegate OnGiveDamage;

};
