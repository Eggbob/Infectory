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
	Moving,
	Jumping,
	Attacking,
	Dead
};

UENUM()
enum class EDamageType : uint8
{
	Light
};

UENUM()
enum class ENPCType : uint8
{
	Test,
	Parasite,
	Hunter,
	Boomer
};


/**
 * 
 */
UCLASS()
class INFECTORY_API UIFEnumDefine : public UUserDefinedEnum
{
	GENERATED_BODY()
	
public:

	template<typename T>
	static FString GetEnumName(T EnumName);
};

template<typename T>
inline FString UIFEnumDefine::GetEnumName(T EnumName)
{
	FString Name = StaticEnum<T>()->GetNameStringByValue(static_cast<__underlying_type(T)>(EnumName));
	return Name;
}
