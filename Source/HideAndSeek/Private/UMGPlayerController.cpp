// Fill out your copyright notice in the Description page of Project Settings.


#include "UMGPlayerController.h"
#include "GameFramework/PlayerState.h"

void AUMGPlayerController::BeginPlay()
{
	Super::BeginPlay();

	FInputModeUIOnly InputMode;

	InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);

	SetInputMode(InputMode);
	bShowMouseCursor = true;;
}