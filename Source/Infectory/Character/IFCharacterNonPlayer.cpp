// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/IFCharacterNonPlayer.h"
#include "Stat/IFStatComponent.h"
#include "Components/CapsuleComponent.h"
#include "AI/IFAIController.h"
#include "Data/IFEnumDefine.h"
#include "Engine/DamageEvents.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AI/IFAI.h"
#include "Item/IFGunBase.h"
#include "Item/IFShield.h"
#include "Animation/IFNonPlayerAnimInstance.h"


AIFCharacterNonPlayer::AIFCharacterNonPlayer()
{
#pragma region MeshSet


	static ConstructorHelpers::FObjectFinder<USkeletalMesh> TestMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/Assets/Characters/Test/Parasite_L_Starkie.Parasite_L_Starkie'"));
	if (TestMeshRef.Object)
	{
		GetMesh()->SetSkeletalMesh(TestMeshRef.Object);
		NPCSkeletalMeshes.Add(ENPCType::Test, TestMeshRef.Object);
	}

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> ParasiteMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/ParasiteZombieBundle01/ParasiteZombie02/ParasiteZombie02_Character/SK_ParasiteZombie02.SK_ParasiteZombie02'"));
	if (ParasiteMeshRef.Object)
	{
		//GetMesh()->SetSkeletalMesh(ParasiteMeshRef.Object);
		NPCSkeletalMeshes.Add(ENPCType::Parasite, ParasiteMeshRef.Object);
		NPCSkeletalMeshes.Add(ENPCType::RangedParasite, ParasiteMeshRef.Object);
	}

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> RangedParasiteMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/ParasiteZombieBundle01/ParasiteZombie02/ParasiteZombie02_Character/SK_ParasiteZombie02.SK_ParasiteZombie02'"));
	if (RangedParasiteMeshRef.Object)
	{
		NPCSkeletalMeshes.Add(ENPCType::RangedParasite, RangedParasiteMeshRef.Object);
	}

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> HunterMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/ParasiteZombieBundle01/ParasiteZombie01/ParasiteZombie01_Character/SK_ParasiteZombie01.SK_ParasiteZombie01'"));
	if (HunterMeshRef.Object)
	{
		NPCSkeletalMeshes.Add(ENPCType::Hunter, HunterMeshRef.Object);
	}

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> HunterMiniMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/ParasiteZombieBundle01/ParasiteZombie01/ParasiteZombie01_Character/SK_HunterMini.SK_HunterMini'"));
	if (HunterMiniMeshRef.Object)
	{
		NPCSkeletalMeshes.Add(ENPCType::MiniHunter, HunterMiniMeshRef.Object);
	}

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> BoomerMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/ParasiteZombieBundle01/ParasiteZombie03/ParasiteZombie03_Character/SK_ParasiteZombie03.SK_ParasiteZombie03'"));
	if (BoomerMeshRef.Object)
	{
		NPCSkeletalMeshes.Add(ENPCType::Boomer, BoomerMeshRef.Object);
	}

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> BigBoomerMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/ParasiteZombieBundle01/ParasiteZombie03/ParasiteZombie03_Character/SM_BoomerShield.SM_BoomerShield'"));
	if (BigBoomerMeshRef.Object)
	{
		NPCSkeletalMeshes.Add(ENPCType::BigBoomer, BigBoomerMeshRef.Object);
	}
	

#pragma endregion

#pragma region AnimationSet

	static ConstructorHelpers::FClassFinder<UAnimInstance> TestAnimInstanceClassRef(TEXT("/Game/Assets/Animation/Enemy/ABP_TestEnemy.ABP_TestEnemy_C"));
	if (TestAnimInstanceClassRef.Class)
	{
		NPCAnimInstances.Add(ENPCType::Test, TestAnimInstanceClassRef.Class);
	}

	static ConstructorHelpers::FClassFinder<UAnimInstance> ParasiteAnimInstanceClassRef(TEXT("/Game/Assets/Animation/Enemy/ABP_Parasite.ABP_Parasite_C"));
	if (ParasiteAnimInstanceClassRef.Class)
	{
		NPCAnimInstances.Add(ENPCType::Parasite, ParasiteAnimInstanceClassRef.Class);
	}

	static ConstructorHelpers::FClassFinder<UAnimInstance> RangedParasiteAnimInstanceClassRef(TEXT("/Game/Assets/Animation/Enemy/ABP_ParasiteRanged.ABP_ParasiteRanged_C"));
	if (RangedParasiteAnimInstanceClassRef.Class)
	{
		NPCAnimInstances.Add(ENPCType::RangedParasite, RangedParasiteAnimInstanceClassRef.Class);
	}

	static ConstructorHelpers::FClassFinder<UAnimInstance> HunterAnimInstanceClassRef(TEXT("/Game/Assets/Animation/Enemy/ABP_Hunter.ABP_Hunter_C"));
	if (HunterAnimInstanceClassRef.Class)
	{
		NPCAnimInstances.Add(ENPCType::Hunter, HunterAnimInstanceClassRef.Class);
	}

	static ConstructorHelpers::FClassFinder<UAnimInstance> MiniHunterAnimInstanceClassRef(TEXT("/Game/Assets/Animation/Enemy/ABP_HunterMini.ABP_HunterMini_C"));
	if (MiniHunterAnimInstanceClassRef.Class)
	{
		NPCAnimInstances.Add(ENPCType::MiniHunter, MiniHunterAnimInstanceClassRef.Class);
	}

	static ConstructorHelpers::FClassFinder<UAnimInstance> BoomerAnimInstanceClassRef(TEXT("/Game/Assets/Animation/Enemy/ABP_Boomer.ABP_Boomer_C"));
	if (BoomerAnimInstanceClassRef.Class)
	{
		NPCAnimInstances.Add(ENPCType::Boomer, BoomerAnimInstanceClassRef.Class);
	}

	static ConstructorHelpers::FClassFinder<UAnimInstance> BigBoomerAnimInstanceClassRef(TEXT("/Game/Assets/Animation/Enemy/ABP_BigBoomer.ABP_BigBoomer_C"));
	if (BigBoomerAnimInstanceClassRef.Class)
	{
		NPCAnimInstances.Add(ENPCType::BigBoomer, BigBoomerAnimInstanceClassRef.Class);
	}

#pragma endregion

	AIControllerClass = AIFAIController::StaticClass();
	
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	CurNpcState = ENPCState::Idle;
	CurNpcMoveType = ENPCMoveType::Walking;
}

