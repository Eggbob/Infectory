#include "Item/IFShield.h"
#include "Components/CapsuleComponent.h"

AIFShield::AIFShield()
{
	PrimaryActorTick.bCanEverTick = false;

	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));
	MeshComp = CreateAbstractDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));

	SetRootComponent(RootComp);
	CapsuleComp->SetupAttachment(RootComp);
	MeshComp->SetupAttachment(CapsuleComp);


	static ConstructorHelpers::FObjectFinder<UStaticMesh> ShieldMeshRef(TEXT("/Script/Engine.StaticMesh'/Game/Assets/Characters/ETC/SM_Shield.SM_Shield'"));
	if (ShieldMeshRef.Object)
	{
		MeshComp->SetStaticMesh(ShieldMeshRef.Object);
	}
}

void AIFShield::SetShieldDestoryedDelegate(const FOnShieldDestoryed& InOnShieldDestoryed)
{
	OnShieldDestoryed.Unbind();
	OnShieldDestoryed = InOnShieldDestoryed;
}

float AIFShield::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);

	Damage = FMath::Clamp(Damage, 0.f, Damage);

	ShieldHP -= Damage;

	if (ShieldHP <= 0)
	{
		OnShieldDestoryed.ExecuteIfBound();
		SetActorHiddenInGame(true);
		SetActorEnableCollision(false);
		CapsuleComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		//Destroy();
	}

	return Damage;
}


