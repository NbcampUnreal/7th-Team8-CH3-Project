#include "HDPlayerController.h"
#include "HDPlayerCharacter.h"
#include "HDMonCharacter.h"
#include "HDGameStateBase.h"
#include "HDGameInstance.h"
#include "EnhancedInputSubsystems.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"
#include "Kismet/GameplayStatics.h"

AHDPlayerController::AHDPlayerController():
	InputMappingContext(nullptr),
	MoveAction(nullptr),
	DashAction(nullptr),
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

	if (HUDWidgetclass)
	{
		HUDWidgetInstance = CreateWidget<UUserWidget>(this, HUDWidgetclass);
		if (HUDWidgetInstance)
		{
			HUDWidgetInstance->AddToViewport();
		}
	}

	FString CurrentStageName = GetWorld()->GetMapName();
	if (CurrentStageName.Contains("MenuStage"))
	{
		ShowMainMenu(false);
	}
}

void AHDPlayerController::StartGame()
{
	if (AHDGameStateBase* HDGameStateBase = Cast<AHDGameStateBase>(UGameplayStatics::GetGameInstance(this)))
	{
		HDGameStateBase->CurrentStageIndex = 0;
		HDGameStateBase->Score = 0;
	}

	UGameplayStatics::OpenLevel(this, FName("Stage1"));
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

		AHDGameStateBase* HDGameStateBase = GetWorld() ? GetWorld()->GetGameState<AHDGameStateBase>() : nullptr;
		if (HDGameStateBase)
		{
			HDGameStateBase->UpdateHUD();
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
				if (AHDGameStateBase* HDGameStateBase = Cast<AHDGameStateBase>(UGameplayStatics::GetGameInstance(this)))
				{
					TotalScoreText->SetText(FText::FromString(FString::Printf(TEXT("Total Score: %d"), HDGameStateBase->Score)));
				}
			}
		}
	}
}

UUserWidget* AHDPlayerController::GetHUDWidget() const
{
	return HUDWidgetInstance;
}