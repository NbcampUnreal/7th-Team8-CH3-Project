// Copyright Epic Games, Inc. All Rights Reserved.

#include "HDPlayerCharacter.h"

// 디폴트값 설정
AHDPlayerCharacter::AHDPlayerCharacter()
{
    // 이 캐릭터가 프레임마다 Tick()을 호출하도록 설정합니다. 이 설정이 필요 없는 경우 비활성화하면 퍼포먼스가 향상됩니다.
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

    // 소유 플레이어가 일반(삼인칭) 바디 메시를 볼 수 없습니다.
    GetMesh()->SetOwnerNoSee(true);
}

// 게임 시작 또는 스폰 시 호출
void AHDPlayerCharacter::BeginPlay()
{
    Super::BeginPlay();

    check(GEngine != nullptr);

    // 디버그 메시지를 5초간 표시합니다. 
    // -1 '키' 값 실행인자가 메시지가 업데이트되거나 새로고침되지 않도록 방지합니다.
    GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("We are using HDPlayerCharacter."));
}

// 프레임마다 호출
void AHDPlayerCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

}

//함수 기능을 입력에 바인딩하기 위해 호출
void AHDPlayerCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    // 'movement' 바인딩을 구성합니다.
    PlayerInputComponent->BindAxis("MoveForward", this, &AHDPlayerCharacter::MoveForward);
    PlayerInputComponent->BindAxis("MoveRight", this, &AHDPlayerCharacter::MoveRight);

    // 'look' 바인딩을 구성합니다.
    PlayerInputComponent->BindAxis("Turn", this, &AHDPlayerCharacter::AddControllerYawInput);
    PlayerInputComponent->BindAxis("LookUp", this, &AHDPlayerCharacter::AddControllerPitchInput);

    // 'action' 바인딩을 구성합니다.
    PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AHDPlayerCharacter::StartJump);
    PlayerInputComponent->BindAction("Jump", IE_Released, this, &AHDPlayerCharacter::StopJump);
    PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AHDPlayerCharacter::Fire);
}

void AHDPlayerCharacter::MoveForward(float Value)
{
    // 어디가 '앞'인지 찾고, 플레이어가 해당 방향으로 이동하고자 한다는 것을 기록합니다.
    FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::X);
    AddMovementInput(Direction, Value);
}

void AHDPlayerCharacter::MoveRight(float Value)
{
    // 어디가 '오른쪽'인지 찾고, 플레이어가 해당 방향으로 이동하고자 한다는 것을 기록합니다.
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

    // 발사체 발사를 시도합니다.
    if (ProjectileClass)
    {
        // 카메라 트랜스폼을 구합니다.
        FVector CameraLocation;
        FRotator CameraRotation;
        GetActorEyesViewPoint(CameraLocation, CameraRotation);

        // MuzzleOffset이 카메라 살짝 앞에서 발사체를 스폰하도록 설정합니다.
        MuzzleOffset.Set(100.0f, 0.0f, 0.0f);

        // MuzzleOffset을 카메라 스페이스에서 월드 스페이스로 변환합니다.
        FVector MuzzleLocation = CameraLocation + FTransform(CameraRotation).TransformVector(MuzzleOffset);

        // 조준이 살짝 위를 향하도록 왜곡합니다.
        FRotator MuzzleRotation = CameraRotation;
        MuzzleRotation.Pitch += 10.0f;

        UWorld* World = GetWorld();
        if (World)
        {
            FActorSpawnParameters SpawnParams;
            SpawnParams.Owner = this;
            SpawnParams.Instigator = GetInstigator();

            // 총구에 발사체를 스폰합니다.
            AHDBowProjectile* Projectile = World->SpawnActor<AHDBowProjectile>(ProjectileClass, MuzzleLocation, MuzzleRotation, SpawnParams);
            if (Projectile)
            {
                // 발사체의 초기 탄도를 설정합니다.
                FVector LaunchDirection = MuzzleRotation.Vector();
                Projectile->FireInDirection(LaunchDirection);
            }
        }
    }
}