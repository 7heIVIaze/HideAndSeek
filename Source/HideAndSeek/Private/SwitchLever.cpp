// CopyrightNotice 2023 Sunggon Kim kimdave205@gmail.com. All Rights Reserved.

#include "SwitchLever.h"
#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ASwitchLever::ASwitchLever()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 메시들의 기본 설정을 해줌. (세세한 설정은 블루프린트 클래스에서 수행)
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
	
	// 타임라인 커브 값이 있다면 타임라인에 할당하고, 재생될 때 실행할 콜백 함수도 바인딩함.
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

// 플레이어가 상호작용할 때 작동할 함수.
void ASwitchLever::OnInteract(class AHorrorGameCharacter* Player)
{
	// 레버 효과음을 재생함.
	if (LeverSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, LeverSound, GetActorLocation());
	}
	
	// 만약 레버가 내려가있는 상태라면 레버를 올리고(타임라인 역재생) 작동해제
	if (bIsLeverOn)
	{
		UpAndDown.Reverse();
	}
	// 레버가 올라가 있는 상태면 레버를 내리고(타임라인 재생) 작동
	else 
	{
		UpAndDown.Play();
	}

	// 레버의 상태값을 반전시킴.
	bIsLeverOn = !bIsLeverOn;
}

// 타임라인 실행 시 호출될 콜백 함수.
void ASwitchLever::ChangeLeverRotation(float Value)
{
	// 레버가 당겨지고 밀어지는 효과를 줌.
	FRotator Rotator(0.f, 0.f, LeverRotateAngle * Value);
	Lever->SetRelativeRotation(Rotator);
}
