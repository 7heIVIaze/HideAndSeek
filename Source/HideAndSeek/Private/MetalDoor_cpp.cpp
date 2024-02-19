// CopyrightNotice=0 2023 Sunggon Kim kimdave205@gmail.com

#include "MetalDoor_cpp.h"
#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"
#include "HideAndSeek/HorrorGameCharacter.h"
#include "AI/Reaper_cpp.h"
#include "AI/Runner_cpp.h"
#include "AI/Brute_cpp.h"
#include "AI/CreatureClass.h"
#include "Components/BoxComponent.h"

// Sets default values
AMetalDoor_cpp::AMetalDoor_cpp()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	DefaultSceneRoot->SetWorldLocation(FVector(0.f, 0.f, 0.f));
	DefaultSceneRoot->SetWorldScale3D(FVector(1.f, 1.f, 1.f));

	RootComponent = DefaultSceneRoot;

	Door = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Door"));
	Door->SetupAttachment(DefaultSceneRoot);
	static ConstructorHelpers::FObjectFinder<UStaticMesh>DoorMesh(TEXT("/Game/Assets/Furniture/basementDoor"));
	if (DoorMesh.Succeeded())
	{
		Door->SetStaticMesh(DoorMesh.Object);

	}
	PlayerOverlapBox = CreateDefaultSubobject<UBoxComponent>(TEXT("PlayerOverlapBox"));
	PlayerOverlapBox->SetupAttachment(DefaultSceneRoot);
	PlayerOverlapBox->OnComponentBeginOverlap.AddDynamic(this, &AMetalDoor_cpp::PlayerBoxBeginOverlap);
	PlayerOverlapBox->OnComponentEndOverlap.AddDynamic(this, &AMetalDoor_cpp::PlayerBoxEndOverlap);

	CreatureOverlapBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CreatureOverlapBox"));
	CreatureOverlapBox->SetupAttachment(DefaultSceneRoot);
	CreatureOverlapBox->OnComponentBeginOverlap.AddDynamic(this, &AMetalDoor_cpp::CreatureBoxBeginOverlap);

	Door->SetCollisionProfileName("ClosedDoor");

	DoorRotateAngle = 90.f;
}

// Called when the game starts or when spawned
void AMetalDoor_cpp::BeginPlay()
{
	Super::BeginPlay();

	SetDoorCollision(false);

	if (CurveFloat)
	{
		FOnTimelineFloat TimelineProgress;
		TimelineProgress.BindDynamic(this, &AMetalDoor_cpp::OpenDoor);
		OpenAndClose.AddInterpFloat(CurveFloat, TimelineProgress);

		FOnTimelineEvent OpenAndCloseFinishedCallback;
		OpenAndCloseFinishedCallback.BindDynamic(this, &AMetalDoor_cpp::ChangeDoorCollision);

		OpenAndClose.SetTimelineFinishedFunc(OpenAndCloseFinishedCallback);
	}
}

// Called every frame
void AMetalDoor_cpp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	OpenAndClose.TickTimeline(DeltaTime);
}

void AMetalDoor_cpp::OnInteract(class AHorrorGameCharacter* Player)
{
	if (!bIsDoorLocked)
	{
		if (bIsDoorClosed)
		{
			//USoundCue* DoorSound = LoadObject<USoundCue>(nullptr, TEXT("/Game/Assets/Sounds/SoundCues/MetalDoorSoundCue"));
			if (DoorOpenSound)
			{
				UGameplayStatics::PlaySoundAtLocation(this, DoorOpenSound, GetActorLocation());
			}
			OpenAndClose.Play();
		//	Door->SetCollisionProfileName("OpenedDoor");
		}
		else
		{
			//USoundCue* DoorSound = LoadObject<USoundCue>(nullptr, TEXT("/Game/Assets/Sounds/SoundCues/MetalDoorSound2Cue"));
			if (DoorCloseSound)
			{
				UGameplayStatics::PlaySoundAtLocation(this, DoorCloseSound, GetActorLocation());
			}
			OpenAndClose.Reverse();
		//	Door->SetCollisionProfileName("ClosedDoor");
		}

		bIsDoorClosed = !bIsDoorClosed;
	}
	else
	{
		Player->SetErrorText(NSLOCTEXT("AMetalDoor_cpp", "When_Metal_Door_Locked", "Locked"), 3);
	}
}