void AIFCharacterNonPlayer::BeginPlay()
{
	Super::BeginPlay();
	AIController = Cast<AIFAIController>(GetController());
	StatComp->OnHpZero.AddUObject(this, &AIFCharacterNonPlayer::SetDead);
	StatComp->bIsNPC = true;

	if (bIsJustSpawn)
	{
		SetNPCType(CurNPCType, CurNPCTier);
	}
}

void AIFCharacterNonPlayer::SetDead()
{
	Super::SetDead();
	CurNpcState = ENPCState::Dead;

	AnimInstance.Get()->StopAllMontages(0.f);
	GetCapsuleComponent()->SetCollisionProfileName("NoCollision");
	GetMesh()->SetCollisionProfileName("Ragdoll");
	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);

	if (AIController)
	{
		AIController.Get()->StopAI();
	}
	
}

void AIFCharacterNonPlayer::SetNPCType(ENPCType NpcName, FName NpcTier)
{
	CurNPCType = NpcName;
	CurNPCTier = NpcTier;

	GetMesh()->SetSkeletalMesh(NPCSkeletalMeshes[CurNPCType].Get());
	GetMesh()->SetAnimInstanceClass(NPCAnimInstances[CurNPCType].Get());

	StatComp.Get()->SetStat(*UIFEnumDefine::GetEnumName(CurNPCType), CurNPCTier);
	GetCharacterMovement()->MaxWalkSpeed = StatComp.Get()->GetBaseStat().MovementSpeed;
	GetMesh()->SetRelativeLocation(StatComp.Get()->GetBaseStat().MeshLocation);
	GetMesh()->SetRelativeScale3D(StatComp.Get()->GetBaseStat().MeshScale);

	FVector MeshExtent = GetMesh()->Bounds.BoxExtent;
	GetCapsuleComponent()->SetCapsuleHalfHeight(MeshExtent.Z);
	GetCapsuleComponent()->SetCapsuleRadius(FMath::Max(MeshExtent.X, MeshExtent.Y));

	if (CurNPCType == ENPCType::RangedParasite)
	{
		ProjectileWeapon = GetWorld()->SpawnActor<AIFGunBase>(GunClass);
		ProjectileWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("spine_03"));
		ProjectileWeapon->SetOwner(this);
		ProjectileWeapon->CachingOwner();
		ProjectileWeapon->SetActorRotation(FRotator(0.f, 0.f, 90.f));
	}
	else if (CurNPCType == ENPCType::BigBoomer)
	{
		ShieldObject = GetWorld()->SpawnActor<AIFShield>(ShieldClass);
		ShieldObject.Get()->SetOwner(this);
		ShieldObject.Get()->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("shieldsocket"));

		ShieldObject.Get()->SetShieldDestoryedDelegate(FOnShieldDestoryed::CreateLambda([&]()
			{
				AIController.Get()->StopAI();
				GetCharacterMovement()->StopActiveMovement();
				AnimInstance.Get()->PlaySpecialHitAnimation();
			}));
	}

	//UE_LOG(LogTemp, Warning, TEXT("MaxWalkSpeed : %f"), GetCharacterMovement()->MaxWalkSpeed);
	//UE_LOG(LogTemp, Warning, TEXT("MovementSpeed : %f"), StatComp->GetBaseStat().MovementSpeed);
	//UE_LOG(LogTemp, Warning, TEXT("MaxWalkSpeed2 : %f"), GetCharacterMovement()->MaxWalkSpeed);

	const UEnum* EnumPtr = StaticEnum<ENPCBoneName>();
	BodyDamageCheckMap.Empty();

	for (int i = 0; i < EnumPtr->NumEnums()-1; i++)
	{
		int RandVal = FMath::RandRange(2, 5);

		BodyDamageCheckMap.Add((ENPCBoneName)EnumPtr->GetValueByIndex(i), RandVal);
	}

	AnimInstance = Cast<UIFNonPlayerAnimInstance>(GetMesh()->GetAnimInstance());
	if (AnimInstance)
	{
		StatComp.Get()->OnHit.AddUObject(AnimInstance, &UIFNonPlayerAnimInstance::PlayHitAnim);
		AnimInstance->OnAttackEnd.BindUObject(this, &AIFCharacterNonPlayer::NotifyAttackActionEnd);
		AnimInstance->OnBackJump.BindUObject(this, &AIFCharacterNonPlayer::StartBackJump);
		AnimInstance->OnBackJumpEnd.BindUObject(this, &AIFCharacterNonPlayer::NotifyBackJumpActionEnd);
		AnimInstance->OnBeforeMoving.BindUObject(this, &AIFCharacterNonPlayer::NotifyBeforeMovingActionEnd);
		AnimInstance->OnHitEnd.BindLambda([&]() { 
			AIController.Get()->RunAI();
		});
	}


	InitPhysicsAnimation();
}



