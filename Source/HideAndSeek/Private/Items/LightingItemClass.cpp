// CopyrightNotice 2023 Sunggon Kim kimdave205@gmail.com. All Rights Reserved.

#include "Items/LightingItemClass.h"
#include "Player/HorrorGameCharacter.h"
//#include "Kismet/GameplayStatics.h"
//#include "ComponentAction/HorrorGameSaveGame.h"
//#include "LevelManager/HorrorGameGameInstance.h"

// Sets default values
ALightingItemClass::ALightingItemClass()
{
 	
}

// 플레이어가 타이머 아이템을 습득하려 할 때 작동할 함수.
void ALightingItemClass::OnInteract(class AHorrorGameCharacter* Player) // Player Click Event
{
	bool Result = Player->TakeItems(this);

	if (Result)
	{
		Super::OnInteract(Player);
		//Destroy();
	}

	//UHorrorGameGameInstance* GameInstance = Cast<UHorrorGameGameInstance>(GetGameInstance());

	//if (GameInstance)
	//{
	//	UHorrorGameSaveGame* SaveData = GameInstance->GetSaveData();
	//	// 아이템을 처음 얻은 상태라면 해당 아이템 문서를 세이브 데이터에 영구히 저장함.
	//	if (SaveData)
	//	{
	//		// 아이템이 라이터인 경우
	//		if (ItemData.ItemNumber == EItemNumber::ITEM_CigarLighter)
	//		{
	//			if (!SaveData->CollectArchives.Item1_CigarLighter)
	//			{
	//				SaveData->CollectArchives.Item1_CigarLighter = true;

	//			}
	//		}
	//		if (!SaveData->CollectArchives.Item9_Cutter)
	//		{
	//			SaveData->CollectArchives.Item9_Cutter = true;
	//			Player->SetArchiveGetText(NSLOCTEXT("ACutter_cpp", "Get_Cutter", "Cutter\nis added in archive"));
	//			SaveData->SaveData();
	//		}
	//	}
	//}
}

bool ALightingItemClass::UseInteract(class AHorrorGameCharacter* Player)
{
	return true;
}

void ALightingItemClass::HoldInteract(class AHorrorGameCharacter* Player)
{
	bool Result = Player->ChangeItems(this);

	if (Result)
	{
		// Play Sound

		// Move to Player And Destroy
		Super::HoldInteract(Player);
		//Destroy();
	}
}

FHorrorGameItemData ALightingItemClass::GetItemData()
{
	Super::GetItemData();

	return ItemData;
}