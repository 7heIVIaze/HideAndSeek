// CopyrightNotice=0 2023 Sunggon Kim kimdave205@gmail.com

#include "Door_cpp.h"
#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"
#include "HorrorGame/HorrorGameCharacter.h"

// Sets default values
ADoor_cpp::ADoor_cpp()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	DefaultSceneRoot->SetWorldLocation(FVector(0.0f, 0.0f, 0.0f));
	DefaultSceneRoot->SetWorldScale3D(FVector(1.0f, 1.0f, 1.0f));

	RootComponent = DefaultSceneRoot;

	Door = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Door"));
	Door->SetupAttachment(DefaultSceneRoot);
	static ConstructorHelpers::FObjectFinder<UStaticMesh>DoorMesh(TEXT("/Game/Assets/Furniture/Door"));
	if (DoorMesh.Succeeded())
	{
		Door->SetStaticMesh(DoorMesh.Object);
		Door->SetRelativeLocation(FVector(0.0f, -60.0f, -160.0f));
		Door->SetRelativeScale3D(FVector(1.0f, 1.0f, 1.0f));
	}

	Door->SetCollisionProfileName("LockedDoor"); 
	//Door->SetCollisionProfileName(TEXT("NoCollision"));
	// 처음 생성자를 통해 인스턴스가 생성될 때 Locked되어 있는 것으로 가정하고 콜리전을 LockedDoor로 설정

	DoorRotateAngle = 90.0f;
}

// Called when the game starts or when spawned
void ADoor_cpp::BeginPlay()
{
	Super::BeginPlay();
	// Door->SetCollisionProfileName("OverlapOnlyPawn");
	Door->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel5, ECollisionResponse::ECR_Ignore);
	if (!bIsDoorLocked)
	{
		Door->SetCollisionProfileName("UnLockedDoor"); // Locked 해제되어있으니까 UnLockedDoor로 변경
	}

	if (!bIsDoorClosed)
	{
		USoundCue* DoorSound = LoadObject<USoundCue>(nullptr, TEXT("/Game/Assets/Sounds/SoundCues/DoorOpenSoundCue"));
		if (DoorSound)
		{
			UGameplayStatics::PlaySoundAtLocation(this, DoorSound, GetActorLocation());
		}
		OpenAndClose.Play();
		Door->SetCollisionProfileName("LockedDoor");
		// AI가 열린 문을 다시 닫지 않도록 하기 위해 LockedDoor로 콜리전 변경함. 더불어 열린 문에 끼이지 않도록 하기 위함도 있음
	}

	if (CurveFloat)
	{
		FOnTimelineFloat TimelineProgress;
		TimelineProgress.BindDynamic(this, &ADoor_cpp::OpenDoor);
		OpenAndClose.AddInterpFloat(CurveFloat, TimelineProgress);
	}
}

// Called every frame
void ADoor_cpp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	OpenAndClose.TickTimeline(DeltaTime);
}

void ADoor_cpp::OnInteract(class AHorrorGameCharacter* Player)
{
	if (!bIsDoorLocked)
	{
		if (bIsDoorClosed)
		{
			USoundCue* DoorSound = LoadObject<USoundCue>(nullptr, TEXT("/Game/Assets/Sounds/SoundCues/DoorOpenSoundCue"));
			if (DoorSound)
			{
				UGameplayStatics::PlaySoundAtLocation(this, DoorSound, GetActorLocation());
			}
			OpenAndClose.Play();
			Door->SetCollisionProfileName("LockedDoor");
			// AI가 열린 문을 다시 닫지 않도록 하기 위해 LockedDoor로 콜리전 변경함. 더불어 열린 문에 끼이지 않도록 하기 위함도 있음
		}
		else
		{
			USoundCue* DoorSound = LoadObject<USoundCue>(nullptr, TEXT("/Game/Assets/Sounds/SoundCues/DoorCloseSoundCue"));
			if (DoorSound)
			{
				UGameplayStatics::PlaySoundAtLocation(this, DoorSound, GetActorLocation());
			}
			OpenAndClose.Reverse();
			Door->SetCollisionProfileName("UnLockedDoor");
			// 잠기지 않고 닫힌 문이니까 AI가 문을 열 수 있도록 UnLockedDoor로 설정. 더불어 닫힌 문들로 인하여 AI가 길을 잃지 않도록 하기 위함도 있음.
		}

		bIsDoorClosed = !bIsDoorClosed;
	}
	else
	{
		Player->SetErrorText(TEXT("Locked"), 3);
	}
}

void ADoor_cpp::AIInteract()
{
	if (!bIsDoorLocked)
	{
		if (bIsDoorClosed)
		{
			USoundCue* DoorSound = LoadObject<USoundCue>(nullptr, TEXT("/Game/Assets/Sounds/SoundCues/DoorOpenSoundCue"));
			if (DoorSound)
			{
				UGameplayStatics::PlaySoundAtLocation(this, DoorSound, GetActorLocation());
			}
			OpenAndClose.Play();
			Door->SetCollisionProfileName("LockedDoor");
			// AI가 열린 문을 다시 닫지 않도록 하기 위해 LockedDoor로 콜리전 변경함. 더불어 열린 문에 끼이지 않도록 하기 위함도 있음
		}
		else
		{
			USoundCue* DoorSound = LoadObject<USoundCue>(nullptr, TEXT("/Game/Assets/Sounds/SoundCues/DoorCloseSoundCue"));
			if (DoorSound)
			{
				UGameplayStatics::PlaySoundAtLocation(this, DoorSound, GetActorLocation());
			}
			OpenAndClose.Reverse();
			Door->SetCollisionProfileName("UnLockedDoor");
			// 잠기지 않고 닫힌 문이니까 AI가 문을 열 수 있도록 UnLockedDoor로 설정. 더불어 닫힌 문들로 인하여 AI가 길을 잃지 않도록 하기 위함도 있음.
		}

		bIsDoorClosed = !bIsDoorClosed;
	}
}

void ADoor_cpp::OpenDoor(float Value)
{
	FRotator Rotator = FRotator(0.0f, DoorRotateAngle * Value, 0.0f);
	Door->SetRelativeRotation(Rotator);
}

void ADoor_cpp::UseInteract(class AHorrorGameCharacter* Player)
{
	if (bIsDoorLocked)
	{
		bIsDoorLocked = false;
		if(bIsDoorClosed)
		Door->SetCollisionProfileName("UnLockedDoor");
	}
}

void ADoor_cpp::SetDoorCollision(bool value)
{ 
	if (value) // true라면 플레이어가 근처에 있는 것이기 때문에 콜리전(물리적 충돌) 활성화
	{
		if (bIsDoorLocked)
		{
			Door->SetCollisionProfileName("LockedDoor");
		}
		else
		{
			if (bIsDoorClosed)
			{
				Door->SetCollisionProfileName("UnLockedDoor");
			}
			else
			{
				Door->SetCollisionProfileName("LockedDoor");
			}
		}
		// Door->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel5, ECollisionResponse::ECR_Block);
	}
	else // false라면 근처에 플레이어가 없는 것이기 때문에 콜리전(물리적 충돌) 비활성화
	{
		// Door->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		Door->SetCollisionProfileName(TEXT("NoCollision"));
		// Door->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel5, ECollisionResponse::ECR_Ignore);
	}
}