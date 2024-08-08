// CopyrightNotice 2023 Sunggon Kim kimdave205@gmail.com. All Rights Reserved.

#include "Furniture/ClassroomDoorActor_cpp.h"
#include "Furniture/ClassroomDoors_cpp.h"
#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"
#include "Components/AudioComponent.h"
#include "AI/Reaper_cpp.h"
#include "AI/Runner_cpp.h"
#include "AI/Brute_cpp.h"
#include "Player/HorrorGameCharacter.h"

// Sets default values
AClassroomDoorActor_cpp::AClassroomDoorActor_cpp()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	FVector DefaultPos = FVector(0.0f, 0.0f, 0.0f);
	FVector DefaultScale = FVector(1.0f, 1.0f, 1.0f);

	// 메시들의 기본 설정을 해줌. (세세한 설정은 블루프린트 클래스에서 수행)
	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	DefaultSceneRoot->SetWorldLocation(DefaultPos);
	DefaultSceneRoot->SetWorldScale3D(DefaultScale);
	RootComponent = DefaultSceneRoot;

	DoorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ClassDoor"));
	DoorMesh->SetupAttachment(DefaultSceneRoot);
	static ConstructorHelpers::FObjectFinder<UStaticMesh>CRDoorObj(TEXT("/Game/Assets/Furniture/ClassroomDoor"));
	if (CRDoorObj.Succeeded())
	{
		DoorMesh->SetStaticMesh(CRDoorObj.Object);
	}
	DoorMesh->SetRelativeLocation(DefaultPos);
	DoorMesh->SetRelativeScale3D(DefaultScale);

	Lock = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Lock"));
	Lock->SetupAttachment(DoorMesh);
	static ConstructorHelpers::FObjectFinder<UStaticMesh>LockMesh(TEXT("/Game/Assets/Furniture/Lock"));
	if (LockMesh.Succeeded())
	{
		Lock->SetStaticMesh(LockMesh.Object);
		Lock->SetRelativeLocation(FVector(168.0f, 5.0f, 170.0f));
		Lock->SetRelativeScale3D(FVector(3.0f, 3.0f, 3.0f));
	}

	DestructionAudio = CreateDefaultSubobject<UAudioComponent>(TEXT("DestructionAudio"));
	DestructionAudio->SetupAttachment(DoorMesh);
	DestructionAudio->OnAudioFinished.AddDynamic(this, &AClassroomDoorActor_cpp::DestructionEndCallback);
	DestructionAudio->SetAutoActivate(false);

	DoorMesh->SetCollisionProfileName("OpenedDoor");
}

// Called when the game starts or when spawned
void AClassroomDoorActor_cpp::BeginPlay()
{
	Super::BeginPlay();

	// 기본적으로 충돌 설정은 비활성화함. 플레이어와 적 개체가 근처에 있어야 활성화됨.
	SetDoorCollision(false);
	
	ClosedLoc = FVector(0.0f, 0.0f, 0.0f);
	OpenLoc = FVector(0.0f, DoorMovement, 0.0f);
	
	// 타임라인 커브 값이 있으면 문이 열리는 애니메이션을 수행할 타임라인을 설정함.
	if (CurveFloat)
	{
		FOnTimelineFloat TimelineProgress;
		TimelineProgress.BindDynamic(this, &AClassroomDoorActor_cpp::DoorOpen);
		
		OpenAndClose.AddInterpFloat(CurveFloat, TimelineProgress);

		// 문이 열리고 나면 충돌 활성화를 시켜 Nav Mesh가 이 액터를 인식하도록 설정함.
		FOnTimelineEvent OpenAndCloseFinishedCallback;
		OpenAndCloseFinishedCallback.BindDynamic(this, &AClassroomDoorActor_cpp::ChangeDoorCollision);

		OpenAndClose.SetTimelineFinishedFunc(OpenAndCloseFinishedCallback);
	}
}

// Called every frame
void AClassroomDoorActor_cpp::Tick(float DeltaTime)
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

// 플레이어가 상호작용을 할 때 작동할 함수.
void AClassroomDoorActor_cpp::OnInteract(class AHorrorGameCharacter* Player)
{
	// 문이 안 잠긴 상태일 경우
	if (!bIsDoorLocked)
	{
		// 문이 열리는 소리를 재생함.
		if (DoorSound)
		{
			UGameplayStatics::PlaySoundAtLocation(this, DoorSound, GetActorLocation());
		}

		// 문이 닫혀있다면 타임라인을 재생하고
		if (bIsDoorClosed)
		{
			OpenAndClose.Play();
		}
		// 문이 열려있다면 타임라인을 역재생함.
		else
		{
			OpenAndClose.Reverse();
		}

		// 그 후 반대쪽 문에게도 짝지 문이 열렸는지 체크하도록 함.
		bIsDoorClosed = !bIsDoorClosed;
		CD_Manager->SetKnowOtherDoorOpen(bIsDoorClosed);
	}
	// 문이 잠긴 상태면 잠겼다는 메시지를 줌.
	else
	{
		Player->SetErrorText(NSLOCTEXT("AClassroomDoorActor_cpp", "When_Classroom_Door_Locked", "Locked"), 3);
	}
}

