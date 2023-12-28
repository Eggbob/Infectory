// Fill out your copyright notice in the Description page of Project Settings.


#include "IFCharacterNonPlayer.h"

AIFCharacterNonPlayer::AIFCharacterNonPlayer()
{
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> CharacterMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/Assets/Characters/Test/Parasite_L_Starkie.Parasite_L_Starkie'"));
	if (CharacterMeshRef.Object)
	{
		GetMesh()->SetSkeletalMesh(CharacterMeshRef.Object);
	}
}

void AIFCharacterNonPlayer::PostInitializeComponents()
{
	Super::PostInitializeComponents();


}

void AIFCharacterNonPlayer::SetDead()
{
	Super::SetDead();
}
