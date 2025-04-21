// CopyrightNotice 2023 Sunggon Kim kimdave205@gmail.com. All Rights Reserved.

#include "Furniture/MetalDoor_cpp.h"
#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"
#include "Player/HorrorGameCharacter.h"
#include "AI/CreatureClass.h"
#include "Components/BoxComponent.h"
#include "Components/AudioComponent.h"

// Sets default values
AMetalDoor_cpp::AMetalDoor_cpp()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 메시들의 기본 설정을 해줌. (세세한 설정은 블루프린트 클래스에서 수행)
	static ConstructorHelpers::FObjectFinder<UStaticMesh>SM_Door(TEXT("/Game/Assets/Furniture/basementDoor"));
	if (SM_Door.Succeeded())
	{
		DoorMesh->SetStaticMesh(SM_Door.Object);

	}

	DestructionAudio->OnAudioFinished.AddDynamic(this, &AMetalDoor_cpp::DestructionFinished);

	DoorMesh->SetCollisionProfileName("ClosedDoor");

	DoorRotateAngle = 90.f;
}

// Called when the game starts or when spawned
void AMetalDoor_cpp::BeginPlay()
{
	Super::BeginPlay();

	SetDoorCollision(false);

	// 타임라인 커브 값이 있다면 타임라인에 할당하고, 재생될 때 실행할 콜백 함수도 바인딩함.
	if (OpenAndCloseCurveFloat)
	{
		FOnTimelineFloat TimelineProgress;
		TimelineProgress.BindDynamic(this, &AMetalDoor_cpp::DoorOpen);
		OpenAndCloseTimeline.AddInterpFloat(OpenAndCloseCurveFloat, TimelineProgress);

		FOnTimelineEvent OpenAndCloseTimelineFinishedCallback;
		OpenAndCloseTimelineFinishedCallback.BindDynamic(this, &AMetalDoor_cpp::ChangeCollisionPreset);

		OpenAndCloseTimeline.SetTimelineFinishedFunc(OpenAndCloseTimelineFinishedCallback);
	}

	// 문이 닫혀있지 않으면, 문을 여는 소리를 재생하고, 타임라인을 재생함.
	if (!bIsDoorClosed)
	{
		if (DoorOpenSound)
		{
			UGameplayStatics::PlaySoundAtLocation(this, DoorOpenSound, GetActorLocation());
		}
		OpenAndCloseTimeline.Play();
	}
}

// Called every frame
void AMetalDoor_cpp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	OpenAndCloseTimeline.TickTimeline(DeltaTime);

	// 문이 부숴지고 있는 동안엔 부수고 있는 개체들의 움직임을 막음
	if (bIsDoorBeingBroken)
	{
		for (auto Yokai : InteractingChaser)
		{
			// 각 요괴별로 위치를 문을 부수기 시작한 그 위치로 고정시킴.
			Yokai.Key->SetActorLocation(Yokai.Value);
		}
	}
}

// 플레이어가 상호작용할 때 작동할 함수.
void AMetalDoor_cpp::OnInteract(class AHorrorGameCharacter* Player)
{
	Super::OnInteract(Player);

	// 문이 잠기지 않았을 때
	if (bIsDoorLocked)
	{
		OnInteractionMessage.Broadcast(NSLOCTEXT("DoorMessage", "DoorLocked", "The door is locked."));

		OnInteractionMessage.Clear();
		return;
	}
	// 문이 잠겼을 때, 잠겼다고 플레이어에게 알려줌.
	else
	{
		// 문이 닫혀있으면
		if (bIsDoorClosed)
		{
			// 문을 여는 소리를 재생하고 타임라인을 재생함.
			if (DoorOpenSound)
			{
				UGameplayStatics::PlaySoundAtLocation(this, DoorOpenSound, GetActorLocation());
			}
			OpenAndCloseTimeline.Play();
		}
		// 문이 열려있으면
		else
		{
			// 문을 닫는 소리를 재생하고 타임라인을 역재생함.
			if (DoorCloseSound)
			{
				UGameplayStatics::PlaySoundAtLocation(this, DoorCloseSound, GetActorLocation());
			}
			OpenAndCloseTimeline.Reverse();
		}

		bIsDoorClosed = !bIsDoorClosed;
	}
}

// 적 개체가 상호작용하는 함수.
void AMetalDoor_cpp::AIInteract(AActor* AICharacter)
{
	// 부숴지고 있는 중이 아닐 경우 작동.
	if (bIsDoorBeingBroken)
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
				// 문이 닫힌 상황이면
				if (bIsDoorClosed)
				{
					// 추격 중일 경우, 문을 부숨
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
					
					// 타임라인을 재생하고, 잠겼는지 파악하는 변수를 열렸다고 설정.
					OpenAndCloseTimeline.Play();
					bIsDoorClosed = false;
				}
			}
		}
	}
}

// 타임라인이 재생될 때 호출될 콜백 함수.
void AMetalDoor_cpp::DoorOpen(float Value)
{
	// 문이 여닫는 효과를 줌.
	FRotator Rotator = FRotator(0.f, DoorRotateAngle * Value, 0.f);
	DoorMesh->SetRelativeRotation(Rotator);
}

// 플레이어가 아이템을 사용할 때 작동할 함수.
bool AMetalDoor_cpp::UseInteract(class AHorrorGameCharacter* Player)
{
	// 문이 잠겨있을 때만 작동함.
	if (bIsDoorLocked)
	{
		// 플레이어에게 잠긴 문을 열었다는 것을 알리고 잠금을 해제함.
		Player->bIsFinishUnlock = true;
		bIsDoorLocked = false;
		return true;
	}

	return false;
}

void AMetalDoor_cpp::ChangeCollisionPreset()
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
void AMetalDoor_cpp::BreakDoor()
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

void AMetalDoor_cpp::DestructionFinished()
{
	// 문이 완전히 부숴진 소리를 재생함.
	if (DoorDestructionSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, DoorDestructionSound, GetActorLocation());
	}

	for (auto InteractCreature : InteractingChaser)
	{
		if (ACreatureClass* Yokai = Cast<ACreatureClass>(InteractCreature.Key))
		{
			Yokai->SetIsStop(false);
		}
	}

	InteractingChaser.Empty();
	bIsDoorLocked = false;
	bIsDoorBroken = true;
	bIsDoorBeingBroken = false;

	// 문이 부숴지는 효과가 나타나도록 지오메트리 컬렉션을 생성함.
	if (GC_Door)
	{
		GetWorld()->SpawnActor<AActor>(GC_Door, DoorMesh->GetComponentLocation(), DoorMesh->GetComponentRotation());
	}

	Destroy();
}