// CopyrightNotice=0 2023 Sunggon Kim kimdave205@gmail.com

#include "ComponentAction/ItemManager.h"
#include "LevelManager.h"
#include "EngineUtils.h"
#include "Drawer_cpp.h"
#include "DrawerClass.h"
#include "Locker_cpp.h"
#include "WardrobeDrawer_cpp.h"
#include "DeskDrawer_cpp.h"
#include "Shelf.h"

// Sets default values
AItemManager::AItemManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void AItemManager::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
//void AItemManager::Tick(float DeltaTime)
//{
//	Super::Tick(DeltaTime);
//
//}

void AItemManager::BindItemSetting(ALevelManager* LevelManager)
{
	if (!bIsBinded)
	{
		LevelManager->LevelStartDelegate.AddDynamic(this, &AItemManager::ItemSetting);
		bIsBinded = true;
	}
}

void AItemManager::ItemSetting()
{
	for (int i = 0; i < Items.Num(); ++i)
	{
		int32 MinVal = 0;
		if (ItemNumbers[i] >= 1)
		{
			MinVal = 1;
		}
		int32 MaxVal = ItemNumbers[i];
		int32 RandItemNumber = FMath::RandRange(MinVal, MaxVal);
		//while (ItemNumbers[i]) // 아이템 개수가 0이 될때까지 배치시킬 오브젝트 찾는 로직
		while (RandItemNumber) // 아이템 개수가 0이 될때까지 배치시킬 오브젝트 찾는 로직
		{
			int32 RandIdx = FMath::RandRange(0, Objects.Num() - 1); // 특정 오브젝트의 랜덤 값을 가져옴
			bool Result = false;
			if (ADrawerClass* Drawer = Cast<ADrawerClass>(Objects[RandIdx]))
			{
				Result = Drawer->SetSpawnItem(Items[i]);
			}
			else if (ALocker_cpp* Locker = Cast<ALocker_cpp>(Objects[RandIdx]))
			{
				Result = Locker->SetSpawnItem(Items[i]);
				//Locker->Item->SetChildActorClass(Items[i]);
			}
			else if (AShelf* Shelf = Cast<AShelf>(Objects[RandIdx]))
			{
				Result = Shelf->SetSpawnItem(Items[i]);
				//int32 RandPosition = FMath::RandRange(0, 2); // 0: Lower, 1: Middle, 2: Upper

				//if (RandPosition == 0)
				//{
				//	Shelf->Lower_Item->SetChildActorClass(Items[i]);
				//}
				//else if (RandPosition == 1)
				//{
				//	Shelf->Middle_Item->SetChildActorClass(Items[i]);
				//}
				//else if (RandPosition == 2)
				//{
				//	Shelf->Upper_Item->SetChildActorClass(Items[i]);
				//}
			}

			if (Result) // 아이템을 성공적으로 스폰 시켰을 때만
			{
				RandItemNumber--; // 남은 아이템 개수를 하나씩 줄임
				//ItemNumbers[i]--; // 남은 아이템 개수를 하나씩 줄임
			}
		}
	}
}