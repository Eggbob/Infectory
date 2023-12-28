// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "IFCharacterBase.generated.h"



UCLASS()
class INFECTORY_API AIFCharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	AIFCharacterBase();

protected:
	virtual void SetDead();
	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UIFStatComponent> Stat;
};
