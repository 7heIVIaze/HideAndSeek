// CopyrightNotice 2023 Sunggon Kim kimdave205@gmail.com. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "HorrorGamePlayerController.generated.h"

/**
 * 
 */
UCLASS()
class HIDEANDSEEK_API AHorrorGamePlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AHorrorGamePlayerController();

	virtual void PostInitializeComponents() override;
	virtual void OnPossess(APawn* aPawn) override;

	class UGameUI* GetMainWidget() const;

	void ChangeInputMode(bool bGameMode = true);

	void ShowDeadUI();

	UFUNCTION(BlueprintCallable)
	void SetDeadUIText(FText inText);

	void ShowMainUI();

	void OnGamePause();

	UFUNCTION(BlueprintCallable)
	void OnGameClear(FString inClearedTime, bool bIsRecordBreaking);

	UFUNCTION(BlueprintCallable)
		void ShowEnding(int EndType); // 0: BadEnding, 1: NormalEnding, 2: TrueEnding

	UFUNCTION(BlueprintCallable)
		void OnDocumentWidget(FText inText, FString inType); // 문서 위젯

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
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = UI)
		TSubclassOf<UUserWidget>ClearWidgetClass;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = UI)
		TSubclassOf<UUserWidget>DocumentWidgetClass;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = UI)
		TSubclassOf<UUserWidget>BadEndingWidgetClass;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = UI)
		TSubclassOf<UUserWidget>NormalEndingWidgetClass;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = UI)
		TSubclassOf<UUserWidget>TrueEndingWidgetClass;

private:
	UPROPERTY()
		class UGameUI* MainWidget;

	UPROPERTY()
		class UDiedWidget* DeadWidget;
	
	UPROPERTY()
		class UPauseWidget* PauseWidget;

	UPROPERTY()
		class UUserWidget* LoadingWidget;

	UPROPERTY()
		class UClearWidget* ClearWidget;

	UPROPERTY()
		class UDocumentWidget* DocumentWidget;

	UPROPERTY()
		class UUserWidget* EndingWidget;

	FInputModeGameOnly GameInputMode;
	FInputModeUIOnly UIInputMode;
	
};
