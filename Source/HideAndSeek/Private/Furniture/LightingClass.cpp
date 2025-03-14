// CopyrightNotice 2023 Sunggon Kim kimdave205@gmail.com. All Rights Reserved.

#include "Furniture/LightingClass.h"
#include "Components/PointLightComponent.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "Player/HorrorGameCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"
#include "AI/CreatureAI.h"
#include "AI/CreatureClass.h"

// Sets default values
ALightingClass::ALightingClass()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	// 불빛 깜빡임 등을 관리해야하므로 active시킴.
	PrimaryActorTick.bCanEverTick = true;

	// Root Component 설정용 컴포넌트임.
	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = RootComp;

	// 전등 메시임
	LightMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LightMesh"));
	LightMesh->SetupAttachment(RootComp);

	// 플레이어처럼 전등의 조명 컴포넌트
	PointLight = CreateDefaultSubobject<UPointLightComponent>(TEXT("PointLight"));
	PointLight->SetupAttachment(RootComp);
	PointLight->SetIntensity(Intensity);

	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("YokaiDetect"));
	BoxCollision->SetupAttachment(RootComp);
	BoxCollision->SetBoxExtent(FVector(1200.f, 1200.f, 300.f));

	SphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("YokaiNearby"));
	SphereCollision->SetupAttachment(RootComp);
	SphereCollision->SetSphereRadius(250.f);

	BoxCollision->OnComponentBeginOverlap.AddDynamic(this, &ALightingClass::BoxOverlapBegin);
	BoxCollision->OnComponentEndOverlap.AddDynamic(this, &ALightingClass::BoxOverlapEnd);
	SphereCollision->OnComponentBeginOverlap.AddDynamic(this, &ALightingClass::SphereOverlapBegin);
	SphereCollision->OnComponentEndOverlap.AddDynamic(this, &ALightingClass::SphereOverlapEnd);

	bIsLightOn = false;
	bIsPlayerIn = false;
	YokaiNearNum = 0;
	YokaiNum = 0;
}

// Called when the game starts or when spawned
void ALightingClass::BeginPlay()
{
	Super::BeginPlay();
	PointLight->SetVisibility(bIsLightOn);

	// 타임라인 커브 값이 있다면 타임라인에 할당하고, 재생될 때 실행할 콜백 함수도 바인딩함.
	if (FlickeringCurveFloat)
	{
		FOnTimelineFloat TimelineProgress;
		TimelineProgress.BindDynamic(this, &ALightingClass::LightFlicker);
		FlickeringLightTimeline.AddInterpFloat(FlickeringCurveFloat, TimelineProgress);
		FlickeringLightTimeline.SetLooping(true);
	}
}

// Called every frame
void ALightingClass::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FlickeringLightTimeline.TickTimeline(DeltaTime);
}

// 플레이어가 상호작용할 때 작동할 함수.
void ALightingClass::OnInteract(class AHorrorGameCharacter* Player)
{
	Super::OnInteract(Player);

	// 불이 켜져있지 않은 경우
	if (!bIsLightOn)
	{
		// 불을 켜졌다고 설정하고
		bIsLightOn = true;

		// 불이 켜지는 소리를 재생 함.
		if (LightSound)
		{
			UGameplayStatics::PlaySoundAtLocation(this, LightSound, GetActorLocation());
		}

		// 포인트 라이트 컴포넌트를 보이게 설정함.
		PointLight->SetVisibility(true);
	}
}

