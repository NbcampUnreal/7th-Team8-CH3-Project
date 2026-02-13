#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "HDPlayerCharacter.generated.h" // 파일명과 일치 확인

UCLASS()
class CH03_SHOOTERGAME_API AHDPlayerCharacter : public ACharacter
{
    GENERATED_BODY()

public:
    // 이 캐릭터의 프로퍼티에 적용되는 디폴트값 설정
    AHDPlayerCharacter();

protected:
    // 게임 시작 또는 스폰 시 호출
    virtual void BeginPlay() override;

    // 스폰할 발사체 클래스 (중요: AHDBowProjectile로 변경)
    UPROPERTY(EditDefaultsOnly, Category = Projectile)
    TSubclassOf<class AHDBowProjectile> ProjectileClass;

public:
    // 프레임마다 호출
    virtual void Tick(float DeltaTime) override;

    // 함수 기능을 입력에 바인딩하기 위해 호출
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    // 이동 입력 처리
    UFUNCTION()
    void MoveForward(float Value);

    UFUNCTION()
    void MoveRight(float Value);

    // 점프 처리
    UFUNCTION()
    void StartJump();

    UFUNCTION()
    void StopJump();

    // 발사체를 발사하는 함수
    UFUNCTION()
    void Fire();

    // FPS 카메라 및 메시
    UPROPERTY(VisibleAnywhere)
    UCameraComponent* FPSCameraComponent;

    UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
    USkeletalMeshComponent* FPSMesh;

    // 총구 오프셋
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
    FVector MuzzleOffset;
};