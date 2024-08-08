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
	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = RootComp;

	LightComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Light"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh>LightMesh(TEXT("/Game/Assets/Furniture/HangingLight"));
	if (LightMesh.Succeeded())
	{
		LightComp->SetStaticMesh(LightMesh.Object);
		LightComp->SetupAttachment(RootComp);
		LightComp->SetRelativeLocation(FVector(0.f, 0.f, 300.f));
	}

	PointLight = CreateDefaultSubobject<UPointLightComponent>(TEXT("LightSource"));
	PointLight->SetupAttachment(RootComp);
	PointLight->SetRelativeLocation(FVector(0.f, 0.f, 280.f));
	PointLight->SetRelativeRotation(FRotator(90.f, 0.f, 0.f));
	PointLight->SourceLength = 200.f;
	PointLight->AttenuationRadius = 800.f;
	PointLight->SetIntensity(Intensity);
	
	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("CreatureDetect"));
	BoxCollision->SetupAttachment(RootComp);
	BoxCollision->SetBoxExtent(FVector(1200.f, 1200.f, 300.f));

	SphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("CreatureNearby"));
	SphereCollision->SetupAttachment(RootComp);
	SphereCollision->SetSphereRadius(250.f);

	BoxCollision->OnComponentBeginOverlap.AddDynamic(this, &AHangingLight::BoxOverlapBegin);
	BoxCollision->OnComponentEndOverlap.AddDynamic(this, &AHangingLight::BoxOverlapEnd);
	SphereCollision->OnComponentBeginOverlap.AddDynamic(this, &AHangingLight::SphereOverlapBegin);
	SphereCollision->OnComponentEndOverlap.AddDynamic(this, &AHangingLight::SphereOverlapEnd);
	
	bIsLightOn = false;
	bIsPlayerIn = false;
	CreatureNearNum = 0;
	CreatureNum = 0;
}

// Called when the game starts or when spawned
void AHangingLight::BeginPlay()
{
	Super::BeginPlay();
	PointLight->SetVisibility(bIsLightOn);
	
	// 타임라인 커브 값이 있다면 타임라인에 할당하고, 재생될 때 실행할 콜백 함수도 바인딩함.
	if (CurveFloat)
	{
		FOnTimelineFloat TimelineProgress;
		TimelineProgress.BindDynamic(this, &AHangingLight::LightFlicker);
		FlickeringLight.AddInterpFloat(CurveFloat, TimelineProgress);
		FlickeringLight.SetLooping(true);
	}
}

// Called every frame
void AHangingLight::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FlickeringLight.TickTimeline(DeltaTime);
}

// 플레이어가 상호작용할 때 작동할 함수.
void AHangingLight::OnInteract()
{
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
void AHangingLight::BoxOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherIndexBody, bool bFromSweep, const FHitResult& SweepResult)
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
				if (CreatureNearNum <= 0)
				{
					FlickeringLight.Play();
				}
			}
		}
		// 충돌한 액터가 리퍼라면
		else if (AReaper_cpp* Creature = Cast<AReaper_cpp>(OtherActor))
		{
			// 근처 적 개체의 수를 증가시키고
			CreatureNum++;
			// 깜빡 거리지 않게 되어있다면
			if (!bIsLightBlink)
			{
				// 깜빡이게 설정한 후
				bIsLightBlink = true;
				// 근처 플레이어가 존재하면 깜빡이게 하는 타임라인을 재생함.
				if (CreatureNearNum <= 0 && bIsPlayerIn)
				{
					FlickeringLight.Play();
				}
			}
		}
	}
}

// 충돌체에 충돌이 끝났을 때 작동할 함수.
void AHangingLight::BoxOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherIndexBody)
{
	if (OtherActor != nullptr && OtherActor != this && OtherComp != nullptr)
	{
		// 충돌한 액터가 플레이어라면
		if (auto Player = Cast<AHorrorGameCharacter>(OtherActor))
		{
			// 플레이어가 숨은 상태가 아니면, 플레이어가 근처에 없다고 설정함.
			if (!Player->bIsHiding)
			{
				bIsPlayerIn = false;
			}

			// 깜빡이게 하는 타임라인도 멈추게 하고, 밝기도 원래의 밝기로 설정함.
			FlickeringLight.Stop();
			if (bIsLightOn)
			{
				PointLight->SetIntensity(Intensity);
			}
		}
		// 충돌한 액터가 리퍼라면
		else if (AReaper_cpp* Creature = Cast<AReaper_cpp>(OtherActor))
		{
			// 근처 적 개체의 개수 카운트를 줄이고
			CreatureNum--;

			// 근처에 적 개체가 없다면
			if (CreatureNum <= 0)
			{
				// 깜빡임을 멈추게 하고 밝기도 원래의 밝기로 설정함.
				bIsLightBlink = false;
				FlickeringLight.Stop();
				PointLight->SetIntensity(Intensity);
			}
		}
	}
}

// 구 형태의 충돌체에 적 개체가 충돌할 때 작동할 함수.
void AHangingLight::SphereOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherIndexBody, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor != nullptr && OtherActor != this && OtherComp != nullptr)
	{
		if (OtherActor->IsA<AReaper_cpp>())
		{
			// 근처에 플레이어가 있고, 불이 켜진 상태면
			if (bIsPlayerIn && bIsLightOn)
			{
				// 깜빡임을 멈추게 하고, 밝기를 0으로 설정함.
				CreatureNearNum++;
				FlickeringLight.Stop();
				PointLight->SetIntensity(0.f);
			}
		}
	}
}

// 구 형태의 충돌체에 적 개체가 충돌이 끝날 때 작동할 함수.
void AHangingLight::SphereOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherIndexBody)
{
	if (OtherActor != nullptr && OtherActor != this && OtherComp != nullptr)
	{
		if (OtherActor->IsA<AReaper_cpp>())
		{
			// 근처에 플레이어가 있고, 불이 켜진 상태면
			if (bIsPlayerIn && bIsLightOn)
			{
				CreatureNearNum--;
				// 깜빡임을 시작하고, 밝기를 원래의 밝기로 설정함.
				if (CreatureNearNum <= 0)
				{
					PointLight->SetIntensity(Intensity);
					FlickeringLight.Play();
				}
			}
		}
	}
}

// 타임라인이 작동할 때 호출될 콜백 함수.
void AHangingLight::LightFlicker(float value)
{
	// 밝기의 범위를 0에서 원래 밝기 사이로 설정하고, 밝기를 조절함.
	float LightIntense = FMath::Clamp(Intensity * value, 0.0f, Intensity);
	
	PointLight->SetIntensity(LightIntense);

	/* DEPRECATED
	if (LightIntense < 0.f)
	{
		PointLight->SetIntensity(0.0f);
	}
	else if (LightIntense > Intensity)
	{
		PointLight->SetIntensity(Intensity);
	}
	else
	{
		PointLight->SetIntensity(LightIntense);
	}
	*/
}