// CopyrightNotice 2023 Sunggon Kim kimdave205@gmail.com. All Rights Reserved.

#include "Altar_cpp.h"
#include "HideAndSeek/HorrorGameCharacter.h"
#include "Components/AudioComponent.h"
#include "AI/Reaper_cpp.h"
#include "End_Mirror.h"
#include "GameUI.h"
#include "HUD/TimerWidget.h"
#include "Kismet/GameplayStatics.h"

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
	
	Sword = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Sword"));
	Sword->SetupAttachment(Altar);
	static ConstructorHelpers::FObjectFinder<UStaticMesh>SM_Sword(TEXT("/Game/Assets/Items/Sword"));
	if (SM_Sword.Succeeded())
	{
		Sword->SetStaticMesh(SM_Sword.Object);
	}
	Mirror = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mirror"));
	Mirror->SetupAttachment(Altar);
	static ConstructorHelpers::FObjectFinder<UStaticMesh>SM_Mirror(TEXT("/Game/Assets/Items/BronzeMirror"));
	if (SM_Mirror.Succeeded())
	{
		Mirror->SetStaticMesh(SM_Mirror.Object);
	}
	Bell = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Bell"));
	Bell->SetupAttachment(Altar);
	static ConstructorHelpers::FObjectFinder<UStaticMesh>SM_Bell(TEXT("/Game/Assets/Items/bell"));
	if (SM_Bell.Succeeded())
	{
		Bell->SetStaticMesh(SM_Bell.Object);
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
	Sword->SetVisibility(false);
	Mirror->SetVisibility(false);
	Bell->SetVisibility(false);
	RespawnTimer = 0.f;
	bIsLevelStart = false;
	PlayerCharacter = Cast<AHorrorGameCharacter>(UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetPawn());
}

