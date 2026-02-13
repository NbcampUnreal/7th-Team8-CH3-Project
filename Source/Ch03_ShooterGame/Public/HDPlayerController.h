#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Components/WidgetComponent.h"
#include "HDPlayerController.generated.h"

class UInputMappingContext;
class UInputAction;

UCLASS()
class CH03_SHOOTERGAME_API AHDPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	AHDPlayerController();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputMappingContext* InputMappingContext;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* MoveAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* DashAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD")
	TSubclassOf<UUserWidget> HUDWidgetclass;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "HUD")
	UUserWidget* HUDWidgetInstance;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD")
	TSubclassOf<UUserWidget> MainMenuWidgetClass;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "HUD")
	UUserWidget* MainMenuWidgetInstacne;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")  // 이건 몬스터 헤더로 옮겨야함
	UWidgetComponent* OverheadWidget;
	
	UFUNCTION(BlueprintPure, Category = "HUD")
	UUserWidget* GetHUDWidget() const;
	UFUNCTION(BlueprintCallable, Category = "HUD")
	void ShowCharacterHUD();
	UFUNCTION(BlueprintCallable, Category = "Munu")
	void ShowMainMenu(bool bIsRestart);
	UFUNCTION(BlueprintCallable, Category = "Munu")
	void StartGame();

	virtual void BeginPlay() override;
	void UpdateHUD();
	void UpdateOverheadHP();
};
