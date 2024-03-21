// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Data/IFCharacterStat.h"
#include "IFStatComponent.generated.h"


DECLARE_MULTICAST_DELEGATE(FOnHpZeroDelegate);
DECLARE_MULTICAST_DELEGATE(FOnHitDelegate);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnHpChangedDelegate, float /*CurrentHp*/);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class INFECTORY_API UIFStatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UIFStatComponent();

	void ForTest();

protected:
	virtual void InitializeComponent() override;

public:	
	void HealHp(float InHealAmount);
	void SetStat(FName NPCName, FName NPCTier);
	float ApplyDamage(float InDamage, FName BoneName);
	float GetMaxHp() const { return MaxHp; }

	FORCEINLINE const FIFCharacterStat& GetBaseStat() const { return BaseStat; }

public:
	FOnHpZeroDelegate OnHpZero;
	FOnHitDelegate OnHit;
	FOnHpChangedDelegate OnHpChanged;

	bool bIsNPC;

protected:
	void SetHp(float NewHp);

	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat)
	float CurrentHp;

	float MaxHp;

private:
	FIFCharacterStat BaseStat;

	UPROPERTY()
	TMap<FString, float> PartDamage;

	UPROPERTY()
	TObjectPtr<class UIFBodyPartDamageData> BodyPartDamageData;
};
