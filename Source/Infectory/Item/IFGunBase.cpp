// Fill out your copyright notice in the Description page of Project Settings.

#include "Item/IFGunBase.h"

AIFGunBase::AIFGunBase()
{
	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));

	SetRootComponent(RootComp);
	Mesh->SetupAttachment(RootComp);
}

FVector AIFGunBase::GetWeaponSocket()
{
	return Mesh->GetSocketTransform("LeftHandSocket").GetLocation();
}




