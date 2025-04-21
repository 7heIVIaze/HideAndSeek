// CopyrightNotice 2024 Sunggon Kim kimdave205@gmail.com. All Rights Reserved.

#include "Player/InventoryComponent.h"
#include "Player/HorrorGameCharacter.h"
#include "Items/PassiveItemClass.h"
#include "Items/LightingItemClass.h"
#include "Items/ConsumableItemClass.h"
#include "Items/LightItem.h"
#include "Items/ActiveItem.h"
#include "Resource/InteractInterface.h"
#include "LevelManager/HorrorGameGameInstance.h"
#include "ComponentAction/HorrorGameSaveGame.h"
#include "Furniture/DoorClass.h"
#include "Furniture/LockerDoorActor_cpp.h"
#include "HUD/GameUI.h"

// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// Defaults
	ConsumableInventoryMaxSize = 5;
	LightingInventoryMaxSize = 2;
	PassiveInventoryMaxSize = 1;
	CurrentInventoryType = 1;
	LightingInventoryNumber = 0;
	ConsumableInventoryNumber = 0;
	CurrentConsumableItemIndex = 0;
	CurrentLightingItemIndex = 0;
	PassiveItemNumber = 0;
	bDoHaveCompass = false;
	bIsExtinguisherSpraying = false;

	// 소비형 아이템 인벤토리 슬롯의 개수는 기본적으로 5개로 설정.
	Consumable_Inventory.Init(FHorrorGameItemData::FHorrorGameItemData(), ConsumableInventoryMaxSize);

	// 조명 아이템 인벤토리 슬롯의 개수는 기본적으로 2개로 설정.
	Lighting_Inventory.Init(FHorrorGameItemData::FHorrorGameItemData(), LightingInventoryMaxSize);

	// 패시브(장비형) 아이템 인벤토리의 슬롯의 개수는 기본적으로 1개로 설정,
	Passive_Inventory.Init(FHorrorGameItemData::FHorrorGameItemData(), PassiveInventoryMaxSize);
	// ...
}


// Called when the game starts
void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerPlayer = Cast<AHorrorGameCharacter>(GetOwner());

	GameUIWidget = OwnerPlayer->GameUIWidget;

	if (GameUIWidget)
	{
		OnItemMessage.AddDynamic(GameUIWidget, &UGameUI::ShowMessage);
	}

	// 소비형 아이템 인벤토리 슬롯의 개수는 기본적으로 5개로 설정.
	Consumable_Inventory.Init(FHorrorGameItemData::FHorrorGameItemData(), ConsumableInventoryMaxSize);

	// 조명 아이템 인벤토리 슬롯의 개수는 기본적으로 2개로 설정.
	Lighting_Inventory.Init(FHorrorGameItemData::FHorrorGameItemData(), LightingInventoryMaxSize);

	// 패시브(장비형) 아이템 인벤토리의 슬롯의 개수는 기본적으로 1개로 설정,
	Passive_Inventory.Init(FHorrorGameItemData::FHorrorGameItemData(), PassiveInventoryMaxSize);

	GameInstance = Cast<UHorrorGameGameInstance>(OwnerPlayer->GetGameInstance());
}


// Called every frame
void UInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

bool UInventoryComponent::AddToInventory(class AItemClass* Item, class AHorrorGameCharacter* Player)
{
	int result = 0;
	FHorrorGameItemData CollectedItemData = Item->GetItemData();
	
	// 현재 아이템이 소비형 아이템인 경우
	if (CollectedItemData.ItemType == EItemType::ITEM_Consumable)
	{
		// 아이템 번호 확인.
		switch (CollectedItemData.ItemNumber)
		{
			// 열쇠 아이템인 경우
			case EItemNumber::ITEM_Key:
			{
				result = AddKey(Item, ConsumableInventoryNumber, Player);

				// 인벤토리 추가에 성공하면, 다음에 저장할 인덱스를 증가시킴.
				if (result == 1)
				{
					ConsumableInventoryNumber++;

					return true;
				}
				// 이미 인벤토리에 있는 경우 다음 저장 위치 인덱스를 증가시킬 필요가 없음.
				else if (result == 2)
				{
					return true;
				}

				// 실패한 경우
				return false;
			}
			// 소음 아이템인 경우
			case EItemNumber::ITEM_Timer:
			{
				result = AddTimer(Item, ConsumableInventoryNumber, Player);

				// 인벤토리 추가에 성공하면, 다음에 저장할 인덱스를 증가시킴.
				if (result == 1)
				{
					ConsumableInventoryNumber++;

					return true;
				}
				// 이미 인벤토리에 있는 경우 다음 저장 위치 인덱스를 증가시킬 필요가 없음.
				else if (result == 2)
				{
					return true;
				}

				// 실패한 경우
				return false;
			}
			// 청동 검인 경우
			case EItemNumber::ITEM_Sword:
			{
				result = AddSword(Item, ConsumableInventoryNumber, Player);

				// 인벤토리 추가에 성공하면, 다음에 저장할 인덱스를 증가시킴.
				if (result == 1)
				{
					ConsumableInventoryNumber++;

					return true;
				}

				// 이미 인벤토리에 존재하거나 얻는데 실패한 경우
				return false;
			}
			// if the item is a mirror
			case EItemNumber::ITEM_Mirror:
			{
				result = AddMirror(Item, ConsumableInventoryNumber, Player);

				// 인벤토리 추가에 성공하면, 다음에 저장할 인덱스를 증가시킴.
				if (result == 1)
				{
					ConsumableInventoryNumber++;

					return true;
				}

				// 이미 인벤토리에 존재하거나 얻는데 실패한 경우
				return false;
			}
			// If the item is a extinguisher
			case EItemNumber::ITEM_Extinguisher:
			{
				result = AddExtinguisher(Item, ConsumableInventoryNumber, Player);

				// 인벤토리 추가에 성공하면, 다음에 저장할 인덱스를 증가시킴.
				if (result == 1)
				{
					ConsumableInventoryNumber++;
					UE_LOG(LogTemp, Warning, TEXT("ConsumableInventoryNumber: %d"), ConsumableInventoryNumber);
					return true;
				}

				// 이미 인벤토리에 존재하거나 얻는데 실패한 경우
				return false;
			}
			// If the item is a cutter
			case EItemNumber::ITEM_Cutter:
			{
				result = AddKey(Item, ConsumableInventoryNumber, Player);

				// 인벤토리 추가에 성공하면, 다음에 저장할 인덱스를 증가시킴.
				if (result == 1)
				{
					ConsumableInventoryNumber++;

					return true;
				}
				
				// 이미 인벤토리에 있는 경우 다음 저장 위치 인덱스를 증가시킬 필요가 없음.
				else if (result == 2)
				{
					return true;
				}

				// 실패한 경우
				return false;
			}
			// If the item is a glowstick
			case EItemNumber::ITEM_Glowstick:
			{
				result = AddGlowstick(Item, ConsumableInventoryNumber, Player);

				// 인벤토리 추가에 성공하면, 다음에 저장할 인덱스를 증가시킴.
				if (result == 1)
				{
					ConsumableInventoryNumber++;

					return true;
				}
				
				// 이미 인벤토리에 있는 경우 다음 저장 위치 인덱스를 증가시킬 필요가 없음.
				else if (result == 2)
				{
					return true;
				}

				// 실패한 경우
				return false;
			}
		}

		// 예외 사항 처리
		return false;
	}
	// 조명 아이템 클래스인 경우
	else if (CollectedItemData.ItemType == EItemType::ITEM_Lighting)
	{
		// 아이템 번호 확인.
		switch (CollectedItemData.ItemNumber)
		{
			// 라이터인 경우
			case EItemNumber::ITEM_CigarLighter:
			{
				result = AddCigarLighter(Item, LightingInventoryNumber, Player);

				// 인벤토리 추가에 성공하면, 다음에 저장할 인덱스를 증가시킴.
				if (result == 1)
				{
					LightingInventoryNumber++;

					return true;
				}

				// 이미 인벤토리에 존재하거나 얻는데 실패한 경우
				return false;
			}
			// 손전등인 경우
			case EItemNumber::ITEM_FlashLight:
			{
				result = AddFlashlight(Item, LightingInventoryNumber, Player);

				// 인벤토리 추가에 성공하면, 다음에 저장할 인덱스를 증가시킴.
				if (result == 1)
				{
					LightingInventoryNumber++;

					return true;
				}

				// 이미 인벤토리에 존재하거나 얻는데 실패한 경우
				return false;
			}
			// 랜턴인 경우
			case EItemNumber::ITEM_Lantern:
			{
				result = AddLantern(Item, LightingInventoryNumber, Player);

				// 인벤토리 추가에 성공하면, 다음에 저장할 인덱스를 증가시킴.
				if (result == 1)
				{
					LightingInventoryNumber++;

					return true;
				}

				// 이미 인벤토리에 존재하거나 얻는데 실패한 경우
				return false;
			}
		}

		// 예외 사항 처리
		return false;
	}
	// 패시브(장비형) 아이템인 경우
	else if (CollectedItemData.ItemType == EItemType::ITEM_Passive)
	{
		// 아이템 번호 확인.
		switch (CollectedItemData.ItemNumber)
		{
			// 청동 방울인 경우
			case EItemNumber::ITEM_Bell:
			{
				result = AddBell(Item, PassiveItemNumber, Player);

				// 인벤토리 추가에 성공하면, 다음에 저장할 인덱스를 증가시킴.
				if (result == 1)
				{
					PassiveItemNumber++;

					return true;
				}
				
				// 이미 인벤토리에 존재하거나 얻는데 실패한 경우
				return false;
			}
		}
	}
	// 특수 아이템인 경우
	else if (CollectedItemData.ItemType == EItemType::ITEM_Unique)
	{
		// 아이템 번호 확인
		switch (CollectedItemData.ItemNumber)
		{
			// 나침반인 경우
			case EItemNumber::ITEM_Compass:
			{
				result = AddCompass(Item, Player);

				// 나침반을 얻은 경우
				if (result == 1)
				{
					bDoHaveCompass = true;
					return true;
				}

				// 실패한 경우
				return false;
			}
			// 환혼석인 경우
			case EItemNumber::ITEM_ResurrectionStone:
			{
				result = AddStone(Item, Player);

				// 환혼석을 수집한 경우
				if (result == 1)
				{
					return true;
				}

				// 실패한 경우
				return false;
			}
		}

		// 예외 사항.
		return false;
	}

	// 예외 사항 처리.
	return false;
}

