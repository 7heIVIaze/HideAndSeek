// CopyrightNotice 2023 Sunggon Kim kimdave205@gmail.com. All Rights Reserved.

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