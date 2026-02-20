#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h" // .generated.h 파일명은 반드시 클래스 파일명과 일치해야 합니다.
#include "HDBowProjectile.h"
#include "HDPlayerCharacter.generated.h"

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

    // 스폰할 화살(발사체) 클래스입니다.
    UPROPERTY(EditDefaultsOnly, Category = Projectile)
    TSubclassOf<class AHDBowProjectile> ProjectileClass;

public:
    // 프레임마다 호출
    virtual void Tick(float DeltaTime) override;

    // 함수 기능을 입력에 바인딩하기 위해 호출
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    // 앞으로 이동 및 뒤로 이동 입력을 처리합니다.
    UFUNCTION()
    void MoveForward(float Value);

    // 오른쪽 이동 및 왼쪽 이동 입력을 처리합니다.
    UFUNCTION()
    void MoveRight(float Value);

    // 키가 눌릴 경우 점프 플래그를 설정합니다.
    UFUNCTION()
    void StartJump();

    // 키가 떼어질 경우 점프 플래그를 지웁니다.
    UFUNCTION()
    void StopJump();

    // 화살을 발사하는 함수입니다.
    UFUNCTION()
    void Fire();

    // FPS 카메라
    UPROPERTY(VisibleAnywhere)
    UCameraComponent* FPSCameraComponent;

    // 일인칭 메시(팔)로, 소유 플레이어에게만 보입니다.
    UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
    USkeletalMeshComponent* FPSMesh;

    // 카메라 위치로부터의 총구 오프셋입니다.
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
    FVector MuzzleOffset;
};