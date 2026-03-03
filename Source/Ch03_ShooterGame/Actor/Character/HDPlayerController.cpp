#include "HDPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "Core/HDGameInstance.h"
#include "Blueprint/UserWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Core/HDGameState.h"
#include "Kismet/GameplayStatics.h"
#include "HDPlayerCharacter.h"
#include "NavigationSystemTypes.h"

AHDPlayerController::AHDPlayerController():
	InputMappingContext(nullptr),
	MoveAction(nullptr),
	DashAction(nullptr),
	AttackAction(nullptr),
	HUDWidgetClass(nullptr),
	HUDWidgetInstance(nullptr),
	MainMenuWidgetClass(nullptr),
	MainMenuWidgetInstance(nullptr),
    GameOverWidgetClass(nullptr),
    GameOverWidgetInstance(nullptr),
    GameClearWidgetClass(nullptr),
    GameClearWidgetInstance(nullptr)
{
}

void AHDPlayerController::BeginPlay()
{
	Super::BeginPlay();
	
	// 마우스 커서 화면 상에 노출
	bShowMouseCursor = true;
	// 게임 + UI 입력을 둘 다 받는 입력 모드 객체를 생성
	FInputModeGameAndUI InputMode;
	// 마우스 커서를 화면 밖으로 못 나가게 잠굼
	InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::LockAlways);
	// 클릭 중에도 커서를 숨기지 않음 (true 하면 클릭 시 커서 숨김)
	InputMode.SetHideCursorDuringCapture(false);
	// 생성 후 설정한 입력 객체의 정보를 컨트롤러에 적용
	SetInputMode(InputMode);

	if (ULocalPlayer* LocalPlayer = GetLocalPlayer())
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
			LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		{
			if (InputMappingContext)
			{
				Subsystem->AddMappingContext(InputMappingContext, 0);
			}
		}
	}

	FString CurrentStageName = GetWorld()->GetMapName();
	if (CurrentStageName.Contains("L_MainMenu"))
	{
		ShowMainMenu();
	}
}

void AHDPlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	AHDPlayerCharacter* MyCharacter = Cast<AHDPlayerCharacter>(GetPawn());

	if (MyCharacter && MyCharacter->bIsRolling == false)
	{
		LookAtMouseCursor(DeltaSeconds);
	}
}

void AHDPlayerController::StartGame()
{
	if (AHDGameState* HDGameState = GetWorld()->GetGameState<AHDGameState>())
		if (UHDGameInstance* HDGameInstance = Cast<UHDGameInstance>(UGameplayStatics::GetGameInstance(this)))
		{
			HDGameState->CurrentLevelIndex = 0;
			HDGameInstance->TotalScore = 0;
		}

	UGameplayStatics::OpenLevel(this, FName("L_Geunjeongjeon"));
	SetPause(false);
}

void AHDPlayerController::ShowCharacterHUD()
{
	if (HUDWidgetInstance)
	{
		HUDWidgetInstance->RemoveFromParent();
		HUDWidgetInstance = nullptr;
	}

	if (MainMenuWidgetInstance)
	{
		MainMenuWidgetInstance->RemoveFromParent();
		MainMenuWidgetInstance = nullptr;
	}

	if (HUDWidgetClass)
	{
		HUDWidgetInstance = CreateWidget<UUserWidget>(this, HUDWidgetClass);
		if (HUDWidgetInstance)
		{
			HUDWidgetInstance->AddToViewport();

			bShowMouseCursor = false;
			SetInputMode(FInputModeGameOnly());
		}

		AHDGameState* HDGameState = GetWorld() ? GetWorld()->GetGameState<AHDGameState>() : nullptr;
		if (HDGameState)
		{
			HDGameState->UpdateHUD();
		}
	}
}

void AHDPlayerController::ShowMainMenu()
{
	if (HUDWidgetInstance)
	{
		HUDWidgetInstance->RemoveFromParent();
		HUDWidgetInstance = nullptr;
	}

	if (MainMenuWidgetInstance)
	{
		MainMenuWidgetInstance->RemoveFromParent();
		MainMenuWidgetInstance = nullptr;
	}

	if (MainMenuWidgetClass)
	{
		MainMenuWidgetInstance = CreateWidget<UUserWidget>(this, MainMenuWidgetClass);
		if (MainMenuWidgetInstance)
		{
			MainMenuWidgetInstance->AddToViewport();

			bShowMouseCursor = true;
			SetInputMode(FInputModeUIOnly());
		}

		if (UTextBlock* ButtonText = Cast<UTextBlock>(MainMenuWidgetInstance->GetWidgetFromName(TEXT("StartButtonText"))))
		{
				ButtonText->SetText(FText::FromString(TEXT("게임 시작")));
		}
	}
}