/// <summary>
/// AI가 Focus할 대상 지정
/// </summary>
/// <param name="TargetActor"></param>
void AIFCharacterNonPlayer::FocusingTarget(TObjectPtr<AActor> TargetActor)
{
	AIController->SetFocus(TargetActor, EAIFocusPriority::Gameplay);
	AIController->SetTarget(TargetActor);
}

/// <summary>
/// 공격 종료 후 실행할 이벤트 등록
/// </summary>
/// <param name="InOnAttackFinished"></param>
void AIFCharacterNonPlayer::SetAIAttackDelegate(const FAICharacterAttackFinished& InOnAttackFinished)
{
	OnAttackFinished.Unbind();
	OnAttackFinished = InOnAttackFinished;
}

void AIFCharacterNonPlayer::SetAIBackJumpDelegate(const FAICharacterBackJumpFinished& InOnBackJumpFinished)
{
	OnBackJumpFinished.Unbind();
	OnBackJumpFinished = InOnBackJumpFinished;
}

void AIFCharacterNonPlayer::SetAIBeforeMovingDelegate(const FAICharacterBeforeMovingFinished& InOnBeforeMovingFinished)
{
	OnBeforeMovingFinished.Unbind();
	OnBeforeMovingFinished = InOnBeforeMovingFinished;
}