bool UInventoryComponent::ChangeCurrentItem(class AItemClass* ChangeItem, class AHorrorGameCharacter* Player)
{
	// 현재 선택 중인 아이템을 다른 아이템으로 변경하는 로직,
	int result = 0;
	bool bResult = false;
	FVector ItemLocation = ChangeItem->GetActorLocation();
	FRotator ItemRotation = ChangeItem->GetActorRotation();
	FHorrorGameItemData ChangeItemData = ChangeItem->GetItemData();
	FHorrorGameItemData* ExistingItem = nullptr;
	bool bAlreadyHaveItem = false;
	bool bHasSpace = false;
	int ExistingItemIndex = -1;

	// 소비형 아이템인 경우
	if (ChangeItemData.ItemType == EItemType::ITEM_Consumable)
	{
		// 인벤토리 내에 추가 공간이 있는지 확인.
		bHasSpace = ConsumableInventoryNumber < ConsumableInventoryMaxSize;
		
		// 또한 인벤토리 내에 해당 아이템이 있는지 확인.
		for (int i = 0; i < ConsumableInventoryNumber; ++i)
		{
			if (Consumable_Inventory[i].ItemNumber == ChangeItemData.ItemNumber)
			{
				ExistingItem = &Consumable_Inventory[i];
				ExistingItemIndex = i;

				bAlreadyHaveItem = true;
				break;
			}
		}

		// 바꿀 아이템 정보.
		FHorrorGameItemData DiscardedItemData = Consumable_Inventory[CurrentConsumableItemIndex];

		// 획득할 아이템 정보 확인.
		switch (ChangeItemData.ItemNumber)
		{
			// 열쇠인 경우
			case EItemNumber::ITEM_Key:
			{
				// 이미 인벤토리에 존재하는 경우
				if (bAlreadyHaveItem)
				{
					// 열쇠는 추가 획득이 가능함. 따라서 개수 증가.
					ExistingItem->ItemCount += ChangeItemData.ItemCount;

					OnItemAcquired.Broadcast(Consumable_Inventory[CurrentConsumableItemIndex], CurrentConsumableItemIndex);

					return true;
				}
				// 인벤토리에 없는 경우
				else
				{
					// 인벤토리 내에 아직 공간이 남아있는 경우
					if (bHasSpace)
					{
						return AddToInventory(ChangeItem, Player);
					}
					// 인벤토리 내에 공간이 없는 경우
					else
					{
						result = ChangeKey(ChangeItem, CurrentConsumableItemIndex, Player);
					}
					break;
				}
			}
			// 소음 유발 아이템인 경우
			case EItemNumber::ITEM_Timer:
			{
				// 이미 인벤토리에 존재하는 경우
				if (bAlreadyHaveItem)
				{
					// 소음 아이템은 추가 획득이 가능함. 따라서 개수 증가.
					ExistingItem->ItemCount += ChangeItemData.ItemCount;

					OnItemAcquired.Broadcast(Consumable_Inventory[CurrentConsumableItemIndex], CurrentConsumableItemIndex);

					return true;
				}
				// 인벤토리에 없는 경우
				else
				{
					// 인벤토리 내에 아직 공간이 남아있는 경우
					if (bHasSpace)
					{
						return AddToInventory(ChangeItem, Player);
					}
					// 인벤토리 내에 공간이 없는 경우
					else
					{
						result = ChangeTimer(ChangeItem, CurrentConsumableItemIndex, Player);
					}
					break;
				}
			}
			// 청동 검인 경우
			case EItemNumber::ITEM_Sword:
			{
				// 이미 인벤토리에 존재하는 경우
				if (bAlreadyHaveItem)
				{
					// 검 아이템은 추가 획득이 불가능함.
					result = ChangeSword(ChangeItem, ExistingItemIndex, Player);

					break;
				}
				// 인벤토리에 없는 경우
				else
				{
					// 인벤토리 내에 아직 공간이 남아있는 경우
					if (bHasSpace)
					{
						return AddToInventory(ChangeItem, Player);
					}
					// 인벤토리 내에 공간이 없는 경우
					else
					{
						result = ChangeSword(ChangeItem, CurrentConsumableItemIndex, Player);
					}
					break;
				}
			}
			// 청동 거울인 경우
			case EItemNumber::ITEM_Mirror:
			{
				// 이미 인벤토리에 존재하는 경우
				if (bAlreadyHaveItem)
				{
					// 거울 아이템은 추가 획득이 불가능함.
					result = ChangeMirror(ChangeItem, ExistingItemIndex, Player);

					break;
				}
				// 인벤토리에 없는 경우
				else
				{
					// 인벤토리 내에 아직 공간이 남아있는 경우
					if (bHasSpace)
					{
						return AddToInventory(ChangeItem, Player);
					}
					// 인벤토리 내에 공간이 없는 경우
					else
					{
						result = ChangeMirror(ChangeItem, CurrentConsumableItemIndex, Player);
					}
					break;
				}
			}
			// 소화기인 경우
			case EItemNumber::ITEM_Extinguisher:
			{
				// 이미 인벤토리에 존재하는 경우
				if (bAlreadyHaveItem)
				{
					// 소화기 아이템은 추가 획득이 불가능함.
					result = ChangeExtinguisher(ChangeItem, ExistingItemIndex, Player);

					break;
				}
				// 인벤토리에 없는 경우
				else
				{
					// 인벤토리 내에 아직 공간이 남아있는 경우
					if (bHasSpace)
					{
						return AddToInventory(ChangeItem, Player);
					}
					// 인벤토리 내에 공간이 없는 경우
					else
					{
						result = ChangeExtinguisher(ChangeItem, CurrentConsumableItemIndex, Player);
					}
					break;
				}
			}
			// 절단기인 경우
			case EItemNumber::ITEM_Cutter:
			{
				// 이미 인벤토리에 존재하는 경우
				if (bAlreadyHaveItem)
				{
					// 절단기 아이템은 추가 획득이 불가능함.
					result = ChangeCutter(ChangeItem, ExistingItemIndex, Player);

					break;
				}
				// 인벤토리에 없는 경우
				else
				{
					// 인벤토리 내에 아직 공간이 남아있는 경우
					if (bHasSpace)
					{
						return AddToInventory(ChangeItem, Player);
					}
					// 인벤토리 내에 공간이 없는 경우
					else
					{
						result = ChangeCutter(ChangeItem, CurrentConsumableItemIndex, Player);
					}
					break;
				}
			}
			// 야광봉인 경우
			case EItemNumber::ITEM_Glowstick:
			{
				// 이미 인벤토리에 존재하는 경우
				if (bAlreadyHaveItem)
				{
					// 야광봉 아이템은 추가 획득이 가능함. 따라서 개수 증가.
					ExistingItem->ItemCount += ChangeItemData.ItemCount;

					OnItemAcquired.Broadcast(Consumable_Inventory[CurrentConsumableItemIndex], CurrentConsumableItemIndex);

					return true;
				}
				// 인벤토리에 없는 경우
				else
				{
					// 인벤토리 내에 아직 공간이 남아있는 경우
					if (bHasSpace)
					{
						return AddToInventory(ChangeItem, Player);
					}
					// 인벤토리 내에 공간이 없는 경우
					else
					{
						result = ChangeGlowstick(ChangeItem, CurrentConsumableItemIndex, Player);
					}
					break;
				}
			}
		}

		// 인벤토리 추가가 완료되면 획득할 아이템이 위치한 위치에 아이템을 생성함.
		if (result == 1)
		{
			UE_LOG(LogTemp, Warning, TEXT("Change Success"));
			// 바꾼 아이템 위치에 버린 아이템을 생성함.
			AItemClass* DiscardedItem = GetWorld()->SpawnActor<AItemClass>(DiscardedItemData.ItemBPClass, ItemLocation, ItemRotation);

			// 생성이 완료된 경우 해당 아이템의 아이템 정보 갱신.
			if (DiscardedItem)
			{
				DiscardedItem->ItemData = DiscardedItemData;
				UE_LOG(LogTemp, Warning, TEXT("Discard Success"));
				return true;
			}
		}

		// 실패한 경우
		return false;
	}
	// 조명 아이템인 경우
	else if (ChangeItemData.ItemType == EItemType::ITEM_Lighting)
	{
		// 인벤토리 내에 추가 공간이 있는지 확인.
		bHasSpace = LightingInventoryNumber < LightingInventoryMaxSize;

		// 또한 인벤토리 내에 해당 아이템이 있는지 확인.
		for (int i = 0; i < LightingInventoryNumber; ++i)
		{
			if (Lighting_Inventory[i].ItemNumber == ChangeItemData.ItemNumber)
			{
				ExistingItem = &Lighting_Inventory[i];
				ExistingItemIndex = i;

				bAlreadyHaveItem = true;
				break;
			}
		}

		// 바꿀 아이템 정보.
		FHorrorGameItemData DiscardedItemData = Lighting_Inventory[CurrentLightingItemIndex];

		// 아이템 번호 확인
		switch (ChangeItemData.ItemNumber)
		{
			// 라이터인 경우
			case EItemNumber::ITEM_CigarLighter:
			{
				// 이미 인벤토리에 존재하는 경우
				if (bAlreadyHaveItem)
				{
					// 라이터 아이템은 추가 획득이 불가능함.
					result = ChangeCigarLighter(ChangeItem, ExistingItemIndex, Player);

					break;
				}
				// 인벤토리에 없는 경우
				else
				{
					// 인벤토리 내에 아직 공간이 남아있는 경우
					if (bHasSpace)
					{
						return AddToInventory(ChangeItem, Player);
					}
					// 인벤토리 내에 공간이 없는 경우
					else
					{
						result = ChangeCigarLighter(ChangeItem, CurrentLightingItemIndex, Player);
					}
					break;
				}
			}
			// 손전등인 경우
			case EItemNumber::ITEM_FlashLight:
			{
				// 이미 인벤토리에 존재하는 경우
				if (bAlreadyHaveItem)
				{
					// 손전등 아이템은 추가 획득이 불가능함.
					result = ChangeFlashlight(ChangeItem, ExistingItemIndex, Player);

					break;
				}
				// 인벤토리에 없는 경우
				else
				{
					// 인벤토리 내에 아직 공간이 남아있는 경우
					if (bHasSpace)
					{
						return AddToInventory(ChangeItem, Player);
					}
					// 인벤토리 내에 공간이 없는 경우
					else
					{
						result = ChangeFlashlight(ChangeItem, CurrentLightingItemIndex, Player);
					}
					break;
				}
			}
			// 랜턴인 경우
			case EItemNumber::ITEM_Lantern:
			{
				// 이미 인벤토리에 존재하는 경우
				if (bAlreadyHaveItem)
				{
					// 랜턴 아이템은 추가 획득이 불가능함.
					result = ChangeLantern(ChangeItem, ExistingItemIndex, Player);

					break;
				}
				// 인벤토리에 없는 경우
				else
				{
					// 인벤토리 내에 아직 공간이 남아있는 경우
					if (bHasSpace)
					{
						return AddToInventory(ChangeItem, Player);
					}
					// 인벤토리 내에 공간이 없는 경우
					else
					{
						result = ChangeLantern(ChangeItem, CurrentLightingItemIndex, Player);
					}
					break;
				}
			}
		}

		// 인벤토리 추가가 완료되면 획득할 아이템이 위치한 위치에 아이템을 생성함.
		if (result == 1)
		{
			UE_LOG(LogTemp, Warning, TEXT("Change Success"));
			// 바꾼 아이템 위치에 버린 아이템을 생성함.
			AItemClass* DiscardedItem = GetWorld()->SpawnActor<AItemClass>(DiscardedItemData.ItemBPClass, ItemLocation, ItemRotation);

			// 생성이 완료된 경우 해당 아이템의 아이템 정보 갱신.
			if (DiscardedItem)
			{
				DiscardedItem->ItemData = DiscardedItemData;
				UE_LOG(LogTemp, Warning, TEXT("Discard Success"));
				return true;
			}
		}

		// 실패한 경우
		return false;
	}
	// 패시브 아이템인 경우
	else if (ChangeItemData.ItemType == EItemType::ITEM_Passive)
	{
		// 인벤토리 내에 추가 공간이 있는지 확인.
		bHasSpace = PassiveItemNumber < PassiveInventoryMaxSize;

		// 또한 인벤토리 내에 해당 아이템이 있는지 확인.
		for (int i = 0; i < PassiveItemNumber; ++i)
		{
			if (Passive_Inventory[i].ItemNumber == ChangeItemData.ItemNumber)
			{
				ExistingItemIndex = i;

				bAlreadyHaveItem = true;
				break;
			}
		}

		// 바꿀 아이템 정보
		FHorrorGameItemData DiscardedItemData = Lighting_Inventory[CurrentLightingItemIndex];

		// 아이템 번호 확인
		switch (ChangeItemData.ItemNumber)
		{
			// 청동 방울인 경우
			case EItemNumber::ITEM_Bell:
			{
				// 이미 인벤토리에 존재하는 경우
				if (bAlreadyHaveItem)
				{
					// 방울 아이템은 추가 획득이 불가능함.
					result = ChangeBell(ChangeItem, ExistingItemIndex, Player);

					break;
				}
				// 인벤토리에 없는 경우
				else
				{
					// 인벤토리 내에 아직 공간이 남아있는 경우
					if (bHasSpace)
					{
						return AddToInventory(ChangeItem, Player);
					}
					// 인벤토리 내에 공간이 없는 경우
					else
					{
						result = ChangeBell(ChangeItem, PassiveItemNumber, Player);
					}
					break;
				}
			}
		}

		// 인벤토리 추가가 완료되면 획득할 아이템이 위치한 위치에 아이템을 생성함.
		if (result == 1)
		{
			UE_LOG(LogTemp, Warning, TEXT("Change Success"));
			// 바꾼 아이템 위치에 버린 아이템을 생성함.
			AItemClass* DiscardedItem = GetWorld()->SpawnActor<AItemClass>(DiscardedItemData.ItemBPClass, ItemLocation, ItemRotation);

			// 생성이 완료된 경우 해당 아이템의 아이템 정보 갱신.
			if (DiscardedItem)
			{
				DiscardedItem->ItemData = DiscardedItemData;
				UE_LOG(LogTemp, Warning, TEXT("Discard Success"));
				return true;
			}
		}

		// failed
		return false;
	}

	return false;
}

