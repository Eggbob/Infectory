// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/IFFootIkComponent.h"
#include "GameFramework/Character.h"
#include "Animation/AnimInstance.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include <Kismet/KismetMathLibrary.h>

UIFFootIkComponent::UIFFootIkComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	IKFeetInterpSpeed = 22.0f;
	IKHipsInterpSpeed = 17.0f;
	IKTraceDistance = 55.0f;
	IKAdjustOffset = 5.0f;

	bDebug = false;
}

IK_TraceInfo UIFFootIkComponent::FootTrace(float TraceDistance, FName SocketName)
{
	IK_TraceInfo TraceInfo;

	FVector SocketLocation = OwnerCharacter->GetMesh()->GetSocketLocation(SocketName);
	FVector LineStart = FVector(SocketLocation.X, SocketLocation.Y, OwnerCharacter->GetActorLocation().Z);
	FVector LineEnd = FVector(SocketLocation.X, SocketLocation.Y, (OwnerCharacter->GetActorLocation().Z - IKCapsuleHalfHeight) - TraceDistance);

	FHitResult HitResult;
	TArray<AActor*> Ignore;
	Ignore.Add(GetOwner());

	EDrawDebugTrace::Type eDebug = EDrawDebugTrace::None;
	if (bDebug == true) eDebug = EDrawDebugTrace::ForOneFrame;

	bool bResult = UKismetSystemLibrary::LineTraceSingle(GetWorld(), LineStart, LineEnd,
		UEngineTypes::ConvertToTraceType(ECC_Visibility), true, Ignore, eDebug, HitResult, true);

	TraceInfo.ImpactLocation = HitResult.ImpactNormal;
	if (HitResult.IsValidBlockingHit())
	{
		float ImpactLength = (HitResult.ImpactPoint - HitResult.TraceEnd).Size();
		TraceInfo.Offset = IKAdjustOffset + (ImpactLength - TraceDistance);
	}
	else
	{
		TraceInfo.Offset = 0.f;
	}

	return TraceInfo;
}

FRotator UIFFootIkComponent::NormalToRotator(FVector Normal)
{
	float fAtan2_1 = UKismetMathLibrary::DegAtan2(Normal.Y, Normal.Z);
	float fAtan2_2 = UKismetMathLibrary::DegAtan2(Normal.X, Normal.Z);
	fAtan2_2 *= -1.f;
	FRotator Result = FRotator(fAtan2_2, 0.0f, fAtan2_1);

	return Result;
}

bool UIFFootIkComponent::IsMoving()
{
	float Speed = OwnerCharacter->GetVelocity().Size();
	if(Speed > 0.f) return true;

	return false;
}


void UIFFootIkComponent::BeginPlay()
{
	Super::BeginPlay();
	
	mDeltaTime = 0.0f;
	OwnerCharacter = Cast<ACharacter>(GetOwner());
	if (OwnerCharacter == nullptr) return;

	IKCapsuleHalfHeight = OwnerCharacter->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();

	SetIKActive(true);
}

void UIFFootIkComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	OwnerCharacter = nullptr;
}


void UIFFootIkComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if(OwnerCharacter == nullptr) return;
	if(mbActive == false) return;

	IKUpdate(DeltaTime);
	IKDebug();
}

void UIFFootIkComponent::SetIKSocketName(FString SocketName_LeftFoot, FString SocketName_RightFoot)
{
	IKSocketName_LeftFoot = FName(*SocketName_LeftFoot);
	IKSocketName_RightFoot = FName(*SocketName_RightFoot);
}

void UIFFootIkComponent::IKResetVars()
{
	IKUpdateFootOffset(mDeltaTime, 0.0f, &IKAnimValue.EffectorLocation_Right, IKFeetInterpSpeed);
	IKUpdateFootOffset(mDeltaTime, 0.0f, &IKAnimValue.EffectorLocation_Left, IKFeetInterpSpeed);

	IKUpdateFootRotation(mDeltaTime, FRotator::ZeroRotator, &IKAnimValue.FootRotation_Right, IKFeetInterpSpeed);
	IKUpdateFootRotation(mDeltaTime, FRotator::ZeroRotator, &IKAnimValue.FootRotation_Left, IKFeetInterpSpeed);

	IKUpdateFootOffset(mDeltaTime, 0.0f, &IKAnimValue.HipOffset, IKHipsInterpSpeed);
	IKUpdateCapsuleHalfHeight(mDeltaTime, 0.0f, true);
}

void UIFFootIkComponent::SetIKActive(bool bActive)
{
	this->mbActive = bActive;
}

void UIFFootIkComponent::SetIKDebug(bool bActive)
{
	bDebug = bActive;
}

