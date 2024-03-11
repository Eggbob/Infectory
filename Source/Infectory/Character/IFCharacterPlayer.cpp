// Fill out your copyright notice in the Description page of Project Settings.


#include "IFCharacterPlayer.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "IFCharacterControlData.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Character/IFCharacterMovementData.h"
#include "Stat/IFStatComponent.h"
#include "Item/IFGunBase.h"
#include "UI/IFUserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Item/IFInventory.h"
#include "Animation/IFPlayerAnimInstance.h"
#include "Item/IFGadget.h"
#include "Game/IFGameMode.h"
#include "Components/SpotLightComponent.h"


AIFCharacterPlayer::AIFCharacterPlayer()
{
	//spring arm 컴포넌트 생성 및 설정
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.f;
	CameraBoom->bUsePawnControlRotation = true;

	//카메라 생성 및 Spring Arm과 연동
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	FlashLight = CreateDefaultSubobject<USpotLightComponent>(TEXT("FlashLight"));
	FlashLight->SetupAttachment(CameraBoom);

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> CharacterMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/ParagonWraith/Characters/Heroes/Wraith/Skins/ODGreen/Meshes/Wraith_ODGreen.Wraith_ODGreen'"));
	if (CharacterMeshRef.Object)
	{
		GetMesh()->SetSkeletalMesh(CharacterMeshRef.Object);
	}

	static ConstructorHelpers::FClassFinder<UAnimInstance> AnimInstanceClassRef(TEXT("/Game/Assets/Animation/Player/ABP_Player.ABP_Player_C"));
	if (AnimInstanceClassRef.Class)
	{
		GetMesh()->SetAnimInstanceClass(AnimInstanceClassRef.Class);
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionShoulderMoveRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Assets/Input/Actions/IA_ShoulderMove.IA_ShoulderMove'"));
	if (nullptr != InputActionShoulderMoveRef.Object)
	{
		ShoulderMoveAction = InputActionShoulderMoveRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionShoulderLookRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Assets/Input/Actions/IA_ShoulderLook.IA_ShoulderLook'"));
	if (nullptr != InputActionShoulderLookRef.Object)
	{
		ShoulderLookAction = InputActionShoulderLookRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionRunRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Assets/Input/Actions/IA_Run.IA_Run'"));
	if (nullptr != InputActionRunRef.Object)
	{
		RunAction = InputActionRunRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionCrouchRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Assets/Input/Actions/IA_Crouch.IA_Crouch'"));
	if (nullptr != InputActionCrouchRef.Object)
	{
		CrouchAction = InputActionCrouchRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionAttackRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Assets/Input/Actions/IA_ShoulderAttack.IA_ShoulderAttack'"));
	if (nullptr != InputActionAttackRef.Object)
	{
		AttackAction = InputActionAttackRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionReloadRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Assets/Input/Actions/IA_Reload.IA_Reload'"));
	if (nullptr != InputActionReloadRef.Object)
	{
		ReloadAction = InputActionReloadRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionToggleAimRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Assets/Input/Actions/IA_ToggleAim.IA_ToggleAim'"));
	if (nullptr != InputActionToggleAimRef.Object)
	{
		ToggleAimAction = InputActionToggleAimRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionSpawnTurret(TEXT("/Script/EnhancedInput.InputAction'/Game/Assets/Input/Actions/IA_SpawnTurret.IA_SpawnTurret'"));
	if (nullptr != InputActionSpawnTurret.Object)
	{
		SpawnTurretAction = InputActionSpawnTurret.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> ChangeWeapon1Ref(TEXT("/Script/EnhancedInput.InputAction'/Game/Assets/Input/Actions/IA_ChangeWeapon1.IA_ChangeWeapon1'"));
	if (nullptr != ChangeWeapon1Ref.Object)
	{
		ChangeWeaponAction1 = ChangeWeapon1Ref.Object;
	}
	static ConstructorHelpers::FObjectFinder<UInputAction> ChangeWeapon2Ref(TEXT("/Script/EnhancedInput.InputAction'/Game/Assets/Input/Actions/IA_ChangeWeapon2.IA_ChangeWeapon2'"));
	if (nullptr != ChangeWeapon2Ref.Object)
	{
		ChangeWeaponAction2 = ChangeWeapon2Ref.Object;
	}
	static ConstructorHelpers::FObjectFinder<UInputAction> ChangeWeapon3Ref(TEXT("/Script/EnhancedInput.InputAction'/Game/Assets/Input/Actions/IA_ChangeWeapon3.IA_ChangeWeapon3'"));
	if (nullptr != ChangeWeapon3Ref.Object)
	{
		ChangeWeaponAction3 = ChangeWeapon3Ref.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> RegistRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Assets/Input/Actions/IA_Regist.IA_Regist'"));
	if (nullptr != RegistRef.Object)
	{
		RegistAction = RegistRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UIFCharacterControlData> ShoulderDataRef(TEXT("/Script/Infectory.IFCharacterControlData'/Game/Assets/CharacterControl/ABC_Shoulder.ABC_Shoulder'"));
	if (ShoulderDataRef.Object)
	{
		CharacterControlManager.Add(ECharacterControlType::Shoulder, ShoulderDataRef.Object);
	}

	static ConstructorHelpers::FObjectFinder<UIFCharacterControlData> ZoomDataRef(TEXT("/Script/Infectory.IFCharacterControlData'/Game/Assets/CharacterControl/ABC_Zoom.ABC_Zoom'"));
	if (ZoomDataRef.Object)
	{
		CharacterControlManager.Add(ECharacterControlType::Zoom, ZoomDataRef.Object);
	}

	static ConstructorHelpers::FObjectFinder<UIFCharacterMovementData> MovementDataRef(TEXT("/Script/Infectory.IFCharacterMovementData'/Game/Assets/CharacterControl/ABC_CharacterMovement.ABC_CharacterMovement'"));
	if (MovementDataRef.Object)
	{
		CharacterMovemntData = MovementDataRef.Object;
	}

	CurControlType = ECharacterControlType::Shoulder;
	CurMoveType = ECharacterMoveType::Walking;
}


void AIFCharacterPlayer::BeginPlay()
{
	Super::BeginPlay();
	SetCharacterControl(CurControlType);

	Inventory = NewObject<UIFInventory>();
	Inventory.Get()->InitInventory(GetWorld());

	SetWeapon();
	UserWidget->UpdateAmmoState(CurGun->GetCurAmmo(), CurGun->GetTotalAmmo());
	CurGun.Get()->AmmoChangedDelegate.BindUObject(UserWidget, &UIFUserWidget::UpdateAmmoState);
	CurGun.Get()->CrossHairDelegate.BindUObject(UserWidget, &UIFUserWidget::UpdateCrossHair);

	AnimInstance = Cast<UIFPlayerAnimInstance>(GetMesh()->GetAnimInstance());
	AnimInstance->OnLeftIKChange.BindUObject(this, &AIFCharacterPlayer::GetGunHandPosition);
	AnimInstance->OnHitAnimFinished.BindLambda([&]() { 
		CurCharacterState = ECharacterState::Idle; 
		UserWidget->ActiveCrossHair(true);
	});
	AnimInstance->OnReloadFinished.BindLambda([&]() { 
		CurCharacterState = ECharacterState::Idle;
		CurGun.Get()->Reload();
	});

	AnimInstance->OnRecover.BindLambda([&]() {
		CurCharacterState = ECharacterState::Idle;
		CameraBoom.Get()->SetRelativeLocation(FVector(0.f, 44.0f, 52.f));
	});

	AnimInstance.Get()->SetFootSound(FootStepSound);

	StatComp->ForTest();
	StatComp->bIsNPC = false;
	StatComp->OnHit.AddUObject(this, &AIFCharacterPlayer::OnHitAction);
	StatComp->OnHpZero.AddUObject(AnimInstance, &UIFPlayerAnimInstance::PlayDeadAnim);
	
	GameMode = Cast<AIFGameMode>(GetWorld()->GetAuthGameMode());

	TWeakObjectPtr<APlayerCameraManager> CameraManager = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0);
	if (CameraManager != nullptr)
	{
		CameraManager.Get()->ViewPitchMax = MaxPitchValue;
		CameraManager.Get()->ViewPitchMin = MinPitchValue;
	}
}

void AIFCharacterPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	SetAudioLitener();
}

/// <summary>
/// 컨트롤 타입 변경
/// </summary>
void AIFCharacterPlayer::ChangeCharacterControl()
{
	if (CurControlType == ECharacterControlType::Shoulder)
	{
		SetCharacterControl(ECharacterControlType::Zoom);
		if (CurMoveType != ECharacterMoveType::Walking)
		{
			PerformRun();
		}
	}
	else if (CurControlType == ECharacterControlType::Zoom)
	{
		CurGun->StopFire();
		SetCharacterControl(ECharacterControlType::Shoulder);
	}
}

/// <summary>
/// 변경한 컨트롤 타입 적용
/// </summary>
/// <param name="NewCharacterControlType"></param>
void AIFCharacterPlayer::SetCharacterControl(ECharacterControlType NewCharacterControlType)
{
	UIFCharacterControlData* NewCharacterControl = CharacterControlManager[NewCharacterControlType];
	check(NewCharacterControl);

	SetCharacterControlData(NewCharacterControl);

	APlayerController* PlayerController = CastChecked<APlayerController>(GetController());
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
	{
		//현재 매핑 컨텍스트 초기화 후 새로운 매핑 컨텍스트 추가
		Subsystem->ClearAllMappings();
		UInputMappingContext* NewMappingContext = NewCharacterControl->InputMappingContext;
		if (NewMappingContext)
		{
			Subsystem->AddMappingContext(NewMappingContext, 0);
		}
	}

	//현재 캐릭터 컨트롤 타입 갱신
	CurControlType = NewCharacterControlType;
}

void AIFCharacterPlayer::OnHitAction()
{
	GameMode.Get()->PlayCameraShake(HitCameraShake);
	CurCharacterState = ECharacterState::Hitting;
	UserWidget.Get()->ActiveCrossHair(false);
	UserWidget.Get()->PlayHitEffect();

	AnimInstance.Get()->SetCurSound(HitSound);
	AnimInstance.Get()->PlayHitAnim();
	IsFiring = false;

	if (CurControlType == ECharacterControlType::Zoom)
	{
		ChangeCharacterControl();
	}
}


void AIFCharacterPlayer::OnLeftMouseClick()
{
	switch (CurCharacterState)
	{
	case ECharacterState::Idle:
		Shoot();
		break;
	case ECharacterState::Building:
		GetTurretLoc();
		break;
	}
}


void AIFCharacterPlayer::Shoot()
{
	if (CurCharacterState != ECharacterState::Idle) return;

	IsFiring = IsFiring ? false : true ;

	if (IsFiring)
	{
		CurGun.Get()->ShootDelegate.Unbind();
		CurGun.Get()->ShootDelegate.BindLambda([&](TSubclassOf<ULegacyCameraShake> CameraShake) {
			GameMode.Get()->PlayCameraShake(CameraShake);
			}); 
		CurGun->FireGunDelegate.Unbind();

		switch (CurControlType)
		{
		case ECharacterControlType::Shoulder:
			CurGun->FireGunDelegate.BindLambda([&](ERangedWeaponType RangedWeaponType) {
				AnimInstance.Get()->PlayFireAnimation();
				});
			break;
		case ECharacterControlType::Zoom:
			CurGun->FireGunDelegate.BindUObject(AnimInstance, &UIFPlayerAnimInstance::AddRecoil);
			break;
		}

		
		CurGun->StartFire(FollowCamera.Get()->GetForwardVector());
	}
	else if(!IsFiring)
	{
		CurGun->StopFire();
	}
}

void AIFCharacterPlayer::Reload()
{
	if (IsFiring) return;
	if (!CurGun.Get()->CanReload()) return;
	if(CurCharacterState == ECharacterState::Reloading) return;

	CurCharacterState = ECharacterState::Reloading;
	AnimInstance.Get()->SetCurSound(CurGun.Get()->ReloadSound);
	AnimInstance.Get()->PlayReloadAnim(CurGun.Get()->GetWeaponType());
}


void AIFCharacterPlayer::StartGrabbing()
{
	CurCharacterState = ECharacterState::Grabbing;
	IsFiring = false;

	UserWidget.Get()->EnableGrabBar(true);

	if (CurControlType == ECharacterControlType::Zoom)
	{
		ChangeCharacterControl();
	}
}

void AIFCharacterPlayer::ResistGrabbing()
{
 	ResistCnt++;
	ResistCnt = FMath::Clamp(ResistCnt, 0, 10);

	UserWidget.Get()->UpdateGrabBarCount(ResistCnt);
	
	if (ResistCnt >= 10)
	{
		ClearGrab(false);
	}
}

void AIFCharacterPlayer::ClearGrab(bool bIsThrowing)
{
	ResistCnt = 0;
	UserWidget.Get()->EnableGrabBar(false);
	CurCharacterState = ECharacterState::Idle;
	RegistGrabDelegate.ExecuteIfBound();


	if (bIsThrowing)
	{
		CurCharacterState = ECharacterState::Lying;

		FTimerHandle TimerHandle;
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateLambda([&]() {
			GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
			}), 1.0f, false);

		GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Flying);
		AnimInstance.Get()->StopAllMontages(0.f);
		AnimInstance.Get()->PlayStunAnimation();

		CameraBoom.Get()->SetRelativeLocation(FVector(-100.f, 44.0f, 0.f));

	/*	AnimInstance.Get()->OnRecover.BindLambda([&]() {
			ClearGrab(false);
		});*/
	}
	
}

void AIFCharacterPlayer::BuildGadget(FVector TurretLoc, FVector SpawnLoc, bool bCanBuild)
{
	if (!bCanBuild) return;
	
	IsFiring = IsFiring ? false : true;

	if (IsFiring)
	{
		TObjectPtr<AIFGadget> Gadget = Inventory.Get()->GetGadget(EGadgetType::Shield);
		if (Gadget == nullptr)
		{
			Gadget = GetWorld()->SpawnActor<AIFGadget>(TurretBP, SpawnLoc, FRotator::ZeroRotator);
		}

		Gadget.Get()->SetActorLocation(SpawnLoc);
		Gadget.Get()->InitGadget(GetController());
		Gadget.Get()->LaunchGadget(TurretLoc);
		Gadget.Get()->GadgetDeInitDelegate.BindLambda([&]() {
			RecallTurret(EGadgetType::Shield);
		});

		AnimInstance.Get()->PlayThrowAnimation();
	}
}

void AIFCharacterPlayer::RecallTurret(EGadgetType GadgetType)
{
	Inventory.Get()->RecallGadget(GadgetType);
}


void AIFCharacterPlayer::ChangeWeapon1()
{
	if (CurGun.Get()->GetWeaponType() == ERangedWeaponType::Rifle) return;
	ChangeWeaponBody(ERangedWeaponType::Rifle);
}

void AIFCharacterPlayer::ChangeWeapon2()
{
	if (CurGun.Get()->GetWeaponType() == ERangedWeaponType::ShotGun) return;
	ChangeWeaponBody(ERangedWeaponType::ShotGun);
}

void AIFCharacterPlayer::ChangeWeapon3()
{
	if (CurGun.Get()->GetWeaponType() == ERangedWeaponType::Projectile) return;
	ChangeWeaponBody(ERangedWeaponType::Projectile);
}

void AIFCharacterPlayer::ChangeWeaponBody(ERangedWeaponType NewWeaponType)
{
	CurWeaponType = NewWeaponType;
	CurGun->StopFire();
	CurCharacterState = ECharacterState::WeaponChanging;

	AnimInstance.Get()->SetCurSound(CurGun.Get()->SwapSound);
	AnimInstance.Get()->PlayWeaponChangeAnim();
	AnimInstance.Get()->OnWeaponChangeFinished.Unbind();
	AnimInstance.Get()->OnWeaponChangeFinished.BindLambda([&]() {
		CurGun->SetActorHiddenInGame(true);
		CurGun = Inventory.Get()->GetRangedWeapon(CurWeaponType);
		CurGun.Get()->SetActorHiddenInGame(false);

		UserWidget->UpdateAmmoState(CurGun->GetCurAmmo(), CurGun->GetTotalAmmo());
		CurGun.Get()->AmmoChangedDelegate.BindUObject(UserWidget, &UIFUserWidget::UpdateAmmoState);
		CurGun.Get()->CrossHairDelegate.BindUObject(UserWidget, &UIFUserWidget::UpdateCrossHair);
		CurCharacterState = ECharacterState::Idle;
		});
}



/// <summary>
/// 캐릭터 컨트롤러 데이터 설정
/// </summary>
/// <param name="CharacterControlData"></param>
void AIFCharacterPlayer::SetCharacterControlData(const UIFCharacterControlData* CharacterControlData)
{
	//Pawn
	bUseControllerRotationYaw = CharacterControlData->bUseControllerRoationYaw;

	// CharacterMovement
	GetCharacterMovement()->bOrientRotationToMovement = CharacterControlData->bOrientRotationToMovement;
	GetCharacterMovement()->bUseControllerDesiredRotation = CharacterControlData->bUsecontrollerDesiredRotation;
	GetCharacterMovement()->RotationRate = CharacterControlData->RoationRate;

	//카메라 설정을 캐릭터 컨트롤 데이터에 맞게 조절
	CameraBoom->TargetArmLength = CharacterControlData->TargetArmLength;
	CameraBoom->SetRelativeRotation(CharacterControlData->RelativeRotation);
	CameraBoom->bUsePawnControlRotation = CharacterControlData->bUsePawnControlRotation;
	CameraBoom->bInheritPitch = CharacterControlData->bInheritPitch;
	CameraBoom->bInheritYaw = CharacterControlData->bInheritYaw;
	CameraBoom->bInheritRoll = CharacterControlData->bInheritRoll;
	CameraBoom->bDoCollisionTest = CharacterControlData->bCollisionTest;
	FollowCamera->FieldOfView = CharacterControlData->Fov;
}


void AIFCharacterPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);
	
	EnhancedInputComponent->BindAction(ShoulderMoveAction, ETriggerEvent::Triggered, this, &AIFCharacterPlayer::ShoulderMove);
	EnhancedInputComponent->BindAction(ShoulderMoveAction, ETriggerEvent::Completed, this, &AIFCharacterPlayer::ShoulderMoveFinish);
	EnhancedInputComponent->BindAction(ShoulderLookAction, ETriggerEvent::Triggered, this, &AIFCharacterPlayer::ShoulderLook);
	EnhancedInputComponent->BindAction(RunAction, ETriggerEvent::Started, this, &AIFCharacterPlayer::PerformRun);
	//EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Started, this, &AIFCharacterPlayer::PerformCrouch);
	EnhancedInputComponent->BindAction(ToggleAimAction, ETriggerEvent::Triggered, this, &AIFCharacterPlayer::ChangeCharacterControl);
	EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Started, this, &AIFCharacterPlayer::OnLeftMouseClick);
	EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Completed, this, &AIFCharacterPlayer::OnLeftMouseClick);
	EnhancedInputComponent->BindAction(ReloadAction, ETriggerEvent::Triggered, this, &AIFCharacterPlayer::Reload);
	EnhancedInputComponent->BindAction(ChangeWeaponAction1, ETriggerEvent::Started, this, &AIFCharacterPlayer::ChangeWeapon1);
	EnhancedInputComponent->BindAction(ChangeWeaponAction2, ETriggerEvent::Started, this, &AIFCharacterPlayer::ChangeWeapon2);
	EnhancedInputComponent->BindAction(ChangeWeaponAction3, ETriggerEvent::Started, this, &AIFCharacterPlayer::ChangeWeapon3);
	EnhancedInputComponent->BindAction(RegistAction, ETriggerEvent::Started, this, &AIFCharacterPlayer::ResistGrabbing);
	EnhancedInputComponent->BindAction(SpawnTurretAction, ETriggerEvent::Started, this, &AIFCharacterPlayer::SetBuildMode);
}

FVector AIFCharacterPlayer::GetGunHandPosition()
{
	return CurGun->GetWeaponSocket();
}

void AIFCharacterPlayer::SetWeapon()
{
	const UEnum* EnumPtr = StaticEnum<ERangedWeaponType>();

	for (int i = 0; i < EnumPtr->NumEnums() - 1; i++)
	{
		AIFGunBase* GunBase = Inventory.Get()->GetRangedWeapon((ERangedWeaponType)EnumPtr->GetValueByIndex(i));

		if (GunBase)
		{
			GunBase->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, WeaponSocketMap[GunBase->GetWeaponType()]);
			GunBase->SetOwner(this);
			GunBase->CachingOwner();
		}
	}

	CurGun = Inventory.Get()->GetRangedWeapon(ERangedWeaponType::Rifle);
	CurGun.Get()->SetActorHiddenInGame(false);
}

void AIFCharacterPlayer::SetupUserWidget(TObjectPtr<UIFUserWidget> InUserWidget)
{
	if (InUserWidget)
	{
		UserWidget = InUserWidget;

	/*	InUserWidget->UpdateAmmoState(Gun->GetCurAmmo(), Gun->GetTotalAmmo());

		Gun.Get()->ReloadDelegate.BindUObject(InUserWidget, &UIFUserWidget::UpdateAmmoState);*/
	}
}

void AIFCharacterPlayer::ShoulderMove(const FInputActionValue& Value)
{
	if (CurCharacterState == ECharacterState::Grabbing || CurCharacterState == ECharacterState::Lying) return;

	//입력값에서 이동 벡터를 추출
	FVector2D MovementVector = Value.Get<FVector2D>();

	// 이동 벡터의 방향을 구함
	FVector MovementDirection = FVector(MovementVector.X, 0.f, 0.f).GetSafeNormal();

	// 이동 방향이 존재하고, 전방으로 이동할 때만
	if (!MovementDirection.IsNearlyZero() && MovementVector.Y < KINDA_SMALL_NUMBER )
	{
		// 이동 방향을 현재 카메라의 방향으로 회전
		FRotator NewYawRotation = Controller->GetControlRotation();
		NewYawRotation.Pitch = 0.0f;
		NewYawRotation.Roll = 0.0f;

		//UE_LOG(LogTemp, Warning, TEXT("%s"), *MovementVector.ToString());

		// 현재 캐릭터의 회전 값을 보간하여 새로운 회전 값을 얻음
		FRotator NewRotation = FMath::RInterpTo(GetActorRotation(), NewYawRotation, GetWorld()->GetDeltaSeconds(), 10.0f);

		// 회전을 적용
		SetActorRotation(NewRotation);
	}


	// 현재 컨트롤러의 회전값을 가져옴
	const FRotator Rotation = Controller->GetControlRotation();
	//Yaw 회전값만 사용하여 회전 행렬을 생성
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	// 회전 행렬을 사용하여 전방과 오른쪽 방향 벡터를 계산
	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	//이동 벡터에 따라 전방 및 오른쪽으로 이동
	AddMovementInput(ForwardDirection, MovementVector.X);
	AddMovementInput(RightDirection, MovementVector.Y);
}

void AIFCharacterPlayer::ShoulderLook(const FInputActionValue& Value)
{
	if (CurCharacterState == ECharacterState::Grabbing || CurCharacterState == ECharacterState::Lying) return;

	//입력값에서 시선 조절 벡터를 추출
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	//Yaw 회전값에 따라 Yaw 회전을 조절
	AddControllerYawInput(LookAxisVector.X);
	// Pitch 회전값에 따라 Pitch 회전을 조절
	AddControllerPitchInput(LookAxisVector.Y);

}

void AIFCharacterPlayer::ShoulderMoveFinish()
{
	if (CurMoveType != ECharacterMoveType::Walking)
	{
		PerformRun();
	}
}

void AIFCharacterPlayer::PerformRun()
{
	switch (CurMoveType)
	{
	case ECharacterMoveType::Walking:
		UserWidget.Get()->ActiveCrossHair(false);
		CurMoveType = ECharacterMoveType::Running;
		if (CurControlType == ECharacterControlType::Zoom)
		{
			ChangeCharacterControl();
		}
		break;
	case ECharacterMoveType::Running:
	case ECharacterMoveType::Sprinting:
		UserWidget.Get()->ActiveCrossHair(true);
		CurMoveType = ECharacterMoveType::Walking;
		break;
	case ECharacterMoveType::Crouching:
		CurMoveType = ECharacterMoveType::Crouching;
		break;
	case ECharacterMoveType::Proning:
		CurMoveType = ECharacterMoveType::Proning;
		break;
	}

	GetCharacterMovement()->MaxWalkSpeed = CharacterMovemntData->MoveSpeed[CurMoveType]; //600.f;
}

void AIFCharacterPlayer::PerformCrouch()
{
	switch (CurMoveType)
	{
	case ECharacterMoveType::Walking:
	case ECharacterMoveType::Running:
	case ECharacterMoveType::Sprinting:
		CurMoveType = ECharacterMoveType::Crouching;
		break;
	case ECharacterMoveType::Crouching:
		CurMoveType = ECharacterMoveType::Walking;
		break;
	case ECharacterMoveType::Proning:
		CurMoveType = ECharacterMoveType::Walking;
		break;
	}

	GetCharacterMovement()->MaxWalkSpeed = CharacterMovemntData->MoveSpeed[CurMoveType];
}


