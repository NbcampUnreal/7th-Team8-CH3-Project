
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/StaticMeshComponent.h"
#include "HDBowProjectile.generated.h"



UCLASS()
class CH03_SHOOTERGAME_API AHDBowProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	AHDBowProjectile();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement)
	USphereComponent* CollisionComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement)
	UProjectileMovementComponent* ProjectileMovementComponent;

	void FireInDirection(const FVector& ShootDirection);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Projectile)
	UStaticMeshComponent* ProjectileMeshComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement)
	UMaterialInstanceDynamic* ProjectileMaterialInstance;

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit);
};


 