void UIFFootIkComponent::IKUpdate(float DeltaTime)
{
	this->mDeltaTime = DeltaTime;

	IK_TraceInfo Trace_Left = FootTrace(IKTraceDistance, IKSocketName_LeftFoot);
	IK_TraceInfo Trace_Right = FootTrace(IKTraceDistance, IKSocketName_RightFoot);

	if (IsMoving() == false)
	{
		IKUpdateFootRotation(DeltaTime, NormalToRotator(Trace_Left.ImpactLocation), &IKAnimValue.FootRotation_Left, IKFeetInterpSpeed);
		IKUpdateFootRotation(DeltaTime, NormalToRotator(Trace_Right.ImpactLocation), &IKAnimValue.FootRotation_Right, IKFeetInterpSpeed);
	}
	else
	{
		IKUpdateFootRotation(DeltaTime, FRotator::ZeroRotator, &IKAnimValue.FootRotation_Left, IKFeetInterpSpeed);
		IKUpdateFootRotation(DeltaTime, FRotator::ZeroRotator, &IKAnimValue.FootRotation_Right, IKFeetInterpSpeed);
	}

	float HipsOffset = UKismetMathLibrary::Min(Trace_Left.Offset, Trace_Right.Offset);
	if(HipsOffset < 0.f == false) HipsOffset = 0.f;
	IKUpdateFootOffset(DeltaTime, HipsOffset, &IKAnimValue.HipOffset, IKHipsInterpSpeed);
	IKUpdateCapsuleHalfHeight(DeltaTime, HipsOffset, false);

	IKUpdateFootOffset(DeltaTime, Trace_Left.Offset - HipsOffset, &IKAnimValue.EffectorLocation_Left, IKFeetInterpSpeed);
	IKUpdateFootOffset(DeltaTime, -1*(Trace_Right.Offset - HipsOffset), &IKAnimValue.EffectorLocation_Right, IKFeetInterpSpeed);

}

void UIFFootIkComponent::IKDebug()
{
	if (bDebug)
	{
		UKismetSystemLibrary::PrintString(GetWorld(), "Foot_L Rotation : " + IKAnimValue.FootRotation_Left.ToString(), true, false, FLinearColor::Red, 0.f);
		UKismetSystemLibrary::PrintString(GetWorld(), "Foot_R Rotation : " + IKAnimValue.FootRotation_Right.ToString(), true, false, FLinearColor::Red, 0.0f);
		UKismetSystemLibrary::PrintString(GetWorld(), "Foot_L EffectorLocation(Z) : " + FString::SanitizeFloat(IKAnimValue.EffectorLocation_Left), true, false, FLinearColor::Red, 0.0f);
		UKismetSystemLibrary::PrintString(GetWorld(), "Foot_R EffectorLocation(Z) : " + FString::SanitizeFloat(IKAnimValue.EffectorLocation_Right), true, false, FLinearColor::Red, 0.0f);
		UKismetSystemLibrary::PrintString(GetWorld(), "HipOffset(Z) : " + FString::SanitizeFloat(IKAnimValue.HipOffset), true, false, FLinearColor::Red, 0.0f);
	}

	if (OwnerCharacter)
	{
		if (OwnerCharacter->GetCapsuleComponent()->bHiddenInGame == bDebug)
		{
			OwnerCharacter->GetCapsuleComponent()->bHiddenInGame = !bDebug;
		}
	}
}

void UIFFootIkComponent::IKUpdateFootOffset(float DeltaTime, float TargetValue, float* EffectorValue, float InterpSpeed)
{
	float InterpValue = UKismetMathLibrary::FInterpTo(*EffectorValue, TargetValue, DeltaTime, InterpSpeed);
	*EffectorValue = InterpValue;
}

void UIFFootIkComponent::IKUpdateFootRotation(float DeltaTime, FRotator TargetValue, FRotator* EffectorValue, float InterpSpeed)
{
	FRotator InterpRotator = UKismetMathLibrary::RInterpTo(*EffectorValue, TargetValue, DeltaTime, InterpSpeed);
	*EffectorValue = InterpRotator;
}

void UIFFootIkComponent::IKUpdateCapsuleHalfHeight(float DeltaTime, float HipsShifts, bool bResetDefault)
{
	UCapsuleComponent* Capsule = OwnerCharacter->GetCapsuleComponent();
	if (Capsule == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("IK : Capsule is NULL"));
		return;
	}

	float fCapsuleHalf = 0.0f;
	if (bResetDefault == true)
	{
		fCapsuleHalf = IKCapsuleHalfHeight;
	}
	else
	{
		float HalfAbsSize = UKismetMathLibrary::Abs(HipsShifts) * 0.5f;
		fCapsuleHalf = IKCapsuleHalfHeight - HalfAbsSize;
	}

	float ScaledCapsuleHalfHeight = Capsule->GetScaledCapsuleHalfHeight();
	float interpValue = UKismetMathLibrary::FInterpTo(ScaledCapsuleHalfHeight, fCapsuleHalf, DeltaTime, IKHipsInterpSpeed);

	Capsule->SetCapsuleHalfHeight(interpValue, true);
}

