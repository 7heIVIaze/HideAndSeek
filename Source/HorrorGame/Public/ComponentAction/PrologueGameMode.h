// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GameFramework/HUD.h"
#include "PrologueGameMode.generated.h"

UCLASS(minimalapi)
class APrologueGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	APrologueGameMode();
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
