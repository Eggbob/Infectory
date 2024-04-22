#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/DataTable.h"
#include "Data/IFEnumDefine.h"
#include "IFItemData.generated.h"

DECLARE_DELEGATE_OneParam(FOnItemEmpty, int32);

USTRUCT(BlueprintType)
struct FIFItemData : public FTableRowBase
{
	GENERATED_BODY()
	
public:
	FIFItemData(const int32& ID, const FString& ItemName, const FString& ToolTip, const FString& IconName, const int32& ItemPrice,
		int32 Amount, int32 MaxAmount, int32 ItemEffectValue, const EItemType& ItemType , const ERangedWeaponType& WeaponType)
	{
		this->ID = ID;
		this->ItemName = ItemName;
		this->ToolTip = ToolTip;
		this->IconName = IconName;
		this->ItemPrice = ItemPrice;
		this->Amount = Amount;
		this->MaxAmount = MaxAmount;
		this->ItemType = ItemType;
		this->ItemEffectValue = ItemEffectValue;
		this->WeaponType = WeaponType;
	}

	FIFItemData()
	{
		ID = 1;
		ItemName = "None";
		ToolTip = "None";
		IconName = "None";
		ItemPrice = 0;
		Amount = 0;
		MaxAmount = 999;
		ItemEffectValue = 0;
		ItemType = EItemType::None;
		WeaponType = ERangedWeaponType::None;
	}

public:
	int32 GetID() const { return ID; }
	FString GetItemName() const { return ItemName; }
	FString GetToolTip() const { return ToolTip; }
	FString GetIconName() const { return IconName; }
	int32 GetItemPrice() const { return ItemPrice; }
	int32 GetAmount() const { return Amount; }
	int32 GetMaxAmount() const { return MaxAmount; }
	int32 GetItemEffectValue() const { return ItemEffectValue; }
	EItemType GetItemType() const { return ItemType; }
	ERangedWeaponType GetWeaponType() const { return WeaponType; }
	class UTexture2D* GetIconTexture()
	{
		if (IconTexture == nullptr)
		{
			/*FString IconPath;
			IconPath = FString::Printf(TEXT("/ Script / Engine.Texture2D'/Game/Assets/Art/ItemIcon/%s.%s'"), *IconName, *IconName);
	
			IconTexture = LoadObject<class UTexture2D>(NULL, *IconPath, NULL, LOAD_None, NULL);*/
		}

		return IconTexture;
	}

	void AddAmount(int32 amount)
	{
		int32 Encrease = Amount + amount;
		this->Amount = FMath::Clamp(Encrease, 0, MaxAmount);
	}

	bool SetAmount(int32 amount)
	{
		this->Amount = FMath::Clamp(amount, 0, MaxAmount);

		if (this->Amount == 0)
		{
			OnItemEmpty.ExecuteIfBound(ItemIndex);

			return false;
		}

		return true;
	}

	bool DcreaseAmount(int32 DecreaseAmount)
	{
		int32 DecreaseItem = Amount - DecreaseAmount;
		Amount = FMath::Clamp(DecreaseItem, 0, MaxAmount);

		if (this->Amount == 0)
		{
			//OnItemEmpty.ExecuteIfBound(ItemIndex);
			return false;
		}

		return true;
	}

	void SetItemIndex(int32 Idx)
	{
		ItemIndex = Idx;
	}

public:
	FOnItemEmpty OnItemEmpty;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data)
	int32 ID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data)
	FString ItemName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data)
	FString ToolTip;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data)
	FString IconName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data)
	int32 ItemPrice;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data)
	int32 Amount; //���� ���� ������ ����

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data)
	int32 MaxAmount; //�ִ� ���� ������ ����

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data)
	int32 ItemEffectValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data)
	EItemType ItemType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data)
	ERangedWeaponType WeaponType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data)
	class UTexture2D* IconTexture;


private:
	int32 ItemIndex;
};