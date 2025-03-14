// CopyrightNotice 2023 Sunggon Kim kimdave205@gmail.com. All Rights Reserved.

#include "Furniture/DoorPush.h"
#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"
#include "Player/HorrorGameCharacter.h"
#include "AI/CreatureClass.h"
#include "Components/AudioComponent.h"
#include "GeometryCollection/GeometryCollectionComponent.h"

// Sets default values
ADoorPush::ADoorPush()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	DestructionAudio->OnAudioFinished.AddDynamic(this, &ADoorPush::DestructionFinished);

	DoorMesh->SetCollisionProfileName("ClosedDoor"); 

	DoorRotateAngle = 90.0f;
}

// Called when the game starts or when spawned
void ADoorPush::BeginPlay()
{
	Super::BeginPlay();
	
	SetDoorCollision(false);

	// 타임라인 커브 값이 있다면 타임라인에 할당하고, 재생될 때 실행할 콜백 함수도 바인딩함.
	if (OpenAndCloseCurveFloat)
	{
		FOnTimelineFloat TimelineProgress;
		TimelineProgress.BindDynamic(this, &ADoorPush::DoorOpen);
		OpenAndCloseTimeline.AddInterpFloat(OpenAndCloseCurveFloat, TimelineProgress);

		FOnTimelineEvent OpenAndCloseTimelineFinishedCallback;
		OpenAndCloseTimelineFinishedCallback.BindDynamic(this, &ADoorPush::ChangeCollisionPreset);

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
void ADoorPush::Tick(float DeltaTime)
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
void ADoorPush::OnInteract(class AHorrorGameCharacter* Player)
{
	Super::OnInteract(Player);

	// 문이 부숴지지 않았을 때 작동.
	if (!bIsDoorBroken)
	{
		// 문이 잠겨 있을 때
		if (bIsDoorLocked)
		{
			OnInteractionMessage.Broadcast(NSLOCTEXT("DoorMessage", "DoorLocked", "The door is locked."));

			OnInteractionMessage.Clear();
			return;
		}
		// 문이 잠기지 않았을 때
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

			// 잠겼는지 파악하는 변수를 반전시킴.
			bIsDoorClosed = !bIsDoorClosed;
		}
	}
}

// 적 개체가 상호작용하는 함수.
void ADoorPush::AIInteract(AActor* AICharacter)
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
void ADoorPush::DoorOpen(float inOpenAndCloseCurveFloat)
{
	// 문이 열리는 효과를 줌.
	FRotator Rotator = FRotator(0.0f, DoorRotateAngle * inOpenAndCloseCurveFloat, 0.0f);
	DoorMesh->SetRelativeRotation(Rotator);
}

// 플레이어가 아이템을 사용할 때 작동할 함수.
bool ADoorPush::UseInteract(class AHorrorGameCharacter* Player)
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
			return true;
		}
	}

	return false;
}

// 문의 충돌 여부를 설정하는 함수.
//void ADoorPush::SetDoorCollision(bool inIsPlayerNear)
//{ 
//	bIsPlayerNear = inIsPlayerNear;
//	if (bIsPlayerNear) // true라면 플레이어가 근처에 있는 것이기 때문에 콜리전(물리적 충돌) 활성화
//	{
//		DoorMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel5, ECollisionResponse::ECR_Block);
//	}
//	else // false라면 근처에 플레이어가 없는 것이기 때문에 콜리전(물리적 충돌) 비활성화
//	{
//		DoorMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel5, ECollisionResponse::ECR_Ignore);
//	}
//}

void ADoorPush::ChangeCollisionPreset()
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
void ADoorPush::BreakDoor()
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

//void ADoorPush::PlayerBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
//{
//	if (OtherActor != nullptr && OtherComp != nullptr && OtherActor != this)
//	{
//		if (AHorrorGameCharacter* Character = Cast<AHorrorGameCharacter>(OtherActor)) // 접촉된 액터가 플레이어여야지만 발동
//		{
//			PlayerCharacter = Character;
//			SetDoorCollision(true);
//		}
//	}
//}
//
//void ADoorPush::PlayerBoxEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
//{
//	if (OtherActor != nullptr && OtherComp != nullptr && OtherActor != this)
//	{
//		if (OtherActor->IsA<AHorrorGameCharacter>()) // 접촉된 액터가 플레이어여야지만 발동
//		{
//			PlayerCharacter = nullptr;
//			SetDoorCollision(false);
//		}
//	}
//}

void ADoorPush::DestructionFinished()
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
	// 문이 부숴지기 전에 충돌 시 날라갈 물리 엔진 활성화와 동시에 바인딩된 함수들 제거함.
	
	Destroy();
}