void AIFCharacterNonPlayer::SetAIWaitingDelegate(const FAICharacterWaitingFinished& InOnWaitingFinished)
{
	OnWaitingFinished.Unbind();
	OnWaitingFinished = InOnWaitingFinished;

}

/// <summary>
/// 공격 시작
/// </summary>
void AIFCharacterNonPlayer::AttackByAI()
{
	CurNpcState = ENPCState::Attacking;
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
	AnimInstance->PlayAttackAnimation(StatComp->GetBaseStat().AttackSpeed);
}

/// <summary>
/// 공격 종료후 delegate 실행
/// </summary>
void AIFCharacterNonPlayer::NotifyAttackActionEnd()
{
	CurNpcState = ENPCState::Idle;
	OnAttackFinished.ExecuteIfBound();
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
}

/// <summary>
/// 백점프 애니메이션 재생
/// </summary>
void AIFCharacterNonPlayer::PeformBackMoveAI()
{
	if (CurNpcMoveType == ENPCMoveType::Crawling)
	{
		NotifyBackJumpActionEnd();
		return;
	}

	CurNpcState = ENPCState::Jumping;
	AnimInstance->PlayBackJumpAnimation();
}

void AIFCharacterNonPlayer::PerformMoving()
{
	AIController->MoveToTarget(GetAIAttackRange());
	//AnimInstance->PlayRandomIdleAnimaiton();
}

void AIFCharacterNonPlayer::PerformWaiting(bool bIsFirstContact)
{
	CurNpcState = ENPCState::Idle;
	float WaitTime = bIsFirstContact ? StatComp->GetBaseStat().EncounterTime : StatComp->GetBaseStat().WaitTime;
	
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateLambda([&]() 
		{
			OnWaitingFinished.ExecuteIfBound();
		}), WaitTime, false);

	AnimInstance->PlayRandomIdleAnimaiton();
}

/// <summary>
/// BackJump 실행
/// </summary>
void AIFCharacterNonPlayer::StartBackJump()
{
	FVector JumpDirection = GetActorForwardVector() * -1.0f * StatComp->GetBaseStat().JumpVelocity;// -300.0f;
	JumpDirection.Z = 400.f;
	LaunchCharacter(JumpDirection, false, false);
}

void AIFCharacterNonPlayer::SetHitWalkSpeed()
{
	FTimerHandle TimerHandle;

	float CurMaxSpeed = GetCharacterMovement()->MaxWalkSpeed;
	GetCharacterMovement()->MaxWalkSpeed = StatComp.Get()->GetBaseStat().MovementSpeed / 2.0f;

	GetWorld()->GetTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateLambda(
		[&]()
		{
			GetCharacterMovement()->MaxWalkSpeed = CurNpcMoveType == ENPCMoveType::Walking ?
				StatComp.Get()->GetBaseStat().MovementSpeed : StatComp.Get()->GetBaseStat().CrawlMovementSpeed;
		}
	), 0.3f, false);
}

