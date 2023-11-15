// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "HorrorGameGameInstance.generated.h"

struct FHorrorGameItemData;
class UDataTable;
UCLASS()
class HORRORGAME_API UHorrorGameGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	UHorrorGameGameInstance();

	virtual void OnStart() override;
	virtual void Init() override;

	UFUNCTION()
		virtual void BeginLoadingScreen(const FString& MapName);

	UFUNCTION()
		virtual void EndLoadingScreen(UWorld* InLoadedWorld);

	FHorrorGameItemData* GetHorrorGameItemData(int32 Name);

	// void ShowStageSelectWidget(bool bIsShow);
	void StopSound();

	//void PlaySound();

	//void ChangeWidget(TSubclassOf<UUserWidget> newWidget);

private:
	UPROPERTY()
	UDataTable* HorrorGameTable;

	UPROPERTY()
		class UUserWidget* LoadingWidget;
	TSubclassOf<UUserWidget>WidgetClass;

	UUserWidget* CurrentWidget;

	class USoundCue* OpeningCue;
	class UAudioComponent* BGM_Comp;
};
