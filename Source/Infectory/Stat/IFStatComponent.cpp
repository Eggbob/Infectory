// Fill out your copyright notice in the Description page of Project Settings.


#include "Stat/IFStatComponent.h"

UIFStatComponent::UIFStatComponent()
{
	bWantsInitializeComponent = true;
}

void UIFStatComponent::InitializeComponent()
{
	Super::InitializeComponent();

	MaxHp = 100;
	SetHp(100);
}

void UIFStatComponent::HealHp(float InHealAmount)
{
	CurrentHp = FMath::Clamp(CurrentHp + InHealAmount, 0, MaxHp);
}

float UIFStatComponent::ApplyDamage(float InDamage)
{
	if (CurrentHp <= KINDA_SMALL_NUMBER)
	{
		return 0.0f;
	}

	const float PrevHp = CurrentHp;
	const float ActualDamage = FMath::Clamp<float>(InDamage, 0, InDamage);


	OnHit.Broadcast();

	SetHp(PrevHp - ActualDamage);
	

	if (CurrentHp <= KINDA_SMALL_NUMBER)
	{
		OnHpZero.Broadcast();
	}

	return 0.0f;
}

void UIFStatComponent::SetHp(float NewHp)
{
	CurrentHp = FMath::Clamp<float>(NewHp, 0.f, MaxHp);
	OnHpChanged.Broadcast(CurrentHp);
}



