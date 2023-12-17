// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Data/IFEnumDefine.h"
#include "IFCharacterMovementData.generated.h"

/**
 * 
 */
UCLASS()
class INFECTORY_API UIFCharacterMovementData : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, Category = Move)
	TMap<ECharacterMoveType, float> MoveSpeed;
};