// 충돌체에 충돌했을 때 작동할 함수.
void ALightingClass::BoxOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherIndexBody, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor != nullptr && OtherActor != this && OtherComp != nullptr)
	{
		// 충돌한 액터가 플레이어라면
		if (OtherActor->IsA<AHorrorGameCharacter>())
		{
			// 플레이어가 근처에 있다고 설정하고
			bIsPlayerIn = true;

			// 불빛이 깜빡여야 한다면, 근처에 적 개체가 있을 때 깜빡이게 하는 타임라인을 재생함.
			if (bIsLightBlink)
			{
				if (YokaiNearNum <= 0)
				{
					FlickeringLightTimeline.Play();
				}
			}
		}
		// 충돌한 액터가 요괴라면
		else if (ACreatureClass* Yokai = Cast<ACreatureClass>(OtherActor))
		{
			// 근처 적 개체의 수를 증가시키고
			YokaiNum++;
			// 깜빡 거리지 않게 되어있다면
			if (!bIsLightBlink)
			{
				// 깜빡이게 설정한 후
				bIsLightBlink = true;
				// 근처 플레이어가 존재하면 깜빡이게 하는 타임라인을 재생함.
				if (YokaiNearNum <= 0 && bIsPlayerIn)
				{
					FlickeringLightTimeline.Play();
				}
			}
		}
	}
}

// 충돌체에 충돌이 끝났을 때 작동할 함수.
void ALightingClass::BoxOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherIndexBody)
{
	if (OtherActor != nullptr && OtherActor != this && OtherComp != nullptr)
	{
		// 충돌한 액터가 플레이어라면
		if (auto Player = Cast<AHorrorGameCharacter>(OtherActor))
		{
			// 플레이어가 숨은 상태가 아니면, 플레이어가 근처에 없다고 설정함.
			if (Player->GetPlayerStatus() != EPlayerStatus::Hiding)
			{
				bIsPlayerIn = false;
			}

			// 깜빡이게 하는 타임라인도 멈추게 하고, 밝기도 원래의 밝기로 설정함.
			FlickeringLightTimeline.Stop();
			if (bIsLightOn)
			{
				PointLight->SetIntensity(Intensity);
			}
		}
		// 충돌한 액터가 요괴라면
		else if (ACreatureClass* Yokai = Cast<ACreatureClass>(OtherActor))
		{
			// 근처 적 개체의 개수 카운트를 줄이고
			YokaiNum--;

			// 근처에 적 개체가 없다면
			if (YokaiNum <= 0)
			{
				// 깜빡임을 멈추게 하고 밝기도 원래의 밝기로 설정함.
				bIsLightBlink = false;
				FlickeringLightTimeline.Stop();
				PointLight->SetIntensity(Intensity);
			}
		}
	}
}

// 구 형태의 충돌체에 적 개체가 충돌할 때 작동할 함수.
void ALightingClass::SphereOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherIndexBody, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor != nullptr && OtherActor != this && OtherComp != nullptr)
	{
		if (OtherActor->IsA<ACreatureClass>())
		{
			// 근처에 플레이어가 있고, 불이 켜진 상태면
			if (bIsPlayerIn && bIsLightOn)
			{
				// 깜빡임을 멈추게 하고, 밝기를 0으로 설정함.
				YokaiNearNum++;
				FlickeringLightTimeline.Stop();
				PointLight->SetIntensity(0.f);
			}
		}
	}
}

// 구 형태의 충돌체에 적 개체가 충돌이 끝날 때 작동할 함수.
void ALightingClass::SphereOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherIndexBody)
{
	if (OtherActor != nullptr && OtherActor != this && OtherComp != nullptr)
	{
		if (OtherActor->IsA<ACreatureClass>())
		{
			// 근처에 플레이어가 있고, 불이 켜진 상태면
			if (bIsPlayerIn && bIsLightOn)
			{
				YokaiNearNum--;
				// 깜빡임을 시작하고, 밝기를 원래의 밝기로 설정함.
				if (YokaiNearNum <= 0)
				{
					PointLight->SetIntensity(Intensity);
					FlickeringLightTimeline.Play();
				}
			}
		}
	}
}

// 타임라인이 작동할 때 호출될 콜백 함수.
void ALightingClass::LightFlicker(float value)
{
	// 밝기의 범위를 0에서 원래 밝기 사이로 설정하고, 밝기를 조절함.
	float LightIntense = FMath::Clamp(Intensity * value, 0.0f, Intensity);

	PointLight->SetIntensity(LightIntense);
}