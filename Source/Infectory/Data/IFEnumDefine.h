// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/UserDefinedEnum.h"
#include "IFEnumDefine.generated.h"

UENUM()
enum class ECharacterControlType : uint8
{
	Shoulder,
	Zoom
};

UENUM()
enum class ECharacterMoveType : uint8
{
	Walking,
	Running,
	Sprinting,
	Crouching,
	Proning,
	Zooming
};


UENUM()
enum class ENPCState : uint8
{
	Idle,
	Walking,
	Running,
	Attacking,
	Dead
};

UENUM()
enum class EDamageType : uint8
{
	Light
};


/**
 * 
 */
UCLASS()
class INFECTORY_API UIFEnumDefine : public UUserDefinedEnum
{
	GENERATED_BODY()
	
};
