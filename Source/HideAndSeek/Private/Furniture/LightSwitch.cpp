// CopyrightNotice 2023 Sunggon Kim kimdave205@gmail.com. All Rights Reserved.

#include "Furniture/LightSwitch.h"
#include "Furniture/SchoolLight.h"
#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ALightSwitch::ALightSwitch()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// 메시들의 기본 설정을 해줌. (세세한 설정은 블루프린트 클래스에서 수행)
	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = RootComp;

	MainMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SwitchFrame"));
	MainMesh->SetupAttachment(RootComp);
	static ConstructorHelpers::FObjectFinder<UStaticMesh>MainObj(TEXT("/Game/Assets/Furniture/LightSwitch_Main"));
	if (MainObj.Succeeded())
	{
		MainMesh->SetStaticMesh(MainObj.Object);
	}

	SwitchMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Switch"));
	SwitchMesh->SetupAttachment(MainMesh);
	static ConstructorHelpers::FObjectFinder<UStaticMesh>SwitchObj(TEXT("/Game/Assets/Furniture/LightSwitch_Switch"));
	if (SwitchObj.Succeeded())
	{
		SwitchMesh->SetStaticMesh(SwitchObj.Object);
	}

	bIsSwitchOn = false;
}

// Called when the game starts or when spawned
void ALightSwitch::BeginPlay()
{
	Super::BeginPlay();
	
}

// 플레이어가 상호작용할 때 작동할 함수.
void ALightSwitch::OnInteract()
{
	// 스위치 음을 재생하고, 스위치 상태를 설정함.
	if (LightSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, LightSound, GetActorLocation());
	}
	SetSwitchStatus();
}

// 스위치 상태를 설정할 함수.
void ALightSwitch::SetSwitchStatus()
{
	// 스위치 상태 값을 반전시킴.
	bIsSwitchOn = !bIsSwitchOn;

	// 켜져있으면 기본 회전값으로 스위치 버튼 메시를 회전시킴.
	if (bIsSwitchOn)
	{
		SwitchMesh->SetRelativeRotation(FRotator(0.f, 0.f, 0.f));
	}
	// 꺼져있으면 스위치 버즌 메시를 Yaw(Z축)를 기준으로 10 회전시킴.
	else
	{	
		SwitchMesh->SetRelativeRotation(FRotator(0.f, 10.f, 0.f));
	}
	SetLightsStatus();
}

// 전등 상태를 설정하는 함수.
void ALightSwitch::SetLightsStatus()
{
	// 블루프린트를 통해 할당한 전등을 하나씩 순찰해 상태를 업데이트함.
	for (auto Light : Lights)
	{
		Cast<ASchoolLight>(Light)->SetLightStatus();
	}
}