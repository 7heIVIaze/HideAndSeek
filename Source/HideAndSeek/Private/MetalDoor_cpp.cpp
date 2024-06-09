// CopyrightNotice 2023 Sunggon Kim kimdave205@gmail.com. All Rights Reserved.

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

	// 메시들의 기본 설정을 해줌. (세세한 설정은 블루프린트 클래스에서 수행)
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

	/*CreatureOverlapBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CreatureOverlapBox"));
	CreatureOverlapBox->SetupAttachment(DefaultSceneRoot);
	CreatureOverlapBox->OnComponentBeginOverlap.AddDynamic(this, &AMetalDoor_cpp::CreatureBoxBeginOverlap);*/

	Door->SetCollisionProfileName("ClosedDoor");

	DoorRotateAngle = 90.f;
}

// Called when the game starts or when spawned
void AMetalDoor_cpp::BeginPlay()
{
	Super::BeginPlay();

	SetDoorCollision(false);

	// 타임라인 커브 값이 있다면 타임라인에 할당하고, 재생될 때 실행할 콜백 함수도 바인딩함.
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

// 플레이어가 상호작용할 때 작동할 함수.
void AMetalDoor_cpp::OnInteract(class AHorrorGameCharacter* Player)
{
	// 문이 잠기지 않았을 때
	if (!bIsDoorLocked)
	{
		// 문이 닫혀있으면
		if (bIsDoorClosed)
		{
			// 문을 여는 소리를 재생하고 타임라인을 재생함.
			if (DoorOpenSound)
			{
				UGameplayStatics::PlaySoundAtLocation(this, DoorOpenSound, GetActorLocation());
			}
			OpenAndClose.Play();
		}
		// 문이 열려있으면
		else
		{
			// 문을 닫는 소리를 재생하고 타임라인을 역재생함.
			if (DoorCloseSound)
			{
				UGameplayStatics::PlaySoundAtLocation(this, DoorCloseSound, GetActorLocation());
			}
			OpenAndClose.Reverse();
		}

		bIsDoorClosed = !bIsDoorClosed;
	}
	// 문이 잠겼을 때, 잠겼다고 플레이어에게 알려줌.
	else
	{
		Player->SetErrorText(NSLOCTEXT("AMetalDoor_cpp", "When_Metal_Door_Locked", "Locked"), 3);
	}
}

// 적 개체가 상호작용하는 함수.
void AMetalDoor_cpp::AIInteract(AActor* Creature)
{
	// 모든 것은 플레이어가 근처에 있을 때만 되도록 설정
	if (bIsPlayerNear)
	{
		// 문이 부서진 상태가 아닌 상황이여야 상호작용 가능
		if (!bIsDoorBroken)
		{
			// 만약 문이 잠긴 상황이라면
			if (bIsDoorLocked)
			{
				// 각 개체별 문을 부수는 동작을 수행할 로직인데 현재는 작동하지 않음.
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

				// 문이 부숴지도록 설정함.
				BreakDoor();
			}
			// 문이 잠기지 않은 상태라면
			else
			{
				// 문이 닫힌 상황이면
				if (bIsDoorClosed)
				{
					// 문이 열리는 소리를 재생함.
					if (DoorOpenSound)
					{
						UGameplayStatics::PlaySoundAtLocation(this, DoorOpenSound, GetActorLocation());
					}
					
					// 타임라인을 재생하고, 잠겼는지 파악하는 변수를 열렸다고 설정.
					OpenAndClose.Play();
					bIsDoorClosed = false;
				}
			}
		}
	}
}

// 문의 충돌 여부를 설정하는 함수.
void AMetalDoor_cpp::SetDoorCollision(bool inIsPlayerNear)
{
	bIsPlayerNear = inIsPlayerNear;
	if (bIsPlayerNear) // true라면 플레이어가 근처에 있는 것이기 때문에 콜리전(물리적 충돌) 활성화
	{
		Door->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel5, ECollisionResponse::ECR_Block);
	}
	else // false라면 근처에 플레이어가 없는 것이기 때문에 콜리전(물리적 충돌) 비활성화
	{
		Door->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel5, ECollisionResponse::ECR_Ignore);
	}
}

// 타임라인이 재생될 때 호출될 콜백 함수.
void AMetalDoor_cpp::OpenDoor(float Value)
{
	// 문이 여닫는 효과를 줌.
	FRotator Rotator = FRotator(0.f, DoorRotateAngle * Value, 0.f);
	Door->SetRelativeRotation(Rotator);
}

// 플레이어가 아이템을 사용할 때 작동할 함수.
void AMetalDoor_cpp::UseInteract(class AHorrorGameCharacter* Player)
{
	// 문이 잠겨있을 때만 작동함.
	if (bIsDoorLocked)
	{
		// 플레이어에게 잠긴 문을 열었다는 것을 알리고 잠금을 해제함.
		Player->bIsFinishUnlock = true;
		bIsDoorLocked = false;
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

// 문이 부숴지는 함수.
void AMetalDoor_cpp::BreakDoor()
{
	// 원래는 Chaos Destruction을 이용해 부숴져야 하지만, 일단 통째로 눕혀지게 설정함.

	// 문이 부숴지기 전에 충돌 시 날라갈 물리 엔진 활성화와 동시에 바인딩된 함수들 제거함.
	Door->SetSimulatePhysics(true);
	PlayerOverlapBox->OnComponentBeginOverlap.RemoveDynamic(this, &AMetalDoor_cpp::PlayerBoxBeginOverlap);
	PlayerOverlapBox->OnComponentEndOverlap.RemoveDynamic(this, &AMetalDoor_cpp::PlayerBoxEndOverlap);

	PlayerOverlapBox->DestroyComponent();
	
	// 문이 부숴지는 소리를 재생함.
	if (DoorBreakSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, DoorBreakSound, GetActorLocation());
	}

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

	// 문이 부숴지는 효과가 나오고 5초 후, 충돌 설정도 없애고 물리 시뮬레이팅도 멈춤.
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

//void AMetalDoor_cpp::CreatureBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
//{
//	if (OtherActor != nullptr && OtherComp != nullptr && OtherActor != this)
//	{
//		if (ACreatureClass* Creature = Cast<ACreatureClass>(OtherActor)) // 접촉된 크리쳐라면 발동
//		{
//			if (bIsPlayerNear) // 플레이어가 근처에 있어야 문이 크리쳐를 가로막기 때문에, 그 경우에만 발동하게 설정
//			{
//				AIInteract(Creature);
//			}
//		}
//	}
//}