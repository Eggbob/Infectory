

#include "UI/IFPlayerHPBar.h"
#include "Components/Image.h"

void UIFPlayerHPBar::SetHPBar(float CurHP)
{
	int32 PerHP = TotalHP / HPBarArray.Num();
	int32 HPAmount = TotalHP / 4;
	int32 ColorIndex = 0;

	for (int i = 0; i < 3; i++)
	{
		int32 MinHP = i * HPAmount;
		int32 MaxHP = (i + 1) * HPAmount;

		if (CurHP <= MaxHP && MinHP >= CurHP)
		{
			ColorIndex = i;
		}
	}

	int32 index = 0;

	for (int i = HPBarArray.Num() - 1; i >= 0; i--)
	{
		if (CurHP >= (index + 1) * PerHP)
		{
			/*FSlateColor TintColor = FSlateColor(HPColor[ColorIndex]);

			HPBarArray[i]->SetBrushTintColor(TintColor);*/
			SetHPColor(i, ColorIndex);
		}
		else
		{
			//FSlateColor TintColor = FSlateColor(EmptyHPColor);
			//HPBarArray[i]->SetBrushTintColor(TintColor);

			SetHPColor(i, 4);
		}

		index++;
	}
}

void UIFPlayerHPBar::InitHPBar(int32 InTotalHP)
{
	TotalHP = InTotalHP;
}

void UIFPlayerHPBar::NativeConstruct()
{
	Super::NativeConstruct();

	for (int i = 1; i < 9; i++)
	{
		HPBarArray.Add(Cast<UImage>(GetWidgetFromName(FName("HP" + FString::FromInt(i)))));
	}
}