bool UInventoryComponent::UseLightingItem(class AHorrorGameCharacter* Player)
{
	if (CurrentLightingItemIndex >= 0 && LightingInventoryNumber > 0)
	{
		EItemNumber ItemNumber = Lighting_Inventory[CurrentLightingItemIndex].ItemNumber;
		
		switch (ItemNumber)
		{
			// 현재 선택 중인 아이템이 라이터인 경우
			case EItemNumber::ITEM_CigarLighter:
			{
				return TurnonCigarlighter(Player);
			}
			// 현재 선택 중인 아이템이 손전등인 경우
			case EItemNumber::ITEM_FlashLight:
			{
				return TurnonFlashlight(Player);
			}
			// 현재 선택 중인 아이템이 랜턴인 경우
			case EItemNumber::ITEM_Lantern:
			{
				return UseLantern(Player);
			}
		}
	}

	return false;
}

bool UInventoryComponent::ChangeAnotherLightingItem(class AHorrorGameCharacter* Player)
{
	// 조명 인벤토리 내의 다른 아이템이 존재하는 경우
	if (CurrentLightingItemIndex >= 0 && LightingInventoryNumber > 1)
	{
		EItemNumber BeforeItemNumber = Lighting_Inventory[CurrentLightingItemIndex].ItemNumber;

		// 현재 선택 중인 인덱스를 증가시키는데, 최대 크기보다 초과한 경우 0으로.
		CurrentLightingItemIndex++;

		if (CurrentLightingItemIndex >= LightingInventoryNumber)
		{
			CurrentLightingItemIndex = 0;
		}

		EItemNumber AfterItemNumber = Lighting_Inventory[CurrentLightingItemIndex].ItemNumber;

		// 다른 컴포넌트/액터/위젯에 현재 선택 중인 조명 아이템이 바뀌었음을 브로드캐스트함.
		OnItemSwitch.Broadcast(Lighting_Inventory[CurrentLightingItemIndex], CurrentLightingItemIndex);

		// 플레이어가 조명 아이템을 켠 상태인 경우
		if (Player->bIsLightOn || Player->bIsLanternOn)
		{
			switch (BeforeItemNumber)
			{
				// 이전에 선택한 아이템이 라이터인 경우
				case EItemNumber::ITEM_CigarLighter:
				{
					TurnonCigarlighter(Player);
					break;
				}
				// 이전에 선택한 아이템이 손전등인 경우
				case EItemNumber::ITEM_FlashLight:
				{
					TurnonFlashlight(Player);
					break;
				}
				// 이전에 선택한 아이템이 랜턴인 경우
				case EItemNumber::ITEM_Lantern:
				{
					UseLantern(Player);
					break;
				}
			}

			switch (AfterItemNumber)
			{
				// 현재 선택한 아이템이 라이터인 경우
				case EItemNumber::ITEM_CigarLighter:
				{
					return TurnonCigarlighter(Player);
				}
				// 현재 선택한 아이템이 손전등인 경우
				case EItemNumber::ITEM_FlashLight:
				{
					return TurnonFlashlight(Player);
				}
				// 현재 선택한 아이템이 랜턴인 경우
				case EItemNumber::ITEM_Lantern:
				{
					return UseLantern(Player);
				}
			}
		}

		return true;
	}

	return false;
}

bool UInventoryComponent::UseConsumableItem(class AHorrorGameCharacter* Player)
{
	// 소비형 아이템 인벤토리 내에 아이템이 하나 이상 있는 경우
	if (CurrentConsumableItemIndex >= 0 && ConsumableInventoryNumber > 0)
	{
		EItemNumber ItemNumber = Consumable_Inventory[CurrentConsumableItemIndex].ItemNumber;

		switch (ItemNumber)
		{
			// 현재 선택한 아이템이 열쇠인 경우
			case EItemNumber::ITEM_Key:
			{
				return UseKey(Player);
			}
			// 현재 선택한 아이템이 소음 아이템인 경우
			case EItemNumber::ITEM_Timer:
			{
				return UseTimer(Player);
			}
			// 현재 선택한 아이템이 청동 검인 경우
			case EItemNumber::ITEM_Sword:
			{
				return UseKey(Player);
			}
			// 현재 선택한 아이템이 청동 거울인 경우
			case EItemNumber::ITEM_Mirror:
			{
				return UseMirror(Player);
			}
			// 현재 선택한 아이템이 소화기인 경우
			case EItemNumber::ITEM_Extinguisher:
			{
				bool Result = Player->UseExtinguisher();
				return Result;
			}
			// 현재 선택한 아이템이 절단기인 경우
			case EItemNumber::ITEM_Cutter:
			{
				return UseCutter(Player);
			}
			// 현재 선택한 아이템이 야광봉인 경우
			case EItemNumber::ITEM_Glowstick:
			{
				return UseGlowstick(Player);
			}
		}
	}

	return false;
}

