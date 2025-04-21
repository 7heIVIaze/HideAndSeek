// CopyrightNotice 2023 Sunggon Kim kimdave205@gmail.com. All Rights Reserved.

#include "Furniture/Lamp_cpp.h"
#include "Components/PointLightComponent.h"
#include "Components/BoxComponent.h"
#include "Player/HorrorGameCharacter.h"
#include "AI/Reaper_cpp.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ALamp_cpp::ALamp_cpp()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 메시들의 기본 설정을 해줌. (세세한 설정은 블루프린트 클래스에서 수행)
	static ConstructorHelpers::FObjectFinder<UStaticMesh>SM_Light(TEXT("/Game/Assets/Furniture/SM_BulkheadLamp"));
	if (SM_Light.Succeeded())
	{
		LightMesh->SetStaticMesh(SM_Light.Object);
		LightMesh->SetupAttachment(RootComp);
	}

	PointLight->SetRelativeLocation(FVector(0.f, 15.f, 0.f));
	PointLight->SetRelativeRotation(FRotator(0.f, 0.f, 0.f));
	PointLight->SourceLength = 200.f;
	PointLight->AttenuationRadius = 800.f;
	
	bIsLightOn = false;
	bIsPlayerIn = false;
	bIsNaturalFlick = false;
	bIsLightBlink = false;
	YokaiNum = 0;
	YokaiNearNum = 0;
}

// Called when the game starts or when spawned
void ALamp_cpp::BeginPlay()
{
	Super::BeginPlay();

	// 자연적으로 깜빡이는 경우
	if (bIsNaturalFlick)
	{
		// 타임 라인 커브가 있을 경우, 타임라인에 함수를 바인딩하고 재생함.
		if (NaturalFlickeringCurveFloat)
		{
			FOnTimelineFloat TimelineProgress;
			TimelineProgress.BindDynamic(this, &ALamp_cpp::LightFlicker);
			FlickeringLightTimeline.AddInterpFloat(NaturalFlickeringCurveFloat, TimelineProgress);
			FlickeringLightTimeline.SetLooping(true);
			FlickeringLightTimeline.Play();
		}
	}
}

// Called every frame
void ALamp_cpp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// 타임 라인이 재생될 때 호출할 콜백 함수.
void ALamp_cpp::LightFlicker(float value)
{
	// 밝기의 범위를 0에서 원래 밝기 사이로 설정하고, 밝기를 조절함.
	float LightIntensity = FMath::Clamp(Intensity * value, 0.0f, Intensity);

	PointLight->SetIntensity(LightIntensity);
}