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

void AAltar_cpp::OnInteract(class AHorrorGameCharacter* Player)
{
	if (Player->GetSwordNumbers() && Player->GetMirrorNumbers() && Player->GetBellNumbers())
	{
		//Destroy();
		if (InteractSound)
		{
			InteractSound->Play();
		}
		Sword->SetVisibility(true);
		Mirror->SetVisibility(true);
		Bell->SetVisibility(true);

		Player->GameUIWidget->TimerWidget->StopTimer(); // 타이머 정지

		for (int i = 0; i < Player->Inventory.Num(); ++i) // 플레이어의 인벤토리 순회하며 아이템 제거
		{
			if (Player->Inventory[i].ItemNumber == 5) // 칼인 경우
			{
				Player->SwordCount--;
				Player->Inventory[i].ItemCount = Player->SwordCount;
				if (Player->Inventory[i].ItemCount == 0)
				{
					Player->Inventory.RemoveAt(i);
					Player->InventoryNum--;
					Player->CurrentItemNum--;
					if (Player->CurrentItemNum < 0 && Player->InventoryNum >= 0)
					{
						Player->CurrentItemNum = 0;
					}

					if (Player->InventoryNum < 0)
					{
						Player->CurrentItemNum = 0;
					}

					FHorrorGameItemData TempItem;
					TempItem.Clear();
					Player->Inventory.Add(TempItem);

				}

				Player->GameUIWidget->SetObjectCount(1, Player->SwordCount);
				Player->CurrentItem();
			}

			if (Player->Inventory[i].ItemNumber == 6) // 방울인 경우
			{
				Player->BellCount--;
				Player->Inventory[i].ItemCount = Player->BellCount;
				if (Player->Inventory[i].ItemCount == 0)
				{
					Player->Inventory.RemoveAt(i);
					Player->InventoryNum--;
					Player->CurrentItemNum--;
					if (Player->CurrentItemNum < 0 && Player->InventoryNum >= 0)
					{
						Player->CurrentItemNum = 0;
					}

					if (Player->InventoryNum < 0)
					{
						Player->CurrentItemNum = 0;
					}

					FHorrorGameItemData TempItem;
					TempItem.Clear();
					Player->Inventory.Add(TempItem);
				}

				Player->GameUIWidget->SetObjectCount(2, Player->BellCount);
				Player->CurrentItem();
			}

			if (Player->Inventory[i].ItemNumber == 7) // 거울인 경우
			{
				Player->MirrorCount--;
				Player->Inventory[i].ItemCount = Player->MirrorCount;
				if (Player->Inventory[i].ItemCount == 0)
				{
					Player->Inventory.RemoveAt(i);
					Player->InventoryNum--;
					Player->CurrentItemNum--;
					if (Player->CurrentItemNum < 0 && Player->InventoryNum >= 0)
					{
						Player->CurrentItemNum = 0;
					}

					if (Player->InventoryNum < 0)
					{
						Player->CurrentItemNum = 0;
					}

					FHorrorGameItemData TempItem;
					TempItem.Clear();
					Player->Inventory.Add(TempItem);
				}

				Player->GameUIWidget->SetObjectCount(3, Player->MirrorCount);
				Player->CurrentItem();
			}
		}

		if (IsValid(EndingMirror)) // Ending Mirror가 존재한 상황이면
		{
			EndingMirror->SetIsCleared(true);
		}
	}
	else
	{
		Player->SetErrorText(NSLOCTEXT("AAltar_cpp", "Altar_Error", "You Need 1 Sword, 1 Mirror, 1 Bell!"), 3);
	}
}

void AAltar_cpp::ReaperSpawn()
{
	FActorSpawnParameters spawnParams;

	//if(Reaper != nullptr)
	//CurrentReaper = GetWorld()->SpawnActor<AReaper_cpp>(Reaper, GetActorLocation()+GetActorForwardVector()*300.f, FRotator(0.f, 0.f, 0.f), spawnParams);
	CurrentReaper = GetWorld()->SpawnActor<AReaper_cpp>(Reaper, SpawnPoint->GetComponentLocation(), FRotator(0.f, 0.f, 0.f), spawnParams);
	bIsLevelStart = true;
}
// Called every frame
void AAltar_cpp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (CurrentReaper == nullptr && bIsLevelStart)
	{
		RespawnTimer += DeltaTime;

		if (RespawnTimer >= 20.f)
		{
			ReaperSpawn();
			RespawnTimer = 0.f;
		}
	}
}

void AAltar_cpp::UnSealedObjectNumber(int32 value)
{
	UnSealedItemNum += value;
	if (CurrentReaper)
	{
		CurrentReaper->UnSealedItemNumber = UnSealedItemNum;
	}

	if (UnSealedItemNum == 3)
	{
		if (CurrentReaper == nullptr)
		{
			FActorSpawnParameters spawnParams;
			CurrentReaper = GetWorld()->SpawnActor<AReaper_cpp>(Reaper, SpawnPoint->GetComponentLocation(), FRotator(0.f, 0.f, 0.f), spawnParams);
		}
		else
		{
			CurrentReaper->SetActorLocation(SpawnPoint->GetComponentLocation());
		}
		PlayerCharacter->OnAnnounce();
	}
}