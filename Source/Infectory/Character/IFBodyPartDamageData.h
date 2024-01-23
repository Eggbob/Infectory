// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Data/IFEnumDefine.h"
#include "IFBodyPartDamageData.generated.h"

/**
 * 
 */
UCLASS()
class INFECTORY_API UIFBodyPartDamageData : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere)
	TMap<ENPCBoneName, float> BodyPartDamageMap;
};
