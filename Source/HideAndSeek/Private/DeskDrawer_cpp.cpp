// CopyrightNotice 2023 Sunggon Kim kimdave205@gmail.com. All Rights Reserved.

#include "DeskDrawer_cpp.h"
#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ADeskDrawer_cpp::ADeskDrawer_cpp()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	/*DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	DefaultSceneRoot->SetWorldLocation(FVector(0.0f, 0.0f, 0.0f));
	FVector DefaultScale = FVector(1.0f, 1.0f, 1.0f);
	DefaultSceneRoot->SetWorldScale3D(DefaultScale);

	RootComponent = DefaultSceneRoot;*/

	/*Drawer = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Drawer"));
	Drawer->SetupAttachment(DefaultSceneRoot);*/
	static ConstructorHelpers::FObjectFinder<UStaticMesh> DrawerMesh(TEXT("/Game/Assets/Furniture/LongDeskDrawer"));
	if (DrawerMesh.Succeeded())
	{
		Drawer->SetStaticMesh(DrawerMesh.Object);
		Drawer->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
		//Drawer->SetRelativeScale3D(DefaultScale);
	}

	DrawerOpenMove = 50.0f;
}

// Called when the game starts or when spawned
void ADeskDrawer_cpp::BeginPlay()
{
	Super::BeginPlay();
	
	if (CurveFloat)
	{
		FOnTimelineFloat TimelineProgress;
		TimelineProgress.BindDynamic(this, &ADeskDrawer_cpp::OpenDrawer);
		OpenAndClose.AddInterpFloat(CurveFloat, TimelineProgress);
	}

	if (DrawerMeshes.Num() > 0)
	{
		int RandIdx = FMath::RandRange(0, DrawerMeshes.Num() - 1);
		Drawer->SetStaticMesh(DrawerMeshes[RandIdx]);
	}
}

// Called every frame
void ADeskDrawer_cpp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	OpenAndClose.TickTimeline(DeltaTime);
}


void ADeskDrawer_cpp::OpenDrawer(float Value)
{
	Super::OpenDrawer(Value);
	FVector Location = FVector(0.0f, DrawerOpenMove * Value, 0.0f);

	Drawer->SetRelativeLocation(Location);
}

void ADeskDrawer_cpp::OnInteract()
{
	//UE_LOG(LogTemp, Warning, TEXT("Interact!"));

	if (bIsDrawerClosed)
	{
	//	USoundCue* DrawerSound = LoadObject<USoundCue>(nullptr, TEXT("/Game/Assets/Sounds/SoundCues/DeskOpenSoundCue"));
		if (DrawerOpenSound)
		{
			UGameplayStatics::PlaySoundAtLocation(this, DrawerOpenSound, GetActorLocation());
		}
		//UE_LOG(LogTemp, Warning, TEXT("OPEN"));
		//OpenAndClose.Play();
	}
	else
	{
		//UE_LOG(LogTemp, Warning, TEXT("CLOSE"));
	//	USoundCue* DrawerSound = LoadObject<USoundCue>(nullptr, TEXT("/Game/Assets/Sounds/SoundCues/DeskCloseSoundCue"));
		if (DrawerCloseSound)
		{
			UGameplayStatics::PlaySoundAtLocation(this, DrawerCloseSound, GetActorLocation());
		}
		//OpenAndClose.Reverse();
	}

	Super::OnInteract();

	//bIsDrawerClosed = !bIsDrawerClosed; // flip flop
}