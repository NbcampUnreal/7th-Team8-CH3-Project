// Fill out your copyright notice in the Description page of Project Settings.


#include "HDMonCharacter.h"
#include "HDMonController.h"
#include "UObject/ConstructorHelpers.h"
#include "Camera/CameraComponent.h"
#include "Components/DecalComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Materials/Material.h"
#include "Engine/World.h"

// Sets default values
AHDMonCharacter::AHDMonCharacter()
{
    PrimaryActorTick.bCanEverTick = false;
    PrimaryActorTick.bStartWithTickEnabled = false;

    AIControllerClass = AHDMonController::StaticClass();
    AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

    GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

    bUseControllerRotationPitch = false;
    bUseControllerRotationYaw = false;
    bUseControllerRotationRoll = false;

    GetCharacterMovement()->bOrientRotationToMovement = true;
    GetCharacterMovement()->RotationRate = FRotator(0.f, 640.f, 0.f);
    GetCharacterMovement()->GetNavMovementProperties()->bUseAccelerationForPaths = true;

    MonMoveSpeed = 300.0f;
    MonMaxHP = 300.f;
    MonHP = MonMaxHP;
    MonAtk = 20.f;
    GetCharacterMovement()->MaxWalkSpeed = MonMoveSpeed;


}

float AHDMonCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
    float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

    if (ActualDamage <= 0.0f)
    {
        return 0.0f;
    }
    UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
    if (AnimInstance && TakeDamageMontage)
    {
        AnimInstance->Montage_Play(TakeDamageMontage);
    }
   
    MonHP = FMath::Clamp(MonHP - ActualDamage, 0.0f, MonMaxHP);

    if (MonHP <= 0.0f)
    {
        OnDeath();
    }

    return ActualDamage;
}

void AHDMonCharacter::OnDeath()
{

    UE_LOG(LogTemp, Warning, TEXT("Monster Died!"));

    UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
    if (AnimInstance && DeathMontage)
    {
        AnimInstance->Montage_Play(DeathMontage);
    }
    
    GetCharacterMovement()->StopMovementImmediately();
    GetCharacterMovement()->DisableMovement();
    
    GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    
    DetachFromControllerPendingDestroy();

    SetLifeSpan(5.0f);
}

void AHDMonCharacter::AttackHitCheck()
{
    FHitResult HitResult;
    FCollisionQueryParams Params(NAME_None, false, this);

    FVector TraceStart = GetActorLocation();
    FVector TraceEnd = GetActorLocation() + GetActorForwardVector() * 100.0f;
    float AttackRadius = 50.0f;

    // 1. 눈에 보이는 빨간 공 그리기 (2초 동안 유지됨)
    DrawDebugSphere(GetWorld(), TraceEnd, AttackRadius, 12, FColor::Red, false, 2.0f);


    bool bResult = GetWorld()->SweepSingleByChannel(
        HitResult,
        GetActorLocation(),
        GetActorLocation() + GetActorForwardVector() * 100.0f, // 사거리 100
        FQuat::Identity,
        ECollisionChannel::ECC_Pawn,
        FCollisionShape::MakeSphere(50.0f),
        Params
    );

    if (bResult)
    {
        if (AActor* Target = HitResult.GetActor())
        {
            // 로그 확인
            UE_LOG(LogTemp, Warning, TEXT("Hit Target: %s"), *Target->GetName());

            // 데미지 적용 (ApplyDamage)
            UGameplayStatics::ApplyDamage(
                Target,
                MonAtk,      // 헤더에 선언한 공격력 변수
                GetController(),   // 가해자(몬스터)의 컨트롤러
                this,              // 가해자(몬스터) 자신
                UDamageType::StaticClass()
            );
        }
    }


}