int UInventoryComponent::AddCigarLighter(class AItemClass* Item, int IndexToInsert, class AHorrorGameCharacter* Player)
{
	// 인벤토리 내에 존재하는 경우
	for (int i = 0; i < LightingInventoryNumber; ++i)
	{
		if (Lighting_Inventory[i].ItemNumber == EItemNumber::ITEM_CigarLighter)
		{
			// 라이터는 추가 획득이 불가능함.
			OnItemMessage.Broadcast(NSLOCTEXT("ItemMessage", "Already_Have", "This item can no longer be carried."));

			return 2;
		}
	}

	// 조명 인벤토리에 공간이 없는 경우
	if (LightingInventoryNumber == 2)
	{
		// 인벤토리가 가득 찼다고 알려줌.
		OnItemMessage.Broadcast(NSLOCTEXT("ItemMessage", "Lighting_Full", "No open Lighting Slots."));
		return 0;
	}

	Lighting_Inventory[IndexToInsert] = Item->GetItemData();

	if (LightingInventoryNumber == 0)
	{
		CurrentLightingItemIndex = IndexToInsert;
	}

	OnItemAcquired.Broadcast(Lighting_Inventory[CurrentLightingItemIndex], CurrentLightingItemIndex);

	if (GameInstance)
	{
		// 처음 얻는 아이템인 경우
		if (!GameInstance->GetIsItemFirstAcquired(EItemNumber::ITEM_CigarLighter))
		{
			OnItemMessage.Broadcast(NSLOCTEXT("ItemMessage", "Get_Lighter", "Lighter is added in archive."));
			GameInstance->SaveItemArchives(EItemNumber::ITEM_CigarLighter);
		}
	}

	return 1;
}

int UInventoryComponent::ChangeCigarLighter(class AItemClass* ChangedItem, int IndexToInsert, class AHorrorGameCharacter* Player)
{
	bool Result = Player->ToggleCigarlight(false);

	Lighting_Inventory[IndexToInsert] = ChangedItem->GetItemData();

	OnItemAcquired.Broadcast(Lighting_Inventory[CurrentLightingItemIndex], CurrentLightingItemIndex);

	return 1;
}

bool UInventoryComponent::TurnonCigarlighter(class AHorrorGameCharacter* Player)
{
	bool Result = false;

	if (Player->bIsLightOn)
	{
		Result = Player->ToggleCigarlight(false);
	}
	else
	{
		Result = Player->ToggleCigarlight(true);
	}

	return Result;
}

int UInventoryComponent::AddFlashlight(class AItemClass* Item, int IndexToInsert, class AHorrorGameCharacter* Player)
{
	// 인벤토리 내에 존재하는 경우
	for (int i = 0; i < LightingInventoryNumber; ++i)
	{
		if (Lighting_Inventory[i].ItemNumber == EItemNumber::ITEM_FlashLight)
		{
			// 손전등은 추가 획득이 불가능함.
			OnItemMessage.Broadcast(NSLOCTEXT("ItemMessage", "Already_Have", "This item can no longer be carried."));

			return 2;
		}
	}

	// 인벤토리 내 공간이 없는 경우
	if (LightingInventoryNumber == 2)
	{
		// 인벤토리가 가득 찼다고 알려줌.
		OnItemMessage.Broadcast(NSLOCTEXT("ItemMessage", "Lighting_Full", "No open Lighting Slots."));

		return 0;
	}

	Lighting_Inventory[IndexToInsert] = Item->GetItemData();
	Player->SetFlashlight(Lighting_Inventory[IndexToInsert].Durability);

	if (LightingInventoryNumber == 0)
	{
		CurrentLightingItemIndex = IndexToInsert;
	}

	OnItemAcquired.Broadcast(Lighting_Inventory[CurrentLightingItemIndex], CurrentLightingItemIndex);

	if (GameInstance)
	{
		// 처음 얻는 아이템인 경우
		if (!GameInstance->GetIsItemFirstAcquired(EItemNumber::ITEM_FlashLight))
		{
			OnItemMessage.Broadcast(NSLOCTEXT("ItemMessage", "Get_Flashlight", "Flashlight is added in archive."));
			GameInstance->SaveItemArchives(EItemNumber::ITEM_FlashLight);
		}
	}

	return 1;
}

int UInventoryComponent::ChangeFlashlight(class AItemClass* ChangedItem, int IndexToInsert, class AHorrorGameCharacter* Player)
{
	bool Result = Player->SetFlashlight(false);

	Lighting_Inventory[IndexToInsert] = ChangedItem->GetItemData();

	OnItemAcquired.Broadcast(Lighting_Inventory[CurrentLightingItemIndex], CurrentLightingItemIndex);

	return 1;
}

bool UInventoryComponent::TurnonFlashlight(class AHorrorGameCharacter* Player)
{
	bool Result = false;
	if (Player->bIsLightOn)
	{
		Result = Player->ToggleFlashlight(false);
	}
	else
	{
		Result = Player->ToggleFlashlight(true);
	}

	if (Result)
	{
		bIsFlashlightOn = Player->bIsLightOn;
	}

	FlashlightTimer = 0.0f;

	return Result;
}

void UInventoryComponent::FlashlightDischarge()
{
	Lighting_Inventory[CurrentLightingItemIndex].ItemCount = 0;

	Lighting_Inventory.RemoveAt(CurrentLightingItemIndex);
	LightingInventoryNumber -= 1;
	CurrentLightingItemIndex -= 1;

	if (CurrentLightingItemIndex < 0 && LightingInventoryNumber > 0)
	{
		CurrentLightingItemIndex = 0;
	}

	if (LightingInventoryNumber <= 0)
	{
		CurrentLightingItemIndex = 0;
	}

	FHorrorGameItemData TempItem;
	TempItem.Clear();
	Lighting_Inventory.Add(TempItem);

	OnItemMessage.Broadcast(NSLOCTEXT("ItemMessage", "Flashlight_Discharge", "Flashlight can no longer be used."));

	OnItemConsumed.Broadcast(Lighting_Inventory[CurrentLightingItemIndex], CurrentLightingItemIndex);
}

int UInventoryComponent::AddKey(class AItemClass* Item, int IndexToInsert, class AHorrorGameCharacter* Player)
{
	// If it is in inventory.
	for (int i = 0; i < ConsumableInventoryNumber; ++i)
	{
		if (Consumable_Inventory[i].ItemNumber == EItemNumber::ITEM_Key)
		{
			// Increase the amound of this item.
			Consumable_Inventory[i].ItemCount += Item->GetItemData().ItemCount;

			OnItemAcquired.Broadcast(Consumable_Inventory[CurrentConsumableItemIndex], CurrentConsumableItemIndex);

			return 2;
		}
	}

	// If the useable inventory is full.
	if (ConsumableInventoryNumber == ConsumableInventoryMaxSize)
	{
		// Print out the inventory is full and use long left click to change item.
		OnItemMessage.Broadcast(NSLOCTEXT("ItemMessage", "Consumable_Full", "No open Consumable Slots.."));
		return 0;
	}

	Consumable_Inventory[IndexToInsert] = Item->GetItemData();

	// If this is the first item the player obtain, register the item in Quick Items.
	if (ConsumableInventoryNumber == 0)
	{
		CurrentConsumableItemIndex = IndexToInsert;
	}

	OnItemAcquired.Broadcast(Consumable_Inventory[CurrentConsumableItemIndex], CurrentConsumableItemIndex);

	if (GameInstance)
	{
		// 처음 얻는 아이템인 경우
		if (!GameInstance->GetIsItemFirstAcquired(EItemNumber::ITEM_Key))
		{
			OnItemMessage.Broadcast(NSLOCTEXT("ItemMessage", "Get_Key", "Key is added in archive."));
			GameInstance->SaveItemArchives(EItemNumber::ITEM_Key);
		}
	}

	return 1;
}

int UInventoryComponent::ChangeKey(class AItemClass* ChangedItem, int IndexToInsert, class AHorrorGameCharacter* Player)
{
	// If it is in inventory.
	for (int i = 0; i < ConsumableInventoryNumber; ++i)
	{
		if (Consumable_Inventory[i].ItemNumber == EItemNumber::ITEM_Key)
		{
			// Increase the amound of this item.
			Consumable_Inventory[i].ItemCount += ChangedItem->GetItemData().ItemCount;

			OnItemAcquired.Broadcast(Consumable_Inventory[CurrentConsumableItemIndex], CurrentConsumableItemIndex);

			return 1;
		}
	}

	Consumable_Inventory[IndexToInsert] = ChangedItem->GetItemData();

	OnItemAcquired.Broadcast(Consumable_Inventory[CurrentConsumableItemIndex], CurrentConsumableItemIndex);

	return 1;
}

