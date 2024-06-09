// CopyrightNotice 2023 Sunggon Kim kimdave205@gmail.com. All Rights Reserved.

#include "Lamp_cpp.h"
#include "Components/PointLightComponent.h"
#include "Components/BoxComponent.h"
#include "HideAndSeek/HorrorGameCharacter.h"
#include "AI/Reaper_cpp.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ALamp_cpp::ALamp_cpp()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 메시들의 기본 설정을 해줌. (세세한 설정은 블루프린트 클래스에서 수행)
	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = RootComp;

	LightComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Light"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh>LightMesh(TEXT("/Game/Assets/Furniture/SM_BulkheadLamp"));
	if (LightMesh.Succeeded())
	{
		LightComp->SetStaticMesh(LightMesh.Object);
		LightComp->SetupAttachment(RootComp);
	}

	PointLight = CreateDefaultSubobject<UPointLightComponent>(TEXT("LightSource"));
	PointLight->SetupAttachment(RootComp);
	PointLight->SetRelativeLocation(FVector(0.f, 15.f, 0.f));
	PointLight->SetRelativeRotation(FRotator(0.f, 0.f, 0.f));
	PointLight->SourceLength = 200.f;
	PointLight->AttenuationRadius = 800.f;
	PointLight->SetIntensity(Intensity);

	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("CreatureDetect"));
	BoxCollision->SetupAttachment(RootComp);
	BoxCollision->SetBoxExtent(FVector(1200.f, 1200.f, 300.f));

	NearBoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("CreatureNearby"));
	NearBoxCollision->SetupAttachment(RootComp);
	NearBoxCollision->SetBoxExtent(FVector(300.f, 300.f, 300.f));

	BoxCollision->OnComponentBeginOverlap.AddDynamic(this, &ALamp_cpp::BoxOverlapBegin);
	BoxCollision->OnComponentEndOverlap.AddDynamic(this, &ALamp_cpp::BoxOverlapEnd);
	NearBoxCollision->OnComponentBeginOverlap.AddDynamic(this, &ALamp_cpp::NearBoxOverlapBegin);
	NearBoxCollision->OnComponentEndOverlap.AddDynamic(this, &ALamp_cpp::NearBoxOverlapEnd);

	bIsLightOn = false;
	bIsPlayerIn = false;
	bIsNaturalFlick = false;
	bIsLightBlink = false;
	CreatureNum = 0;
	CreatureNearNum = 0;
}

// Called when the game starts or when spawned
void ALamp_cpp::BeginPlay()
{
	Super::BeginPlay();
	PointLight->SetVisibility(bIsLightOn);

	// 자연적으로 깜빡이는 경우
	if (bIsNaturalFlick)
	{
		// 타임 라인 커브가 있을 경우, 타임라인에 함수를 바인딩하고 재생함.
		if (NaturalCurveFloat)
		{
			FOnTimelineFloat TimelineProgress;
			TimelineProgress.BindDynamic(this, &ALamp_cpp::LightFlicker);
			FlickeringLight.AddInterpFloat(NaturalCurveFloat, TimelineProgress);
			FlickeringLight.SetLooping(true);
			FlickeringLight.Play();
		}
	}
	// 자연적으로 깜빡이는 경우가 아닌 경우
	else
	{
		// 타임라인 커브가 있을 경우, 타임라인에 함수를 바인딩함.
		if (CurveFloat)
		{
			FOnTimelineFloat TimelineProgress;
			TimelineProgress.BindDynamic(this, &ALamp_cpp::LightFlicker);
			FlickeringLight.AddInterpFloat(CurveFloat, TimelineProgress);
			FlickeringLight.SetLooping(true);
		}
	}
}

// Called every frame
void ALamp_cpp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FlickeringLight.TickTimeline(DeltaTime);
}

// 플레이어가 상호작용할 때 작동할 함수.
void ALamp_cpp::OnInteract()
{
	// 불을 켠 상태가 아닐 경우, 불을 켜는 로직을 수행함.
	if (!bIsLightOn)
	{
		bIsLightOn = true;
		PointLight->SetVisibility(true);
	}
}

