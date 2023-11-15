// Fill out your copyright notice in the Description page of Project Settings.

#include "WardrobeDrawer_cpp.h"
//#include "Components/AudioComponent.h"
#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AWardrobeDrawer_cpp::AWardrobeDrawer_cpp()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	/*DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	DefaultSceneRoot->SetWorldLocation(FVector(0.f, 0.f, 0.f));
	DefaultSceneRoot->SetWorldScale3D(FVector(1.f, 1.f, 1.f));

	RootComponent = DefaultSceneRoot;

	Drawer = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Drawer"));
	Drawer->SetupAttachment(DefaultSceneRoot);*/
	static ConstructorHelpers::FObjectFinder<UStaticMesh> DrawerMesh(TEXT("/Game/Assets/Furniture/wardrobeDrawer"));
	if (DrawerMesh.Succeeded())
	{
		Drawer->SetStaticMesh(DrawerMesh.Object);
		Drawer->SetRelativeLocation(FVector(0.f, 0.f, 0.f));
		Drawer->SetRelativeScale3D(FVector(2.f, 2.f, 1.7f));
	}

	/*DrawerSound = CreateDefaultSubobject<UAudioComponent>(TEXT("DrawerSound"));
	static ConstructorHelpers::FObjectFinder<USoundBase>SoundBase(TEXT("/Game/Assets/Sounds/Drawer_Opening"));
	if (SoundBase.Succeeded())
	{
		DrawerSound->SetSound(SoundBase.Object);
	}
	DrawerSound->SetAutoActivate(false);*/

	DrawerOpenMove = -80.f;
}

// Called when the game starts or when spawned
void AWardrobeDrawer_cpp::BeginPlay()
{
	Super::BeginPlay();
	
	if (CurveFloat)
	{
		FOnTimelineFloat TimelineProgress;
		TimelineProgress.BindDynamic(this, &AWardrobeDrawer_cpp::OpenDrawer);
		OpenAndClose.AddInterpFloat(CurveFloat, TimelineProgress);
	}
}

// Called every frame
void AWardrobeDrawer_cpp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	OpenAndClose.TickTimeline(DeltaTime);
}

void AWardrobeDrawer_cpp::OpenDrawer(float Value)
{
	Super::OpenDrawer(Value);
	FVector Location = FVector(0.f, DrawerOpenMove * Value, 0.f);

	Drawer->SetRelativeLocation(Location);
}

void AWardrobeDrawer_cpp::OnInteract()
{
	if (bIsDrawerClosed)
	{
		USoundCue* DrawerSound = LoadObject<USoundCue>(nullptr, TEXT("/Game/Assets/Sounds/SoundCues/DeskOpenSoundCue"));
		if (DrawerSound)
		{
			UGameplayStatics::PlaySoundAtLocation(this, DrawerSound, GetActorLocation());
		}
		/*DrawerSound->Play();*/
		//OpenAndClose.Play();
	}
	else
	{
		USoundCue* DrawerSound = LoadObject<USoundCue>(nullptr, TEXT("/Game/Assets/Sounds/SoundCues/DeskCloseSoundCue"));
		if (DrawerSound)
		{
			UGameplayStatics::PlaySoundAtLocation(this, DrawerSound, GetActorLocation());
		}
		//DrawerSound->Play();
		//OpenAndClose.Reverse();
	}
	Super::OnInteract();
	//bIsDrawerClosed = !bIsDrawerClosed;
}