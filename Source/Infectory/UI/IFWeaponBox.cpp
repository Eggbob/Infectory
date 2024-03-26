// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/IFWeaponBox.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"



UIFWeaponBox::UIFWeaponBox(const FObjectInitializer& Object) : Super(Object)
{
	static ConstructorHelpers::FObjectFinder<UTexture2D> RifleImgRef(TEXT("/Script/Engine.Texture2D'/Game/Assets/Art/ItemIcon/Rifle.Rifle'"));
	if (RifleImgRef.Object)
	{
		WeaponTextureMap.Add(ERangedWeaponType::Rifle, RifleImgRef.Object) ;
	}

	static ConstructorHelpers::FObjectFinder<UTexture2D> ShotGunImgRef(TEXT("/Script/Engine.Texture2D'/Game/Assets/Art/ItemIcon/ShotGun.ShotGun'"));
	if (ShotGunImgRef.Object)
	{
		WeaponTextureMap.Add(ERangedWeaponType::ShotGun, ShotGunImgRef.Object);
	}

	static ConstructorHelpers::FObjectFinder<UTexture2D> GrenadeImgRef(TEXT("/Script/Engine.Texture2D'/Game/Assets/Art/ItemIcon/GrenadeLauncher.GrenadeLauncher'"));
	if (GrenadeImgRef.Object)
	{
		WeaponTextureMap.Add(ERangedWeaponType::Projectile, GrenadeImgRef.Object);
	}
}

void UIFWeaponBox::NativeConstruct()
{
	Super::NativeConstruct();

	WeaponImage = Cast<UImage>(GetWidgetFromName(TEXT("WeaponImage")));
	AmmoText = Cast<UTextBlock>(GetWidgetFromName(TEXT("AmmoText")));
}

void UIFWeaponBox::SetWeapon(ERangedWeaponType WeaponType, int32 CurAmmo, int32 TotalAmmo)
{
	WeaponImage->SetBrushFromTexture(WeaponTextureMap[WeaponType]);
	AmmoText->SetText(FText::FromString(FString::Printf(TEXT("%d/%d"), CurAmmo, TotalAmmo)));
}






