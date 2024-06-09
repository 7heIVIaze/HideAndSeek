// CopyrightNotice 2023 Sunggon Kim kimdave205@gmail.com. All Rights Reserved.

#include "SchoolLight.h"
#include "Components/PointLightComponent.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "HideAndSeek/HorrorGameCharacter.h"
#include "Cabinet_cpp.h"
#include "Wardrobe_cpp.h"
#include "AI/CreatureClass.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ASchoolLight::ASchoolLight()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 메시들의 기본 설정을 해줌. (세세한 설정은 블루프린트 클래스에서 수행)
	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = RootComp;

	LightMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SchoolLight"));
	LightMesh->SetupAttachment(RootComp);
	static ConstructorHelpers::FObjectFinder<UStaticMesh>LightObj(TEXT("/Game/Assets/Furniture/CeilingLight2"));
	if (LightObj.Succeeded())
	{
		LightMesh->SetStaticMesh(LightObj.Object);
	}

	LightComp = CreateDefaultSubobject<UPointLightComponent>(TEXT("Light"));
	LightComp->SetupAttachment(LightMesh);
	LightComp->SetIntensity(Intensity);

	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("CreatureDetect"));
	BoxCollision->SetupAttachment(RootComp);
	BoxCollision->SetBoxExtent(FVector(1200.f, 1200.f, 300.f));

	SphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("CreatureNearby"));
	SphereCollision->SetupAttachment(RootComp);
	SphereCollision->SetSphereRadius(250.f);

	BoxCollision->OnComponentBeginOverlap.AddDynamic(this, &ASchoolLight::BoxOverlapBegin);
	BoxCollision->OnComponentEndOverlap.AddDynamic(this, &ASchoolLight::BoxOverlapEnd);
	SphereCollision->OnComponentBeginOverlap.AddDynamic(this, &ASchoolLight::SphereOverlapBegin);
	SphereCollision->OnComponentEndOverlap.AddDynamic(this, &ASchoolLight::SphereOverlapEnd);

	bIsLightOn = false;
	bIsPlayerIn = false;
	CreatureNearNum = 0;
	CreatureNum = 0;
}

// Called when the game starts or when spawned
void ASchoolLight::BeginPlay()
{
	Super::BeginPlay();
	LightComp->SetVisibility(bIsLightOn);
	
	// 타임라인 커브 값이 있다면 타임라인에 할당하고, 재생될 때 실행할 콜백 함수도 바인딩함.
	if (CurveFloat)
	{
		FOnTimelineFloat TimelineProgress;
		TimelineProgress.BindDynamic(this, &ASchoolLight::LightFlicker);
		FlickeringLight.AddInterpFloat(CurveFloat, TimelineProgress);
		FlickeringLight.SetLooping(true);
	}
}

// Called every frame
void ASchoolLight::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FlickeringLight.TickTimeline(DeltaTime);
}

// 전등의 상태를 설정할 함수.
void ASchoolLight::SetLightStatus()
{
	// 상태 값을 반전시켜, 라이트 컴포넌트에도 설정해줌.
	bIsLightOn = !bIsLightOn;
	
	LightComp->SetVisibility(bIsLightOn);
}

// 충돌체에 충돌이 감지될 때 작동할 함수.
void ASchoolLight::BoxOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherIndexBody, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor != nullptr && OtherActor != this && OtherComp != nullptr)
	{
		// 충돌한 액터가 플레이어일 경우
		if (OtherActor->IsA<AHorrorGameCharacter>())
		{
			// 플레이어가 감지되었다고 설정하고
			bIsPlayerIn = true;
			
			// 요괴가 근처에 있어 깜빡이는 경우, 타임라인을 재생함.
			if (bIsLightBlink)
			{
				if (CreatureNearNum <= 0)
				{
					FlickeringLight.Play();
				}
			}
		}
		// 충돌한 액터가 요괴 류일 경우
		else if (OtherActor->IsA<ACreatureClass>())
		{
			// 근처 요괴의 수를 증가시키고
			CreatureNum++;
			
			//  요괴가 근처에 있어 깜빡이지 않는 경우, 플레이어가 근처에 있으면 타임라인을 재생함.
			if (!bIsLightBlink)
			{
				bIsLightBlink = true;
				if (CreatureNearNum <= 0 && bIsPlayerIn)
				{
					FlickeringLight.Play();
				}
			}
		}
	}
}

// 충돌체에 충돌이 끝날 때 작동할 함수.
void ASchoolLight::BoxOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherIndexBody)
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

			// 타임라인을 정지하고 포인트 라이트의 세기를 기본으로 설정함.
			FlickeringLight.Stop();
			
			if (bIsLightOn)
			{
				LightComp->SetIntensity(Intensity);
			}
		}
		// 충돌이 끝난 액터가 요괴 류인 경우
		else if (OtherActor->IsA<ACreatureClass>())
		{
			// 근처 요괴의 수를 줄임.
			CreatureNum--;
			
			// 근처 요괴 수가 0 이하일 경우
			if (CreatureNum <= 0)
			{
				// 타임라인을 정지하고 포인트 라이트의 세기를 기본으로 설정함.
				bIsLightBlink = false;
				FlickeringLight.Stop();
				LightComp->SetIntensity(Intensity);
			}
		}
	}
}

// 구형 충돌체에 충돌이 감지되면 작동할 함수.
void ASchoolLight::SphereOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherIndexBody, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor != nullptr && OtherActor != this && OtherComp != nullptr)
	{
		// 충돌한 액터가 요괴 류인 경우
		if (OtherActor->IsA<ACreatureClass>())
		{
			// 플레이어가 근처에 있으면서 불이 켜진 상태면
			if (bIsPlayerIn && bIsLightOn)
			{
				// 근처 요괴 수를 증가시키고, 타임라인을 정지하고, 밝기 세기를 0으로 설정함.
				CreatureNearNum++;
				FlickeringLight.Stop();
				LightComp->SetIntensity(0.f);
			}
		}
	}
}

// 구형 충돌체에 충돌이 끝나면 작동할 함수.
void ASchoolLight::SphereOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherIndexBody)
{
	if (OtherActor != nullptr && OtherActor != this && OtherComp != nullptr)
	{
		// 충돌이 끝난 액터가 요괴 류인 경우
		if (OtherActor->IsA<ACreatureClass>())
		{
			// 플레이어가 근처에 있으면서 불이 켜진 상태면
			if (bIsPlayerIn && bIsLightOn)
			{
				// 근처 요괴 수를 감소시키고, 타임라인을 재생하고, 밝기 세기를 기본 세기로 설정함.
				CreatureNearNum--;
				if (CreatureNearNum <= 0)
				{
					LightComp->SetIntensity(Intensity);
					FlickeringLight.Play();
				}
			}
		}
	}
}

// 타임 라인이 재생될 때 호출할 콜백 함수.
void ASchoolLight::LightFlicker(float value)
{
	// 밝기의 범위를 0에서 원래 밝기 사이로 설정하고, 밝기를 조절함.
	float LightIntense = FMath::Clamp(Intensity * value, 0.0f, Intensity);

	LightComp->SetIntensity(LightIntense);
	/*if (LightIntense < 0.f)
	{
		LightComp->SetIntensity(0.0f);
	}
	else if (LightIntense > Intensity)
	{
		LightComp->SetIntensity(Intensity);
	}
	else
	{
		LightComp->SetIntensity(LightIntense);
	}*/
}
