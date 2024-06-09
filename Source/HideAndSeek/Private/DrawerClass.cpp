// CopyrightNotice 2023 Sunggon Kim kimdave205@gmail.com. All Rights Reserved.

#include "DrawerClass.h"

// Sets default values
ADrawerClass::ADrawerClass()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	// 메시들의 기본 설정을 해줌. (세세한 설정은 블루프린트 클래스에서 수행)
	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	DefaultSceneRoot->SetWorldLocation(FVector(0.0f, 0.0f, 0.0f));
	FVector DefaultScale = FVector(1.0f, 1.0f, 1.0f);
	DefaultSceneRoot->SetWorldScale3D(DefaultScale);

	RootComponent = DefaultSceneRoot;

	Drawer = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Drawer"));
	Drawer->SetupAttachment(DefaultSceneRoot);

	Item = CreateDefaultSubobject<UChildActorComponent>(TEXT("Item"));
	Item->SetupAttachment(Drawer);
	Item->SetRelativeLocation(FVector(0.0f, 0.0f, -15.0f));

	bIsItemSpawned = false;
}

// Called when the game starts or when spawned
void ADrawerClass::BeginPlay()
{
	Super::BeginPlay();

	// 아이템이 배치된 상태라면, 배치되었다고 설정함.
	if (Item->GetChildActorClass() != nullptr)
	{
		bIsItemSpawned = true;
	}
	 
	// 타임라인 커브 값이 있다면 타임라인에 할당하고, 재생될 때 실행할 콜백 함수도 바인딩함.
	if (CurveFloat)
	{
		FOnTimelineFloat TimelineProgress;
		TimelineProgress.BindDynamic(this, &ADrawerClass::OpenDrawer);
		OpenAndClose.AddInterpFloat(CurveFloat, TimelineProgress); // 오류 해결 TObjectPtr이 아닌 FTimeline이 좋은 듯
	}
}

// Called every frame
void ADrawerClass::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	OpenAndClose.TickTimeline(DeltaTime);
}

// 열리는데 사용할 콜백 함수.
void ADrawerClass::OpenDrawer(float Value)
{
	FVector Location = FVector(0.0f, DrawerOpenMove * Value, 0.0f);
}

// 플레이어가 상호작용할 때 작동할 함수.
void ADrawerClass::OnInteract()
{
	// 닫혀있는 경우.
	if (bIsDrawerClosed)
	{
		// 타임 라인을 재생함.
		OpenAndClose.Play();

		// DEPRECATED
		/*
		// 서랍을 처음 여는 것이라면, 아이템을 생성하거나 미생성
		if (!bIsFirstOpen) 
		{
			// 아이템이 안 나올 확률
			float RandomValue = FMath::FRandRange(0.0f, 1.0f);
			int idx = -1;

			// 아이템이 나올 확률에 해당한다면
			if (RandomValue > fNoItemProbability)
			{
				float fTotalProbability = 1.0f - fNoItemProbability;
				RandomValue = FMath::FRandRange(0.0f, fTotalProbability);
				
				// 
				for (int i = 0; i < ItemProbability.Num(); ++i)
				{
					if (i > 0)
					{
						if (RandomValue < ItemProbability[i] && RandomValue >= ItemProbability[i - 1])
						{
							idx = i;
							break;
						}
					}
					else
					{
						if (RandomValue < ItemProbability[i] && RandomValue >= 0)
						{
							idx = i;
							break;
						}
					}
				}

				if (idx >= 0)
				{
					Item->SetChildActorClass(Items[idx]);
				}
			}
			bIsFirstOpen = true;
		}
		*/
		
	}
	// 열려있는 경우
	else
	{
		// 타임 라인을 역재생함.
		OpenAndClose.Reverse();
	}
	bIsDrawerClosed = !bIsDrawerClosed; // flip flop
}

// 아이템을 배치할 함수.
bool ADrawerClass::SetSpawnItem(TSubclassOf<AActor> inItem)
{
	// 아이템이 없는 경우에만
	if (!GetIsItemSpawned())
	{
		// 아이템 차일드 액터에 해당하는 아이템을 배치하고 배치되었다고 설정함.
		Item->SetChildActorClass(inItem);
		bIsItemSpawned = true;

		return true;
	}

	return false;
}

bool ADrawerClass::GetIsItemSpawned()
{
	return bIsItemSpawned;
}