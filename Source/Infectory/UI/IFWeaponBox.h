// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Data/IFEnumDefine.h"
#include "Blueprint/UserWidget.h"
#include "Data/IFGunStat.h"
#include "IFWeaponBox.generated.h"

/**
 * 
 */
UCLASS()
class INFECTORY_API UIFWeaponBox : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UIFWeaponBox(const FObjectInitializer& ObjectInitializer);
	void SetWeapon(ERangedWeaponType WeaponType, int32 CurAmmo, int32 TotalAmmo);
	
protected:
	virtual void NativeConstruct() override;
	
private:
	UPROPERTY()
	TObjectPtr<class UImage> WeaponImage;
	
	UPROPERTY()
	TObjectPtr<class UTextBlock> AmmoText;

	UPROPERTY()
	TMap<ERangedWeaponType, UTexture2D*> WeaponTextureMap;
};
