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
	OverheadWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("OverheadWidget"));  // 여기 세줄 캐릭터 CPP로 옮겨야함
	OverheadWidget->SetupAttachment(GetMesh());
	OverheadWidget->SetWidgetSpace(EWidgetSpace::Screen);
}

void AHDPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (ULocalPlayer* LocalPlayer = GetLocalPlayer())
	{

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
	if (UHDGameInstance* HDGameInstance = Cast<UHDGameInstance>(UGameplayStatics::GetGameInstance(this)))
	{
		HDGameInstance->CurrentStageIndex = 0;
		HDGameInstance->TotalScore = 0;
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
				if (UHDGameInstance* HDGameInstacne = Cast<UHDGameInstance>(UGameplayStatics::GetGameInstance(this)))
				{
					TotalScoreText->SetText(FText::FromString(FString::Printf(TEXT("Total Score: %d"), HDGameInstacne->TotalScore)));
				}
			}
		}
	}
}

UUserWidget* AHDPlayerController::GetHUDWidget() const
{
	return HUDWidgetInstance;
}

void AHDPlayerController::UpdateHUD()  // 이 함수 게임 스테이트로 옮겨야함, StageTimerHandle, TotalScore, CurrentStageIndex 변수 필요
{
	if (APlayerController* PlayerController = Cast<APlayerController>(PlayerController))
	{
		if (AHDPlayerController* HDPlayerController = Cast<AHDPlayerController>(HDPlayerController))
		{
			if (UUserWidget* HUDWidget = HDPlayerController->GetHUDWidget())
			{
				if (UProgressBar* PlayerHPProgressBar = Cast<UProgressBar>(HUDWidget->GetWidgetFromName(TEXT("CharacterHPBar"))))
				{
					if (AHDPlayerCharacter* HDPlayerCharacter = Cast<AHDPlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn()))
					{
						float Precent = (float)HDPlayerCharacter->HP / HDPlayerCharacter->MaxHP;
						PlayerHPProgressBar->SetPercent(Precent);
					}
				}

				if (UProgressBar* PlayerManaProgressBar = Cast<UProgressBar>(HUDWidget->GetWidgetFromName(TEXT("CharacterManaBar"))))
				{
					if (AHDPlayerCharacter* HDPlayerCharacter = Cast<AHDPlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn()))
					{
						float Precent = (float)HDPlayerCharacter->Mana / HDPlayerCharacter->MaxMana;
						PlayerManaProgressBar->SetPercent(Precent);
					}
				}

				if (UTextBlock* TimeText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("TimeText"))))
				{
					float RemainingTime = GetWorldTimerManager().GetTimerRemaining(StageTimerHandle);
					TimeText->SetText(FText::FromString(FString::Printf(TEXT("Time : %.1f"), RemainingTime)));
				}

				if (UTextBlock* ScoreText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("ScoreText"))))
				{
					if (UGameInstance* GameInstance = GetGameInstance())
					{
						UHDGameInstance* HDGameInstance = Cast<UHDGameInstance>(GameInstance);
						if (HDGameInstance)
						{
							ScoreText->SetText(FText::FromString(FString::Printf(TEXT("Score: %d"), HDGameInstance->TotalScore)));
						}
					}
				}

				if (UTextBlock* StageIndexText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("StageText"))))
				{
					StageIndexText->SetText(FText::FromString(FString::Printf(TEXT("%d Stage"), CurrentStageIndex + 1)));
				}
			}
		}
	}
}

void AHDPlayerController::UpdateOverheadHP()   // 이 함수는 몬스터 CPP로 옮겨야함
{
	if (!OverheadWidget) return;

	UUserWidget* OverheadWidgetInstacne = OverheadWidget->GetUserWidgetObject();
	if (OverheadWidgetInstacne) return;
	
	if (UProgressBar* MonsterOverheadHPBar = Cast<UProgressBar>(OverheadWidgetInstacne->GetWidgetFromName("MonsterOverheadHP")))
	{
		if (AHDMonCharacter* HDMonCharacter = Cast<AHDMonCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn()))
		{
			float Precent = (float)HDMonCharacter->HP / HDMonCharacter->MaxHP;
			MonsterOverheadHPBar->SetPercent(Precent);
		}
	}
}