void AMetalDoor_cpp::AIInteract(AActor* Creature)
{
	if (!bIsDoorBroken) // 문이 부서진 상태가 아닌 상황이여야 상호작용 가능
	{
		if (bIsDoorLocked) // 만약 문이 잠긴 상황이라면
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, FString::Printf(TEXT("Door Is Locked!")));
			if (auto Reaper = Cast<AReaper_cpp>(Creature))
			{
				Reaper->SetIsStop(true);
				InteractingCreatures.Add(Reaper);
			}
			else if (auto Runner = Cast<ARunner_cpp>(Creature))
			{
				Runner->SetIsStop(true);
				InteractingCreatures.Add(Runner);
			}
			else if (auto Brute = Cast<ABrute_cpp>(Creature))
			{
				Brute->SetIsStop(true);
				InteractingCreatures.Add(Brute);
			}
			BreakDoor();
		}
		else
		{
			if (bIsDoorClosed)
			{
				//		USoundCue* DoorSound = LoadObject<USoundCue>(nullptr, TEXT("/Game/Assets/Sounds/SoundCues/DoorOpenSoundCue"));
				if (DoorOpenSound)
				{
					UGameplayStatics::PlaySoundAtLocation(this, DoorOpenSound, GetActorLocation());
				}
				OpenAndClose.Play();
				//	Door->SetCollisionProfileName("OpenedDoor");
					// AI가 열린 문을 다시 닫지 않도록 하기 위해 OpenedDoor로 콜리전 변경함. 더불어 열린 문에 끼이지 않도록 하기 위함도 있음
				bIsDoorClosed = false;
			}
		}
	}
}

void AMetalDoor_cpp::SetDoorCollision(bool inIsPlayerNear)
{
	bIsPlayerNear = inIsPlayerNear;
	if (bIsPlayerNear) // true라면 플레이어가 근처에 있는 것이기 때문에 콜리전(물리적 충돌) 활성화
	{
		Door->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel5, ECollisionResponse::ECR_Block);
		// Door->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel5, ECollisionResponse::ECR_Block);
	}
	else // false라면 근처에 플레이어가 없는 것이기 때문에 콜리전(물리적 충돌) 비활성화
	{
		// Door->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		Door->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel5, ECollisionResponse::ECR_Ignore);
		// Door->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel5, ECollisionResponse::ECR_Ignore);
	}
}


void AMetalDoor_cpp::OpenDoor(float Value)
{
	FRotator Rotator = FRotator(0.f, DoorRotateAngle * Value, 0.f);
	Door->SetRelativeRotation(Rotator);
}

void AMetalDoor_cpp::UseInteract(class AHorrorGameCharacter* Player)
{
	if (bIsDoorLocked)
	{
		bIsDoorLocked = false;
		//Door->SetCollisionProfileName("ClosedDoor");
	}
}

void AMetalDoor_cpp::ChangeDoorCollision()
{
	if (bIsDoorClosed)
	{
		Door->SetCollisionProfileName("ClosedDoor");
	}
	else
	{
		Door->SetCollisionProfileName("OpenedDoor");
	}
}

void AMetalDoor_cpp::BreakDoor()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, FString::Printf(TEXT("BreakDoor Called!")));

	//FActorSpawnParameters spawnParams;
	Door->SetSimulatePhysics(true);
	//Door->SetCollisionProfileName(TEXT("NoCollision"));
	PlayerOverlapBox->OnComponentBeginOverlap.RemoveDynamic(this, &AMetalDoor_cpp::PlayerBoxBeginOverlap);
	PlayerOverlapBox->OnComponentEndOverlap.RemoveDynamic(this, &AMetalDoor_cpp::PlayerBoxEndOverlap);

	PlayerOverlapBox->DestroyComponent();
	for (AActor* InteractCreature : InteractingCreatures)
	{
		if (AReaper_cpp* Reaper = Cast<AReaper_cpp>(InteractCreature))
		{
			Reaper->SetIsStop(false);
		}
		else if (ARunner_cpp* Runner = Cast<ARunner_cpp>(InteractCreature))
		{
			Runner->SetIsStop(false);
		}
		else if (ABrute_cpp* Brute = Cast<ABrute_cpp>(InteractCreature))
		{
			Brute->SetIsStop(false);
		}
	}
	InteractingCreatures.Empty();
	bIsDoorLocked = false;
	bIsDoorBroken = true;

	GetWorld()->GetTimerManager().SetTimer(Timer, FTimerDelegate::CreateLambda([&]() {
		Door->SetSimulatePhysics(false);
		Door->SetCollisionProfileName(TEXT("NoCollision"));

		GetWorld()->GetTimerManager().ClearTimer(Timer);
	}), 5.f, false);
}

void AMetalDoor_cpp::PlayerBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor != nullptr && OtherComp != nullptr && OtherActor != this)
	{
		if (OtherActor->IsA<AHorrorGameCharacter>()) // 접촉된 액터가 플레이어여야지만 발동
		{
			SetDoorCollision(true);
		}
	}
}

void AMetalDoor_cpp::PlayerBoxEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor != nullptr && OtherComp != nullptr && OtherActor != this)
	{
		if (OtherActor->IsA<AHorrorGameCharacter>()) // 접촉된 액터가 플레이어여야지만 발동
		{
			SetDoorCollision(false);
		}
	}
}

void AMetalDoor_cpp::CreatureBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor != nullptr && OtherComp != nullptr && OtherActor != this)
	{
		if (ACreatureClass* Creature = Cast<ACreatureClass>(OtherActor)) // 접촉된 크리쳐라면 발동
		{
			if (bIsPlayerNear) // 플레이어가 근처에 있어야 문이 크리쳐를 가로막기 때문에, 그 경우에만 발동하게 설정
			{
				AIInteract(Creature);
			}
		}
	}
}