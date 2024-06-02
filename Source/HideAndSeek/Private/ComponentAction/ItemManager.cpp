// CopyrightNotice 2023 Sunggon Kim kimdave205@gmail.com. All Rights Reserved.

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

// Level Manager에 델리게이트를 바인딩할 함수.
void AItemManager::BindItemSetting(ALevelManager* LevelManager)
{
	// 바인딩 안 된 경우, 델리케이트에 바인딩시킴.
	if (!bIsBinded)
	{
		LevelManager->LevelStartDelegate.AddDynamic(this, &AItemManager::ItemSetting);
		bIsBinded = true;
	}
}

// 맵 생성이 완료된 후 작동될 함수로 Level Manager에 바인딩될 함수.
void AItemManager::ItemSetting()
{
	// 아이템 하나 씩 수행함
	for (int i = 0; i < Items.Num(); ++i)
	{
		// 각 아이템별로 맵에 배치될 최소 개수는 1개임. 하지만 0개가 배치되는 겅우, 최소 개수는 0개임
		int32 MinVal = 0;
		if (ItemNumbers[i] >= 1)
		{
			MinVal = 1;
		}
		// 아이템의 최대 개수는 배열에 있는 개수임.
		int32 MaxVal = ItemNumbers[i];

		// 최소 개수와 최대 개수의 사이에서 랜덤한 값 하나를 가져와서 해당 아이템이 배치될 개수를 확정지음.
		int32 RandItemNumber = FMath::RandRange(MinVal, MaxVal);
		
		// 아이템 개수가 0이 될때까지 배치시킬 오브젝트 찾는 로직
		while (RandItemNumber)
		{
			// 배치시킬 오브젝트에 접근하기 위한 랜덤 인덱스 값을 가져옴
			int32 RandIdx = FMath::RandRange(0, Objects.Num() - 1);
			bool Result = false;
			
			// 해당 인덱스의 오브젝트가 서랍류일 경우, 해당 서랍 류에 배치함.
			if (ADrawerClass* Drawer = Cast<ADrawerClass>(Objects[RandIdx]))
			{
				Result = Drawer->SetSpawnItem(Items[i]);
			}
			// 해당 인덱스의 오브젝트가 사물함일 경우, 해당 사물함에 배치함.
			else if (ALocker_cpp* Locker = Cast<ALocker_cpp>(Objects[RandIdx]))
			{
				Result = Locker->SetSpawnItem(Items[i]);
			}
			// 해당 인덱스의 오브젝트가 선반일 경우, 해당 선반에 배치함.
			else if (AShelf* Shelf = Cast<AShelf>(Objects[RandIdx]))
			{
				Result = Shelf->SetSpawnItem(Items[i]);
			}

			// 아이템을 성공적으로 스폰 시켰을 때만 남은 아이템 개수를 하나씩 줄임
			if (Result)
			{
				RandItemNumber--;
			}
		}
	}
}