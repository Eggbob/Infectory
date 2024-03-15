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
	FIFBossPatternData() :Pattern(EBossPattern::Idle), BeadCount(0), PatternDelay(0.0f), PatternDamage(0) {}

	FIFBossPatternData(EBossPattern Pattern, int BeadCount, float PatternDelay, int PatternDamage)
		: Pattern(Pattern), BeadCount(BeadCount), PatternDelay(PatternDelay), PatternDamage(PatternDamage)
	{
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Pattern)
		EBossPattern Pattern;
		
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Pattern)
		int BeadCount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Pattern)
		float PatternDelay;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Pattern)
		int PatternDamage;

};