// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "HorrorGamePlayerController.generated.h"

/**
 * 
 */
UCLASS()
class HORRORGAME_API AHorrorGamePlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AHorrorGamePlayerController();

	virtual void PostInitializeComponents() override;
	virtual void OnPossess(APawn* aPawn) override;

	class UGameUI* GetMainWidget() const;

	void ChangeInputMode(bool bGameMode = true);

	void ShowDeadUI();

	void ShowMainUI();

	void OnGamePause();

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = UI)
		TSubclassOf<UUserWidget>MainWidgetClass;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = UI)
		TSubclassOf<UUserWidget>DeadWidgetClass;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = UI)
		TSubclassOf<UUserWidget>PauseWidgetClass;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = UI)
		TSubclassOf<UUserWidget>LoadingWidgetClass;

private:

	UPROPERTY()
		class UGameUI* MainWidget;

	UPROPERTY()
		class UDiedWidget* DeadWidget;
	
	UPROPERTY()
		class UPauseWidget* PauseWidget;

	UPROPERTY()
		class UUserWidget* LoadingWidget;

	FInputModeGameOnly GameInputMode;
	FInputModeUIOnly UIInputMode;
	
};
