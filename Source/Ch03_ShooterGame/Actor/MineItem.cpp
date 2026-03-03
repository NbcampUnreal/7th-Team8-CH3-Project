// Fill out your copyright notice in the Description page of Project Settings.


#include "MineItem.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"


AMineItem::AMineItem()
{
	ExplosionDelay = 2.0f;
	ExplosionRadius = 300.0f;
	ExplosionDamage = 30.0f;
	ItemType = "Mine";
    bHasExploded = false;

    ExplosionCollision = CreateDefaultSubobject<USphereComponent>(TEXT("ExplosionCollision"));
    ExplosionCollision->InitSphereRadius(ExplosionRadius);
    ExplosionCollision->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
    ExplosionCollision->SetupAttachment(Scene);
}



void AMineItem::ActivateItem(AActor* Activator)
{

    if (bHasExploded) return;


    Super::ActivateItem(Activator);

   
    GetWorld()->GetTimerManager().SetTimer(ExplosionTimerHandle, this, &AMineItem::Explode, ExplosionDelay);
}

void AMineItem::Explode()
{
    UParticleSystemComponent* Particle = nullptr;

    if (ExplosionParticle)
    {
         Particle = UGameplayStatics::SpawnEmitterAtLocation(
            GetWorld(),
            ExplosionParticle,
            GetActorLocation(),
            GetActorRotation(),
            false
        );
         bHasExploded = true;
    }

    if (ExplosionSound)
    {
        UGameplayStatics::PlaySoundAtLocation(
            GetWorld(),
            ExplosionSound,
            GetActorLocation()
            );
    }

    TArray<AActor*> OverlappingActors;
    ExplosionCollision->GetOverlappingActors(OverlappingActors);

    for (AActor* Actor : OverlappingActors)
    {
        if (Actor && Actor->ActorHasTag("Player"))
        {

            UGameplayStatics::ApplyDamage(
                Actor,             
                ExplosionDamage,    
                GetInstigatorController(), 
                this,              
                UDamageType::StaticClass() 

            


            );
            GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, FString::Printf(TEXT("Player damaged %d by MineItem"), ExplosionDamage));
        }
    }
    DestroyItem();

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


