// CopyrightNotice 2023 Sunggon Kim kimdave205@gmail.com. All Rights Reserved.

#include "Furniture/Altar_cpp.h"
#include "Player/HorrorGameCharacter.h"
#include "Player/InventoryComponent.h"
#include "Components/AudioComponent.h"
#include "AI/Reaper_cpp.h"
#include "Furniture/End_Mirror.h"
#include "HUD/GameUI.h"
#include "HUD/TimerWidget.h"
#include "Kismet/GameplayStatics.h"
#include "LevelManager/HorrorGameGameInstance.h"

// Sets default values
AAltar_cpp::AAltar_cpp()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 메시들의 기본 설정을 해줌. (세세한 설정은 블루프린트 클래스에서 수행)
	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = RootComp;

	Altar = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Altar"));
	Altar->SetupAttachment(RootComp);
	static ConstructorHelpers::FObjectFinder<UStaticMesh>SM_Altar(TEXT("/Game/Assets/Furniture/Altar"));
	if (SM_Altar.Succeeded())
	{
		Altar->SetStaticMesh(SM_Altar.Object);
	}
	
	SpawnPoint = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SpawnPoint"));
	SpawnPoint->SetupAttachment(RootComp);

	InteractSound = CreateDefaultSubobject<UAudioComponent>(TEXT("Sound"));
	InteractSound->SetAutoActivate(false);

	UnSealedItemNum = 0;
}

// Called when the game starts or when spawned
void AAltar_cpp::BeginPlay()
{
	Super::BeginPlay();
	
	// 당연히 오브젝트를 얻어 제단에 바친 상태가 아니므로 안 보이게 설정함.
	RespawnTimer = 0.f;
	bIsLevelStart = false;
	PlayerCharacter = Cast<AHorrorGameCharacter>(UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetPawn());

	if (UHorrorGameGameInstance* GameInstance = Cast<UHorrorGameGameInstance>(GetGameInstance()))
	{
		ObjectAmountNeeded = GameInstance->GetObjectCountToCollect();
	}

	// 플레이어가 있는 경우
	if (PlayerCharacter)
	{
		PlayerCharacter->GetInventoryComponent()->OnObjectAcquired.AddDynamic(this, &AAltar_cpp::UnSealedObjectNumber);
		OnInteractionMessage.AddDynamic(PlayerCharacter->GameUIWidget, &UGameUI::ShowMessage);
	}
}

// 플레이어가 상호작용할 때 작동할 함수.
void AAltar_cpp::OnInteract(class AHorrorGameCharacter* Player)
{
	// 플레이어가 오브젝트를 다 모은 경우
	if (Player->GetInventoryComponent()->ObjectCount == ObjectAmountNeeded)
	{
		// 클리어했다고 델리게이트 이벤트를 호출함.
		OnClear.Broadcast();

		// Ending Mirror가 존재한 상황이면 클리어가 가능한 상태임을 알림.
		if (IsValid(EndingMirror))
		{
			EndingMirror->SetIsCleared(true);
		}
	}
	// 필요한 오브젝트 아이템을 모으지 못했다면, 아이템이 필요하다고 알림.
	else
	{
		OnInteractionMessage.Broadcast(NSLOCTEXT("AAltar_cpp", "Altar_Error", "Five reincarnation stones are required."));
	}
}

// 리퍼를 스폰시킬 함수.
void AAltar_cpp::ReaperSpawn()
{
	FActorSpawnParameters spawnParams;

	// 제단의 스폰 포인트 지점에서 리퍼를 스폰시킴.
	CurrentReaper = GetWorld()->SpawnActor<AReaper_cpp>(Reaper, SpawnPoint->GetComponentLocation(), FRotator(0.f, 0.f, 0.f), spawnParams);

	// 현재 리퍼가 맵에 존재한다면, 리퍼의 봉인 여부를 설정함.
	if (CurrentReaper)
	{
		CurrentReaper->SetCurrentStatus(UnSealedItemNum);
	}

	// 만약 레벨이 시작한 상태가 아니면, 레벨이 시작되었다고 설정함.
	if (!bIsLevelStart)
	{
		bIsLevelStart = true;
	}

	RespawnTimer = 0.f;
}
// Called every frame
void AAltar_cpp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 현재 챕터가 시작되었는데 리퍼가 처치되는 등 맵에 배치되어있지 않다면
	if (CurrentReaper == nullptr && bIsLevelStart)
	{
		// 리스폰 대기 시간을 계속 흐르게 함.
		RespawnTimer += DeltaTime;

		// 20초가 지난 상태라면, 리퍼를 스폰하게 하고 리스폰 타이머를 0초로 설정함.
		if (RespawnTimer >= 20.f)
		{
			ReaperSpawn();
			RespawnTimer = 0.f;
		}
	}
}

// 플레이어가 오브젝트 아이템을 수집할 때마다 호출될 함수.
void AAltar_cpp::UnSealedObjectNumber(int32 value)
{
	// 현재 배치된 레벨의 기믹이 존재하는 경우에만 작동하도록.
	if (bHasLevelGimmick)
	{
		// 봉인 해제된 아이템의 개수를 더함.
		UnSealedItemNum += value;

		// 플레이어가 모든 아이템을 다 모은 경우
		if (UnSealedItemNum == ObjectAmountNeeded)
		{
			// 현재 리퍼가 존재하지 않는다면(처치되었거나, 소멸 등) 리퍼를 스폰 지점에 즉시 리스폰 시킴.
			if (CurrentReaper == nullptr)
			{
				ReaperSpawn();
			}
			// 리퍼가 존재한다면, 리퍼의 위치를 이 제단의 스폰 지점으로 이동 시킴.
			else
			{
				CurrentReaper->SetActorLocation(SpawnPoint->GetComponentLocation());
			}

			// 그리고 플레이어에게 리퍼의 봉인이 해제되었다는 투의 문구를 출력함.
			PlayerCharacter->OnAnnounce();
		}
	}
}