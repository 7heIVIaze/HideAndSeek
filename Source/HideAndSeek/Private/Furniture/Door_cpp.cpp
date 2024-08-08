// CopyrightNotice 2023 Sunggon Kim kimdave205@gmail.com. All Rights Reserved.

#include "Furniture/Door_cpp.h"
#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"
#include "Player/HorrorGameCharacter.h"
#include "AI/Reaper_cpp.h"
#include "AI/Runner_cpp.h"
#include "AI/Brute_cpp.h"
#include "AI/CreatureClass.h"
#include "Components/BoxComponent.h"
#include "Components/AudioComponent.h"
#include "GeometryCollection/GeometryCollectionComponent.h"

// Sets default values
ADoor_cpp::ADoor_cpp()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 메시들의 기본 설정을 해줌. (세세한 설정은 블루프린트 클래스에서 수행)
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

	DestructionAudio = CreateDefaultSubobject<UAudioComponent>(TEXT("DestructionAudio"));
	DestructionAudio->SetupAttachment(Door);
	DestructionAudio->OnAudioFinished.AddDynamic(this, &ADoor_cpp::DestructionEndCallback);
	DestructionAudio->SetAutoActivate(false);

	Door->SetCollisionProfileName("ClosedDoor"); 

	DoorRotateAngle = 90.0f;
}

// Called when the game starts or when spawned
void ADoor_cpp::BeginPlay()
{
	Super::BeginPlay();
	
	SetDoorCollision(false);

	// 타임라인 커브 값이 있다면 타임라인에 할당하고, 재생될 때 실행할 콜백 함수도 바인딩함.
	if (CurveFloat)
	{
		FOnTimelineFloat TimelineProgress;
		TimelineProgress.BindDynamic(this, &ADoor_cpp::OpenDoor);
		OpenAndClose.AddInterpFloat(CurveFloat, TimelineProgress);

		FOnTimelineEvent OpenAndCloseFinishedCallback;
		OpenAndCloseFinishedCallback.BindDynamic(this, &ADoor_cpp::ChangeDoorCollision);

		OpenAndClose.SetTimelineFinishedFunc(OpenAndCloseFinishedCallback);
	}

	// 문이 닫혀있지 않으면, 문을 여는 소리를 재생하고, 타임라인을 재생함.
	if (!bIsDoorClosed)
	{
		if (DoorOpenSound)
		{
			UGameplayStatics::PlaySoundAtLocation(this, DoorOpenSound, GetActorLocation());
		}
		OpenAndClose.Play();
	}
}

// Called every frame
void ADoor_cpp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	OpenAndClose.TickTimeline(DeltaTime);

	// 문이 부숴지고 있는 동안엔 부수고 있는 개체들의 움직임을 막음
	if (bIsDoorBreaking)
	{
		for (auto Creature : InteractingCreatures)
		{
			// 각 요괴별로 위치를 문을 부수기 시작한 그 위치로 고정시킴.
			Creature.Key->SetActorLocation(Creature.Value);
		}
	}
}

// 플레이어가 상호작용할 때 작동할 함수.
void ADoor_cpp::OnInteract(class AHorrorGameCharacter* Player)
{
	// 문이 부숴지지 않았을 때 작동.
	if (!bIsDoorBroken)
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

			// 잠겼는지 파악하는 변수를 반전시킴.
			bIsDoorClosed = !bIsDoorClosed;
		}
		// 문이 잠겼을 때, 잠겼다고 플레이어에게 알려줌.
		else
		{
			Player->SetErrorText(NSLOCTEXT("ADoor_cpp", "When_Door_Locked", "Locked"), 3);
		}
	}
}

