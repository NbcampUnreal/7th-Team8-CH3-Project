#include "HDPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"
#include "Core/HDGameState.h"
#include "Kismet/GameplayStatics.h"

AHDPlayerController::AHDPlayerController():
	InputMappingContext(nullptr),
	MoveAction(nullptr),
	DashAction(nullptr),
	AttackAction(nullptr),
	HUDWidgetClass(nullptr),
	HUDWidgetInstance(nullptr),
	MainMenuWidgetClass(nullptr),
	MainMenuWidgetInstance(nullptr)
{
}

void AHDPlayerController::BeginPlay()
{
	Super::BeginPlay();
	
	bShowMouseCursor = true;
	FInputModeGameAndUI InputMode;
	InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::LockAlways);
	InputMode.SetHideCursorDuringCapture(false);
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
		ShowMainMenu(false);
	}
}

void AHDPlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	LookAtMouseCursor(DeltaSeconds);
}

void AHDPlayerController::StartGame()
{
	if (AHDGameState* HDGameState = GetWorld()->GetGameState<AHDGameState>())
	{
		HDGameState->CurrentLevelIndex = 0;
		HDGameState->Score = 0;
	}

	UGameplayStatics::OpenLevel(this, FName("L_Prototyping"));
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

void AHDPlayerController::ShowMainMenu(bool bIsRestart)
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
			if (bIsRestart)
			{
				ButtonText->SetText(FText::FromString(TEXT("Restart")));
			}
			else
			{
				ButtonText->SetText(FText::FromString(TEXT("Start")));
			}
		}

		if (bIsRestart)
		{
			UFunction* PlayAnimFunc = MainMenuWidgetInstance->FindFunction(FName("PlayGameOverAnim"));
			if (PlayAnimFunc)
			{
				MainMenuWidgetInstance->ProcessEvent(PlayAnimFunc, nullptr);
			}

			if (UTextBlock* TotalScoreText = Cast<UTextBlock>(MainMenuWidgetInstance->GetWidgetFromName("TotalScoreText")))
			{
				if (AHDGameState* HDGameState = Cast<AHDGameState>(UGameplayStatics::GetGameInstance(this)))
				{
					TotalScoreText->SetText(FText::FromString(FString::Printf(TEXT("Total Score: %d"), HDGameState->Score)));
				}
			}
		}
	}
}

void AHDPlayerController::LookAtMouseCursor(float DeltaTime)
{
	// 조종 중인 캐릭터 가져오기, 없으면 리턴
	AActor* ControlledActor = GetPawn();
	if (!ControlledActor) return;
	
	FHitResult HitResult;
	bool bHitSuccessful = GetHitResultUnderCursor(ECC_Visibility, true, HitResult);
	if (bHitSuccessful)
	{
		float RotationSpeed = 20.0f;
		FVector TargetLocation = HitResult.Location;
		FVector PawnLocation = ControlledActor->GetActorLocation();
		
		TargetLocation.Z = PawnLocation.Z;
		
		FVector Direction = TargetLocation - PawnLocation;
		FRotator CurrentRotation = ControlledActor->GetActorRotation();
		FRotator TargetRotation = Direction.Rotation();
		FRotator SmoothRotation = FMath::RInterpTo(CurrentRotation, TargetRotation, DeltaTime, RotationSpeed);
		
		ControlledActor->SetActorRotation(SmoothRotation);
	}
}