bool UInventoryComponent::UseKey(class AHorrorGameCharacter* Player)
{
	AActor* HitActor = Player->GetActorToInteract();
	bool Result = false;
	int32 UsedItemIndex = CurrentConsumableItemIndex;

	// if the actor to interact with is a type of door class
	if (HitActor)
	{
		if (HitActor->IsA<ADoorClass>())
		{
			// Call Use Interact.
			if (HitActor->GetClass()->ImplementsInterface(UInteractInterface::StaticClass()))
			{
				auto ActorInterface = Cast<IInteractInterface>(HitActor);
				Result = ActorInterface->UseInteract(Player);
			}
		}
	}

	// If succeed to unlock the door.
	if (Result)
	{
		// Reduce the key.
		Consumable_Inventory[UsedItemIndex].ItemCount -= 1;

		// When all keys are used up
		if (Consumable_Inventory[UsedItemIndex].ItemCount <= 0)
		{
			Consumable_Inventory.RemoveAt(UsedItemIndex);
			ConsumableInventoryNumber--;


			if (UsedItemIndex == CurrentConsumableItemIndex)
			{
				CurrentConsumableItemIndex--;
			}

			// If the item index is below -1 even though there are useable items remaining in the inventory, set the index to 0
			if (ConsumableInventoryNumber > 0 && CurrentConsumableItemIndex < 0)
			{
				CurrentConsumableItemIndex = 0;
			}

			// If there is no items in the inventory, set the index to 0.
			if (ConsumableInventoryNumber <= 0)
			{
				CurrentConsumableItemIndex = 0;
			}

			// Make the temp item data and insert to the inventory.
			FHorrorGameItemData TempItemData;
			TempItemData.Clear();
			Consumable_Inventory.Add(TempItemData);
		}
	}

	OnItemConsumed.Broadcast(Consumable_Inventory[CurrentConsumableItemIndex], CurrentConsumableItemIndex);

	return Result;
}

int UInventoryComponent::AddTimer(class AItemClass* Item, int IndexToInsert, class AHorrorGameCharacter* Player)
{
	// If it is in inventory.
	for (int i = 0; i < ConsumableInventoryNumber; ++i)
	{
		if (Consumable_Inventory[i].ItemNumber == EItemNumber::ITEM_Timer)
		{
			// Increase the amound of this item.
			Consumable_Inventory[i].ItemCount += Item->GetItemData().ItemCount;

			OnItemAcquired.Broadcast(Consumable_Inventory[CurrentConsumableItemIndex], CurrentConsumableItemIndex);
			return 2;
		}
	}

	// If the useable inventory is full.
	if (ConsumableInventoryNumber == ConsumableInventoryMaxSize)
	{
		// Print out the inventory is full and use long left click to change item.
		OnItemMessage.Broadcast(NSLOCTEXT("ItemMessage", "Consumable_Full", "No open Consumable Slots.."));
		return 0;
	}

	Consumable_Inventory[IndexToInsert] = Item->GetItemData();

	// If this is the first item the player obtain, register the item in Quick Items.
	if (ConsumableInventoryNumber == 0)
	{
		CurrentConsumableItemIndex = IndexToInsert;
	}

	OnItemAcquired.Broadcast(Consumable_Inventory[CurrentConsumableItemIndex], CurrentConsumableItemIndex);

	if (GameInstance)
	{
		// 처음 얻는 아이템인 경우
		if (!GameInstance->GetIsItemFirstAcquired(EItemNumber::ITEM_Timer))
		{
			OnItemMessage.Broadcast(NSLOCTEXT("ItemMessage", "Get_Timer", "Timer is added in archive."));
			GameInstance->SaveItemArchives(EItemNumber::ITEM_Timer);
		}
	}

	return 1;
}

int UInventoryComponent::ChangeTimer(class AItemClass* ChangedItem, int IndexToInsert, class AHorrorGameCharacter* Player)
{
	// If it is in inventory.
	for (int i = 0; i < ConsumableInventoryNumber; ++i)
	{
		if (Consumable_Inventory[i].ItemNumber == EItemNumber::ITEM_Key)
		{
			// Increase the amound of this item.
			Consumable_Inventory[i].ItemCount += ChangedItem->GetItemData().ItemCount;

			OnItemAcquired.Broadcast(Consumable_Inventory[CurrentConsumableItemIndex], CurrentConsumableItemIndex);

			return 1;
		}
	}

	Consumable_Inventory[IndexToInsert] = ChangedItem->GetItemData();

	OnItemAcquired.Broadcast(Consumable_Inventory[CurrentConsumableItemIndex], CurrentConsumableItemIndex);

	return 1;
}

bool UInventoryComponent::UseTimer(class AHorrorGameCharacter* Player)
{
	bool result = false;
	int UsedItemIndex = CurrentConsumableItemIndex;

	//The player calls a function that has the effect of throwing a timer and receives the result.
	result = Player->UseNoiseItem();

	// If the player succeeds in throwing the timer
	if (result)
	{
		// Reduce the number of timers by one.
		Consumable_Inventory[UsedItemIndex].ItemCount--;

		// If the number of timers is 0 with the above logic
		if (Consumable_Inventory[UsedItemIndex].ItemCount == 0)
		{
			// If the timer index and the index of the currently selected slot are the same
			if (UsedItemIndex == CurrentConsumableItemIndex)
			{
				CurrentConsumableItemIndex--;
			}

			// Remove the array of timer indices and decrease the number of items in the inventory by one.
			Consumable_Inventory.RemoveAt(UsedItemIndex);
			ConsumableInventoryNumber--;

			// If there is more than one usable item in the inventory, but the index of the slot being selected is less than 0, set it to 0.
			if (ConsumableInventoryNumber > 0 && CurrentConsumableItemIndex < 0)
			{
				CurrentConsumableItemIndex = 0;
			}

			// If there are no items in the available item inventory, set the selected slot index to 0.
			if (ConsumableInventoryNumber <= 0)
			{
				CurrentConsumableItemIndex = 0;
			}

			// Add dummy data to the inventory slot and add it to the back of the inventory array.
			FHorrorGameItemData TempItemData;
			TempItemData.Clear();
			Consumable_Inventory.Add(TempItemData);
		}
	}

	OnItemConsumed.Broadcast(Consumable_Inventory[CurrentConsumableItemIndex], CurrentConsumableItemIndex);

	return result;
}

int UInventoryComponent::AddMirror(class AItemClass* Item, int IndexToInsert, class AHorrorGameCharacter* Player)
{
	// If it is in inventory.
	for (int i = 0; i < ConsumableInventoryNumber; ++i)
	{
		if (Consumable_Inventory[i].ItemNumber == EItemNumber::ITEM_Mirror)
		{
			// Notifies that only one item can be obtained, and use long left click to change item.
			OnItemMessage.Broadcast(NSLOCTEXT("ItemMessage", "Already_Have", "This item can no longer be carried."));

			return 0;
		}
	}

	// If the useable inventory is full.
	if (ConsumableInventoryNumber == ConsumableInventoryMaxSize)
	{
		// Print out the inventory is full and use long left click to change item.
		OnItemMessage.Broadcast(NSLOCTEXT("ItemMessage", "Consumable_Full", "No open Consumable Slots.."));

		return 0;
	}

	Consumable_Inventory[IndexToInsert] = Item->GetItemData();


	// If this is the first item the player obtain, register the item in Quick Items.
	if (ConsumableInventoryNumber == 0)
	{
		CurrentConsumableItemIndex = IndexToInsert;
		//Player->SetMirrorVisibility(true);
	}

	OnItemAcquired.Broadcast(Consumable_Inventory[CurrentConsumableItemIndex], CurrentConsumableItemIndex);

	if (GameInstance)
	{
		// 처음 얻는 아이템인 경우
		if (!GameInstance->GetIsItemFirstAcquired(EItemNumber::ITEM_Mirror))
		{
			OnItemMessage.Broadcast(NSLOCTEXT("ItemMessage", "Get_Mirror", "Bronze Mirror is added in archive."));
			GameInstance->SaveItemArchives(EItemNumber::ITEM_Mirror);
		}
	}

	return 1;
}

int UInventoryComponent::ChangeMirror(class AItemClass* ChangedItem, int IndexToInsert, class AHorrorGameCharacter* Player)
{
	Consumable_Inventory[IndexToInsert] = ChangedItem->GetItemData();

	OnItemAcquired.Broadcast(Consumable_Inventory[CurrentConsumableItemIndex], CurrentConsumableItemIndex);

	return 1;
}

bool UInventoryComponent::UseMirror(class AHorrorGameCharacter* Player)
{
	OnItemConsumed.Broadcast(Consumable_Inventory[CurrentConsumableItemIndex], CurrentConsumableItemIndex);

	return false;
}

int UInventoryComponent::AddSword(class AItemClass* Item, int IndexToInsert, class AHorrorGameCharacter* Player)
{
	// If it is in inventory.
	// This inventory is not sorted.
	for (int i = 0; i < ConsumableInventoryNumber; ++i)
	{
		if (Consumable_Inventory[i].ItemNumber == EItemNumber::ITEM_Sword)
		{
			// Notifies that only one item can be obtained, and use long left click to change item.
			OnItemMessage.Broadcast(NSLOCTEXT("ItemMessage", "Already_Have", "This item can no longer be carried."));

			return 0;
		}
	}

	// If the useable inventory is full.
	if (ConsumableInventoryNumber == ConsumableInventoryMaxSize)
	{
		// Print out the inventory is full and use long left click to change item.
		OnItemMessage.Broadcast(NSLOCTEXT("ItemMessage", "Consumable_Full", "No open Consumable Slots.."));

		return 0;
	}

	Consumable_Inventory[IndexToInsert] = Item->GetItemData();

	// If this is the first item the player obtain, register the item in Quick Items.
	if (ConsumableInventoryNumber == 0)
	{
		CurrentConsumableItemIndex = IndexToInsert;
	}

	OnItemAcquired.Broadcast(Consumable_Inventory[CurrentConsumableItemIndex], CurrentConsumableItemIndex);

	if (GameInstance)
	{
		// 처음 얻는 아이템인 경우
		if (!GameInstance->GetIsItemFirstAcquired(EItemNumber::ITEM_Sword))
		{
			OnItemMessage.Broadcast(NSLOCTEXT("ItemMessage", "Get_Sword", "Bronze Sword is added in archive."));
			GameInstance->SaveItemArchives(EItemNumber::ITEM_Sword);
		}
	}

	return 1;
}

