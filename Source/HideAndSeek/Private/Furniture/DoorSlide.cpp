// CopyrightNotice 2023 Sunggon Kim kimdave205@gmail.com. All Rights Reserved.

#include "Furniture/DoorSlide.h"
#include "Furniture/ClassroomDoors_cpp.h"
#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"
#include "Components/AudioComponent.h"
#include "AI/CreatureClass.h"
#include "Player/HorrorGameCharacter.h"

// Sets default values
ADoorSlide::ADoorSlide()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	Lock = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Lock"));
	Lock->SetupAttachment(DoorMesh);
	static ConstructorHelpers::FObjectFinder<UStaticMesh>LockMesh(TEXT("/Game/Assets/Furniture/Lock"));
	if (LockMesh.Succeeded())
	{
		Lock->SetStaticMesh(LockMesh.Object);
		Lock->SetRelativeLocation(FVector(168.0f, 5.0f, 170.0f));
		Lock->SetRelativeScale3D(FVector(3.0f, 3.0f, 3.0f));
	}

	DestructionAudio->OnAudioFinished.AddDynamic(this, &ADoorSlide::DestructionFinished);

	DoorMesh->SetCollisionProfileName("OpenedDoor");
}

// Called when the game starts or when spawned
void ADoorSlide::BeginPlay()
{
	Super::BeginPlay();

	// 기본적으로 충돌 설정은 비활성화함. 플레이어와 적 개체가 근처에 있어야 활성화됨.
	SetDoorCollision(false);
	
	ClosedLoc = FVector(0.0f, 0.0f, 0.0f);
	OpenLoc = FVector(0.0f, DoorMovement, 0.0f);
	
	// 타임라인 커브 값이 있으면 문이 열리는 애니메이션을 수행할 타임라인을 설정함.
	if (OpenAndCloseCurveFloat)
	{
		FOnTimelineFloat TimelineProgress;
		TimelineProgress.BindDynamic(this, &ADoorSlide::DoorOpen);
		
		OpenAndCloseTimeline.AddInterpFloat(OpenAndCloseCurveFloat, TimelineProgress);

		// 문이 열리고 나면 충돌 활성화를 시켜 Nav Mesh가 이 액터를 인식하도록 설정함.
		FOnTimelineEvent OpenAndCloseFinishedCallback;
		OpenAndCloseFinishedCallback.BindDynamic(this, &ADoorSlide::ChangeCollisionPreset);

		OpenAndCloseTimeline.SetTimelineFinishedFunc(OpenAndCloseFinishedCallback);
	}
}

// Called every frame
void ADoorSlide::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	OpenAndCloseTimeline.TickTimeline(DeltaTime);

	// 문이 부숴지고 있는 동안엔 부수고 있는 개체들의 움직임을 막음
	if (bIsDoorBeingBroken)
	{
		for (auto Creature : InteractingChaser)
		{
			// 각 요괴별로 위치를 문을 부수기 시작한 그 위치로 고정시킴.
			Creature.Key->SetActorLocation(Creature.Value);
		}
	}
}

// 플레이어가 상호작용을 할 때 작동할 함수.
void ADoorSlide::OnInteract(class AHorrorGameCharacter* Player)
{
	// 문이 잠긴 상태면 잠겼다는 메시지를 줌.
	if (bIsDoorLocked)
	{
		OnInteractionMessage.Broadcast(NSLOCTEXT("DoorMessage", "DoorLocked", "The door is locked."));

		OnInteractionMessage.Clear();
	}
	// 문이 안 잠긴 상태일 경우
	else
	{
		// 문이 닫혀있다면 타임라인을 재생하고
		if (bIsDoorClosed)
		{
			// 문이 열리는 소리를 재생함.
			if (DoorOpenSound)
			{
				UGameplayStatics::PlaySoundAtLocation(this, DoorOpenSound, GetActorLocation());
			}
		
			OpenAndCloseTimeline.Play();
		}
		// 문이 열려있다면 타임라인을 역재생함.
		else
		{
			// 문이 닫히는 소리를 재생함.
			if (DoorCloseSound)
			{
				UGameplayStatics::PlaySoundAtLocation(this, DoorCloseSound, GetActorLocation());
			}

			OpenAndCloseTimeline.Reverse();
		}

		// 그 후 반대쪽 문에게도 짝지 문이 열렸는지 체크하도록 함.
		bIsDoorClosed = !bIsDoorClosed;
		CD_Manager->SetKnowOtherDoorOpen(bIsDoorClosed);
	}
}

