// CopyrightNotice 2023 Sunggon Kim kimdave205@gmail.com. All Rights Reserved.

#include "Furniture/DoorClass.h"
#include "Components/AudioComponent.h"
#include "Components/BoxComponent.h"
#include "HUD/GameUI.h"
#include "Player/HorrorGameCharacter.h"

// Sets default values
ADoorClass::ADoorClass()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Setting Meshes(Detailed settings should be set in the derived class(C++/Blueprints.) 
	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = RootComp;

	DoorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Door"));
	DoorMesh->SetupAttachment(RootComp);

	DestructionAudio = CreateDefaultSubobject<UAudioComponent>(TEXT("DestructionAudio"));
	DestructionAudio->SetupAttachment(DoorMesh);

	PlayerOverlapBox = CreateDefaultSubobject<UBoxComponent>(TEXT("PlayerOverlapBox"));
	PlayerOverlapBox->SetupAttachment(RootComp);
	PlayerOverlapBox->OnComponentBeginOverlap.AddDynamic(this, &ADoorClass::PlayerBoxBeginOverlap);
	PlayerOverlapBox->OnComponentEndOverlap.AddDynamic(this, &ADoorClass::PlayerBoxEndOverlap);
}

// Called when the game starts or when spawned
void ADoorClass::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADoorClass::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ADoorClass::OnInteract(class AHorrorGameCharacter* Player)
{
	// Event Binding
	UGameUI* GameWidget = Player->GameUIWidget;

	if (GameWidget)
	{
		if (!OnInteractionMessage.IsAlreadyBound(GameWidget, &UGameUI::ShowMessage))
		{
			OnInteractionMessage.AddDynamic(GameWidget, &UGameUI::ShowMessage);
		}
	}
}

bool ADoorClass::UseInteract(class AHorrorGameCharacter* Player)
{
	return true;
}

void ADoorClass::DoorOpen(float inCurveFloat)
{

}

bool ADoorClass::UnlockDoor()
{
	return true;
}

void ADoorClass::BreakDoor()
{

}

void ADoorClass::AIInteract(class AActor* AICharacter)
{

}

void ADoorClass::ChangeCollisionPreset()
{

}

void ADoorClass::DestructionFinished()
{

}

// 문의 충돌 여부를 설정하는 함수.
void ADoorClass::SetDoorCollision(bool inIsPlayerNear)
{
	bIsPlayerNear = inIsPlayerNear;
	
	// true라면 플레이어가 근처에 있는 것이기 때문에 콜리전(물리적 충돌) 활성화
	if (bIsPlayerNear)
	{
		DoorMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel5, ECollisionResponse::ECR_Block);
	}
	// false라면 근처에 플레이어가 없는 것이기 때문에 콜리전(물리적 충돌) 비활성화
	else
	{
		DoorMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel5, ECollisionResponse::ECR_Ignore);
	}
}

void ADoorClass::PlayerBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor != nullptr && OtherComp != nullptr && OtherActor != this)
	{
		if (AHorrorGameCharacter* Character = Cast<AHorrorGameCharacter>(OtherActor)) // 접촉된 액터가 플레이어여야지만 발동
		{
			PlayerCharacter = Character;
			SetDoorCollision(true);
		}
	}
}

void ADoorClass::PlayerBoxEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor != nullptr && OtherComp != nullptr && OtherActor != this)
	{
		if (OtherActor->IsA<AHorrorGameCharacter>()) // 접촉된 액터가 플레이어여야지만 발동
		{
			PlayerCharacter = nullptr;
			SetDoorCollision(false);
		}
	}
}
