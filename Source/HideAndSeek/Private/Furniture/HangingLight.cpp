// CopyrightNotice 2023 Sunggon Kim kimdave205@gmail.com. All Rights Reserved.

#include "Furniture/HangingLight.h"
#include "Components/PointLightComponent.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "Player/HorrorGameCharacter.h"
#include "AI/Reaper_cpp.h"
#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AHangingLight::AHangingLight()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 메시들의 기본 설정을 해줌. (세세한 설정은 블루프린트 클래스에서 수행)
	static ConstructorHelpers::FObjectFinder<UStaticMesh>SM_Light(TEXT("/Game/Assets/Furniture/HangingLight"));
	if (SM_Light.Succeeded())
	{
		LightMesh->SetStaticMesh(SM_Light.Object);
		LightMesh->SetupAttachment(RootComp);
		LightMesh->SetRelativeLocation(FVector(0.f, 0.f, 300.f));
	}

	PointLight->SetRelativeLocation(FVector(0.f, 0.f, 280.f));
	PointLight->SetRelativeRotation(FRotator(90.f, 0.f, 0.f));
	PointLight->SourceLength = 200.f;
	PointLight->AttenuationRadius = 800.f;
	PointLight->SetIntensity(Intensity);
}

// Called when the game starts or when spawned
void AHangingLight::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AHangingLight::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// 타임라인이 작동할 때 호출될 콜백 함수.
void AHangingLight::LightFlicker(float value)
{
	// 밝기의 범위를 0에서 원래 밝기 사이로 설정하고, 밝기를 조절함.
	float LightIntense = FMath::Clamp(Intensity * value, 0.0f, Intensity);
	
	PointLight->SetIntensity(LightIntense);
}