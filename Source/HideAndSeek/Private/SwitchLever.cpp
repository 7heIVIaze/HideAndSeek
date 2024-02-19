// CopyrightNotice=0 2023 Sunggon Kim kimdave205@gmail.com

#include "SwitchLever.h"
#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ASwitchLever::ASwitchLever()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = RootComp;

	LeverBody = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LeverBody"));
	LeverBody->SetupAttachment(RootComp);
	static ConstructorHelpers::FObjectFinder<UStaticMesh>SM_Lever(TEXT("/Game/Assets/Furniture/SM_Lever"));
	if (SM_Lever.Succeeded())
	{
		LeverBody->SetStaticMesh(SM_Lever.Object);
	}

	Lever = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Lever"));
	Lever->SetupAttachment(RootComp);
	static ConstructorHelpers::FObjectFinder<UStaticMesh>SM_Handle(TEXT("/Game/Assets/Furniture/SM_Handle"));
	if (SM_Handle.Succeeded())
	{
		Lever->SetStaticMesh(SM_Handle.Object);
	}

	LeverRotateAngle = 80.f;
	bIsLeverOn = false;
}

// Called when the game starts or when spawned
void ASwitchLever::BeginPlay()
{
	Super::BeginPlay();
	
	if (CurveFloat)
	{
		FOnTimelineFloat TimelineProgress;
		TimelineProgress.BindDynamic(this, &ASwitchLever::ChangeLeverRotation);
		UpAndDown.AddInterpFloat(CurveFloat, TimelineProgress);
	}
}

// Called every frame
void ASwitchLever::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpAndDown.TickTimeline(DeltaTime);
}

void ASwitchLever::OnInteract(class AHorrorGameCharacter* Player)
{
	if (LeverSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, LeverSound, GetActorLocation());
	}
	if (bIsLeverOn) // 만약 레버가 내려가있는 상태라면 레버를 올리고 작동해제
	{
		UpAndDown.Reverse();
	}
	else // 레버가 올라가 있는 상태면 레버를 내리고 작동
	{
		UpAndDown.Play();
	}
	bIsLeverOn = !bIsLeverOn;
}

void ASwitchLever::ChangeLeverRotation(float Value)
{
	FRotator Rotator(0.f, 0.f, LeverRotateAngle * Value);
	Lever->SetRelativeRotation(Rotator);
}
