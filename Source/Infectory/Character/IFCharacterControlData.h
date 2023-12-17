// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "IFCharacterControlData.generated.h"

/**
 * 
 */
UCLASS()
class INFECTORY_API UIFCharacterControlData : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	UIFCharacterControlData();

	UPROPERTY(EditAnywhere, Category = Pawn)
	bool bUseControllerRoationYaw = 1;

	UPROPERTY(EditAnywhere, Category = CharacterMovement)
	bool bOrientRotationToMovement = 1;

	UPROPERTY(EditAnywhere, Category = CharacterMovement)
	bool bUsecontrollerDesiredRotation = 1;

	UPROPERTY(EditAnywhere, Category = CharacterMovement)
	FRotator RoationRate;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	TObjectPtr<class UInputMappingContext> InputMappingContext;

	UPROPERTY(EditAnywhere, Category = SpringArm)
	float TargetArmLength;

	UPROPERTY(EditAnywhere, Category = SpringArm)
	FRotator RelativeRotation;

	UPROPERTY(EditAnywhere, Category = SpringArm)
	bool bUsePawnControlRotation = 1;

	UPROPERTY(EditAnywhere, Category = SpringArm)
	bool bInheritPitch = 1;

	UPROPERTY(EditAnywhere, Category = SpringArm)
	bool bInheritYaw = 1;

	UPROPERTY(EditAnywhere, Category = SpringArm)
	bool bInheritRoll = 1;

	UPROPERTY(EditAnywhere, Category = SpringArm)
	bool bCollisionTest = 1;
};