int UInventoryComponent::ChangeSword(class AItemClass* ChangedItem, int IndexToInsert, class AHorrorGameCharacter* Player)
{
	Consumable_Inventory[IndexToInsert] = ChangedItem->GetItemData();

	OnItemAcquired.Broadcast(Consumable_Inventory[CurrentConsumableItemIndex], CurrentConsumableItemIndex);

	return 1;
}

bool UInventoryComponent::UseSword(class AHorrorGameCharacter* Player)
{
	bool result = true;
	int UsedItemIndex = CurrentConsumableItemIndex;

	result = Player->UseSword();

	if (result)
	{
		// Reduce the sword durability.
		Consumable_Inventory[UsedItemIndex].Durability--;

		// if the durability is 0
		if (Consumable_Inventory[UsedItemIndex].Durability <= 0)
		{
			// Reduce the number of sowrd.
			Consumable_Inventory[UsedItemIndex].ItemCount = 0;
		}

		// if the number of sword is 0
		if (Consumable_Inventory[UsedItemIndex].ItemCount == 0)
		{
			// if currently selected item index is same with useditemindex, reduce selected item index.
			if (CurrentConsumableItemIndex == UsedItemIndex)
			{
				CurrentConsumableItemIndex--;
			}

			// Remove from the inventory and reduce the number of items in the inventory.
			Consumable_Inventory.RemoveAt(UsedItemIndex);
			ConsumableInventoryNumber--;

			// If the cursor is below - 1 even though there are items remaining in the inventory, the cursor is set to 0.
			if (CurrentConsumableItemIndex < 0 && ConsumableInventoryNumber > 0)
			{
				CurrentConsumableItemIndex = 0;
			}

			// If the inventory is empty, set the cursor to 0.
			if (ConsumableInventoryNumber <= 0)
			{
				CurrentConsumableItemIndex = 0;
			}

			OnItemMessage.Broadcast(NSLOCTEXT("ItemMessage", "Sword_Discharge", "No open Consumable Slots.."));

			// Create temporary data and add it to the end of the inventory.
			FHorrorGameItemData TempItem;
			TempItem.Clear();
			Consumable_Inventory.Add(TempItem);
		}
	}

	OnItemConsumed.Broadcast(Consumable_Inventory[CurrentConsumableItemIndex], CurrentConsumableItemIndex);

	return result;
}

int UInventoryComponent::AddBell(class AItemClass* Item, int IndexToInsert, class AHorrorGameCharacter* Player)
{
	// Get the item data
	FHorrorGameItemData ItemData = Item->GetItemData();

	// if the item type is none, that is not item
	if (ItemData.ItemType == EItemType::ITEM_NONE)
	{


		return 0;
	}

	// If it is in inventory
	if (PassiveItemNumber > 0)
	{
		for (int i = 0; i < PassiveItemNumber; ++i)
		{
			if (Passive_Inventory[i].ItemNumber == ItemData.ItemNumber)
			{
				// Notifies that it is already in the inventory.
				OnItemMessage.Broadcast(NSLOCTEXT("ItemMessage", "Already_Have", "This item can no longer be carried."));

				return 0;
			}
		}
	}

	// If the maximum number that can be stored in the inventory is exceeded, it cannot be obtained.
	if (PassiveItemNumber >= PassiveInventoryMaxSize)
	{
		OnItemMessage.Broadcast(NSLOCTEXT("ItemMessage", "Passive_Full", "No open Passive Slots.."));

		return 0;
	}

	// If this is an item obtained for the first time, assign the item information to the index to be added.
	Passive_Inventory[IndexToInsert] = ItemData;

	OnItemAcquired.Broadcast(Passive_Inventory[PassiveItemNumber], PassiveItemNumber);

	if (GameInstance)
	{
		// 처음 얻는 아이템인 경우
		if (!GameInstance->GetIsItemFirstAcquired(EItemNumber::ITEM_Bell))
		{
			OnItemMessage.Broadcast(NSLOCTEXT("ItemMessage", "Get_Bell", "Bronze Bell is added in archive."));
			GameInstance->SaveItemArchives(EItemNumber::ITEM_Bell);
		}
	}

	return 1;
}

int UInventoryComponent::ChangeBell(class AItemClass* ChangedItem, int IndexToInsert, class AHorrorGameCharacter* Player)
{
	// If this is an item obtained for the first time, assign the item information to the index to be added.
	Passive_Inventory[IndexToInsert] = ChangedItem->GetItemData();

	OnItemAcquired.Broadcast(Passive_Inventory[PassiveItemNumber], PassiveItemNumber);

	return 1;
}

bool UInventoryComponent::UseBell(class AHorrorGameCharacter* Player)
{
	// Reduce the stress guage and refill the health point.

	OnItemConsumed.Broadcast(Passive_Inventory[PassiveItemNumber], PassiveItemNumber);

	return true;
}

int UInventoryComponent::AddExtinguisher(class AItemClass* Item, int IndexToInsert, class AHorrorGameCharacter* Player)
{
	// Get the item data
	FHorrorGameItemData ItemData = Item->GetItemData();

	// if the item type is none, that is not item
	if (ItemData.ItemType == EItemType::ITEM_NONE)
	{
		return 0;
	}

	// If it is in inventory
	for (int i = 0; i < ConsumableInventoryNumber; ++i)
	{
		if (Consumable_Inventory[i].ItemNumber == ItemData.ItemNumber)
		{
			// Change a new item using hold right click.
			OnItemMessage.Broadcast(NSLOCTEXT("ItemMessage", "Already_Have", "This item can no longer be carried."));

			return 2;
		}
	}

	// If the maximum number that can be stored in the inventory is exceeded, it cannot be obtained.
	if (ConsumableInventoryNumber >= ConsumableInventoryMaxSize)
	{

		OnItemMessage.Broadcast(NSLOCTEXT("ItemMessage", "Consumable_Full", "No open Consumable Slots.."));
		return 0;
	}

	// If this is an item obtained for the first time, assign the item information to the index to be added.
	Consumable_Inventory[IndexToInsert] = ItemData;
	Player->SetExtinguisher(Consumable_Inventory[IndexToInsert].Durability);

	UE_LOG(LogTemp, Warning, TEXT("Extinguisher Durability: %d"), Consumable_Inventory[IndexToInsert].Durability);

	// If there were no items in the inventory, set the selected inventory index to 0.
	if (CurrentConsumableItemIndex < 0)
	{
		CurrentConsumableItemIndex = IndexToInsert;
	}

	OnItemAcquired.Broadcast(Consumable_Inventory[CurrentConsumableItemIndex], CurrentConsumableItemIndex);

	if (GameInstance)
	{
		// 처음 얻는 아이템인 경우
		if (!GameInstance->GetIsItemFirstAcquired(EItemNumber::ITEM_Extinguisher))
		{
			OnItemMessage.Broadcast(NSLOCTEXT("ItemMessage", "Get_Extinguisher", "Fire Extinguisher is added in archive."));
			GameInstance->SaveItemArchives(EItemNumber::ITEM_Extinguisher);
		}
	}

	return 1;
}

int UInventoryComponent::ChangeExtinguisher(class AItemClass* ChangedItem, int IndexToInsert, class AHorrorGameCharacter* Player)
{
	// Get the item data
	FHorrorGameItemData ItemData = ChangedItem->GetItemData();

	// If this is an item obtained for the first time, assign the item information to the index to be added.
	Consumable_Inventory[IndexToInsert] = ItemData;

	OnItemAcquired.Broadcast(Consumable_Inventory[CurrentConsumableItemIndex], CurrentConsumableItemIndex);

	return 1;
}

bool UInventoryComponent::UseExtinguisher()
{
	UE_LOG(LogTemp, Warning, TEXT("UseExtinguisher Called."));

	// find the fire extinguisher index.
	int ExtinguisherIndex = Consumable_Inventory.IndexOfByPredicate([](const FHorrorGameItemData& Element)
	{
		return Element.ItemNumber == EItemNumber::ITEM_Extinguisher;
	});

	if (ExtinguisherIndex == INDEX_NONE)
	{
		return false;
	}

	// Reduce the durability of the fire extinguisher.
	Consumable_Inventory[ExtinguisherIndex].Durability -= 1;

	UE_LOG(LogTemp, Warning, TEXT("Durability Left: %d"), Consumable_Inventory[ExtinguisherIndex].Durability);

	OnDurabilityChanged.Broadcast(Consumable_Inventory[ExtinguisherIndex], ExtinguisherIndex);
	
	// If the fire extinguisher runs out
	if (Consumable_Inventory[CurrentConsumableItemIndex].Durability <= 0)
	{
		// Remove from inventory, decrease maximum number of inventory and current inventory cursor by 1.
		Consumable_Inventory.RemoveAt(ExtinguisherIndex);
		ConsumableInventoryNumber--;

		if (ExtinguisherIndex == CurrentConsumableItemIndex)
		{
			CurrentConsumableItemIndex--;
		}

		// If the cursor is below - 1 even though there are items remaining in the inventory, set the cursor to 0.
		if (CurrentConsumableItemIndex < 0 && ConsumableInventoryNumber > 0)
		{
			CurrentConsumableItemIndex = 0;
		}

		// If inventory is empty, set cursor to 0.
		if (ConsumableInventoryNumber <= 0)
		{
			CurrentConsumableItemIndex = 0;
		}

		// Create temporary data and add it to the end of the inventory.
		FHorrorGameItemData TempItem;
		TempItem.Clear();
		Consumable_Inventory.Add(TempItem);

		OnItemMessage.Broadcast(NSLOCTEXT("ItemMessage", "Extinguisher_Discharge", "Fire Extinguisher can no longer be used."));

		if (CurrentConsumableItemIndex < 0)
		{
			OnItemConsumed.Broadcast(Consumable_Inventory[0], 0);
		}
		else
		{
			OnItemConsumed.Broadcast(Consumable_Inventory[CurrentConsumableItemIndex], CurrentConsumableItemIndex);
		}
	}

	return true;
}

