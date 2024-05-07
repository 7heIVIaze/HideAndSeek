// CopyrightNotice=0 2023 Sunggon Kim kimdave205@gmail.com

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
	GameplaySettingWidget UMETA(DisplayName = "GameplaySettingWidget"),
	GraphicsSettingWidget UMETA(DisplayName = "GraphicsSettingWidget"),
	ArchiveWidget UMETA(DisplayName = "ArchiveWidget"),
};
/**
 * 
 */
UCLASS()
class HIDEANDSEEK_API AStartGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	AStartGameMode();

	UFUNCTION()
		void ChangeWidget(WidgetType widgetNumber);

	UFUNCTION()
	void SetCurrentWidget(UUserWidget* inWidgetObject);

protected:
	virtual void BeginPlay() override;

private:
	TSubclassOf<UUserWidget>StartLevelWidgetClass;
	TSubclassOf<UUserWidget>StageSelectWidgetClass;
	TSubclassOf<UUserWidget>OptionSettingWidgetClass;
	TSubclassOf<UUserWidget>GameSettingWidgetClass;
	TSubclassOf<UUserWidget>GraphicsSettingWidgetClass;
	TSubclassOf<UUserWidget>ArchiveWidgetClass;
	class UStartWidget* StartWidget;
	class UUserWidget* CurrentWidget;

	class UAudioComponent* AudioComponent;
	
	class USoundCue* OpeningCue;
	//FInputModeUIOnly UIInputMode;
};
