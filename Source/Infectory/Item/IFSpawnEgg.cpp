#include "Item/IFSpawnEgg.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Character/IFCharacterNonPlayer.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "IFSpawnEgg.h"

AIFSpawnEgg::AIFSpawnEgg()
{
	PrimaryActorTick.bCanEverTick = false;

	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComp"));
	RootComponent = BoxComp;

	EggMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("EggMesh"));
	EggMesh->SetupAttachment(BoxComp);

	static ConstructorHelpers::FClassFinder<AIFCharacterNonPlayer> NpcClassRef(TEXT("/Game/Assets/Blueprint/BP_EnemyNPC.BP_EnemyNPC_C"));
	if (NpcClassRef.Class)
	{
		NPCClass = NpcClassRef.Class;
	}

	static ConstructorHelpers::FClassFinder<AIFCharacterNonPlayer> BoomerClassRef(TEXT("/Game/Assets/Blueprint/BP_MiniBoomer.BP_MiniBoomer_C"));
	if (BoomerClassRef.Class)
	{
		BoomerClass = BoomerClassRef.Class;
	}
}

void AIFSpawnEgg::LaunchEgg(AActor * InTarget, ENPCType InNpcType, FName InNPCTier)
{
	FVector LaunchDirec = InTarget->GetActorLocation() - GetActorLocation();

	float ProjectileSpeed = 1000.f;
	float Gravity = 980.f;
	float TimeToTarget = LaunchDirec.Size() / ProjectileSpeed;
	FVector LaunchVelocity = LaunchDirec / TimeToTarget;
	LaunchVelocity.Z += 0.5f * Gravity * TimeToTarget;

	ProjectileMovementComp->Velocity = LaunchVelocity;
	ProjectileMovementComp->InitialSpeed = LaunchVelocity.Size();
	ProjectileMovementComp->MaxSpeed = LaunchVelocity.Size();

	NpcType = InNpcType;
	NPCTier = InNPCTier;
	TargetActor = InTarget;

	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &AIFSpawnEgg::SpawnNPC, 2.f, false);
}


void AIFSpawnEgg::BeginPlay()
{
	Super::BeginPlay();
	
	ProjectileMovementComp = GetComponentByClass<UProjectileMovementComponent>();
}

void AIFSpawnEgg::SpawnNPC()
{
	FVector SpawnVector = GetActorLocation();
	SpawnVector.Z += 100.f;

	TObjectPtr<AIFCharacterNonPlayer> NPC;
	if (NpcType == ENPCType::MiniBoomer)
	{
		NPC = GetWorld()->SpawnActor<AIFCharacterNonPlayer>(BoomerClass, SpawnVector, GetActorRotation());
	}
	else
	{
		NPC = GetWorld()->SpawnActor<AIFCharacterNonPlayer>(NPCClass, SpawnVector, GetActorRotation());
	}

	NPC->SetNPCType(NpcType, NPCTier);
	NPC->FocusingTarget(TargetActor);

	if (SpawnEffect)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), SpawnEffect, GetActorLocation());
		UGameplayStatics::SpawnSoundAtLocation(GetWorld(), SpawnSound, GetActorLocation());
	}

	OnFinish.ExecuteIfBound(this);
}