// 적 개체가 상호작용하는 함수.
void AClassroomDoorActor_cpp::AIInteract(AActor* Creature)
{
	// 플레이어가 근처에 있을 때만 작동하도록 함.
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
					if (Brute->GetIsStop())
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
				// 이 문과 옆 문도 닫힌 상황이면
				if (bIsDoorClosed && bIsOtherDoorClosed)
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
					if (DoorSound)
					{
						UGameplayStatics::PlaySoundAtLocation(this, DoorSound, GetActorLocation());
					}

					// 타임라인을 재생하고 반대쪽 문에도 알림.
					OpenAndClose.Play();
					
					bIsDoorClosed = false;
					CD_Manager->SetKnowOtherDoorOpen(bIsDoorClosed);
				}
			}
		}
	}
}

// 플레이어가 아이템을 사용할 때 작동할 함수.
void AClassroomDoorActor_cpp::UseInteract(class AHorrorGameCharacter* Player)
{
	// 문이 부숴지지 않았을 때만 작동함.
	if (!bIsDoorBroken)
	{
		// 문이 잠겨있을 때만 작동함.
		if (bIsDoorLocked)
		{
			// 플레이어에게 잠긴 문을 열었다는 것을 알리고 문에 걸린 자물쇠를 제거함.
			Player->bIsFinishUnlock = true;
			bIsDoorLocked = false;
			Lock->DestroyComponent();

			// 그 후 반대쪽 문에게도 잠긴 문이 열렸음을 알림.
			CD_Manager->SetDoorUnlock();
		}
	}
}

// 타임라인이 재생되면서 호출할 콜백 함수.
void AClassroomDoorActor_cpp::DoorOpen(float Value)
{
	// 정해진 위치까지 메시를 이동시킴.
	FVector Movement = FVector(DoorMovement * Value, 0.0f, 0.0f);
	DoorMesh->SetRelativeLocation(Movement);
}

// 문이 열리게 하는 함수. CD Manager를 통해서 호출됨.
void AClassroomDoorActor_cpp::SetDoorUnlock()
{
	// 잠긴 문이 열렸고 문에 걸린 자물쇠를 제거함
	bIsDoorLocked = false;
	Lock->DestroyComponent();
	DoorMesh->SetCollisionProfileName("ClosedDoor");
}

// 다른 문이 열렸음을 알게 하는 함수. CD Manager를 통해서 호출됨.
void AClassroomDoorActor_cpp::SetKnowOtherDoorOpen(bool value)
{
	bIsOtherDoorClosed = value;
}

// 문의 충돌 여부를 설정하는 함수.
void AClassroomDoorActor_cpp::SetDoorCollision(bool inIsPlayerNear)
{
	bIsPlayerNear = inIsPlayerNear;
	// true라면 플레이어가 근처에 있는 것이기 때문에 콜리전(물리적 충돌) 활성화
	if (inIsPlayerNear) 
	{
		DoorMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel5, ECollisionResponse::ECR_Block);
	}
	// false라면 근처에 플레이어가 없는 것이기 때문에 콜리전(물리적 충돌) 비활성화
	else 
	{
		DoorMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel5, ECollisionResponse::ECR_Ignore);
	}
}

void AClassroomDoorActor_cpp::ChangeDoorCollision()
{
	if (bIsDoorClosed)
	{
		DoorMesh->SetCollisionProfileName("ClosedDoor");
	}
	else
	{
		DoorMesh->SetCollisionProfileName("OpenedDoor");
	}
}

// 문이 부숴지는 함수.
void AClassroomDoorActor_cpp::BreakDoor()
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

void AClassroomDoorActor_cpp::DestructionEndCallback()
{
	if (bIsDoorBroken)
	{
		return;
	}
	//// 문이 완전히 부숴진 소리를 재생함.
	//if (DoorBreakSound)
	//{
	//	UGameplayStatics::PlaySoundAtLocation(this, DoorBreakSound, GetActorLocation());
	//}

	//// 문이 부숴지기 전에 충돌 시 날라갈 물리 엔진 활성화와 동시에 바인딩된 함수들 제거함.
	bIsDoorBreaking = false;

	DoorMesh->SetSimulatePhysics(true);

	if (InteractingCreatures.Num() > 0)
	{
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
	}

	bIsDoorLocked = false;
	bIsDoorBroken = true;

	/*DoorMesh->SetSimulatePhysics(false);
	DoorMesh->SetCollisionProfileName(TEXT("NoCollision"));*/

	// 반대쪽 문도 부숴지게 설정함.
	CD_Manager->BreakDoor();

	// 문이 부숴지는 효과가 나타나도록 지오메트리 컬렉션을 생성함.
	/*if (GC_Door)
	{
		GetWorld()->SpawnActor<AActor>(GC_Door, DoorMesh->GetComponentLocation(), DoorMesh->GetComponentRotation());
	}*/

	Destroy();
}