// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GameFramework/HUD.h"
#include "HorrorGameGameMode.generated.h"

UCLASS(minimalapi)
class AHorrorGameGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AHorrorGameGameMode();

	UFUNCTION(BlueprintCallable)
		void StartBackGroundMusic();
//
protected:
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;


private:
	TSubclassOf<UUserWidget>LevelWidgetClass;
	class UGameUI* MainWidget;

	class UAudioComponent* AudioComponent;

	class USoundCue* MainCue;
};



