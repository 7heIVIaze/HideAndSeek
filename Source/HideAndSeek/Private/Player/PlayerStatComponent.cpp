// CopyrightNotice 2023 Sunggon Kim kimdave205@gmail.com. All Rights Reserved.

#include "Player/PlayerStatComponent.h"
#include "Player/HorrorGameCharacter.h"

// Sets default values for this component's properties
UPlayerStatComponent::UPlayerStatComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	PlayerStatus = EPlayerStatus::Loading;
	// ...
}


// Called when the game starts
void UPlayerStatComponent::BeginPlay()
{
	Super::BeginPlay();

	// 소유자 설정
	OwnerCharacter = Cast<AHorrorGameCharacter>(GetOwner());
	
}

void UPlayerStatComponent::InitializeComponent()
{
	Super::InitializeComponent();
}


// Called every frame
void UPlayerStatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UPlayerStatComponent::SetPlayerStatus(EPlayerStatus inPlayerStatus)
{
	PlayerStatus = inPlayerStatus;

	//OwnerCharacter->SetPlayerStatus(PlayerStatus);
}