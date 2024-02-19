// Fill out your copyright notice in the Description page of Project Settings.

#include "WardrobeDrawer_cpp.h"
#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AWardrobeDrawer_cpp::AWardrobeDrawer_cpp()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	static ConstructorHelpers::FObjectFinder<UStaticMesh> DrawerMesh(TEXT("/Game/Assets/Furniture/wardrobeDrawer"));
	if (DrawerMesh.Succeeded())
	{
		Drawer->SetStaticMesh(DrawerMesh.Object);
		Drawer->SetRelativeLocation(FVector(0.f, 0.f, 0.f));
		Drawer->SetRelativeScale3D(FVector(2.f, 2.f, 1.7f));
	}

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
		//USoundCue* DrawerSound = LoadObject<USoundCue>(nullptr, TEXT("/Game/Assets/Sounds/SoundCues/DeskOpenSoundCue"));
		if (DrawerOpenSound)
		{
			UGameplayStatics::PlaySoundAtLocation(this, DrawerOpenSound, GetActorLocation());
		}
	}
	else
	{
		//USoundCue* DrawerCloseSound = LoadObject<USoundCue>(nullptr, TEXT("/Game/Assets/Sounds/SoundCues/DeskCloseSoundCue"));
		if (DrawerCloseSound)
		{
			UGameplayStatics::PlaySoundAtLocation(this, DrawerCloseSound, GetActorLocation());
		}
		
	}
	Super::OnInteract();
}