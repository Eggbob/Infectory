// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "IFGameMode.generated.h"

/**
 * 
 */
UCLASS()
class INFECTORY_API AIFGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	AIFGameMode();

	virtual void Logout(AController* Exiting) override;
	TObjectPtr<class AIFObjectPoolManager> GetPoolManager();

	UFUNCTION(BlueprintImplementableEvent)
	void PlayCameraShake(TSubclassOf<class ULegacyCameraShake> CameraShake);

private:
	UPROPERTY()
	TObjectPtr<class AIFObjectPoolManager> PoolManager;
};