// 충돌체에 충돌이 감지될 때 작동할 함수.
void ALamp_cpp::BoxOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherIndexBody, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor != nullptr && OtherActor != this && OtherComp != nullptr)
	{
		// 충돌한 액터가 플레이어일 경우
		if (OtherActor->IsA<AHorrorGameCharacter>())
		{
			// 플레이어가 감지되었다고 설정하고
			bIsPlayerIn = true;

			// 불이 자연적으로 깜빡이지 않고, 요괴가 근처에 있어 깜빡이는 경우, 타임라인을 재생함.
			if (bIsLightBlink)
			{
				if (CreatureNearNum > 0 && !bIsNaturalFlick)
				{
					FlickeringLight.Play();
				}
			}
		}
		// 충돌한 액터가 리퍼일 경우
		if (OtherActor->IsA<AReaper_cpp>())
		{
			// 근처 요괴의 수를 증가시키고
			CreatureNum++;
			
			// 불이 자연적으로 깜빡이지 않고, 요괴가 근처에 있어 깜빡이는 경우, 플레이어가 근처에 있으면 타임라인을 재생함.
			if (!bIsLightBlink)
			{
				bIsLightBlink = true;
				if (!bIsLightBlink && !bIsNaturalFlick && bIsPlayerIn)
				{
					FlickeringLight.Play();
				}
			}
		}
	}
}

// 충돌체에 충돌이 끝날 때 작동할 함수.
void ALamp_cpp::BoxOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherIndexBody)
{
	if (OtherActor != nullptr && OtherActor != this && OtherComp != nullptr)
	{
		// 충돌이 끝난 액터가 플레이어일 경우
		if (auto Player = Cast<AHorrorGameCharacter>(OtherActor))
		{
			// 플레이어가 숨은 상태가 아닌 경우, 플레이어가 근처에 없다고 설정함.
			if (!Player->bIsHiding)
			{
				bIsPlayerIn = false;
			}

			// 자연적으로 깜빡이지 않는 경우
			if (!bIsNaturalFlick)
			{
				// 타임라인을 정지하고 포인트 라이트의 세기를 기본으로 설정함.
				FlickeringLight.Stop();

				if (bIsLightOn)
				{
					PointLight->SetIntensity(Intensity);
				}
			}
		}
		// 충돌이 끝난 액터가 리퍼일 경우
		if (OtherActor->IsA<AReaper_cpp>())
		{
			// 근처 요괴의 수를 줄임.
			CreatureNum--;

			// 근처 요괴 수가 0 이하일 경우
			if (CreatureNum <= 0)
			{
				// 불 깜빡이는 것을 멈추게 함.
				bIsLightBlink = false;

				// 자연적으로 깜빡이지 않는 경우
				if (!bIsNaturalFlick)
				{
					// 타임라인을 정지하고 포인트 라이트의 세기를 기본으로 설정함.
					FlickeringLight.Stop();
					if (bIsLightOn)
					{
						PointLight->SetIntensity(Intensity);
					}
				}
			}
		}
	}
}

// 구형 충돌체에 충돌이 감지되면 작동할 함수.
void ALamp_cpp::NearBoxOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherIndexBody, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor != nullptr && OtherActor != this && OtherComp != nullptr)
	{
		// 충돌한 액터가 리퍼일 경우
		if (OtherActor->IsA<AReaper_cpp>())
		{
			// 플레이어가 근처에 있으면서 불이 켜진 상태면
			if (bIsPlayerIn && bIsLightOn)
			{
				// 근처 요괴 수를 증가시키고, 불이 자연적으로 깜빡이지 않는 경우 타임라인을 정지하고, 밝기 세기를 0으로 설정함.
				CreatureNearNum++;
				if (!bIsNaturalFlick)
				{
					FlickeringLight.Stop();
					PointLight->SetIntensity(0.f);
				}
			}
		}
	}
}

// 구형 충돌체에 충돌이 끝나면 작동할 함수.
void ALamp_cpp::NearBoxOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherIndexBody)
{
	if (OtherActor != nullptr && OtherActor != this && OtherComp != nullptr)
	{
		// 충돌이 끝난 액터가 리퍼일 경우
		if (OtherActor->IsA<AReaper_cpp>())
		{
			// 플레이어가 근처에 있으면서 불이 켜진 상태면
			if (bIsPlayerIn && bIsLightOn)
			{
				// 근처 요괴 수를 감소시키고, 불이 자연적으로 깜빡이지 않는 경우 타임라인을 재생하고, 밝기 세기를 기본 세기로 설정함.
				CreatureNearNum--;
				if (CreatureNearNum <= 0)
				{
					if (!bIsNaturalFlick)
					{
						PointLight->SetIntensity(Intensity);
						FlickeringLight.Play();
					}
				}
			}
		}
	}
}

// 타임 라인이 재생될 때 호출할 콜백 함수.
void ALamp_cpp::LightFlicker(float value)
{
	// 밝기의 범위를 0에서 원래 밝기 사이로 설정하고, 밝기를 조절함.
	float LightIntensity = FMath::Clamp(Intensity * value, 0.0f, Intensity);
	/*float LightIntensity = Intensity * value;

	if (LightIntensity < 0.f)
	{
		LightIntensity = 0.f;
	}
	else if (LightIntensity > Intensity)
	{
		LightIntensity = Intensity;
	}*/

	PointLight->SetIntensity(LightIntensity);
}