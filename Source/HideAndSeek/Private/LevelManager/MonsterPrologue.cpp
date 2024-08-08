// CopyrightNotice 2023 Sunggon Kim kimdave205@gmail.com. All Rights Reserved.

#include "LevelManager/MonsterPrologue.h"
#include "Components/BoxComponent.h"
#include "Player/HorrorGameCharacter.h"
#include "AI/Reaper_cpp.h"

// Sets default values
AMonsterPrologue::AMonsterPrologue()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// 메시들의 기본 설정을 해줌. (세세한 설정은 블루프린트 클래스에서 수행)
	SpawnPoint = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SpawnPoint"));
	RootComponent = SpawnPoint;

	PlayerEnterBox = CreateDefaultSubobject<UBoxComponent>(TEXT("PlayerEnterBox"));
	PlayerEnterBox->SetupAttachment(SpawnPoint);
	PlayerEnterBox->OnComponentBeginOverlap.AddDynamic(this, &AMonsterPrologue::PlayerEnterBeginOverlap);
}

// Called when the game starts or when spawned
void AMonsterPrologue::BeginPlay()
{
	Super::BeginPlay();
	
}

// 충돌체에 충돌이 감지될 때 작동할 함수.
void AMonsterPrologue::PlayerEnterBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor != nullptr && OtherActor != this && OtherComp != nullptr)
	{
		// 충돌된 액터가 플레이어일 경우
		if (AHorrorGameCharacter* player = Cast<AHorrorGameCharacter>(OtherActor))
		{
			// 처음 감지된 상태라면
			if (!bIsPlayerIn)
			{
				// 도움말을 플레이어에게 알려줌.
				FText text = NSLOCTEXT("AMonsterPrologue", "Overlapped_MonsterPrologue", "You can avoid creatures by hiding in lockers");
				player->SetExplainText(text, 3);
			
				// 그 후 리퍼를 생성해 프롤로그 맵임을 설정함.
				FActorSpawnParameters spawnParams;
				AReaper_cpp* reaper = GetWorld()->SpawnActor<AReaper_cpp>(ObjectToSpawn, GetActorLocation(), FRotator(0.f, 0.f, 0.f), spawnParams);
				reaper->bIsCollectMode = false;

				reaper->PatrolPointList = PatrolPointLists;
				
				// 반복되지 않게 감지된 후라고 설정함.
				bIsPlayerIn = true;
			}
		}
	}
}