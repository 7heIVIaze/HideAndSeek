// CopyrightNotice=0 2023 Sunggon Kim kimdave205@gmail.com

#include "HangingLight.h"
#include "Components/PointLightComponent.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "HorrorGame/HorrorGameCharacter.h"
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
	//if (bIsLightBlink)
	//{
	//	if (bIsPlayerIn && bIsLightOn)
	//	{
	//		/*for (int i = 0; i < Creatures.Num(); ++i)
	//		{
	//			float EachDist = FVector::Distance(GetActorLocation(), Creatures[i]->GetActorLocation());
	//			if (Distances.Num() > i)
	//			{
	//				Distances.Add(EachDist);
	//			}
	//			else
	//			{
	//				Distances[i] = EachDist;
	//			}

	//			Distance = Distances[i];
	//		}*/
	//		/*TimeCount++;
	//		float Distance = FVector::Distance(GetActorLocation(), Creature->GetActorLocation());
	//		if (Distance < 100.f) Distance = 0.f;
	//		BlinkBlendTime = Distance / 100;*/

	//		for (auto Creature : Creatures)
	//		{
	//			Distance = CalculateDistance(Creature);

	//			if (Distance > 250.f)
	//			{
	//				FlickeringLight.Play();
	//			}
	//			else
	//			{
	//				FlickeringLight.Stop();
	//				PointLight->SetIntensity(0.f);
	//			}
	//		}
	//	}
	//}
}

void AHangingLight::OnInteract()
{
	if (!bIsLightOn)
	{
		bIsLightOn = true;
	// 	bCurLightStatus = bIsLightOn;
		// GetWorld()->GetTimerManager().SetTimer(_loopStaminaTimerHandle, this, &AHorrorGameCharacter::StaminaChange, 1.f, false);
		USoundCue* LightSound = LoadObject<USoundCue>(nullptr, TEXT("/Game/Assets/Sounds/SoundCues/LightOnSoundCue"));
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
			//if (Creatures.Num() > 0)
			//{
			//	// int idx = Creatures.Find(Creature);
			//	Creatures.Remove(Creature);
			//	// Distances.RemoveAt(idx);
			//	if (Creatures.IsEmpty())
			//	{
			//		bIsLightBlink = false;
			//		if (bIsLightOn) PointLight->SetIntensity(Intensity);
			//	}
			//}
			//else
			//{
			//	bIsLightBlink = false;
			//	if (bIsLightOn) PointLight->SetIntensity(Intensity);
			//}
			//// if (!bCurLightStatus && bIsLightOn) PointLight->SetVisibility(true);
			//// Creature = nullptr;
		}
	}
}

void AHangingLight::SphereOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherIndexBody, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor != nullptr && OtherActor != this && OtherComp != nullptr)
	{
		/*if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Sphere Actor Overlapped: %s"), *OtherActor->GetName()));*/
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
		/*if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Sphere Actor Overlapped End: %s"), *OtherActor->GetName()));*/
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

//float AHangingLight::CalculateDistance(AReaper_cpp* Creature)
//{
//	if (Creature)
//	{
//		FVector CreatureLocation = Creature->GetActorLocation();
//		FVector LightLocation = GetActorLocation();
//		return FVector::Distance(CreatureLocation, LightLocation);
//	}
//	return -1.f;
//}