
#include "Game/IFNPCSpawner.h"
#include "Character/IFCharacterNonPlayer.h"
#include "Components/BillboardComponent.h"

AIFNPCSpawner::AIFNPCSpawner()
{
	SpawnPoint = CreateDefaultSubobject<UBillboardComponent>(TEXT("SpawnPoint"));
	SpawnPoint->SetupAttachment(RootComponent);

	static ConstructorHelpers::FClassFinder<AIFCharacterNonPlayer> NpcClassRef(TEXT("/Game/Assets/Blueprint/BP_EnemyNPC.BP_EnemyNPC_C"));
	if(NpcClassRef.Class)
	{
		NPCClass = NpcClassRef.Class;
	}
}

void AIFNPCSpawner::SpawnNPC()
{
	SpawnedNPC = GetWorld()->SpawnActor<AIFCharacterNonPlayer>(NPCClass, GetActorLocation(), GetActorRotation());
	if (SpawnedNPC)
	{
		SpawnedNPC->SetNPCType(CurNPCType, CurNPCTier);
	}
}



