// CopyrightNotice 2023 Sunggon Kim kimdave205@gmail.com. All Rights Reserved.

#include "Items/PassiveItemClass.h"
#include "Player/HorrorGameCharacter.h"

// Sets default values
APassiveItemClass::APassiveItemClass()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	

}

void APassiveItemClass::OnInteract(class AHorrorGameCharacter* Player)
{

}

bool APassiveItemClass::UseInteract(class AHorrorGameCharacter* Player)
{
	return false;
}

void APassiveItemClass::HoldInteract(class AHorrorGameCharacter* Player)
{

}

FHorrorGameItemData APassiveItemClass::GetItemData()
{
	Super::GetItemData();

	return ItemData;
}