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
#include "Item/IFGunBase.h"
#include "Animation/IFAnimInstance.h"

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

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionToggleAimRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Assets/Input/Actions/IA_ToggleAim.IA_ToggleAim'"));
	if (nullptr != InputActionToggleAimRef.Object)
	{
		ToggleAimAction = InputActionToggleAimRef.Object;
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

	Gun = GetWorld()->SpawnActor<AIFGunBase>(GunClass);
	Gun->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("WeaponSocket"));
	Gun->SetOwner(this);
	Gun->CachingOwner();

	AnimInstance = Cast<UIFAnimInstance>(GetMesh()->GetAnimInstance());

	AnimInstance->OnLeftIKChange.BindUObject(this, &AIFCharacterPlayer::GetGunHandPosition);
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

void AIFCharacterPlayer::Shoot()
{
	if (CurControlType == ECharacterControlType::Zoom)
	{
		Gun->PullTrigger();
	}
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
	EnhancedInputComponent->BindAction(ShoulderLookAction, ETriggerEvent::Triggered, this, &AIFCharacterPlayer::ShoulderLook);
	EnhancedInputComponent->BindAction(RunAction, ETriggerEvent::Started, this, &AIFCharacterPlayer::PerformRun);
	EnhancedInputComponent->BindAction(RunAction, ETriggerEvent::Completed, this, &AIFCharacterPlayer::PerformRun);
	EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Started, this, &AIFCharacterPlayer::PerformCrouch);
	EnhancedInputComponent->BindAction(ToggleAimAction, ETriggerEvent::Triggered, this, &AIFCharacterPlayer::ChangeCharacterControl);
	EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Started, this, &AIFCharacterPlayer::Shoot);
}

FVector AIFCharacterPlayer::GetGunHandPosition()
{
	return Gun->GetWeaponSocket();
}

void AIFCharacterPlayer::ShoulderMove(const FInputActionValue& Value)
{
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
	//입력값에서 시선 조절 벡터를 추출
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	//Yaw 회전값에 따라 Yaw 회전을 조절
	AddControllerYawInput(LookAxisVector.X);
	// Pitch 회전값에 따라 Pitch 회전을 조절
	AddControllerPitchInput(LookAxisVector.Y);
}

void AIFCharacterPlayer::PerformRun()
{
	switch (CurMoveType)
	{
	case ECharacterMoveType::Walking:
		CurMoveType = ECharacterMoveType::Running;
		break;
	case ECharacterMoveType::Running:
	case ECharacterMoveType::Sprinting:
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

