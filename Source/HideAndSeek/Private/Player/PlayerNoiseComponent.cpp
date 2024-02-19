// CopyrightNotice=0 2023 Sunggon Kim kimdave205@gmail.com

#include "Player/PlayerNoiseComponent.h"
#include "Components/SphereComponent.h"

// Sets default values for this component's properties
UPlayerNoiseComponent::UPlayerNoiseComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	//SphereCollider = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollider"));
	// ...
}


// Called when the game starts
void UPlayerNoiseComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
//void UPlayerNoiseComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
//{
//	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
//
//	// ...
//}

void UPlayerNoiseComponent::OnOverlapBegin(class UPrimitiveComponent* newComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

}