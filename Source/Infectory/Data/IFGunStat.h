
#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Data/IFEnumDefine.h"
#include "IFGunStat.generated.h"

USTRUCT(BlueprintType)
struct FIFGunStat : public FTableRowBase
{
	GENERATED_BODY()

public:
	FIFGunStat() : MaxRange(0), Damage(0), MaxAmmo(0), MaxCapacity(0), FireDelay(0.0f) {}

	FIFGunStat(const FString& Name, int MaxRange, int Damage, int MaxAmmo, int MaxCapacity, float FireDelay)
		: Name(Name), MaxRange(MaxRange), Damage(Damage), MaxAmmo(MaxAmmo), MaxCapacity(MaxCapacity), FireDelay(FireDelay)
	{
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	FString Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	int MaxRange;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	int Damage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	int MaxAmmo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	int MaxCapacity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	float FireDelay;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	int ProjectileSpeed;


};