int UInventoryComponent::AddCutter(class AItemClass* Item, int IndexToInsert, class AHorrorGameCharacter* Player)
{
	// Get the item data
	FHorrorGameItemData ItemData = Item->GetItemData();

	// if the item type is none, that is not item
	if (ItemData.ItemType == EItemType::ITEM_NONE)
	{
		return 0;
	}

	// If it is in inventory
	for (int i = 0; i < ConsumableInventoryNumber; ++i)
	{
		if (Consumable_Inventory[i].ItemNumber == ItemData.ItemNumber)
		{
			OnItemMessage.Broadcast(NSLOCTEXT("ItemMessage", "Already_Have", "This item can no longer be carried."));

			return 2;
		}
	}

	// If the maximum number that can be stored in the inventory is exceeded, it cannot be obtained.
	if (ConsumableInventoryNumber >= ConsumableInventoryMaxSize)
	{
		OnItemMessage.Broadcast(NSLOCTEXT("ItemMessage", "Consumable_Full", "No open Consumable Slots.."));

		return 0;
	}

	// If this is an item obtained for the first time, assign the item information to the index to be added.
	Consumable_Inventory[IndexToInsert] = ItemData;

	// If there were no items in the inventory, set the selected inventory index to 0.
	if (CurrentConsumableItemIndex < 0)
	{
		CurrentConsumableItemIndex = IndexToInsert;
	}

	OnItemAcquired.Broadcast(Consumable_Inventory[CurrentConsumableItemIndex], CurrentConsumableItemIndex);

	if (GameInstance)
	{
		// 처음 얻는 아이템인 경우
		if (!GameInstance->GetIsItemFirstAcquired(EItemNumber::ITEM_Cutter))
		{
			OnItemMessage.Broadcast(NSLOCTEXT("ItemMessage", "Get_Cutter", "Cutter is added in archive."));
			GameInstance->SaveItemArchives(EItemNumber::ITEM_Cutter);
		}
	}

	return 1;
}

int UInventoryComponent::ChangeCutter(class AItemClass* ChangedItem, int IndexToInsert, class AHorrorGameCharacter* Player)
{
	// Get the item data
	FHorrorGameItemData ItemData = ChangedItem->GetItemData();

	// If this is an item obtained for the first time, assign the item information to the index to be added.
	Consumable_Inventory[IndexToInsert] = ItemData;

	OnItemAcquired.Broadcast(Consumable_Inventory[CurrentConsumableItemIndex], CurrentConsumableItemIndex);

	return 1;
}

bool UInventoryComponent::UseCutter(class AHorrorGameCharacter* Player)
{
	AActor* HitActor = Player->GetActorToInteract();
	bool result = false;
	int UsedItemIndex = CurrentConsumableItemIndex;

	// If there is a hit actor
	if (HitActor)
	{
		// Unlock some cases.
		if (ALockerDoorActor_cpp* LockerDoor = Cast<ALockerDoorActor_cpp>(HitActor))
		{
			// 사물함 문이 잠겨있다면
			if (LockerDoor->bIsDoorLocked)
			{
				// 해당 액터의 아이템 사용 효과 메서드를 호출함.
				result = LockerDoor->UseInteract(Player);
			}
		}
		// 그 액터가 문 클래스일 경우
		else if (ADoorClass* Door = Cast<ADoorClass>(HitActor))
		{
			// 해당 문이 잠겨있다면
			if (Door->bIsDoorLocked)
			{
				// 해당 액터의 아이템 사용 효과 메서드를 호출함.
				result = Door->UseInteract(Player);
			}
		}
	}

	// If the result of useinteract is true
	if (result)
	{
		// 절단기 사용 음을 재생함.
		Player->UseCutter();
		// Reduce the durability of the cutter.
		Consumable_Inventory[UsedItemIndex].Durability--;

		// If it runs out
		if (Consumable_Inventory[UsedItemIndex].Durability <= 0)
		{
			Consumable_Inventory[UsedItemIndex].ItemCount--;

			if (Consumable_Inventory[UsedItemIndex].ItemCount <= 0)
			{
				// if currently selected item index is same with useditemindex, reduce selected item index.
				if (CurrentConsumableItemIndex == UsedItemIndex)
				{
					CurrentConsumableItemIndex--;
				}

				// Remove from the inventory and reduce the number of items in the inventory.
				Consumable_Inventory.RemoveAt(UsedItemIndex);
				ConsumableInventoryNumber--;

				// If the cursor is below - 1 even though there are items remaining in the inventory, the cursor is set to 0.
				if (CurrentConsumableItemIndex < 0 && ConsumableInventoryNumber > 0)
				{
					CurrentConsumableItemIndex = 0;
				}

				// If the inventory is empty, set the cursor to 0.
				if (ConsumableInventoryNumber <= 0)
				{
					CurrentConsumableItemIndex = 0;
				}

				OnItemMessage.Broadcast(NSLOCTEXT("ItemMessage", "Cutter_Discharge", "Cutter can no longer be used."));

				// Create temporary data and add it to the end of the inventory.
				FHorrorGameItemData TempItem;
				TempItem.Clear();
				Consumable_Inventory.Add(TempItem);
			}
		}
	}

	OnItemConsumed.Broadcast(Consumable_Inventory[CurrentConsumableItemIndex], CurrentConsumableItemIndex);

	return result;
}

int UInventoryComponent::AddLantern(class AItemClass* Item, int IndexToInsert, class AHorrorGameCharacter* Player)
{
	// Get the item data
	FHorrorGameItemData ItemData = Item->GetItemData();

	// if the item type is none, that is not item
	if (ItemData.ItemType == EItemType::ITEM_NONE)
	{
		return 0;
	}

	// If it is in inventory
	for (int i = 0; i < LightingInventoryNumber; ++i)
	{
		if (Lighting_Inventory[i].ItemNumber == ItemData.ItemNumber)
		{
			// Cannot get this item.
			OnItemMessage.Broadcast(NSLOCTEXT("ItemMessage", "Already_Have", "This item can no longer be carried."));

			return 0;
		}
	}

	// If the maximum number that can be stored in the inventory is exceeded, it cannot be obtained.
	if (LightingInventoryNumber >= LightingInventoryMaxSize)
	{
		OnItemMessage.Broadcast(NSLOCTEXT("ItemMessage", "Lighting_Full", "No open Lighting Slots.."));

		return 0;
	}

	// If this is an item obtained for the first time, assign the item information to the index to be added.
	Lighting_Inventory[IndexToInsert] = ItemData;

	// If there were no items in the inventory, set the selected inventory index to 0.
	if (CurrentLightingItemIndex < 0)
	{
		CurrentLightingItemIndex = IndexToInsert;
	}

	OnItemAcquired.Broadcast(Lighting_Inventory[CurrentLightingItemIndex], CurrentLightingItemIndex);

	if (GameInstance)
	{
		// 처음 얻는 아이템인 경우
		if (!GameInstance->GetIsItemFirstAcquired(EItemNumber::ITEM_Lantern))
		{
			OnItemMessage.Broadcast(NSLOCTEXT("ItemMessage", "Get_Lantern", "Soul Lantern is added in archive."));
			GameInstance->SaveItemArchives(EItemNumber::ITEM_Lantern);
		}
	}

	return 1;
}

int UInventoryComponent::ChangeLantern(class AItemClass* ChangedItem, int IndexToInsert, class AHorrorGameCharacter* Player)
{
	Player->SetLanternOn(false);

	// Get the item data
	FHorrorGameItemData ItemData = ChangedItem->GetItemData();

	// If this is an item obtained for the first time, assign the item information to the index to be added.
	Lighting_Inventory[IndexToInsert] = ItemData;

	// If there were no items in the inventory, set the selected inventory index to 0.
	OnItemAcquired.Broadcast(Lighting_Inventory[CurrentLightingItemIndex], CurrentLightingItemIndex);

	return 1;
}

bool UInventoryComponent::UseLantern(class AHorrorGameCharacter* Player)
{
	bool result = false;

	if (Player->bIsLanternOn)
	{
		result = Player->SetLanternOn(false);
	}
	else
	{
		result = Player->SetLanternOn(true);
	}

	return result;
}

