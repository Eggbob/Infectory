// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "IFCharacterBase.h"
#include "InputActionValue.h"
#include "Data/IFEnumDefine.h"
#include "Interface/IFGetDefineTypeInterface.h"
#include "IFCharacterPlayer.generated.h"


UCLASS()
class INFECTORY_API AIFCharacterPlayer : public AIFCharacterBase, public IIFGetDefineTypeInterface
{
	GENERATED_BODY()
	
public:
	AIFCharacterPlayer();

protected:
	virtual void BeginPlay() override;
	void ChangeCharacterControl();
	void SetCharacterControlData(const class UIFCharacterControlData* CharacterControlData);
	void SetCharacterControl(ECharacterControlType NewCharacterControlType);


private:
	FVector GetGunHandPosition();

public:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	FORCEINLINE virtual ECharacterMoveType GetMoveType() override { return CurMoveType; }
	FORCEINLINE virtual ECharacterControlType GetControlType() override { return CurControlType; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE class AIFGunBase* GetCurGun() { return Gun.Get(); }

protected:
	//카메라 Arm
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, Meta = (AllowPrivateAcess = "true"))
	TObjectPtr<class USpringArmComponent> CameraBoom;

	//카메라
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, Meta = (AllowPrivateAcess = "true"))
	TObjectPtr<class UCameraComponent> FollowCamera;

	//이동 인풋 액션
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAcess = "true"))
	TObjectPtr<class UInputAction> ShoulderMoveAction;

	//카메라 회전 인풋 액션
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAcess = "true"))
	TObjectPtr<class UInputAction> ShoulderLookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAcess = "true"))
	TObjectPtr<class UInputAction> ToggleAimAction;

	//앉기 인풋 액션
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAcess = "true"))
	TObjectPtr<class UInputAction> CrouchAction;

	//공격 인풋 액션
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAcess = "true"))
	TObjectPtr<class UInputAction> RunAction;

	//공격 인풋 액션
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAcess = "true"))
	TObjectPtr<class UInputAction> AttackAction;

	UPROPERTY(EditAnywhere, Category = CharacterControl, Meta = (AllowPrivateAccess = "true"))
	TMap<ECharacterControlType, class UIFCharacterControlData*> CharacterControlManager;

	UPROPERTY(EditAnywhere, Category = CharacterControl, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UIFCharacterMovementData> CharacterMovemntData;

	ECharacterControlType CurControlType;

	void ShoulderMove(const FInputActionValue& Value);
	void ShoulderLook(const FInputActionValue& Value);
	void PerformRun();
	void PerformCrouch();

private:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class AIFGunBase> GunClass;

	UPROPERTY()
	TObjectPtr<class AIFGunBase> Gun;

	UPROPERTY()
	TObjectPtr<class UIFAnimInstance> AnimInstance;

	UPROPERTY()
	ECharacterMoveType CurMoveType;
};
