// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "IFCharacterBase.h"
#include "InputActionValue.h"
#include "Interface/IFGetDefineTypeInterface.h"
#include "Interface/IFUserWidgetInterface.h"
#include "IFCharacterPlayer.generated.h"


UCLASS()
class INFECTORY_API AIFCharacterPlayer : public AIFCharacterBase, public IIFGetDefineTypeInterface, public IIFUserWidgetInterface
{
	GENERATED_BODY()
	
public:
	AIFCharacterPlayer();

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	FORCEINLINE virtual ECharacterMoveType GetPlayerMoveType() override { return CurMoveType; }
	FORCEINLINE virtual ECharacterControlType GetPlayerControlType() override { return CurControlType; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE class AIFGunBase* GetCurGun() { return Gun.Get(); }

protected:
	virtual void BeginPlay() override;
	void ChangeCharacterControl();
	void SetCharacterControlData(const class UIFCharacterControlData* CharacterControlData);
	void SetupUserWidget(TObjectPtr<class UIFUserWidget> InUserWidget) override;
	void SetCharacterControl(ECharacterControlType NewCharacterControlType);
	void OnHitAction();

	void ShoulderMove(const FInputActionValue& Value);
	void ShoulderLook(const FInputActionValue& Value);
	void ShoulderMoveFinish();
	void PerformRun();
	void PerformCrouch();
	void Shoot();
	void Reload();


private:
	FVector GetGunHandPosition();

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

	//이동 인풋 액션
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAcess = "true"))
	TObjectPtr<class UInputAction> RunAction;

	//공격 인풋 액션
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAcess = "true"))
	TObjectPtr<class UInputAction> AttackAction;

	//재장전 인풋 액션
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAcess = "true"))
	TObjectPtr<class UInputAction> ReloadAction;

	UPROPERTY(EditAnywhere, Category = CharacterControl, Meta = (AllowPrivateAccess = "true"))
	TMap<ECharacterControlType, class UIFCharacterControlData*> CharacterControlManager;

	UPROPERTY(EditAnywhere, Category = CharacterControl, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UIFCharacterMovementData> CharacterMovemntData;

	UPROPERTY(EditAnywhere, Category = Camera, Meta = (AllowPrivateAccess = "true"))
	float MaxPitchValue;

	UPROPERTY(EditAnywhere, Category = Camera, Meta = (AllowPrivateAccess = "true"))
	float MinPitchValue;

	ECharacterControlType CurControlType;
	ECharacterState CurCharacterState;
	
private:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class AIFGunBase> GunClass;

	UPROPERTY()
	TObjectPtr<class AIFGunBase> Gun;

	UPROPERTY()
	TObjectPtr<class UIFPlayerAnimInstance> AnimInstance;

	UPROPERTY()
	ECharacterMoveType CurMoveType;

	UPROPERTY()
	TObjectPtr<UIFUserWidget> UserWidget;

	bool IsFiring = false;



};
