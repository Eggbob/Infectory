// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "IFFootIkComponent.generated.h"

typedef struct IK_TraceInfo
{
	float Offset;
	FVector ImpactLocation;
};

USTRUCT(Atomic, BlueprintType)
struct FIKAnimValue
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float EffectorLocation_Left;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float EffectorLocation_Right;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FRotator FootRotation_Left;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FRotator FootRotation_Right;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float HipOffset;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class INFECTORY_API UIFFootIkComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UIFFootIkComponent();
	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void SetIKSocketName(FString SocketName_LeftFoot, FString SocketName_RightFoot);
	void IKResetVars();
	void SetIKActive(bool bActive);
	void SetIKDebug(bool bActive);

	FORCEINLINE FIKAnimValue GetIKAnimValue() { return IKAnimValue; }
	FORCEINLINE bool GetIKDebugState() { return bDebug; }

private:
	void IKUpdate(float DeltaTime);
	void IKDebug();
	void IKUpdateFootOffset(float DeltaTime, float TargetValue, float* EffectorValue, float InterpSpeed);
	void IKUpdateFootRotation(float DeltaTime, FRotator TargetValue, FRotator* EffectorValue, float InterpSpeed);
	void IKUpdateCapsuleHalfHeight(float DeltaTime, float HipsShifts, bool bResetDefault);

	IK_TraceInfo FootTrace(float TraceDistance, FName SocketName);
	FRotator NormalToRotator(FVector Normal);

	bool IsMoving();

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "IK Values Socket")
	FName IKSocketName_LeftFoot = "IK_LeftFoot";

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "IK Values Socket")
	FName IKSocketName_RightFoot = "IK_RightFoot";

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "IK Values")
	float IKAdjustOffset;

	//! Max Linetrace distance
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "IK Values")
	float IKTraceDistance;

	//! Hip(pelvis) move speed
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "IK Values")
	float IKHipsInterpSpeed;

	//! Leg joing move speed
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "IK Values")
	float IKFeetInterpSpeed;

	//! Enable debug mode
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "IK Debug")
	bool bDebug;

private:
	UPROPERTY()
	TObjectPtr<class ACharacter> OwnerCharacter;
	
	FIKAnimValue IKAnimValue;

	float IKCapsuleHalfHeight;
	float DelayTime = 0.0f;
	float mDeltaTime = 0.0f;
	bool mbActive = false;

};
