// CopyrightNotice 2023 Sunggon Kim kimdave205@gmail.com. All Rights Reserved.

#include "Player/PlayerItemComponent.h"
#include "CustomDataTables.h"

#define PASSIVEITEMSTARTNUMBER 10

// Sets default values for this component's properties
UPlayerItemComponent::UPlayerItemComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	//SphereCollider = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollider"));
	// ...

}


// Called when the game starts
void UPlayerItemComponent::BeginPlay()
{
	Super::BeginPlay();

	Inventory.Init(FHorrorGameItemData::FHorrorGameItemData(), 9);
	// ...
	
}

// Called every frame
//void UPlayerNoiseComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
//{
//	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
//
//	// ...
//}

void UPlayerItemComponent::GetPassiveItem(int inPassiveItemNumber)
{

}

void UPlayerItemComponent::AddToInventory(int ItemNumber)
{

}

void UPlayerItemComponent::RemoveFromInventory()
{

}

void UPlayerItemComponent::GetCigarLight()
{
	
}

void UPlayerItemComponent::UseItem()
{

}

void UPlayerItemComponent::GetFlashLight()
{

}