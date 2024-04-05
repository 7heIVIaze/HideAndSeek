// CopyrightNotice=0 2023 Sunggon Kim kimdave205@gmail.com

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GameFramework/HUD.h"
#include "HorrorGameGameMode.generated.h"

// DECLARE_DELEGATE(FOnPlayerNearCreatureDelegate)

UCLASS(minimalapi)
class AHorrorGameGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AHorrorGameGameMode();

	UFUNCTION(BlueprintCallable)
		void StartBackGroundMusic();

	UFUNCTION(BlueprintCallable)
		void StopBackGroundMusic();

	UFUNCTION(BlueprintCallable)
		void PlayNervousBackGroundMusic();

	UFUNCTION(BlueprintCallable)
		void StopNervousBackGroundMusic();

	UFUNCTION(BlueprintCallable)
		void PlayChaseBackGroundMusic();

	UFUNCTION(BlueprintCallable)
		void StopChaseBackGroundMusic();

	UFUNCTION(BlueprintCallable)
		void PlayMirrorUseBackGroundMusic();

	UFUNCTION(BlueprintCallable)
		void StopMirrorUseBackGroundMusic();

	UFUNCTION(BlueprintCallable)
		void PlayDiedBackGroundMusic();

	UFUNCTION(BlueprintCallable)
		void PlayClearBackGroundMusic();

	UFUNCTION(BlueprintCallable)
		void PlayEndingBackGroundMusic();
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
	class USoundCue* ChaseCue;
	class USoundCue* MirrorUseCue;
	class USoundCue* DiedCue;
	class USoundCue* ClearCue;
	class USoundCue* EndingCue;

	// FOnPlayerNearCreatureDelegate OnPlayerNearCreature;
};