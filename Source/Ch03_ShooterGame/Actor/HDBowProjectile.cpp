// Copyright Epic Games, Inc. All Rights Reserved.

#include "HDBowProjectile.h"
#include "Character/HDPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Controller.h"
#include "UObject/ConstructorHelpers.h"
#include "Materials/Material.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Engine/Engine.h"
#include "GameFramework/DamageType.h"

// 디폴트값 설정
AHDBowProjectile::AHDBowProjectile()
{
	PrimaryActorTick.bCanEverTick = true;

	if (!RootComponent)
	{
		RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("ProjectileSceneComponent"));
	}

	//if (!CollisionComponent)
	//{
	//	// 스피어를 단순 콜리전 표현으로 사용합니다.
	//	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));

	//	// 스피어의 콜리전 프로파일 이름을 'Projectile'로 설정합니다.
	//	CollisionComponent->BodyInstance.SetCollisionProfileName(TEXT("Projectile"));

	//	// 컴포넌트가 어딘가에 부딪힐 때 호출되는 이벤트입니다.
	//	CollisionComponent->OnComponentHit.AddDynamic(this, &AHDBowProjectile::OnHit);

	//	// 스피어의 콜리전 반경을 설정합니다.
	//	CollisionComponent->InitSphereRadius(15.0f);

	//	// 루트 컴포넌트가 콜리전 컴포넌트가 되도록 설정합니다.
	//	RootComponent = CollisionComponent;
	//}
	if (!CollisionComponent)
	{
		CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));

		CollisionComponent->BodyInstance.SetCollisionProfileName(TEXT("Projectile"));

		CollisionComponent->SetNotifyRigidBodyCollision(true);

		CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		CollisionComponent->SetCollisionResponseToAllChannels(ECR_Block);

		CollisionComponent->OnComponentHit.AddDynamic(this, &AHDBowProjectile::OnHit);

		CollisionComponent->InitSphereRadius(15.0f);

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


	if (!ProjectileMeshComponent)
	{
		ProjectileMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMeshComponent"));
		static ConstructorHelpers::FObjectFinder<UStaticMesh>Mesh(TEXT("'/Game/Projectile/Sphere.Sphere'"));
		if (Mesh.Succeeded())
		{
			ProjectileMeshComponent->SetStaticMesh(Mesh.Object);
		}

		static ConstructorHelpers::FObjectFinder<UMaterial>Material(TEXT("'/Game/Projectile/SphereMaterial.SphereMaterial'"));
		if (Material.Succeeded())
		{
			ProjectileMaterialInstance = UMaterialInstanceDynamic::Create(Material.Object, ProjectileMeshComponent);
		}

		ProjectileMeshComponent->SetMaterial(0, ProjectileMaterialInstance);
		ProjectileMeshComponent->SetRelativeScale3D(FVector(0.09f, 0.09f, 0.09f));
		ProjectileMeshComponent->SetupAttachment(RootComponent);
	}

	InitialLifeSpan = 3.0f;

}
void AHDBowProjectile::BeginPlay()
{
	Super::BeginPlay();

}

void AHDBowProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AHDBowProjectile::FireInDirection(const FVector& ShootDirection)
{
	ProjectileMovementComponent->Velocity = ShootDirection * ProjectileMovementComponent->InitialSpeed;
}


void AHDBowProjectile::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor && GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Blue, FString::Printf(TEXT("Arrow Hit: %s"), *OtherActor->GetName()));
	}

	if (OtherActor != nullptr && OtherActor != this)
	{
		float TestDamage = 20.0f;
		UGameplayStatics::ApplyDamage(
			OtherActor,
			TestDamage,
			GetInstigatorController(),
			this,
			UDamageType::StaticClass()
		);
	}
	Destroy();

}
