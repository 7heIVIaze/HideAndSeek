// CopyrightNotice=0 2023 Sunggon Kim kimdave205@gmail.com

#include "HangingLight.h"
#include "Components/PointLightComponent.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "HideAndSeek/HorrorGameCharacter.h"
#include "Cabinet_cpp.h"
#include "Wardrobe_cpp.h"
#include "AI/Reaper_cpp.h"
#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AHangingLight::AHangingLight()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

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

void AHangingLight::OnInteract()
{
	if (!bIsLightOn)
	{
		bIsLightOn = true;
	// 	bCurLightStatus = bIsLightOn;
		// GetWorld()->GetTimerManager().SetTimer(_loopStaminaTimerHandle, this, &AHorrorGameCharacter::StaminaChange, 1.f, false);
		//USoundCue* LightSound = LoadObject<USoundCue>(nullptr, TEXT("/Game/Assets/Sounds/SoundCues/LightOnSoundCue"));
		if (LightSound)
		{
			UGameplayStatics::PlaySoundAtLocation(this, LightSound, GetActorLocation());
		}
		PointLight->SetVisibility(true);
	}
}

void AHangingLight::BoxOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherIndexBody, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor != nullptr && OtherActor != this && OtherComp != nullptr)
	{
		if (OtherActor->IsA<AHorrorGameCharacter>())
		{
			bIsPlayerIn = true;
			if (bIsLightBlink)
			{
				if(CreatureNearNum <= 0)
					FlickeringLight.Play();
			}
		}

		else if (AReaper_cpp* Creature = Cast<AReaper_cpp>(OtherActor))
		{
			CreatureNum++;
			if (!bIsLightBlink)
			{
				bIsLightBlink = true;
				if (CreatureNearNum <= 0 && bIsPlayerIn)
					FlickeringLight.Play();
			}
			// Creatures.Add(Creature);
		}
	}
}

void AHangingLight::BoxOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherIndexBody)
{
	if (OtherActor != nullptr && OtherActor != this && OtherComp != nullptr)
	{
		if (auto Player = Cast<AHorrorGameCharacter>(OtherActor))
		{
			if(!Player->bIsHiding)
				bIsPlayerIn = false;
			FlickeringLight.Stop();
			if (bIsLightOn) PointLight->SetIntensity(Intensity);
		}
		else if (AReaper_cpp* Creature = Cast<AReaper_cpp>(OtherActor))
		{
			CreatureNum--;
			if (CreatureNum <= 0)
			{
				bIsLightBlink = false;
				FlickeringLight.Stop();
				PointLight->SetIntensity(Intensity);
			}
		}
	}
}

void AHangingLight::SphereOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherIndexBody, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor != nullptr && OtherActor != this && OtherComp != nullptr)
	{
		if (OtherActor->IsA<AReaper_cpp>())
		{
			if (bIsPlayerIn && bIsLightOn)
			{
				CreatureNearNum++;
				FlickeringLight.Stop();
				PointLight->SetIntensity(0.f);
			}
		}
	}
}

void AHangingLight::SphereOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherIndexBody)
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
					PointLight->SetIntensity(Intensity);
					FlickeringLight.Play();
				}
			}
		}
	}
}

void AHangingLight::LightFlicker(float value)
{
	float LightIntense = Intensity * value;
	
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
}