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
	FORCEINLINE class AIFGunBase* GetCurGun() { return CurGun.Get(); }

	UFUNCTION(BlueprintImplementableEvent)
	void PlayCameraShake(TSubclassOf<class ULegacyCameraShake> CameraShake);

	UFUNCTION(BlueprintImplementableEvent)
	void SetAudioLitener();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
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

	void ChangeWeapon1();
	void ChangeWeapon2();
	void ChangeWeapon3();
	void ChangeWeaponBody(ERangedWeaponType NewWeaponType);

private:
	FVector GetGunHandPosition();
	void SetGunPos();

protected:
	//카메라 Arm
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, Meta = (AllowPrivateAcess = "true"))
	TObjectPtr<class USpringArmComponent> CameraBoom;

	//카메라
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, Meta = (AllowPrivateAcess = "true"))
	TObjectPtr<class UCameraComponent> FollowCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Light, Meta = (AllowPrivateAcess = "true"))
	TObjectPtr<class USpotLightComponent> FlashLight;

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

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class ULegacyCameraShake> HitCameraShake;
	
	//재장전 인풋 액션
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAcess = "true"))
	TObjectPtr<class UInputAction> ReloadAction;

	UPROPERTY(EditAnywhere)
	TObjectPtr<USoundBase> HitSound;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAcess = "true"))
	TObjectPtr<class UInputAction> ChangeWeaponAction1;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAcess = "true"))
	TObjectPtr<class UInputAction> ChangeWeaponAction2;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAcess = "true"))
	TObjectPtr<class UInputAction> ChangeWeaponAction3;

	UPROPERTY(EditAnywhere, Category = CharacterControl, Meta = (AllowPrivateAccess = "true"))
	TMap<ECharacterControlType, class UIFCharacterControlData*> CharacterControlManager;

	UPROPERTY(EditAnywhere, Category = CharacterControl, Meta = (AllowPrivateAccess = "true"))
	TMap<ERangedWeaponType, FName> WeaponSocketMap;

	UPROPERTY(EditAnywhere, Category = CharacterControl, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UIFCharacterMovementData> CharacterMovemntData;

	UPROPERTY(EditAnywhere, Category = Camera, Meta = (AllowPrivateAccess = "true"))
	float MaxPitchValue;

	UPROPERTY(EditAnywhere, Category = Camera, Meta = (AllowPrivateAccess = "true"))
	float MinPitchValue;

	ERangedWeaponType CurWeaponType;
	ECharacterControlType CurControlType;
	ECharacterState CurCharacterState;
	
private:
	UPROPERTY()
	TObjectPtr<class AIFGunBase> CurGun;

	UPROPERTY()
	TObjectPtr<class UIFInventory> Inventory;

	UPROPERTY()
	TObjectPtr<class UIFPlayerAnimInstance> AnimInstance;

	UPROPERTY()
	ECharacterMoveType CurMoveType;

	UPROPERTY()
	TObjectPtr<UIFUserWidget> UserWidget;


	bool IsFiring = false;



};
