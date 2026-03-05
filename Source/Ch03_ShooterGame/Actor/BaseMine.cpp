// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseMine.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"

ABaseMine::ABaseMine()
{
	PrimaryActorTick.bCanEverTick = false;

	Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	SetRootComponent(Scene);

	Collision = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
	Collision->SetupAttachment(Scene);
	Collision->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->SetupAttachment(Collision);
	
	Collision->OnComponentBeginOverlap.AddDynamic(this, &ABaseMine::OnItemOverlap);
	Collision->OnComponentEndOverlap.AddDynamic(this, &ABaseMine::OnItemEndOverlap);

}

void ABaseMine::OnItemOverlap(
	UPrimitiveComponent* OverlappedComp,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor->ActorHasTag("Player") || OtherActor->ActorHasTag("Monster"))
	{
		ActivateItem(OtherActor);
	}
}

void ABaseMine::OnItemEndOverlap(
	UPrimitiveComponent* OverlappedComp,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex)
{
}
void ABaseMine::ActivateItem(AActor* Activator)
{

	UParticleSystemComponent* Particle = nullptr;

	if (PickupParticle)
	{
		UGameplayStatics::SpawnEmitterAtLocation(
			GetWorld(),
			PickupParticle,
			GetActorLocation(),
			GetActorRotation(),
			true
		);
	}
	if (PickupSound)
	{
		UGameplayStatics::PlaySoundAtLocation(
			GetWorld(),
			PickupSound,
			GetActorLocation()
		);
	}

	if (Particle)
	{
		FTimerHandle DestroyParticleTimerHandle;

		GetWorld()->GetTimerManager().SetTimer(
			DestroyParticleTimerHandle,
			[Particle]()
			{
				Particle->DestroyComponent();

			},
			2.0f,
			false
		);
	}
}

FName ABaseMine::GetItemType() const
{
	return ItemType;
}

void ABaseMine::DestroyItem()
{
	Destroy();
}