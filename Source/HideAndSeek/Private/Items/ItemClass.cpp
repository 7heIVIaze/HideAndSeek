// CopyrightNotice 2023 Sunggon Kim kimdave205@gmail.com. All Rights Reserved.

#include "Items/ItemClass.h"
#include "Player/HorrorGameCharacter.h"
#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"

AItemClass::AItemClass()
{
	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = RootComp;

	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMesh"));
	ItemMesh->SetupAttachment(RootComp);

	ItemMesh->SetCollisionProfileName(TEXT("ItemObjects"));
}

void AItemClass::OnInteract(class AHorrorGameCharacter* Player)
{
	// Super::OnInteract();
	if (ItemAcquireSoundCue)
	{
		UGameplayStatics::PlaySound2D(this, ItemAcquireSoundCue);
	}
	UKismetSystemLibrary::MoveComponentTo(RootComp, Player->GetActorLocation(), GetActorRotation(), false, false, 0.1f, true, EMoveComponentAction::Move, FLatentActionInfo(0, 123, TEXT("MoveFinish"), this));
}

bool AItemClass::UseInteract(class AHorrorGameCharacter* Player)
{
	return true;
}

void AItemClass::HoldInteract(class AHorrorGameCharacter* Player)
{
	if (ItemAcquireSoundCue)
	{
		UGameplayStatics::PlaySound2D(this, ItemAcquireSoundCue);
	}
	UKismetSystemLibrary::MoveComponentTo(RootComp, Player->GetActorLocation(), GetActorRotation(), false, false, 0.1f, true, EMoveComponentAction::Move, FLatentActionInfo(0, 123, TEXT("MoveFinish"), this));
}

void AItemClass::MoveFinish()
{
	Destroy();
}

void AItemClass::SetItemData(int inItemIndex)
{
	if (ItemDataTable)
	{
		FHorrorGameItemData* RowData = ItemDataTable->FindRow<FHorrorGameItemData>(*FString::FromInt(inItemIndex), TEXT(""));

		if (RowData)
		{
			ItemData.ItemName = RowData->ItemName;
			ItemData.ItemNumber = RowData->ItemNumber;
			ItemData.ItemType = RowData->ItemType;
			ItemData.ItemIcon = RowData->ItemIcon;
			ItemData.ItemCount = RowData->ItemCount;
			ItemData.Durability = RowData->Durability;
			ItemData.ItemDescription = RowData->ItemDescription;
			ItemData.ItemBPClass = RowData->ItemBPClass;
		}
	}
}