// 적 개체가 상호작용하는 함수.
void ADoorSlide::AIInteract(AActor* AICharacter)
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
				// 문을 부수는 동작을 수행함
				if (auto Yokai = Cast<ACreatureClass>(AICharacter))
				{
					InteractingChaser.Add(Yokai, Yokai->GetActorLocation());
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
					// 추격 중일 경우, 문을 부수는 동작을 수행함.
					if (auto Yokai = Cast<ACreatureClass>(AICharacter))
					{
						if (Yokai->bIsChase)
						{
							InteractingChaser.Add(Yokai, Yokai->GetActorLocation());

							BreakDoor();
							return;
						}
					}

					// 문이 열리는 소리를 재생함.
					if (DoorOpenSound)
					{
						UGameplayStatics::PlaySoundAtLocation(this, DoorOpenSound, GetActorLocation());
					}

					// 타임라인을 재생하고 반대쪽 문에도 알림.
					OpenAndCloseTimeline.Play();
					
					bIsDoorClosed = false;
					CD_Manager->SetKnowOtherDoorOpen(bIsDoorClosed);
				}
			}
		}
	}
}

// 플레이어가 아이템을 사용할 때 작동할 함수.
bool ADoorSlide::UseInteract(class AHorrorGameCharacter* Player)
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

			return true;
		}
	}

	return false;
}

// 타임라인이 재생되면서 호출할 콜백 함수.
void ADoorSlide::DoorOpen(float inCurveFloat)
{
	// 정해진 위치까지 메시를 이동시킴.
	FVector Movement = FVector(DoorMovement * inCurveFloat, 0.0f, 0.0f);
	DoorMesh->SetRelativeLocation(Movement);
}

// 문이 열리게 하는 함수. CD Manager를 통해서 호출됨.
void ADoorSlide::SetDoorUnlock()
{
	// 잠긴 문이 열렸고 문에 걸린 자물쇠를 제거함
	bIsDoorLocked = false;
	Lock->DestroyComponent();
	DoorMesh->SetCollisionProfileName("ClosedDoor");
}

// 다른 문이 열렸음을 알게 하는 함수. CD Manager를 통해서 호출됨.
void ADoorSlide::SetKnowOtherDoorOpen(bool value)
{
	bIsOtherDoorClosed = value;
}

void ADoorSlide::ChangeCollisionPreset()
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
void ADoorSlide::BreakDoor()
{
	// 문이 부숴지는 소리를 재생함.
	if (!bIsDoorBeingBroken)
	{
		DestructionAudio->Play();
	}

	// 카메라 쉐이크 재생함.
	if (PlayerCharacter)
	{
		PlayerCharacter->StartCameraShake();
	}

	// 원래는 Chaos Destruction을 이용해 부숴져야 하지만, 일단 통째로 눕혀지게 설정함.
	bIsDoorBeingBroken = true;

	for (auto InteractCreature : InteractingChaser)
	{
		if (ACreatureClass* Yokai = Cast<ACreatureClass>(InteractCreature.Key))
		{
			Yokai->SetIsStop(true);
		}
	}
}

void ADoorSlide::DestructionFinished()
{
	if (bIsDoorBroken)
	{
		return;
	}
	
	// 문이 부숴지기 전에 충돌 시 날라갈 물리 엔진 활성화와 동시에 바인딩된 함수들 제거함.
	bIsDoorBeingBroken = false;

	DoorMesh->SetSimulatePhysics(true);

	if (InteractingChaser.Num() > 0)
	{
		for (auto InteractCreature : InteractingChaser)
		{
			if (ACreatureClass* Yokai = Cast<ACreatureClass>(InteractCreature.Key))
			{
				Yokai->SetIsStop(false);
			}
		}

		InteractingChaser.Empty();
	}

	bIsDoorLocked = false;
	bIsDoorBroken = true;

	// 반대쪽 문도 부숴지게 설정함.
	CD_Manager->BreakDoor();

	Destroy();
}