void AIFCharacterNonPlayer::NotifyBackJumpActionEnd()
{
	CurNpcState = ENPCState::Idle;
	OnBackJumpFinished.ExecuteIfBound();
}

void AIFCharacterNonPlayer::NotifyBeforeMovingActionEnd()
{
	CurNpcState = ENPCState::Moving;
	OnBeforeMovingFinished.ExecuteIfBound();
}

float AIFCharacterNonPlayer::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);

	if (const FCustomDamageEvent* CustomDamageEvent = static_cast<const FCustomDamageEvent*>(&DamageEvent))
	{
		//TODo Fix Enum
		FString BoneNameStr = CustomDamageEvent->BoneName.ToString();
		ENPCBoneName BoneEnum = UIFEnumDefine::StringToEnum(BoneNameStr);
		BodyDamageCheckMap[BoneEnum] -= 1;

		if (BodyDamageCheckMap[BoneEnum] <= 0)
		{
			PlayHitReaction(CustomDamageEvent->BoneName, CustomDamageEvent->HitResult);
		}
	}

	FocusingTarget(DamageCauser);
	SetHitWalkSpeed();

	return Damage;
}

void AIFCharacterNonPlayer::AttackHitCheck()
{
	if (CurNPCType == ENPCType::RangedParasite)
	{
		ProjectileWeapon->StartFire();
		return;
	}
	else
	{
		MeleeAttackCheck();
		return;
	}
}

void AIFCharacterNonPlayer::MeleeAttackCheck()
{
	FHitResult OutHitResult;
	FCollisionQueryParams Params(SCENE_QUERY_STAT(Attack), false, this);

	const float AttackRange = StatComp->GetBaseStat().AttackRange;
	const float AttackRadius = 100.f;//StatComp->GetRadi;
	//const float AttackDamage = Stat->GetTotalStat().Attack;
	const FVector Start = GetActorLocation() + GetActorForwardVector() * GetCapsuleComponent()->GetScaledCapsuleRadius();
	const FVector End = Start + GetActorForwardVector() * AttackRange;

	bool HitDetected = GetWorld()->SweepSingleByChannel(OutHitResult, Start, End, FQuat::Identity, ECollisionChannel::ECC_GameTraceChannel3, FCollisionShape::MakeSphere(AttackRadius), Params);
	if (HitDetected)
	{
		//TODO Check Damage
		FDamageEvent DamageEvent;
		OutHitResult.GetActor()->TakeDamage(StatComp->GetBaseStat().Attack, DamageEvent, GetController(), this);
	}

#if ENABLE_DRAW_DEBUG

	FVector CapsuleOrigin = Start + (End - Start) * 0.5f;
	float CapsuleHalfHeight = AttackRange * 0.5f;
	FColor DrawColor = HitDetected ? FColor::Green : FColor::Red;

	DrawDebugCapsule(GetWorld(), CapsuleOrigin, CapsuleHalfHeight, AttackRadius, FRotationMatrix::MakeFromZ(GetActorForwardVector()).ToQuat(), DrawColor, false, 5.0f);

#endif
}

void AIFCharacterNonPlayer::ChangeNPCMoveMode()
{
	CurNpcMoveType = ENPCMoveType::Crawling;
	GetCharacterMovement()->MaxWalkSpeed = StatComp.Get()->GetBaseStat().CrawlMovementSpeed;
	AnimInstance->StopAllMontages(0.0f);
}

float AIFCharacterNonPlayer::GetAIPatrolRadius()
{
	return 800.0f;
}

float AIFCharacterNonPlayer::GetAIDetectRange()
{
	return StatComp->GetBaseStat().DetectRange; // 400.0f;
}

float AIFCharacterNonPlayer::GetAIAttackRange()
{
	return StatComp->GetBaseStat().AttackRange; //150.0f;
}

float AIFCharacterNonPlayer::GetAITurnSpeed()
{
	return 2.0f;
}


