// Fill out your copyright notice in the Description page of Project Settings.


#include "IFGameMode.h"
#include "Player/IFPlayerController.h"

AIFGameMode::AIFGameMode()
{
	static ConstructorHelpers::FClassFinder<APawn> DefaultPawnClassRef(TEXT("/Script/Engine.Blueprint'/Game/Assets/Blueprint/BP_PlayerCharacter.BP_PlayerCharacter_C'"));
	if (DefaultPawnClassRef.Class)
	{
		DefaultPawnClass = DefaultPawnClassRef.Class;
	}

	/*static ConstructorHelpers::FClassFinder<APlayerController> PlayerControllerClassRef(TEXT("/Script/Infectory.IFPlayerController"));
	if (PlayerControllerClassRef.Class)
	{
		PlayerControllerClass = PlayerControllerClassRef.Class;
	}*/

	static ConstructorHelpers::FClassFinder<AIFPlayerController> PlayerControllerClassRef(TEXT("/Script/Engine.Blueprint'/Game/Assets/Blueprint/BP_IFPlayerController.BP_IFPlayerController_C'"));
	if (PlayerControllerClassRef.Class)
	{
		PlayerControllerClass = PlayerControllerClassRef.Class;
	}
}
