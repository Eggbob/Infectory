

#include "Game/IFObjPool.h"

AIFObjPool::AIFObjPool()
{
	PrimaryActorTick.bCanEverTick = false;

	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("ObjectPool"));

	SetRootComponent(RootComp);

}


void AIFObjPool::BeginPlay()
{
	Super::BeginPlay();
	
	CreateCount = 0;

	EnableMap = TMap<FString, TObjectPtr<AActor>>();
	EnableMap.Empty();
}

void AIFObjPool::Clear()
{
	for (auto& obj : PoolArray)
	{
		if (obj != nullptr && IsValid(obj))
		{
			WorldObject.Get()->DestroyActor(obj);
		}
	}

	DisableQueue.Empty();
	EnableMap.Reset();
	EnableMap.Empty();
}

void AIFObjPool::Init(TSubclassOf<AActor> Original, UWorld* _World, int size)
{
	OriginalObject = Original;
	MaxSize = size;

	if (_World != nullptr)
	{
		WorldObject = _World;
	}
	else
	{
		WorldObject = GetWorld();
	}

	for (int i = 0; i < size; i++)
	{
		Push(CreateObjPool());
	}
}

TObjectPtr<AActor> AIFObjPool::Pop(AActor* Parent)
{
	TObjectPtr<AActor> Poolable = nullptr;

	if (IsValid(*DisableQueue.Peek()) && !DisableQueue.IsEmpty())
	{
		DisableQueue.Dequeue(Poolable);
	}
	else
	{
		Poolable = CreateObjPool();
	}

	if (Poolable != nullptr && Poolable.Get()->IsValidLowLevel())
	{
		if (Parent != nullptr)
		{
			Poolable.Get()->AttachToActor(Parent, FAttachmentTransformRules::KeepRelativeTransform);
		}
		else
		{
			Poolable.Get()->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		}

		Poolable.Get()->SetActorHiddenInGame(false);
		Poolable.Get()->SetActorEnableCollision(true);
		Poolable.Get()->SetActorTickEnabled(true);

		EnableMap.Add(Poolable.Get()->GetName(), Poolable);
	}
	else
	{
		Poolable = WorldObject.Get()->SpawnActor<AActor>(OriginalObject, GetActorLocation(), FRotator::ZeroRotator);
	}

	return Poolable.Get();
}

void AIFObjPool::Push(TObjectPtr<AActor> Object)
{
	if (Object != nullptr && IsValid(Object))
	{
		Object.Get()->SetActorHiddenInGame(true);
		Object.Get()->SetActorEnableCollision(false);
		Object.Get()->SetActorTickEnabled(false);
		Object.Get()->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);

		if (EnableMap.Contains(Object.Get()->GetName()))
		{
			EnableMap.Remove(Object.Get()->GetName());
		}

		DisableQueue.Enqueue(Object.Get());
	}
}

TObjectPtr<AActor> AIFObjPool::CreateObjPool()
{
	TObjectPtr<AActor> Obj = WorldObject.Get()->SpawnActor<AActor>(OriginalObject, GetActorLocation(), FRotator::ZeroRotator);

	if (Obj != nullptr && IsValid(Obj))
	{
		FString formattedNum = FString::Printf(TEXT("%03d"), CreateCount);
		CreateCount++;
		FString nameWithNumber = FString::Printf(TEXT("%s_%s"), *OriginalObject.GetDefaultObject()->GetName(), *formattedNum);

		Obj.Get()->Rename(*nameWithNumber, nullptr, REN_None);
		PoolArray.Add(Obj);

		return Obj;
	}

	return nullptr;
}