// 적 개체가 상호작용하는 함수.
void ADoor_cpp::AIInteract(AActor* Creature)
{
	// 부숴지고 있는 중이 아닐 경우 작동.
	if (bIsDoorBreaking)
	{
		return;
	}

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
					//Reaper->SetIsStop(true);
					// 이미 부수고 있는 개체가 이 행동을 반족하지 않게 하도록 함.
					if (Reaper->GetIsStop())
					{
						return;
					}

					InteractingCreatures.Add(Reaper, Reaper->GetActorLocation());
				}
				else if (auto Runner = Cast<ARunner_cpp>(Creature))
				{
					//Runner->SetIsStop(true);
					// 이미 부수고 있는 개체가 이 행동을 반족하지 않게 하도록 함.
					if (Runner->GetIsStop())
					{
						return;
					}

					InteractingCreatures.Add(Runner, Runner->GetActorLocation());
				}
				else if (auto Brute = Cast<ABrute_cpp>(Creature))
				{
					//Brute->SetIsStop(true);
					// 이미 부수고 있는 개체가 이 행동을 반족하지 않게 하도록 함.
					if(Brute->GetIsStop())
					{
						return;
					}

					InteractingCreatures.Add(Brute, Brute->GetActorLocation());
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
					// 각 개체별 추격 중일 경우, 문을 부수는 동작을 수행할 로직인데 현재는 작동하지 않음.
					if (auto Reaper = Cast<AReaper_cpp>(Creature))
					{
						if (Reaper->bIsChase)
						{
						//	Reaper->SetIsStop(true);
							// 이미 부수고 있는 개체가 이 행동을 반족하지 않게 하도록 함.
							if (Reaper->GetIsStop())
							{
								return;
							}
							InteractingCreatures.Add(Reaper, Reaper->GetActorLocation());
							
							BreakDoor();
							return;
						}
					}
					else if (auto Runner = Cast<ARunner_cpp>(Creature))
					{
						if (Runner->bIsChase)
						{
							//Runner->SetIsStop(true);
							// 이미 부수고 있는 개체가 이 행동을 반족하지 않게 하도록 함.
							if (Runner->GetIsStop())
							{
								return;
							}
							InteractingCreatures.Add(Runner, Runner->GetActorLocation());
							
							BreakDoor();
							return;
						}
					}
					else if (auto Brute = Cast<ABrute_cpp>(Creature))
					{
						if (Brute->bIsChase)
						{
							//Brute->SetIsStop(true);
							// 이미 부수고 있는 개체가 이 행동을 반족하지 않게 하도록 함.
							if (Brute->GetIsStop())
							{
								return;
							}
							InteractingCreatures.Add(Brute, Brute->GetActorLocation());
							
							BreakDoor();
							return;
						}
					}

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

// 타임라인이 재생될 때 호출될 콜백 함수.
void ADoor_cpp::OpenDoor(float Value)
{
	// 문이 열리는 효과를 줌.
	FRotator Rotator = FRotator(0.0f, DoorRotateAngle * Value, 0.0f);
	Door->SetRelativeRotation(Rotator);
}

// 플레이어가 아이템을 사용할 때 작동할 함수.
void ADoor_cpp::UseInteract(class AHorrorGameCharacter* Player)
{
	// 문이 부숴지지 않았을 때만 작동함.
	if (!bIsDoorBroken)
	{
		// 문이 잠겨있을 때만 작동함.
		if (bIsDoorLocked)
		{
			// 플레이어에게 잠긴 문을 열었다는 것을 알리고 잠금을 해제함.
			Player->bIsFinishUnlock = true;
			bIsDoorLocked = false;
		}
	}
}

// 문의 충돌 여부를 설정하는 함수.
void ADoor_cpp::SetDoorCollision(bool inIsPlayerNear)
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

// 문이 부숴지는 함수.
void ADoor_cpp::BreakDoor()
{
	// 문이 부숴지는 소리를 재생함.
	if (!bIsDoorBreaking)
	{
		DestructionAudio->Play();
	}

	// 카메라 쉐이크 재생함.
	if (PlayerCharacter)
	{
		PlayerCharacter->StartCameraShake();
	}

	// 원래는 Chaos Destruction을 이용해 부숴져야 하지만, 일단 통째로 눕혀지게 설정함.
	bIsDoorBreaking = true;

	for (auto InteractCreature : InteractingCreatures)
	{
		if (AReaper_cpp* Reaper = Cast<AReaper_cpp>(InteractCreature.Key))
		{
			Reaper->SetIsStop(true);
		}
		else if (ARunner_cpp* Runner = Cast<ARunner_cpp>(InteractCreature.Key))
		{
			Runner->SetIsStop(true);
		}
		else if (ABrute_cpp* Brute = Cast<ABrute_cpp>(InteractCreature.Key))
		{
			Brute->SetIsStop(true);
		}
	}
}

void ADoor_cpp::PlayerBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor != nullptr && OtherComp != nullptr && OtherActor != this)
	{
		if (AHorrorGameCharacter* Character = Cast<AHorrorGameCharacter>(OtherActor)) // 접촉된 액터가 플레이어여야지만 발동
		{
			PlayerCharacter = Character;
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
			PlayerCharacter = nullptr;
			SetDoorCollision(false);
		}
	}
}

void ADoor_cpp::DestructionEndCallback()
{
	// 문이 완전히 부숴진 소리를 재생함.
	if (DoorBreakSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, DoorBreakSound, GetActorLocation());
	}
	
	for (auto InteractCreature : InteractingCreatures)
	{
		if (AReaper_cpp* Reaper = Cast<AReaper_cpp>(InteractCreature.Key))
		{
			Reaper->SetIsStop(false);
		}
		else if (ARunner_cpp* Runner = Cast<ARunner_cpp>(InteractCreature.Key))
		{
			Runner->SetIsStop(false);
		}
		else if (ABrute_cpp* Brute = Cast<ABrute_cpp>(InteractCreature.Key))
		{
			Brute->SetIsStop(false);
		}
	}

	InteractingCreatures.Empty();
	bIsDoorLocked = false;
	bIsDoorBroken = true;
	bIsDoorBreaking = false;
	
	// 문이 부숴지는 효과가 나타나도록 지오메트리 컬렉션을 생성함.
	if (GC_Door)
	{
		GetWorld()->SpawnActor<AActor>(GC_Door, Door->GetComponentLocation(), Door->GetComponentRotation());
	}
	// 문이 부숴지기 전에 충돌 시 날라갈 물리 엔진 활성화와 동시에 바인딩된 함수들 제거함.
	
	Destroy();

	/*Door->SetSimulatePhysics(true);
	PlayerOverlapBox->OnComponentBeginOverlap.RemoveDynamic(this, &ADoor_cpp::PlayerBoxBeginOverlap);
	PlayerOverlapBox->OnComponentEndOverlap.RemoveDynamic(this, &ADoor_cpp::PlayerBoxEndOverlap);

	PlayerOverlapBox->DestroyComponent();


	Door->SetSimulatePhysics(false);
	Door->SetCollisionProfileName(TEXT("NoCollision"));*/
}