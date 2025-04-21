// CopyrightNotice 2023 Sunggon Kim kimdave205@gmail.com. All Rights Reserved.

#include "Furniture/SchoolLight.h"
#include "Components/PointLightComponent.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "Player/HorrorGameCharacter.h"
#include "AI/CreatureClass.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ASchoolLight::ASchoolLight()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 메시들의 기본 설정을 해줌. (세세한 설정은 블루프린트 클래스에서 수행)
	static ConstructorHelpers::FObjectFinder<UStaticMesh>LightObj(TEXT("/Game/Assets/Furniture/CeilingLight2"));
	if (LightObj.Succeeded())
	{
		LightMesh->SetStaticMesh(LightObj.Object);
	}

	PointLight->SetIntensity(Intensity);
}

// Called when the game starts or when spawned
void ASchoolLight::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ASchoolLight::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// 전등의 상태를 설정할 함수.
void ASchoolLight::SetLightStatus()
{
	// 상태 값을 반전시켜, 라이트 컴포넌트에도 설정해줌.
	bIsLightOn = !bIsLightOn;
	
	PointLight->SetVisibility(bIsLightOn);
}

// 타임 라인이 재생될 때 호출할 콜백 함수.
void ASchoolLight::LightFlicker(float value)
{
	// 밝기의 범위를 0에서 원래 밝기 사이로 설정하고, 밝기를 조절함.
	float LightIntense = FMath::Clamp(Intensity * value, 0.0f, Intensity);

	PointLight->SetIntensity(LightIntense);
}
