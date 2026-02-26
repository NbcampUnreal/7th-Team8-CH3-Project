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
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* AttackAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD")
	TSubclassOf<UUserWidget> HUDWidgetClass;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "HUD")
	UUserWidget* HUDWidgetInstance;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD")
	TSubclassOf<UUserWidget> MainMenuWidgetClass;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "HUD")
	UUserWidget* MainMenuWidgetInstance;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD")
	TSubclassOf<UUserWidget> GameOverWidgetClass;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "HUD")
	UUserWidget* GameOverWidgetInstance;
	
	
	UFUNCTION(BlueprintPure, Category = "HUD")
	UUserWidget* GetHUDWidget() const { return HUDWidgetInstance; }
	
	UFUNCTION(BlueprintCallable, Category = "HUD")
	void ShowCharacterHUD();
	
	UFUNCTION(BlueprintCallable, Category = "Menu")
	void ShowMainMenu();
	
	UFUNCTION(BlueprintCallable, Category = "Menu")
	void ShowGameOverHUD();
	
	UFUNCTION(BlueprintCallable, Category = "Menu")
	void StartGame();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	void LookAtMouseCursor(float DeltaTime);
};
