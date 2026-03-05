// Fill out your copyright notice in the Description page of Project Settings.


#include "MineItem.h"
#include "Actor/Character/HDNormalMonster.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"


AMineItem::AMineItem()
{
	ExplosionDelay = 2.0f;
	ExplosionRadius = 300.0f;
	ExplosionDamage = 40.0f;
	ItemType = "Mine";
    bHasExploded = false;

    ExplosionCollision = CreateDefaultSubobject<USphereComponent>(TEXT("ExplosionCollision"));
    ExplosionCollision->InitSphereRadius(ExplosionRadius);
    ExplosionCollision->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
    ExplosionCollision->SetupAttachment(Scene);
    
    ExplosionCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    ExplosionCollision->SetCollisionResponseToAllChannels(ECR_Ignore);
    ExplosionCollision->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
}



void AMineItem::ActivateItem(AActor* Activator)
{
    if (bHasExploded) return;
    
    if (Activator->ActorHasTag("Monster"))
    {
        Super::ActivateItem(Activator);
        Explode();
    }
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
    
    UKismetSystemLibrary::SphereOverlapActors(
        GetWorld(),
        GetActorLocation(),
        ExplosionRadius, // 폭발 반경
        TArray<TEnumAsByte<EObjectTypeQuery>>(),
        AActor::StaticClass(),
        TArray<AActor*>(),
        OverlappingActors
    );

    for (AActor* Actor : OverlappingActors)
    {
        if (Actor && Actor->ActorHasTag("Monster"))
        {
            UGameplayStatics::ApplyDamage(
                Actor,             
                ExplosionDamage,    
                GetInstigatorController(), 
                this,              
                UDamageType::StaticClass()
            );
        }
    }
    SetActorEnableCollision(false);
    SetActorHiddenInGame(true);
    Destroy();

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