// 플레이어가 상호작용할 때 작동할 함수.
void AAltar_cpp::OnInteract(class AHorrorGameCharacter* Player)
{
	// 플레이어가 오브젝트 별로(청동 검, 청동 방울, 청동 거울) 1개 이상씩 수집한 상태라면
	if (Player->GetSwordNumbers() && Player->GetMirrorNumbers() && Player->GetBellNumbers())
	{
		// 효과음을 재생함.
		if (InteractSound)
		{
			InteractSound->Play();
		}

		// 각 오브젝트 아이템이 올려져 있는 것처럼 보이게 설정함.
		Sword->SetVisibility(true);
		Mirror->SetVisibility(true);
		Bell->SetVisibility(true);

		// 게임 플레이 타이머 정지.
		Player->GameUIWidget->TimerWidget->StopTimer();

		// 플레이어의 인벤토리를 순회하며 오브젝트 아이템 제거하는 로직임.
		for (int i = 0; i < Player->Inventory.Num(); ++i) 
		{ 
			// 인벤토리의 현재 인덱스에 위치한 아이템이 청동 검인 경우
			if (Player->Inventory[i].ItemNumber == 5)
			{
				// 청동 검의 개수를 빼고 해당 개수를 인벤토리에도 적용함.
				Player->SwordCount -= 1;
				Player->Inventory[i].ItemCount = Player->SwordCount;

				// 현재 인벤토리에 아이템이 없는 경우.
				if (Player->Inventory[i].ItemCount == 0)
				{
					// 해당 위치의 인벤토리를 삭제하고, 인벤토리의 최대 인덱스와 현재 가리키고 있는 인덱스를 1 감소시킴.
					Player->Inventory.RemoveAt(i);
					Player->InventoryNum--;
					Player->CurrentItemNum--;

					// 인벤토리에 아이템이 있음에도, 현재 가리키고 있는 인벤토리의 인덱스가 0 미만인 경우
					if (Player->CurrentItemNum < 0 && Player->InventoryNum >= 0)
					{
						// 가리키고 있는 인벤토리의 인덱스를 0으로 설정함.
						Player->CurrentItemNum = 0;
					}

					// 만약 인벤토리에 아이템이 없어도 현재 가리키고 있는 인벤토리의 인덱스를 0으로 설정함.
					if (Player->InventoryNum < 0)
					{
						Player->CurrentItemNum = 0;
					}

					// 임시 데이터를 만들어서 인벤토리에 추가함.
					FHorrorGameItemData TempItem;
					TempItem.Clear();
					Player->Inventory.Add(TempItem);

					// 그 후 i의 값을 다시 줄여서 검사하도록 함.
					i--;
				}

				// 메인 UI의 오브젝트 위젯과 인벤토리 위젯의 상태를 업데이트 함.
				Player->GameUIWidget->SetObjectCount(1, Player->SwordCount);
				Player->CurrentItem();

			}
			// 인벤토리의 현재 인덱스에 위치한 아이템이 청동 방울인 경우
			else if (Player->Inventory[i].ItemNumber == 6)
			{
				// 청동 방울의 개수를 빼고 해당 개수를 인벤토리에도 적용함.
				Player->BellCount--;
				Player->Inventory[i].ItemCount = Player->BellCount;

				// 현재 인벤토리에 아이템이 없는 경우.
				if (Player->Inventory[i].ItemCount == 0)
				{
					// 해당 위치의 인벤토리를 삭제하고, 인벤토리의 최대 인덱스와 현재 가리키고 있는 인덱스를 1 감소시킴.
					Player->Inventory.RemoveAt(i);
					Player->InventoryNum--;
					Player->CurrentItemNum--;
					
					// 인벤토리에 아이템이 있음에도, 현재 가리키고 있는 인벤토리의 인덱스가 0 미만인 경우
					if (Player->CurrentItemNum < 0 && Player->InventoryNum >= 0)
					{
						// 가리키고 있는 인벤토리의 인덱스를 0으로 설정함.
						Player->CurrentItemNum = 0;
					}

					// 만약 인벤토리에 아이템이 없어도 현재 가리키고 있는 인벤토리의 인덱스를 0으로 설정함.
					if (Player->InventoryNum < 0)
					{
						Player->CurrentItemNum = 0;
					}

					// 임시 데이터를 만들어서 인벤토리에 추가함.
					FHorrorGameItemData TempItem;
					TempItem.Clear();
					Player->Inventory.Add(TempItem);

					// 그 후 i의 값을 다시 줄여서 검사하도록 함.
					i--;
				}

				// 메인 UI의 오브젝트 위젯과 인벤토리 위젯의 상태를 업데이트 함.
				Player->GameUIWidget->SetObjectCount(2, Player->BellCount);
				Player->CurrentItem();
			}
			// 인벤토리의 현재 인덱스에 위치한 아이템이 청동 거울인 경우
			else if (Player->Inventory[i].ItemNumber == 7)
			{
				// 청동 거울의 개수를 빼고 해당 개수를 인벤토리에도 적용함.
				Player->MirrorCount--;
				Player->Inventory[i].ItemCount = Player->MirrorCount;

				// 현재 인벤토리에 아이템이 없는 경우.
				if (Player->Inventory[i].ItemCount == 0)
				{
					// 해당 위치의 인벤토리를 삭제하고, 인벤토리의 최대 인덱스와 현재 가리키고 있는 인덱스를 1 감소시킴.
					Player->Inventory.RemoveAt(i);
					Player->InventoryNum--;
					Player->CurrentItemNum--;
					
					// 인벤토리에 아이템이 있음에도, 현재 가리키고 있는 인벤토리의 인덱스가 0 미만인 경우
					if (Player->CurrentItemNum < 0 && Player->InventoryNum >= 0)
					{
						// 가리키고 있는 인벤토리의 인덱스를 0으로 설정함.
						Player->CurrentItemNum = 0;
					}

					// 만약 인벤토리에 아이템이 없어도 현재 가리키고 있는 인벤토리의 인덱스를 0으로 설정함.
					if (Player->InventoryNum < 0)
					{
						Player->CurrentItemNum = 0;
					}

					// 임시 데이터를 만들어서 인벤토리에 추가함.
					FHorrorGameItemData TempItem;
					TempItem.Clear();
					Player->Inventory.Add(TempItem);

					// 그 후 i의 값을 다시 줄여서 검사하도록 함.
					i--;
				}

				// 메인 UI의 오브젝트 위젯과 인벤토리 위젯의 상태를 업데이트 함.
				Player->GameUIWidget->SetObjectCount(3, Player->MirrorCount);
				Player->CurrentItem();
			}
		}
		// Ending Mirror가 존재한 상황이면 클리어가 가능한 상태임을 알림.
		if (IsValid(EndingMirror)) 
		{
			EndingMirror->SetIsCleared(true);
		}
	}
	// 필요한 오브젝트 아이템을 모으지 못했다면, 아이템이 필요하다고 알림.
	else
	{
		Player->SetErrorText(NSLOCTEXT("AAltar_cpp", "Altar_Error", "You Need 1 Sword, 1 Mirror, 1 Bell!"), 3);
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
		CurrentReaper->UnSealedItemNumber = UnSealedItemNum;
	}

	// 만약 레벨이 시작한 상태가 아니면, 레벨이 시작되었다고 설정함.
	if (!bIsLevelStart)
	{
		bIsLevelStart = true;
	}
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
	// 봉인 해제된 아이템의 개수를 더함.
	UnSealedItemNum += value;

	// 현재 리퍼가 맵에 존재한다면, 리퍼의 봉인 여부를 설정함.
	if (CurrentReaper)
	{
		CurrentReaper->UnSealedItemNumber = UnSealedItemNum;
	}

	// 봉인 해제된 아이템이 3개일 경우
	if (UnSealedItemNum == 3)
	{
		// 현재 리퍼가 존재하지 않는다면(처치되었거나, 소멸 등) 리퍼를 스폰 지점에 즉시 리스폰 시킴.
		if (CurrentReaper == nullptr)
		{
			FActorSpawnParameters spawnParams;
			CurrentReaper = GetWorld()->SpawnActor<AReaper_cpp>(Reaper, SpawnPoint->GetComponentLocation(), FRotator(0.f, 0.f, 0.f), spawnParams);
			RespawnTimer = 0.f;
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