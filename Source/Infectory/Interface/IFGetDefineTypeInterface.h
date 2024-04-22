// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Data/IFEnumDefine.h"
#include "IFGetDefineTypeInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UIFGetDefineTypeInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class INFECTORY_API IIFGetDefineTypeInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual ECharacterMoveType GetPlayerMoveType() { return ECharacterMoveType::Walking; }
	virtual ECharacterControlType GetPlayerControlType() { return ECharacterControlType::Zoom; }
	virtual ECharacterState GetPlayerStateType() { return ECharacterState::Idle; }
	virtual ENPCState GetNPCState() { return ENPCState::Dead; }
	virtual ENPCMoveType GetNPCMoveType() { return ENPCMoveType::Walking; }
};
