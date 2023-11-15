// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "StartGameMode.generated.h"

UENUM(BlueprintType)
enum class WidgetType : uint8
{
	StartWidget UMETA(DisplayName = "StartWidget"),
	LevelSelectWidget UMETA(DisplayName = "LevelSelectWidget"),
	OptionSettingWidget UMETA(DisplayName = "OptionSettingWidget"),
	//GameplaySettingWidget UMETA(DisplayName = "GameplaySettingWidget"),

};
/**
 * 
 */
UCLASS()
class HORRORGAME_API AStartGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	AStartGameMode();

	UFUNCTION()
		void ChangeWidget(WidgetType widgetNumber);

protected:
	virtual void BeginPlay() override;

private:
	TSubclassOf<UUserWidget>StartLevelWidgetClass;
	TSubclassOf<UUserWidget>StageSelectWidgetClass;
	TSubclassOf<UUserWidget>OptionSettingWidgetClass;
	class UStartWidget* StartWidget;
	class UUserWidget* CurrentWidget;

	class UAudioComponent* AudioComponent;
	
	class USoundCue* OpeningCue;
	//FInputModeUIOnly UIInputMode;
};
