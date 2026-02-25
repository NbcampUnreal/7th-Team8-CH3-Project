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
	HUDWidgetclass(nullptr),
	HUDWidgetInstance(nullptr),
	MainMenuWidgetClass(nullptr),
	MainMenuWidgetInstacne(nullptr)
{
}

void AHDPlayerController::BeginPlay()
{
	Super::BeginPlay();

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

	if (MainMenuWidgetInstacne)
	{
		MainMenuWidgetInstacne->RemoveFromParent();
		MainMenuWidgetInstacne = nullptr;
	}

	if (HUDWidgetclass)
	{
		HUDWidgetInstance = CreateWidget<UUserWidget>(this, HUDWidgetclass);
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

	if (MainMenuWidgetInstacne)
	{
		MainMenuWidgetInstacne->RemoveFromParent();
		MainMenuWidgetInstacne = nullptr;
	}

	if (MainMenuWidgetClass)
	{
		MainMenuWidgetInstacne = CreateWidget<UUserWidget>(this, MainMenuWidgetClass);
		if (MainMenuWidgetInstacne)
		{
			MainMenuWidgetInstacne->AddToViewport();

			bShowMouseCursor = true;
			SetInputMode(FInputModeUIOnly());
		}

		if (UTextBlock* ButtonText = Cast<UTextBlock>(MainMenuWidgetInstacne->GetWidgetFromName(TEXT("StartButtonText"))))
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
			UFunction* PlayAnimFunc = MainMenuWidgetInstacne->FindFunction(FName("PlayGameOverAnim"));
			if (PlayAnimFunc)
			{
				MainMenuWidgetInstacne->ProcessEvent(PlayAnimFunc, nullptr);
			}

			if (UTextBlock* TotalScoreText = Cast<UTextBlock>(MainMenuWidgetInstacne->GetWidgetFromName("TotalScoreText")))
			{
				if (AHDGameState* HDGameState = Cast<AHDGameState>(UGameplayStatics::GetGameInstance(this)))
				{
					TotalScoreText->SetText(FText::FromString(FString::Printf(TEXT("Total Score: %d"), HDGameState->Score)));
				}
			}
		}
	}
}

UUserWidget* AHDPlayerController::GetHUDWidget() const
{
	return HUDWidgetInstance;
}