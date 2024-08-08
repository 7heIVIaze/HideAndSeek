// CopyrightNotice 2023 Sunggon Kim kimdave205@gmail.com. All Rights Reserved.

#include "Player/UMGPlayerController.h"
#include "GameFramework/PlayerState.h"

// UI용 컨트롤러
void AUMGPlayerController::BeginPlay()
{
	Super::BeginPlay();

	FInputModeUIOnly InputMode;

	InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);

	SetInputMode(InputMode);
	bShowMouseCursor = true;;
}