// Fill out your copyright notice in the Description page of Project Settings.


#include "HDBowProjectile.h"

// Sets default values
AHDBowProjectile::AHDBowProjectile()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	if (!RootComponent)
	{
		RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("ProjectileSceneComponent"));
	}

	if (!CollisionComponent)
	{
		// 스피어를 단순 콜리전 표현으로 사용합니다.
		CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
		// 스피어의 콜리전 반경을 설정합니다.
		CollisionComponent->InitSphereRadius(15.0f);
		// 루트 컴포넌트가 콜리전 컴포넌트가 되도록 설정합니다.
		RootComponent = CollisionComponent;
	}
	if (!ProjectileMovementComponent)
	
	{
		// 이 컴포넌트를 사용하여 이 발사체의 이동을 주도합니다.
		ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
		ProjectileMovementComponent->SetUpdatedComponent(CollisionComponent);
		ProjectileMovementComponent->InitialSpeed = 3000.0f;
		ProjectileMovementComponent->MaxSpeed = 3000.0f;
		ProjectileMovementComponent->bRotationFollowsVelocity = true;
		ProjectileMovementComponent->bShouldBounce = true;
		ProjectileMovementComponent->Bounciness = 0.3f;
		ProjectileMovementComponent->ProjectileGravityScale = 0.0f;
	}
}

// Called when the game starts or when spawned
void AHDBowProjectile::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AHDBowProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


// 발사 방향으로의 발사체 속도를 초기화하는 함수입니다.
void AHDBowProjectile::FireInDirection(const FVector& ShootDirection)
{
	ProjectileMovementComponent->Velocity = ShootDirection * ProjectileMovementComponent->InitialSpeed;
}