// Copyright Epic Games, Inc. All Rights Reserved.

#include "HDPlayerCharacter.h"
#include "HDBowProjectile.h" // 발사체 클래스 헤더 포함
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"

// 디폴트값 설정
AHDPlayerCharacter::AHDPlayerCharacter()
{
    // 이 캐릭터가 프레임마다 Tick()을 호출하도록 설정합니다.
    PrimaryActorTick.bCanEverTick = true;

    // 일인칭 카메라 컴포넌트를 생성합니다.
    FPSCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
    check(FPSCameraComponent != nullptr);

    // 캡슐 컴포넌트에 카메라 컴포넌트를 어태치합니다.
    FPSCameraComponent->SetupAttachment(CastChecked<USceneComponent, UCapsuleComponent>(GetCapsuleComponent()));

    // 카메라가 눈 약간 위에 위치하도록 합니다.
    FPSCameraComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 50.0f + BaseEyeHeight));

    // 폰이 카메라 회전을 제어하도록 합니다.
    FPSCameraComponent->bUsePawnControlRotation = true;

    // 소유 플레이어의 일인칭 메시 컴포넌트를 생성합니다.
    FPSMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FirstPersonMesh"));
    check(FPSMesh != nullptr);

    // 소유 플레이어만 이 메시를 볼 수 있습니다.
    FPSMesh->SetOnlyOwnerSee(true);

    // FPS 메시를 FPS 카메라에 어태치합니다.
    FPSMesh->SetupAttachment(FPSCameraComponent);

    // 일부 인바이런먼트 섀도잉을 비활성화하여 단일 메시 같은 느낌을 보존합니다.
    FPSMesh->bCastDynamicShadow = false;
    FPSMesh->CastShadow = false;

    // 소유 플레이어가 일반(삼인칭) 바디 메시를 볼 수 없도록 설정합니다.
    GetMesh()->SetOwnerNoSee(true);

    // 기본 총구 오프셋 설정
    MuzzleOffset = FVector(100.0f, 0.0f, 0.0f);
}

// 게임 시작 또는 스폰 시 호출
void AHDPlayerCharacter::BeginPlay()
{
    Super::BeginPlay();

    if (GEngine)
    {
        // 디버그 메시지를 표시합니다.
        GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("We are using HDPlayerCharacter."));
    }
}

// 프레임마다 호출
void AHDPlayerCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

// 함수 기능을 입력에 바인딩하기 위해 호출
void AHDPlayerCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    // 'movement' 바인딩
    PlayerInputComponent->BindAxis("MoveForward", this, &AHDPlayerCharacter::MoveForward);
    PlayerInputComponent->BindAxis("MoveRight", this, &AHDPlayerCharacter::MoveRight);

    // 'look' 바인딩
    PlayerInputComponent->BindAxis("Turn", this, &AHDPlayerCharacter::AddControllerYawInput);
    PlayerInputComponent->BindAxis("LookUp", this, &AHDPlayerCharacter::AddControllerPitchInput);

    // 'action' 바인딩
    PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AHDPlayerCharacter::StartJump);
    PlayerInputComponent->BindAction("Jump", IE_Released, this, &AHDPlayerCharacter::StopJump);
    PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AHDPlayerCharacter::Fire);
}

void AHDPlayerCharacter::MoveForward(float Value)
{
    // 컨트롤러 회전값을 기준으로 앞방향 벡터를 구해 이동 입력으로 전달합니다.
    FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::X);
    AddMovementInput(Direction, Value);
}

void AHDPlayerCharacter::MoveRight(float Value)
{
    // 컨트롤러 회전값을 기준으로 오른쪽 방향 벡터를 구해 이동 입력으로 전달합니다.
    FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::Y);
    AddMovementInput(Direction, Value);
}

void AHDPlayerCharacter::StartJump()
{
    bPressedJump = true;
}

void AHDPlayerCharacter::StopJump()
{
    bPressedJump = false;
}

void AHDPlayerCharacter::Fire()
{
    // 발사체 클래스가 설정되어 있는지 확인합니다.
    if (ProjectileClass)
    {
        // 스폰 위치 및 회전값 계산
        FVector MuzzleLocation = GetActorLocation() + (GetActorRotation().RotateVector(MuzzleOffset));
        FRotator MuzzleRotation = GetControlRotation();

        UWorld* World = GetWorld();
        if (World)
        {
            FActorSpawnParameters SpawnParams;
            SpawnParams.Owner = this;
            SpawnParams.Instigator = GetInstigator();

            // 월드에 발사체(AHDBowProjectile)를 스폰합니다.
            AHDBowProjectile* Projectile = World->SpawnActor<AHDBowProjectile>(ProjectileClass, MuzzleLocation, MuzzleRotation, SpawnParams);

            if (Projectile)
            {
                // 발사 방향 벡터를 구하여 발사체의 속도를 초기화합니다.
                FVector LaunchDirection = MuzzleRotation.Vector();
                Projectile->FireInDirection(LaunchDirection);
            }
        }
    }
}