void AHDPlayerController::ShowGameOverUI()
{
	if (GameOverWidgetInstance)
		return;
	
	if (GameOverWidgetClass)
	{
		GameOverWidgetInstance = CreateWidget<UUserWidget>(this, GameOverWidgetClass);
		if (GameOverWidgetInstance)
		{
			GameOverWidgetInstance->AddToViewport();
			
			bShowMouseCursor = true;
			SetInputMode(FInputModeUIOnly());
		}
		
		if (UTextBlock* MainMenuText = Cast<UTextBlock>(GameOverWidgetInstance->GetWidgetFromName("MainMenuText")))
		{
			MainMenuText->SetText(FText::FromString(FString::Printf(TEXT("시작 화면으로"))));
		}
		
		if (UTextBlock* RestartText = Cast<UTextBlock>(GameOverWidgetInstance->GetWidgetFromName("RestartText")))
		{
			RestartText->SetText(FText::FromString(FString::Printf(TEXT("재시작"))));
		}
		
		if (UTextBlock* TotalScoreText = Cast<UTextBlock>(GameOverWidgetInstance->GetWidgetFromName("TotalScoreText")))
		{
			if (UHDGameInstance* HDGameInstance = Cast<UHDGameInstance>(UGameplayStatics::GetGameInstance(this)))
			{
				TotalScoreText->SetText(FText::FromString(FString::Printf(TEXT("Total Score: %d"), HDGameInstance->TotalScore)));
			}
		}
	}
}

void AHDPlayerController::ShowGameClearUI()
{
	if (GameClearWidgetInstance)
		return;
	
	if (GameClearWidgetClass)
	{
		GameClearWidgetInstance = CreateWidget<UUserWidget>(this, GameClearWidgetClass);
		if (GameClearWidgetInstance)
		{
			GameClearWidgetInstance->AddToViewport();
			
			bShowMouseCursor = true;
			SetInputMode(FInputModeGameOnly());
		}
		
		if (UTextBlock* GameClearText = Cast<UTextBlock>(GameClearWidgetInstance->GetWidgetFromName("GameClearText")))
		{
			GameClearText->SetText(FText::FromString(FString::Printf(TEXT("퇴치 성공"))));
		}
		
		if (UTextBlock* TotalScoreText = Cast<UTextBlock>(GameClearWidgetInstance->GetWidgetFromName("TotalScoreText")))
		{
			if (UHDGameInstance* HDGameInstance = Cast<UHDGameInstance>(UGameplayStatics::GetGameInstance(this)))
			{
				TotalScoreText->SetText(FText::FromString(FString::Printf(TEXT("최종 점수 : %d"), HDGameInstance->TotalScore)));
			}
		}
	}
}

void AHDPlayerController::LookAtMouseCursor(float DeltaTime) // 마우스 위치를 따라 캐릭터가 회전하게 만들어주는 함수
{
	// 조종 중인 캐릭터 가져오기, 없으면 리턴
	AActor* ControlledActor = GetPawn();
	if (!ControlledActor) return;
	
	// 마우스가 충돌한 위치를 저장할 변수
	FHitResult HitResult;
	// 마우스 커서 아래에 보이지 않는 레이저를 쏴 눈으로 보이는 것들 중에서 충돌했는지 확인해서 처음으로 레이저를 맞은 결과물을 결과를 HitResult 변수에 저장, bool 변수니 성공 여부를 bool 값으로 받음
	bool bHitSuccessful = GetHitResultUnderCursor(ECC_Visibility, true, HitResult);
	// bHitSuccessful 변수가 true 라면
	if (bHitSuccessful)
	{
		// 캐릭터 회전 속도 설정
		float RotationSpeed = 60.0f;
		// 마우스 커서의 보이지 않는 레이저에 맞은 월드의 위치를 저장
		FVector TargetLocation = HitResult.Location;
		// 현재 캐릭터의 위치 가져오기
		FVector PawnLocation = ControlledActor->GetActorLocation();
	
		// TargetLocation의 Z 값과 PawnLocation의 Z 값을 동일하게 맞춤
		TargetLocation.Z = PawnLocation.Z;
		
		// 캐릭터 위치에서 마우스 위치까지의 방향을 구함
		FVector Direction = TargetLocation - PawnLocation;
		// 캐릭터가 보고 있는 방향을 가져옴
		FRotator CurrentRotation = ControlledActor->GetActorRotation();
		// 아까 구한 방향의 회전값을 저장
		FRotator TargetRotation = Direction.Rotation();
		// 지금 보고 있는 방향에서 목표 방향까지 천천히 부드럽게 회전
		FRotator SmoothRotation = FMath::RInterpTo(CurrentRotation, TargetRotation, DeltaTime, RotationSpeed);
		
		// 계산된 회전을 캐릭터에 적용
		ControlledActor->SetActorRotation(SmoothRotation);
	}
}
