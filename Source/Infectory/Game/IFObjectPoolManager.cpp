#include "Game/IFObjectPoolManager.h"
#include "Game/IFObjPool.h"

// Sets default values
AIFObjectPoolManager::AIFObjectPoolManager()
{
	PrimaryActorTick.bCanEverTick = false;

}

AIFObjectPoolManager::~AIFObjectPoolManager()
{
	Clear();
}

void AIFObjectPoolManager::Push(TObjectPtr<AActor> PoolObj)
{
	if (PoolObj == nullptr)
		return;


	FString Name = PoolObj.Get()->GetName().LeftChop(4);

	if (!PoolMap.Contains(Name))
	{
		UE_LOG(LogTemp, Warning, TEXT("Actor Destory : %s"), *PoolObj->GetName());
		PoolObj.Get()->Destroy();
		return;
	}

	PoolMap[Name]->Push(PoolObj);
}

void AIFObjectPoolManager::CreatePool(TSubclassOf<AActor> _Original, UWorld* World, int count)
{
	if (PoolMap.Contains(_Original.GetDefaultObject()->GetName())) return;


	if (WorldObject == nullptr || !IsValid(WorldObject))
		WorldObject = World;

	FVector loc = this->GetActorLocation();
	TObjectPtr<AIFObjPool> NewPool = World->SpawnActor<AIFObjPool>(AIFObjPool::StaticClass());
	NewPool.Get()->SetActorLocation(loc);
	NewPool.Get()->Init(_Original, World, count);

	if (RootActor != nullptr)
	{
		NewPool.Get()->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);
	}
	PoolMap.Add(_Original.Get()->GetDefaultObject()->GetName(), NewPool);
	return;
}

TObjectPtr<AActor> AIFObjectPoolManager::Pop(TSubclassOf<AActor> original, UWorld* World, AActor* Parent)
{
	if (original == nullptr)
		return nullptr;

	if (PoolMap.IsEmpty())
	{
		CreatePool(original, World, 80);
		return PoolMap[original.Get()->GetDefaultObject()->GetName()]->Pop(Parent);
	}
	else if (!PoolMap.Contains(original.GetDefaultObject()->GetName()))
	{
		CreatePool(original, World, 80);
		return PoolMap[original.Get()->GetDefaultObject()->GetName()]->Pop(Parent);
	}

	return PoolMap[original.Get()->GetDefaultObject()->GetName()]->Pop(Parent);
}

void AIFObjectPoolManager::Clear()
{
	if (WorldObject == nullptr || !IsValid(WorldObject))
	{
		UE_LOG(LogTemp, Warning, TEXT("WorldObject is Null"));
		return;
	}

	for (auto& Pool : PoolMap)
	{
		Pool.Value->Clear();
		WorldObject.Get()->DestroyActor(Pool.Value);
	}

	PoolMap.Empty();
	PoolMap.Reset();
}



void AIFObjectPoolManager::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	PoolMap = TMap<FString, TObjectPtr<AIFObjPool>>();
	RootActor = nullptr;
	PoolMap.Empty();
	PoolMap.Reset();
}

void AIFObjectPoolManager::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Clear();
}

