// CopyrightNotice 2023 Sunggon Kim kimdave205@gmail.com. All Rights Reserved.

#include "HUD/ObjectWidget.h"
#include "Components/Image.h"
#include "Player/InventoryComponent.h"
#include "LevelManager/HorrorGameGameInstance.h"
#include "LevelManager/HorrorGameGameMode.h"
#include "Kismet/GameplayStatics.h"

// 오브젝트 수집 위젯 생성자
void UObjectWidget::NativeConstruct()
{
	Super::NativeConstruct();

	ObjectImg.Add(ObjectImg1);
	ObjectImg.Add(ObjectImg2);
	ObjectImg.Add(ObjectImg3);
	ObjectImg.Add(ObjectImg4);
	ObjectImg.Add(ObjectImg5);

}

void UObjectWidget::Init()
{
	AHorrorGameGameMode* GameMode = Cast<AHorrorGameGameMode>(UGameplayStatics::GetGameMode(this));
	UHorrorGameGameInstance* GameInstance = Cast<UHorrorGameGameInstance>(GetGameInstance());

	// Get the number of object to collect.
	if (GameMode)
	{
		if (GameInstance)
		{
			ObjectNumber = GameInstance->GetObjectCountToCollect();
		}
	}

	for (int i = 0; i < ObjectImg.Num(); ++i)
	{
		if (ObjectNumber <= i)
		{
			ObjectImg[i]->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}

bool UObjectWidget::InitializeWidget(class UInventoryComponent* InventoryComponent)
{
	if (InventoryComponent)
	{
		InventoryComponent->OnObjectAcquired.AddDynamic(this, &UObjectWidget::RefreshObjectCount);
		InventoryComponent->OnObjectConsumed.AddDynamic(this, &UObjectWidget::RefreshObjectCount);
		return true;
	}

	return false;
}

void UObjectWidget::RefreshObjectCount(int32 ObjectCount)
{
	for (int i = 0; i < ObjectNumber; ++i)
	{
		if (i < ObjectCount)
		{
			ObjectImg[i]->SetBrushFromTexture(CollectedObject);
		}
		else
		{
			ObjectImg[i]->SetBrushFromTexture(NotCollectedObject);
		}
	}
}