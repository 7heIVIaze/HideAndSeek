// CopyrightNotice=0 2023 Sunggon Kim kimdave205@gmail.com

#include "SchoolLight.h"
#include "Components/PointLightComponent.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "HorrorGame/HorrorGameCharacter.h"
#include "Cabinet_cpp.h"
#include "Wardrobe_cpp.h"
#include "AI/CreatureClass.h"
#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ASchoolLight::ASchoolLight()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

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

void ASchoolLight::SetLightStatus()
{
	bIsLightOn = !bIsLightOn;
	/*USoundCue* LightSound = LoadObject<USoundCue>(nullptr, TEXT("/Game/Assets/Sounds/SoundCues/LightOnSoundCue"));
	if (LightSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, LightSound, GetActorLocation());
	}*/
	LightComp->SetVisibility(bIsLightOn);
}

void ASchoolLight::BoxOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherIndexBody, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor != nullptr && OtherActor != this && OtherComp != nullptr)
	{
		if (OtherActor->IsA<AHorrorGameCharacter>())
		{
			bIsPlayerIn = true;
			if (bIsLightBlink)
			{
				if (CreatureNearNum <= 0)
					FlickeringLight.Play();
			}
		}

		/*if (ACabinet_cpp* Cabinet = Cast<ACabinet_cpp>(OtherActor))
		{
			if (Cabinet->bIsHiding)
				bIsPlayerIn = true;
			else
				bIsPlayerIn = false;

			if (bIsLightBlink)
			{
				if (CreatureNearNum <= 0)
					FlickeringLight.Play();
			}
		}

		if (AWardrobe_cpp* Wardrobe = Cast<AWardrobe_cpp>(OtherActor))
		{
			if (Wardrobe->bIsHiding)
				bIsPlayerIn = true;
			else
				bIsPlayerIn = false;

			if (bIsLightBlink)
			{
				if (CreatureNearNum <= 0)
					FlickeringLight.Play();
			}
		}*/

		else if (OtherActor->IsA<ACreatureClass>())
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

void ASchoolLight::BoxOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherIndexBody)
{
	if (OtherActor != nullptr && OtherActor != this && OtherComp != nullptr)
	{
		if (auto Player = Cast<AHorrorGameCharacter>(OtherActor))
		{
			if (!Player->bIsHiding)
				bIsPlayerIn = false;
			FlickeringLight.Stop();
			if (bIsLightOn) LightComp->SetIntensity(Intensity);
		}
		else if (OtherActor->IsA<ACreatureClass>())
		{
			CreatureNum--;
			if (CreatureNum <= 0)
			{
				bIsLightBlink = false;
				FlickeringLight.Stop();
				LightComp->SetIntensity(Intensity);
			}
		}
	}
}

void ASchoolLight::SphereOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherIndexBody, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor != nullptr && OtherActor != this && OtherComp != nullptr)
	{
		/*if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Sphere Actor Overlapped: %s"), *OtherActor->GetName()));*/
		if (OtherActor->IsA<ACreatureClass>())
		{
			if (bIsPlayerIn && bIsLightOn)
			{
				CreatureNearNum++;
				FlickeringLight.Stop();
				LightComp->SetIntensity(0.f);
			}
		}
	}
}

void ASchoolLight::SphereOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherIndexBody)
{
	if (OtherActor != nullptr && OtherActor != this && OtherComp != nullptr)
	{
		/*if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Sphere Actor Overlapped End: %s"), *OtherActor->GetName()));*/
		if (OtherActor->IsA<ACreatureClass>())
		{
			if (bIsPlayerIn && bIsLightOn)
			{
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

void ASchoolLight::LightFlicker(float value)
{
	float LightIntense = Intensity * value;

	if (LightIntense < 0.f)
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
	}
}
