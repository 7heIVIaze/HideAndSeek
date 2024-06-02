// CopyrightNotice 2023 Sunggon Kim kimdave205@gmail.com. All Rights Reserved.

#include "Furniture/DistributionBox.h"
#include "HideAndSeek/HorrorGameCharacter.h"
#include "AI/Reaper_cpp.h"
#include "Furniture/DBox_Door.h"

// Sets default values
ADistributionBox::ADistributionBox()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = RootComp;

	BoxMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Box"));
	BoxMesh->SetupAttachment(RootComp);
	static ConstructorHelpers::FObjectFinder<UStaticMesh>SM_DBoxDoor(TEXT("/Game/Assets/Furniture/SM_DistributionBox_Body"));
	if (SM_DBoxDoor.Succeeded())
	{
		BoxMesh->SetStaticMesh(SM_DBoxDoor.Object);
	}

	SpawnPoint = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SpawnPoint"));
	SpawnPoint->SetupAttachment(RootComp);
	bIsPowered = false;
}

// Called when the game starts or when spawned
void ADistributionBox::BeginPlay()
{
	Super::BeginPlay();
	
}

// 플레이어가 분전함과 상호작용 시 수행할 함수.
void ADistributionBox::OnInteract(class AHorrorGameCharacter* Player)
{
	// 문이 있는 경우에 작동함
	if (BoxDoor != nullptr)
	{
		// 문이 잠긴 상태면, 문이 잠겼다는 텍스트를 출력함
		if (BoxDoor->bIsDoorLocked)
		{
			Player->SetErrorText(NSLOCTEXT("ADistributionBox", "BoxDoorLocked", "Distribution box door is locked"), 3);
		}
		// 잠기지 않은 상태면
		else
		{
			// 문이 닫힌 상태면, 문이 닫혀있다는 텍스트룰 출력함.
			if (BoxDoor->bIsDoorClosed)
			{
				Player->SetErrorText(NSLOCTEXT("ADistributionBox", "BoxDoorClosed", "Distribution box door is closed"), 3);
			}
			// 그게 아니라면
			else
			{
				// 전원이 켜지지 않은 경우에만 작동해서 전원을 켠 상태로 만듦.
				if (!bIsPowered)
				{
					bIsPowered = true;

					// 탈출구 문을 없애는 함수
					for (int i = 0; i < BasementDoor.Num(); ++i)
					{
						FVector NewLocation = BasementDoor[i]->GetActorLocation();
						NewLocation.X -= 300;

						BasementDoor[i]->SetActorLocation(NewLocation);
					}

					// 벽을 없애고
					if (IsValid(Wall))
					{
						Wall->Destroy();
					}
					FRotator Rotator(0.f, 0.f, 0.f);

					// 마지막 추격전을 위한 리퍼 생성
					if (IsValid(ReaperClass))
					{
						FActorSpawnParameters SpawnParams;
						AReaper_cpp* Reaper = GetWorld()->SpawnActor<AReaper_cpp>(ReaperClass, SpawnPoint->GetComponentLocation(), Rotator, SpawnParams);
						if (Reaper)
						{
							Reaper->bIsCollectMode = false;
							Reaper->SetCurrentStatus(4);
						}
					}
				}
			}
		}

	}
}