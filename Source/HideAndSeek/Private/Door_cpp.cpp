// CopyrightNotice=0 2023 Sunggon Kim kimdave205@gmail.com

#include "Door_cpp.h"
#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"
#include "HideAndSeek/HorrorGameCharacter.h"
#include "AI/Reaper_cpp.h"
#include "AI/Runner_cpp.h"
#include "AI/Brute_cpp.h"
#include "AI/CreatureClass.h"
#include "Components/BoxComponent.h"
//#include "GeometryCollection/GeometryCollectionComponent.h"

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

	PlayerOverlapBox = CreateDefaultSubobject<UBoxComponent>(TEXT("PlayerOverlapBox"));
	PlayerOverlapBox->SetupAttachment(DefaultSceneRoot);
	PlayerOverlapBox->OnComponentBeginOverlap.AddDynamic(this, &ADoor_cpp::PlayerBoxBeginOverlap);
	PlayerOverlapBox->OnComponentEndOverlap.AddDynamic(this, &ADoor_cpp::PlayerBoxEndOverlap);

	CreatureOverlapBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CreatureOverlapBox"));
	CreatureOverlapBox->SetupAttachment(DefaultSceneRoot);
	CreatureOverlapBox->OnComponentBeginOverlap.AddDynamic(this, &ADoor_cpp::CreatureBoxBeginOverlap);

	Door->SetCollisionProfileName("ClosedDoor"); 
	//Door->SetCollisionProfileName(TEXT("NoCollision"));
	// 처음 생성자를 통해 인스턴스가 생성될 때 Locked되어 있는 것으로 가정하고 콜리전을 OpenedDoor로 설정

	DoorRotateAngle = 90.0f;
}

// Called when the game starts or when spawned
void ADoor_cpp::BeginPlay()
{
	Super::BeginPlay();
	
	SetDoorCollision(false);

	if (!bIsDoorClosed)
	{
		if (DoorOpenSound)
		{
			UGameplayStatics::PlaySoundAtLocation(this, DoorOpenSound, GetActorLocation());
		}
		OpenAndClose.Play();
	//	Door->SetCollisionProfileName("OpenedDoor");
		// AI가 열린 문을 다시 닫지 않도록 하기 위해 OpenedDoor로 콜리전 변경함. 더불어 열린 문에 끼이지 않도록 하기 위함도 있음
	}

	if (CurveFloat)
	{
		FOnTimelineFloat TimelineProgress;
		TimelineProgress.BindDynamic(this, &ADoor_cpp::OpenDoor);
		OpenAndClose.AddInterpFloat(CurveFloat, TimelineProgress);

		FOnTimelineEvent OpenAndCloseFinishedCallback;
		OpenAndCloseFinishedCallback.BindDynamic(this, &ADoor_cpp::ChangeDoorCollision);

		OpenAndClose.SetTimelineFinishedFunc(OpenAndCloseFinishedCallback);
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
	if (!bIsDoorBroken)
	{
		if (!bIsDoorLocked)
		{
			if (bIsDoorClosed)
			{
			//	USoundCue* DoorSound = LoadObject<USoundCue>(nullptr, TEXT("/Game/Assets/Sounds/SoundCues/DoorOpenSoundCue"));
				if (DoorOpenSound)
				{
					UGameplayStatics::PlaySoundAtLocation(this, DoorOpenSound, GetActorLocation());
				}
				OpenAndClose.Play();
			//	Door->SetCollisionProfileName("OpenedDoor");
				// AI가 열린 문을 다시 닫지 않도록 하기 위해 OpenedDoor로 콜리전 변경함. 더불어 열린 문에 끼이지 않도록 하기 위함도 있음
			}
			else
			{
			//	USoundCue* DoorSound = LoadObject<USoundCue>(nullptr, TEXT("/Game/Assets/Sounds/SoundCues/DoorCloseSoundCue"));
				if (DoorCloseSound)
				{
					UGameplayStatics::PlaySoundAtLocation(this, DoorCloseSound, GetActorLocation());
				}
				OpenAndClose.Reverse();
			//	Door->SetCollisionProfileName("ClosedDoor");
				// 잠기지 않고 닫힌 문이니까 AI가 문을 열 수 있도록 ClosedDoor로 설정. 더불어 닫힌 문들로 인하여 AI가 길을 잃지 않도록 하기 위함도 있음.
			}

			bIsDoorClosed = !bIsDoorClosed;
		}
		else
		{
			Player->SetErrorText(NSLOCTEXT("ADoor_cpp", "When_Door_Locked", "Locked"), 3);
		}
	}
}

void ADoor_cpp::AIInteract(AActor* Creature)
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

void ADoor_cpp::OpenDoor(float Value)
{
	FRotator Rotator = FRotator(0.0f, DoorRotateAngle * Value, 0.0f);
	Door->SetRelativeRotation(Rotator);
}

void ADoor_cpp::UseInteract(class AHorrorGameCharacter* Player)
{
	if (!bIsDoorBroken)
	{
		if (bIsDoorLocked)
		{
			bIsDoorLocked = false;
			/*if(bIsDoorClosed)
			Door->SetCollisionProfileName("ClosedDoor");*/
		}
	}
}

void ADoor_cpp::SetDoorCollision(bool inIsPlayerNear)
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

void ADoor_cpp::ChangeDoorCollision()
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

void ADoor_cpp::BreakDoor()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, FString::Printf(TEXT("BreakDoor Called!")));
	
	//FActorSpawnParameters spawnParams;
	Door->SetSimulatePhysics(true);
	//Door->SetCollisionProfileName(TEXT("NoCollision"));
	PlayerOverlapBox->OnComponentBeginOverlap.RemoveDynamic(this, &ADoor_cpp::PlayerBoxBeginOverlap);
	PlayerOverlapBox->OnComponentEndOverlap.RemoveDynamic(this, &ADoor_cpp::PlayerBoxEndOverlap);
	
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

void ADoor_cpp::PlayerBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor != nullptr && OtherComp != nullptr && OtherActor != this)
	{
		if (OtherActor->IsA<AHorrorGameCharacter>()) // 접촉된 액터가 플레이어여야지만 발동
		{
			UE_LOG(LogTemp, Warning, TEXT("%s: Player Near"), *(this->GetName()));
			SetDoorCollision(true);
		}
	}
}

void ADoor_cpp::PlayerBoxEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor != nullptr && OtherComp != nullptr && OtherActor != this)
	{
		if (OtherActor->IsA<AHorrorGameCharacter>()) // 접촉된 액터가 플레이어여야지만 발동
		{
			SetDoorCollision(false);
		}
	}
}

void ADoor_cpp::CreatureBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("%s: Somethine Overlapped!"), *(this->GetName()));
	if (OtherActor != nullptr && OtherComp != nullptr && OtherActor != this && OtherComp->IsA(USkeletalMeshComponent::StaticClass()))
	{
		UE_LOG(LogTemp, Warning, TEXT("%s: Overlapped %s"), *(this->GetName()), *(OtherActor->GetName()));
		if (ACreatureClass* Creature = Cast<ACreatureClass>(OtherActor)) // 접촉된 크리쳐라면 발동
		{
			if (bIsPlayerNear) // 플레이어가 근처에 있어야 문이 크리쳐를 가로막기 때문에, 그 경우에만 발동하게 설정
			{
				AIInteract(Creature);
			}
		}
	}
}