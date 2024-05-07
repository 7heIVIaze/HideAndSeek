// CopyrightNotice=0 2023 Sunggon Kim kimdave205@gmail.com

#include "Player/PlayerPassiveItemComponent.h"
#include "CustomDataTables.h"

#define PASSIVEITEMSTARTNUMBER 10

// Sets default values for this component's properties
UPlayerPassiveItemComponent::UPlayerPassiveItemComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	//SphereCollider = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollider"));
	// ...
}


// Called when the game starts
void UPlayerPassiveItemComponent::BeginPlay()
{
	Super::BeginPlay();

	PassiveItem.Clear();
	// ...
	
}

// Called every frame
//void UPlayerNoiseComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
//{
//	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
//
//	// ...
//}

void UPlayerPassiveItemComponent::GetPassiveItem(int inPassiveItemNumber)
{

}