int UInventoryComponent::AddGlowstick(class AItemClass* Item, int IndexToInsert, class AHorrorGameCharacter* Player)
{
	// Get the item data
	FHorrorGameItemData ItemData = Item->GetItemData();

	// if the item type is none, that is not item
	if (ItemData.ItemType == EItemType::ITEM_NONE)
	{
		return 0;
	}

	// If it is in inventory
	for (int i = 0; i < ConsumableInventoryNumber; ++i)
	{
		if (Consumable_Inventory[i].ItemNumber == ItemData.ItemNumber)
		{
			// Add the amount of the item.
			Consumable_Inventory[i].ItemCount += ItemData.ItemCount;

			return 2;
		}
	}

	// If the maximum number that can be stored in the inventory is exceeded, it cannot be obtained.
	if (ConsumableInventoryNumber >= ConsumableInventoryMaxSize)
	{
		OnItemMessage.Broadcast(NSLOCTEXT("ItemMessage", "Consumable_Full", "No open Consumable Slots.."));

		return 0;
	}

	// If this is an item obtained for the first time, assign the item information to the index to be added.
	Consumable_Inventory[IndexToInsert] = ItemData;

	// If there were no items in the inventory, set the selected inventory index to 0.
	if (CurrentConsumableItemIndex < 0)
	{
		CurrentConsumableItemIndex = IndexToInsert;
	}

	OnItemAcquired.Broadcast(Consumable_Inventory[CurrentConsumableItemIndex], CurrentConsumableItemIndex);

	if (GameInstance)
	{
		// 처음 얻는 아이템인 경우
		if (!GameInstance->GetIsItemFirstAcquired(EItemNumber::ITEM_Glowstick))
		{
			OnItemMessage.Broadcast(NSLOCTEXT("ItemMessage", "Get_Glowstick", "Glowstick is added in archive."));
			GameInstance->SaveItemArchives(EItemNumber::ITEM_Glowstick);
		}
	}

	return 1;
}

int UInventoryComponent::ChangeGlowstick(class AItemClass* ChangedItem, int IndexToInsert, class AHorrorGameCharacter* Player)
{
	// If it is in inventory.
	for (int i = 0; i < ConsumableInventoryNumber; ++i)
	{
		if (Consumable_Inventory[i].ItemNumber == EItemNumber::ITEM_Key)
		{
			// Increase the amound of this item.
			Consumable_Inventory[i].ItemCount += ChangedItem->GetItemData().ItemCount;

			OnItemAcquired.Broadcast(Consumable_Inventory[CurrentConsumableItemIndex], CurrentConsumableItemIndex);

			return 1;
		}
	}

	// Get the item data
	FHorrorGameItemData ItemData = ChangedItem->GetItemData();

	// If this is an item obtained for the first time, assign the item information to the index to be added.
	Consumable_Inventory[IndexToInsert] = ItemData;

	OnItemAcquired.Broadcast(Consumable_Inventory[CurrentConsumableItemIndex], CurrentConsumableItemIndex);

	return 1;
}

bool UInventoryComponent::UseGlowstick(class AHorrorGameCharacter* Player)
{
	bool result = false;
	int UsedItemIndex = CurrentConsumableItemIndex;

	result = Player->UseGlowstick();

	if (result)
	{
		// Reduce the number of glowsticks.
		Consumable_Inventory[UsedItemIndex].ItemCount--;

		// If the number of it is 0
		if (Consumable_Inventory[UsedItemIndex].ItemCount <= 0)
		{
			// if currently selected item index is same with useditemindex, reduce selected item index.
			if (CurrentConsumableItemIndex == UsedItemIndex)
			{
				CurrentConsumableItemIndex--;
			}

			// Remove from the inventory and reduce the number of items in the inventory.
			Consumable_Inventory.RemoveAt(UsedItemIndex);
			ConsumableInventoryNumber--;

			// If the cursor is below - 1 even though there are items remaining in the inventory, the cursor is set to 0.
			if (CurrentConsumableItemIndex < 0 && ConsumableInventoryNumber > 0)
			{
				CurrentConsumableItemIndex = 0;
			}

			// If the inventory is empty, set the cursor to 0.
			if (ConsumableInventoryNumber <= 0)
			{
				CurrentConsumableItemIndex = 0;
			}

			// Create temporary data and add it to the end of the inventory.
			FHorrorGameItemData TempItem;
			TempItem.Clear();
			Consumable_Inventory.Add(TempItem);
		}
	}

	OnItemConsumed.Broadcast(Consumable_Inventory[CurrentConsumableItemIndex], CurrentConsumableItemIndex);

	return result;
}

int UInventoryComponent::AddCompass(class AItemClass* Item, AHorrorGameCharacter* Player)
{
	if (bDoHaveCompass)
	{
		// Notifies that already have the compass
		OnItemMessage.Broadcast(NSLOCTEXT("ItemMessage", "Already_Have", "This item can no longer be carried."));

		return 0;
	}

	bDoHaveCompass = true;

	return 1;
}

bool UInventoryComponent::UseCompass(class AHorrorGameCharacter* Player)
{
	bool result = false;
	result = Player->UseCompass();

	return result;
}

int UInventoryComponent::AddStone(class AItemClass* Item, AHorrorGameCharacter* Player)
{
	// if the player collected more than 5, return false.
	if (ObjectCount < 5)
	{
		ObjectCount++;
		OnObjectAcquired.Broadcast(ObjectCount);

		return 1;
	}

	OnItemMessage.Broadcast(NSLOCTEXT("ItemMessage", "Object_Full", "This stone can no longer be carried."));

	return 0;
}

bool UInventoryComponent::UseStone(class AHorrorGameCharacter* Player)
{
	// Give a new life.
	if (ObjectCount > 0)
	{
		ObjectCount--;
		OnObjectConsumed.Broadcast(ObjectCount);

		return true;
	}

	return false;
}

void UInventoryComponent::MoveItemIndex(int AddValue)
{
	// CurrentItemIndex += AddValue;

	if (bIsExtinguisherSpraying)
	{
		return;
	}

	CurrentConsumableItemIndex += AddValue;

	if (CurrentConsumableItemIndex >= ConsumableInventoryNumber)
	{
		CurrentConsumableItemIndex = 0;
	}

	if (CurrentConsumableItemIndex < 0)
	{
		CurrentConsumableItemIndex = ConsumableInventoryNumber - 1;
	}

	OnItemSwitch.Broadcast(Consumable_Inventory[CurrentConsumableItemIndex], CurrentConsumableItemIndex);

}

void UInventoryComponent::DecreaseDurability(EItemType InventoryType, EItemNumber ItemNumber)
{
	switch (InventoryType)
	{
		case EItemType::ITEM_Consumable:
		{
			Consumable_Inventory[CurrentConsumableItemIndex].Durability--;

			OnDurabilityChanged.Broadcast(Consumable_Inventory[CurrentConsumableItemIndex], CurrentConsumableItemIndex);

			// if the durability is 0
			if (Consumable_Inventory[CurrentConsumableItemIndex].Durability <= 0)
			{
				// Reduce the number of sowrd.
				Consumable_Inventory[CurrentConsumableItemIndex].ItemCount = 0;
			}

			// if the number of sword is 0
			if (Consumable_Inventory[CurrentConsumableItemIndex].ItemCount == 0)
			{
				OnItemConsumed.Broadcast(Consumable_Inventory[CurrentConsumableItemIndex], CurrentConsumableItemIndex);

				// Remove from the inventory and reduce the number of items in the inventory.
				Consumable_Inventory.RemoveAt(CurrentConsumableItemIndex);
				ConsumableInventoryNumber--;

				CurrentConsumableItemIndex--;

				// If the cursor is below - 1 even though there are items remaining in the inventory, the cursor is set to 0.
				if (CurrentConsumableItemIndex < 0 && ConsumableInventoryNumber > 0)
				{
					CurrentConsumableItemIndex = 0;
				}

				// If the inventory is empty, set the cursor to -1.
				if (ConsumableInventoryNumber <= 0)
				{
					CurrentConsumableItemIndex = -1;
				}

				// Create temporary data and add it to the end of the inventory.
				FHorrorGameItemData TempItem;
				TempItem.Clear();
				Consumable_Inventory.Add(TempItem);

				if (CurrentConsumableItemIndex < 0)
				{
					OnItemConsumed.Broadcast(Consumable_Inventory[0], 0);
				}
				else
				{
					OnItemConsumed.Broadcast(Consumable_Inventory[CurrentConsumableItemIndex], CurrentConsumableItemIndex);
				}
			}

			break;
		}
		case EItemType::ITEM_Lighting:
		{
			Lighting_Inventory[CurrentLightingItemIndex].Durability -= 1;

			OnDurabilityChanged.Broadcast(Lighting_Inventory[CurrentLightingItemIndex], CurrentLightingItemIndex);

			// when the battery runs out
			if (Lighting_Inventory[CurrentLightingItemIndex].Durability <= 0)
			{
				switch (ItemNumber)
				{
					case EItemNumber::ITEM_FlashLight:
					{
						TurnonFlashlight(OwnerPlayer);
						// FlashlightDischarge();
						break;
					}
				}

				Lighting_Inventory[CurrentLightingItemIndex].ItemCount = 0;

				Lighting_Inventory.RemoveAt(CurrentLightingItemIndex);
				LightingInventoryNumber -= 1;
				CurrentLightingItemIndex -= 1;

				if (CurrentLightingItemIndex < 0 && LightingInventoryNumber > 0)
				{
					CurrentLightingItemIndex = 0;
				}

				if (LightingInventoryNumber <= 0)
				{
					CurrentLightingItemIndex = 0;
				}

				FHorrorGameItemData TempItem;
				TempItem.Clear();
				Lighting_Inventory.Add(TempItem);

				if (CurrentLightingItemIndex < 0 || CurrentLightingItemIndex >= LightingInventoryMaxSize)
				{
					OnItemConsumed.Broadcast(Lighting_Inventory[0], 0);
				}
				else
				{
					OnItemConsumed.Broadcast(Lighting_Inventory[CurrentLightingItemIndex], CurrentLightingItemIndex);
				}
			}

			break;
		}
		case EItemType::ITEM_Passive:
		{

			break;
		}
		default:
		{

		}
	}
}