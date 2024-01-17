
#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Data/IFEnumDefine.h"
#include "IFCharacterStat.generated.h"

USTRUCT(BlueprintType)
struct FIFCharacterStat : public FTableRowBase
{
	GENERATED_BODY()

public:
	FIFCharacterStat() : MaxHp(0.0f), Attack(0.0f), AttackRange(0.0f), AttackSpeed(0.0f), JumpVelocity(0.0f) {}

	FIFCharacterStat(const FString& Name, float MaxHp, float Attack, float AttackRange, float AttackSpeed, float MovementSpeed, float JumpVelocity)
		: Name(Name), MaxHp(MaxHp), Attack(Attack), AttackRange(AttackRange), AttackSpeed(AttackSpeed), MovementSpeed(MovementSpeed), JumpVelocity(JumpVelocity), MeshLocation(MeshLocation)
	{
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	FString Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	FString NPCTier;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	int MaxHp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	int Attack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	float AttackRange;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	float AttackSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	float MovementSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	float JumpVelocity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite,	Category = Stat)
	float EncounterTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	float WaitTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	FVector MeshLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	FVector MeshScale;
};
