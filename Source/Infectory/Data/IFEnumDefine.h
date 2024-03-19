// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/UserDefinedEnum.h"
#include "Engine/DamageEvents.h"
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
enum class ECharacterState : uint8
{
	Idle,
	Moving,
	Hitting,
	Attacking,
	Reloading,
	WeaponChanging,
	Building,
	Grabbing,
	Lying,
	Dead
};


UENUM()
enum class ENPCState : uint8
{
	Idle,
	Moving,
	Jumping,
	Attacking,
	Lying,
	BeforeDead,
	Dead
};


UENUM()
enum class ENPCMoveType : uint8
{
	Walking,
	Crawling,
};

UENUM()
enum class EProjectileDamageType : uint8
{
	None,
	Light,
	Explosive,
	BossAttack,
	Spawner
};

UENUM()
enum class EBossPattern : uint8
{
	None = 0,
	Pierce = 1,
	SpawnBomb = 2,
	Range = 3,
	Breath = 4,
	SpawnEnemy = 5,
	Grab = 6,
};

UENUM()
enum class EBossState : uint8
{
	Idle,
	Attack,
	Hit,
	Dead
};

UENUM()
enum class ENPCType : uint8
{
	Test,
	Parasite,
	Hunter,
	Boomer,
	MiniHunter,
	RangedParasite,
	BigBoomer,
	MiniBoomer,
	Boss
};

UENUM()
enum class EGadgetType : uint8
{
	Turret,
	Shield,
};

UENUM()
enum class ETentaclePattern : uint8
{
	None,
	Pierce,
	Grab,
};

UENUM()
enum class ERangedWeaponType : uint8
{
	Rifle,
	ShotGun,
	Projectile,
	EnemyProjectile,
	Thrower
};

UENUM()
enum class ENPCBoneName : uint8
{
	pelvis UMETA(DisplayName = "골반"),
	thigh_l UMETA(DisplayName = "왼쪽 허벅지"),
	calf_l UMETA(DisplayName = "왼쪽 무릎"),
	foot_l UMETA(DisplayName = "왼쪽 발"),
	thigh_r UMETA(DisplayName = "오른쪽 허벅지"),
	calf_r UMETA(DisplayName = "오른쪽 무릎"),
	foot_r UMETA(DisplayName = "오른쪽 발"),
	spine_01 UMETA(DisplayName = "복부"),
	spine_02 UMETA(DisplayName = "가슴"),
	spine_03 UMETA(DisplayName = "쇄골"),
	upperarm_l UMETA(DisplayName = "왼쪽 팔"),
	hand_l UMETA(DisplayName = "왼쪽 손"),
	upperarm_r UMETA(DisplayName = "오른쪽 팔"),
	hand_r UMETA(DisplayName = "오른쪽 손"),
	head UMETA(DisplayName = "머리")
};



USTRUCT(Atomic, BlueprintType)
struct FCustomDamageEvent : public FDamageEvent
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName BoneName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector HitLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FHitResult HitResult;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EProjectileDamageType DamageType = EProjectileDamageType::Light; 

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

	static ENPCBoneName StringToEnum(FString& StringValue);
};

template<typename T>
inline FString UIFEnumDefine::GetEnumName(T EnumName)
{
	FString Name = StaticEnum<T>()->GetNameStringByValue(static_cast<__underlying_type(T)>(EnumName));
	return Name;
}

inline ENPCBoneName UIFEnumDefine::StringToEnum(FString& StringValue)
{
	UEnum* EnumPtr = StaticEnum<ENPCBoneName>();

	int32 EnumValue = EnumPtr->GetValueByNameString(StringValue);
	return (EnumValue != INDEX_NONE) ? static_cast<ENPCBoneName>(EnumValue) : ENPCBoneName::pelvis;
}
