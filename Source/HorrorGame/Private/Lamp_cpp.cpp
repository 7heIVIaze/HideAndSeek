// CopyrightNotice=0 2023 Sunggon Kim

#include "Lamp_cpp.h"
#include "Components/PointLightComponent.h"
#include "Components/BoxComponent.h"
#include "HorrorGame/HorrorGameCharacter.h"
#include "AI/Reaper_cpp.h"
#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ALamp_cpp::ALamp_cpp()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

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

	if (bIsNaturalFlick)
	{
		if (NaturalCurveFloat)
		{
			FOnTimelineFloat TimelineProgress;
			TimelineProgress.BindDynamic(this, &ALamp_cpp::LightFlicker);
			FlickeringLight.AddInterpFloat(NaturalCurveFloat, TimelineProgress);
			FlickeringLight.SetLooping(true);
			FlickeringLight.Play();
		}
	}
	else
	{
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

void ALamp_cpp::OnInteract()
{
	if (!bIsLightOn)
	{
		bIsLightOn = true;
		// USoundCue* LightSound
		PointLight->SetVisibility(true);
	}
}

void ALamp_cpp::BoxOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherIndexBody, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor != nullptr && OtherActor != this && OtherComp != nullptr)
	{
		if (OtherActor->IsA<AHorrorGameCharacter>())
		{
			bIsPlayerIn = true;
			if (bIsLightBlink)
			{
				if(CreatureNearNum > 0 && !bIsNaturalFlick)
					FlickeringLight.Play();
			}
		}

		if (OtherActor->IsA<AReaper_cpp>())
		{
			CreatureNum++;
			if (!bIsLightBlink)
			{
				bIsLightBlink = true;
				if (!bIsLightBlink && !bIsNaturalFlick && bIsPlayerIn) 
					FlickeringLight.Play();
			}
		}
	}
}

void ALamp_cpp::BoxOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherIndexBody)
{
	if (OtherActor != nullptr && OtherActor != this && OtherComp != nullptr)
	{
		if (auto Player = Cast<AHorrorGameCharacter>(OtherActor))
		{
			if (!Player->bIsHiding)
				bIsPlayerIn = false;
			if (!bIsNaturalFlick)
			{
				FlickeringLight.Stop();
				if (bIsLightOn)
					PointLight->SetIntensity(Intensity);
			}
		}

		if (OtherActor->IsA<AReaper_cpp>())
		{
			CreatureNum--;
			if (CreatureNum <= 0)
			{
				bIsLightBlink = false;
				if (!bIsNaturalFlick)
				{
					FlickeringLight.Stop();
					if (bIsLightOn)
						PointLight->SetIntensity(Intensity);
				}
			}
		}
	}
}

void ALamp_cpp::NearBoxOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherIndexBody, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor != nullptr && OtherActor != this && OtherComp != nullptr)
	{
		if (OtherActor->IsA<AReaper_cpp>())
		{
			if (bIsPlayerIn && bIsLightOn)
			{
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

void ALamp_cpp::NearBoxOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherIndexBody)
{
	if (OtherActor != nullptr && OtherActor != this && OtherComp != nullptr)
	{
		if (OtherActor->IsA<AReaper_cpp>())
		{
			if (bIsPlayerIn && bIsLightOn)
			{
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

void ALamp_cpp::LightFlicker(float value)
{
	float LightIntensity = Intensity * value;

	if (LightIntensity < 0.f)
	{
		LightIntensity = 0.f;
	}
	else if (LightIntensity > Intensity)
	{
		LightIntensity = Intensity;
	}

	PointLight->SetIntensity(LightIntensity);
}