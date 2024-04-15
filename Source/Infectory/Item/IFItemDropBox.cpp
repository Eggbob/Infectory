

#include "Item/IFItemDropBox.h"
#include "Components/WidgetComponent.h"
#include "UI/IFItemInfo.h"
#include "Data/IFGameSingleton.h"
#include "UI/IFItemBox.h"


AIFItemDropBox::AIFItemDropBox()
{
	PrimaryActorTick.bCanEverTick = true;

}

void AIFItemDropBox::OpenItemBox()
{
	ItemInfo->ShowItemData(ItemData);

	//ItemBox.Get()->InitItemBox(false);
	//ItemBox.Get()->BindItemData(ItemData);
}

void AIFItemDropBox::BeginPlay()
{
	Super::BeginPlay();
	
	UWidgetComponent* WidgetComponent = this->GetComponentByClass<UWidgetComponent>();
	
	ItemInfo = Cast<UIFItemInfo>(WidgetComponent->GetUserWidgetObject());
}

void AIFItemDropBox::GetItemData()
{
	ItemData = UIFGameSingleton::Get().GetItemData(ItemNum);
}



