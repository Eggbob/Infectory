#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "IFEnumDefine.h"
#include "IFBossPatternData.generated.h"

USTRUCT(BlueprintType)
struct FIFBossPatternData : public FTableRowBase
{
	GENERATED_BODY()
	
public:
	FIFBossPatternData() :Pattern(EBossPattern::Idle), PatternDuration(0.0f), PatternDelay(0.0f), PatternSpeed(0.0f), PatternRange(0.0f), PatternDamage(0), PatternProjectileSpeed(0) {}

	FIFBossPatternData(EBossPattern Pattern, float PatternDuration, float PatternDelay, float PatternSpeed, float PatternRange, int PatternDamage, int PatternProjectileSpeed)
		: Pattern(Pattern), PatternDuration(PatternDuration), PatternDelay(PatternDelay), PatternSpeed(PatternSpeed), PatternRange(PatternRange), PatternDamage(PatternDamage), PatternProjectileSpeed(PatternProjectileSpeed)
	{
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Pattern)
		EBossPattern Pattern;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Pattern)
		float PatternDuration;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Pattern)
		float PatternDelay;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Pattern)
		float PatternSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Pattern)
		float PatternRange;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Pattern)
		int PatternDamage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Pattern)
		int PatternProjectileSpeed;
};