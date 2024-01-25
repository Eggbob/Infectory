// Fill out your copyright notice in the Description page of Project Settings.


#include "Stat/IFStatComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Character/IFBodyPartDamageData.h"
#include "Data/IFGameSingleton.h"
#include "Data/IFEnumDefine.h"

UIFStatComponent::UIFStatComponent()
{
	bWantsInitializeComponent = true;

	static ConstructorHelpers::FObjectFinder<UIFBodyPartDamageData> BodyPartDataRef(TEXT("/Script/Infectory.IFBodyPartDamageData'/Game/Assets/CharacterControl/ABC_BodyPartDamage.ABC_BodyPartDamage'"));
	if (BodyPartDataRef.Object)
	{
		BodyPartDamageData = BodyPartDataRef.Object;
	}
}

void UIFStatComponent::ForTest()
{
	MaxHp = 1000000000;
	SetHp(MaxHp);
}

void UIFStatComponent::InitializeComponent()
{
	Super::InitializeComponent();
}

void UIFStatComponent::HealHp(float InHealAmount)
{
	CurrentHp = FMath::Clamp(CurrentHp + InHealAmount, 0, MaxHp);
}

/// <summary>
/// 캐릭터 스탯 지정
/// </summary>
/// <param name="NPCName"></param>
void UIFStatComponent::SetStat(FName NPCName, FName NPCTier)
{
	BaseStat = UIFGameSingleton::Get().GetCharacterStat(NPCName, NPCTier);
	MaxHp = BaseStat.MaxHp;
	SetHp(MaxHp);
}


float UIFStatComponent::ApplyDamage(float InDamage, FName BoneName)
{
	if (CurrentHp <= KINDA_SMALL_NUMBER)
	{
		return 0.0f;
	}

	const float PrevHp = CurrentHp;
	float RealDamage = InDamage;

	if (bIsNPC)
	{
		FString BoneNameStr = BoneName.ToString();
		ENPCBoneName NPCBoneName = UIFEnumDefine::StringToEnum(BoneNameStr);

		RealDamage = BodyPartDamageData.Get()->BodyPartDamageMap[NPCBoneName] * InDamage;
		//UE_LOG(LogTemp, Warning, TEXT("MultiPlyDamage : %f, BoneName %s"), BodyPartDamageData.Get()->BodyPartDamageMap[NPCBoneName], *BoneName.ToString());

	}

	const float ActualDamage = FMath::Clamp<float>(RealDamage, 0, RealDamage);
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



