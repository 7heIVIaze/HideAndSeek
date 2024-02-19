// CopyrightNotice=0 2023 Sunggon Kim kimdave205@gmail.com

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

	UFUNCTION(BlueprintCallable)
		void PlayNervousBackGroundMusic();

	UFUNCTION(BlueprintCallable)
		void StopNervousBackGroundMusic();
	//
protected:
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;


private:
	TSubclassOf<UUserWidget>LevelWidgetClass;
	class UGameUI* MainWidget;

	class UAudioComponent* AudioComponent;
	class UAudioComponent* NervousAudioComponent;

	class USoundCue* MainCue;
	class USoundCue* NervousCue;
};
