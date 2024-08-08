// CopyrightNotice 2023 Sunggon Kim kimdave205@gmail.com. All Rights Reserved.

#include "Player/PlayerItemComponent.h"
#include "Player/HorrorGameCharacter.h"

#define PASSIVEITEMSTARTNUMBER 10

// Sets default values for this component's properties
UPlayerItemComponent::UPlayerItemComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	//SphereCollider = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollider"));
	// ...

}


// Called when the game starts
void UPlayerItemComponent::BeginPlay()
{
	Super::BeginPlay();

	Inventory.Init(FHorrorGameItemData::FHorrorGameItemData(), 9);

	PlayerCharacter = Cast<AHorrorGameCharacter>(GetOwner());
	// ...
	
}

// Called every frame
//void UPlayerNoiseComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
//{
//	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
//
//	// ...
//}

void UPlayerItemComponent::AddPassiveItem(int inPassiveItemNumber)
{

}

bool UPlayerItemComponent::AddToInventory(int ItemNumber)
{

	return true;
}

bool UPlayerItemComponent::RemoveFromInventory()
{

	return true;
}

bool UPlayerItemComponent::AddCigarLight()
{
	// 라이터 아이템의 데이터 가져옴.
	FHorrorGameItemData* CigarLighterData = ItemTable->FindRow<FHorrorGameItemData>(*FString::FromInt(1), TEXT(""));

	// 인벤토리에 있는지 체크해 내부에 존재할 경우
	if (CigarLighterData)
	{
		// 라이터 아이템은 인벤토리에서 항상 첫 번째로 배치되므로 0번만 조사하면 됨.
		if (Inventory[0].ItemNumber == CigarLighterData->ItemNumber)
		{
			return true;
		}
	
		// 인벤토리에 저장할 수 있는 최대 수를 넘었을 경우, 못 얻음
		if (InventorySize >= 9)
		{
			ItemErrorText = NSLOCTEXT("UPlayerItemComponent", "Full_Inventory", "Your Inventory is FULL!. You CANNOT get more items");
			PlayerCharacter->SetErrorText(ItemErrorText, 3);
			return false;
		}

		Inventory[++InventorySize].ItemName = CigarLighterData->ItemName;
		Inventory[InventorySize].ItemNumber = CigarLighterData->ItemNumber;
		Inventory[InventorySize].ItemIcon = CigarLighterData->ItemIcon;
		Inventory[InventorySize].Type = CigarLighterData->Type;
		Inventory[InventorySize].ItemCount = 1;

		// 인벤토리 내부에 아이템이 없었다면 선택한 인벤토리 인덱스를 0으로 설정함.
		if (CurrentSelectedSlot < 0)
		{
			CurrentSelectedSlot = 0;
		}

		InventorySorting();

		return true;
	}

	return false;
}

bool UPlayerItemComponent::AddFlashLight()
{
	// 플래시라이트 아이템의 데이터 가져옴.
	FHorrorGameItemData* FlashLightData = ItemTable->FindRow<FHorrorGameItemData>(*FString::FromInt(2), TEXT(""));

	// 인벤토리에 있는지 체크해 내부에 존재할 경우
	if (FlashLightData)
	{
		// 플래시라이트 아이템은 최대 두 번째 인벤토리에 저장되므로 이까지만 조사하면 됨.
		for (int i = 0; i < 2; ++i)
		{
			if (Inventory[i].ItemNumber == FlashLightData->ItemNumber)
			{
				// 플래시 라이트 배터리 잔량을 초기화해줌.
				Inventory[i].ItemCount = 1;
				FlashLightBattery = 200;
				return true;
			}
		}

		// 인벤토리에 저장할 수 있는 최대 수를 넘었을 경우, 못 얻음
		if (InventorySize >= 9)
		{
			ItemErrorText = NSLOCTEXT("UPlayerItemComponent", "Full_Inventory", "Your Inventory is FULL!. You CANNOT get more items");
			PlayerCharacter->SetErrorText(ItemErrorText, 3);
			return false;
		}

		Inventory[++InventorySize].ItemName = FlashLightData->ItemName;
		Inventory[InventorySize].ItemNumber = FlashLightData->ItemNumber;
		Inventory[InventorySize].ItemIcon = FlashLightData->ItemIcon;
		Inventory[InventorySize].Type = FlashLightData->Type;
		Inventory[InventorySize].ItemCount = 1;

		// 인벤토리 내부에 아이템이 없었다면 선택한 인벤토리 인덱스를 0으로 설정함.
		if (CurrentSelectedSlot < 0)
		{
			CurrentSelectedSlot = 0;
		}

		InventorySorting();

		return true;
	}

	return false;
}

bool UPlayerItemComponent::AddKey()
{
	// 라이터 아이템의 데이터 가져옴.
	FHorrorGameItemData* KeyData = ItemTable->FindRow<FHorrorGameItemData>(*FString::FromInt(3), TEXT(""));

	if (KeyData)
	{
		// 인벤토리에 있는지 체크해 내부에 존재할 경우, 열쇠 아이템은 최대 세 번째 인덱스에 존재함.
		for (int i = 0; i < 3; ++i)
		{
			if (Inventory[i].ItemNumber == KeyData->ItemNumber)
			{
				// 아이템 개수만 증가시킴.
				Inventory[i].ItemCount++;
				return true;
			}
		}

		// 인벤토리에 저장할 수 있는 최대 수를 넘었을 경우, 못 얻음
		if (InventorySize >= 9)
		{
			ItemErrorText = NSLOCTEXT("UPlayerItemComponent", "Full_Inventory", "Your Inventory is FULL!. You CANNOT get more items");
			PlayerCharacter->SetErrorText(ItemErrorText, 3);
			return false;
		}

		Inventory[++InventorySize].ItemName = KeyData->ItemName;
		Inventory[InventorySize].ItemNumber = KeyData->ItemNumber;
		Inventory[InventorySize].ItemIcon = KeyData->ItemIcon;
		Inventory[InventorySize].Type = KeyData->Type;
		Inventory[InventorySize].ItemCount = 1;

		// 인벤토리 내부에 아이템이 없었다면 선택한 인벤토리 인덱스를 0으로 설정함.
		if (CurrentSelectedSlot < 0)
		{
			CurrentSelectedSlot = 0;
		}

		InventorySorting();

		return true;
	}

	return false;
}

bool UPlayerItemComponent::AddTimer()
{
	// 라이터 아이템의 데이터 가져옴.
	FHorrorGameItemData* TimerData= ItemTable->FindRow<FHorrorGameItemData>(*FString::FromInt(4), TEXT(""));

	// 인벤토리에 있는지 체크해 내부에 존재할 경우
	if (TimerData)
	{
		// Binary Search를 통해 인벤토리 내부에 타이머가 있는지 체크.
		int left = 0, right = InventorySize;
		int mid = 0;
		
		while (left <= right)
		{
			mid = left + (right - left) / 2;

			if (Inventory[mid].ItemNumber == TimerData->ItemNumber)
			{
				break;
			}
			else if (Inventory[mid].ItemNumber > TimerData->ItemNumber)
			{
				right = mid - 1;
			}
			else
			{
				left = mid + 1;
			}
		}

		if (Inventory[mid].ItemNumber == TimerData->ItemNumber)
		{
			Inventory[mid].ItemCount++;

			return true;
		}

		// 인벤토리에 저장할 수 있는 최대 수를 넘었을 경우, 못 얻음
		if (InventorySize >= 9)
		{
			ItemErrorText = NSLOCTEXT("UPlayerItemComponent", "Full_Inventory", "Your Inventory is FULL!. You CANNOT get more items");
			PlayerCharacter->SetErrorText(ItemErrorText, 3);
			return false;
		}

		Inventory[++InventorySize].ItemName = TimerData->ItemName;
		Inventory[InventorySize].ItemNumber = TimerData->ItemNumber;
		Inventory[InventorySize].ItemIcon = TimerData->ItemIcon;
		Inventory[InventorySize].Type = TimerData->Type;
		Inventory[InventorySize].ItemCount = 1;

		// 인벤토리 내부에 아이템이 없었다면 선택한 인벤토리 인덱스를 0으로 설정함.
		if (CurrentSelectedSlot < 0)
		{
			CurrentSelectedSlot = 0;
		}

		InventorySorting();

		return true;
	}

	return false;
}

bool UPlayerItemComponent::AddSword()
{
	// 라이터 아이템의 데이터 가져옴.
	FHorrorGameItemData* SwordData = ItemTable->FindRow<FHorrorGameItemData>(*FString::FromInt(5), TEXT(""));

	// 인벤토리에 있는지 체크해 내부에 존재할 경우
	if (SwordData)
	{
		// Binary Search를 통해 인벤토리 내부에 청동 검이 있는지 체크.
		int left = 0, right = InventorySize;
		int mid = 0;

		while (left <= right)
		{
			mid = left + (right - left) / 2;

			if (Inventory[mid].ItemNumber == SwordData->ItemNumber)
			{
				break;
			}
			else if (Inventory[mid].ItemNumber > SwordData->ItemNumber)
			{
				right = mid - 1;
			}
			else
			{
				left = mid + 1;
			}
		}

		if (Inventory[mid].ItemNumber == SwordData->ItemNumber)
		{
			SwordCount++;
			Inventory[mid].ItemCount++;

			return true;
		}

		// 인벤토리에 저장할 수 있는 최대 수를 넘었을 경우, 못 얻음
		if (InventorySize >= 9)
		{
			ItemErrorText = NSLOCTEXT("UPlayerItemComponent", "Full_Inventory", "Your Inventory is FULL!. You CANNOT get more items");
			PlayerCharacter->SetErrorText(ItemErrorText, 3);
			return false;
		}

		Inventory[++InventorySize].ItemName = SwordData->ItemName;
		Inventory[InventorySize].ItemNumber = SwordData->ItemNumber;
		Inventory[InventorySize].ItemIcon = SwordData->ItemIcon;
		Inventory[InventorySize].Type = SwordData->Type;
		Inventory[InventorySize].ItemCount = 1;

		// 인벤토리 내부에 아이템이 없었다면 선택한 인벤토리 인덱스를 0으로 설정함.
		if (CurrentSelectedSlot < 0)
		{
			CurrentSelectedSlot = 0;
		}

		InventorySorting();

		return true;
	}

	return false;
}

bool UPlayerItemComponent::AddBell()
{
	// 라이터 아이템의 데이터 가져옴.
	FHorrorGameItemData* BellData = ItemTable->FindRow<FHorrorGameItemData>(*FString::FromInt(6), TEXT(""));

	// 인벤토리에 있는지 체크해 내부에 존재할 경우
	if (BellData)
	{
		// Binary Search를 통해 인벤토리 내부에 청동 방울이 있는지 체크.
		int left = 0, right = InventorySize;
		int mid = 0;

		while (left <= right)
		{
			mid = left + (right - left) / 2;

			if (Inventory[mid].ItemNumber == BellData->ItemNumber)
			{
				break;
			}
			else if (Inventory[mid].ItemNumber > BellData->ItemNumber)
			{
				right = mid - 1;
			}
			else
			{
				left = mid + 1;
			}
		}

		if (Inventory[mid].ItemNumber == BellData->ItemNumber)
		{
			BellCount++;
			Inventory[mid].ItemCount++;

			return true;
		}

		// 인벤토리에 저장할 수 있는 최대 수를 넘었을 경우, 못 얻음
		if (InventorySize >= 9)
		{
			ItemErrorText = NSLOCTEXT("UPlayerItemComponent", "Full_Inventory", "Your Inventory is FULL!. You CANNOT get more items");
			PlayerCharacter->SetErrorText(ItemErrorText, 3);
			return false;
		}

		Inventory[++InventorySize].ItemName = BellData->ItemName;
		Inventory[InventorySize].ItemNumber = BellData->ItemNumber;
		Inventory[InventorySize].ItemIcon = BellData->ItemIcon;
		Inventory[InventorySize].Type = BellData->Type;
		Inventory[InventorySize].ItemCount = 1;

		// 인벤토리 내부에 아이템이 없었다면 선택한 인벤토리 인덱스를 0으로 설정함.
		if (CurrentSelectedSlot < 0)
		{
			CurrentSelectedSlot = 0;
		}

		InventorySorting();

		return true;
	}

	return false;
}

bool UPlayerItemComponent::AddMirror()
{
	// 라이터 아이템의 데이터 가져옴.
	FHorrorGameItemData* MirrorData = ItemTable->FindRow<FHorrorGameItemData>(*FString::FromInt(7), TEXT(""));

	// 인벤토리에 있는지 체크해 내부에 존재할 경우
	if (MirrorData)
	{
		// Binary Search를 통해 인벤토리 내부에 청동 거울이 있는지 체크.
		int left = 0, right = InventorySize;
		int mid = 0;

		while (left <= right)
		{
			mid = left + (right - left) / 2;

			if (Inventory[mid].ItemNumber == MirrorData->ItemNumber)
			{
				break;
			}
			else if (Inventory[mid].ItemNumber > MirrorData->ItemNumber)
			{
				right = mid - 1;
			}
			else
			{
				left = mid + 1;
			}
		}

		if (Inventory[mid].ItemNumber == MirrorData->ItemNumber)
		{
			MirrorCount++;
			Inventory[mid].ItemCount++;

			return true;
		}

		// 인벤토리에 저장할 수 있는 최대 수를 넘었을 경우, 못 얻음
		if (InventorySize >= 9)
		{
			ItemErrorText = NSLOCTEXT("UPlayerItemComponent", "Full_Inventory", "Your Inventory is FULL!. You CANNOT get more items");
			PlayerCharacter->SetErrorText(ItemErrorText, 3);
			return false;
		}

		Inventory[++InventorySize].ItemName = MirrorData->ItemName;
		Inventory[InventorySize].ItemNumber = MirrorData->ItemNumber;
		Inventory[InventorySize].ItemIcon = MirrorData->ItemIcon;
		Inventory[InventorySize].Type = MirrorData->Type;
		Inventory[InventorySize].ItemCount = 1;

		// 인벤토리 내부에 아이템이 없었다면 선택한 인벤토리 인덱스를 0으로 설정함.
		if (CurrentSelectedSlot < 0)
		{
			CurrentSelectedSlot = 0;
		}

		InventorySorting();

		return true;
	}

	return false;
}

bool UPlayerItemComponent::AddExtinguisher()
{
	// 라이터 아이템의 데이터 가져옴.
	FHorrorGameItemData* ExtinguisherData = ItemTable->FindRow<FHorrorGameItemData>(*FString::FromInt(8), TEXT(""));

	// 인벤토리에 있는지 체크해 내부에 존재할 경우
	if (ExtinguisherData)
	{
		// Binary Search를 통해 인벤토리 내부에 소화기가 있는지 체크.
		int left = 0, right = InventorySize;
		int mid = 0;

		while (left <= right)
		{
			mid = left + (right - left) / 2;

			if (Inventory[mid].ItemNumber == ExtinguisherData->ItemNumber)
			{
				break;
			}
			else if (Inventory[mid].ItemNumber > ExtinguisherData->ItemNumber)
			{
				right = mid - 1;
			}
			else
			{
				left = mid + 1;
			}
		}

		if (Inventory[mid].ItemNumber == ExtinguisherData->ItemNumber)
		{
			Inventory[mid].ItemCount = 1;
			ExtinguisherLeft = 100;

			return true;
		}

		// 인벤토리에 저장할 수 있는 최대 수를 넘었을 경우, 못 얻음
		if (InventorySize >= 9)
		{
			ItemErrorText = NSLOCTEXT("UPlayerItemComponent", "Full_Inventory", "Your Inventory is FULL!. You CANNOT get more items");
			PlayerCharacter->SetErrorText(ItemErrorText, 3);
			return false;
		}

		Inventory[++InventorySize].ItemName = ExtinguisherData->ItemName;
		Inventory[InventorySize].ItemNumber = ExtinguisherData->ItemNumber;
		Inventory[InventorySize].ItemIcon = ExtinguisherData->ItemIcon;
		Inventory[InventorySize].Type = ExtinguisherData->Type;
		Inventory[InventorySize].ItemCount = 1;

		// 인벤토리 내부에 아이템이 없었다면 선택한 인벤토리 인덱스를 0으로 설정함.
		if (CurrentSelectedSlot < 0)
		{
			CurrentSelectedSlot = 0;
		}

		InventorySorting();

		return true;
	}

	return false;
}

bool UPlayerItemComponent::AddCutter()
{
	// 라이터 아이템의 데이터 가져옴.
	FHorrorGameItemData* CutterData = ItemTable->FindRow<FHorrorGameItemData>(*FString::FromInt(9), TEXT(""));

	// 인벤토리에 있는지 체크해 내부에 존재할 경우
	if (CutterData)
	{
		// Binary Search를 통해 인벤토리 내부에 절단기가 있는지 체크.
		int left = 0, right = InventorySize;
		int mid = 0;

		while (left <= right)
		{
			mid = left + (right - left) / 2;

			if (Inventory[mid].ItemNumber == CutterData->ItemNumber)
			{
				break;
			}
			else if (Inventory[mid].ItemNumber > CutterData->ItemNumber)
			{
				right = mid - 1;
			}
			else
			{
				left = mid + 1;
			}
		}

		if (Inventory[mid].ItemNumber == CutterData->ItemNumber)
		{
			Inventory[mid].ItemCount = 1;

			return true;
		}

		// 인벤토리에 저장할 수 있는 최대 수를 넘었을 경우, 못 얻음
		if (InventorySize >= 9)
		{
			ItemErrorText = NSLOCTEXT("UPlayerItemComponent", "Full_Inventory", "Your Inventory is FULL!. You CANNOT get more items");
			PlayerCharacter->SetErrorText(ItemErrorText, 3);
			return false;
		}

		Inventory[++InventorySize].ItemName = CutterData->ItemName;
		Inventory[InventorySize].ItemNumber = CutterData->ItemNumber;
		Inventory[InventorySize].ItemIcon = CutterData->ItemIcon;
		Inventory[InventorySize].Type = CutterData->Type;
		Inventory[InventorySize].ItemCount = 1;

		// 인벤토리 내부에 아이템이 없었다면 선택한 인벤토리 인덱스를 0으로 설정함.
		if (CurrentSelectedSlot < 0)
		{
			CurrentSelectedSlot = 0;
		}

		InventorySorting();

		return true;
	}

	return false;
}

bool UPlayerItemComponent::AddLantern()
{
	// 라이터 아이템의 데이터 가져옴.
	FHorrorGameItemData* LanternData = ItemTable->FindRow<FHorrorGameItemData>(*FString::FromInt(10), TEXT(""));

	// 인벤토리에 있는지 체크해 내부에 존재할 경우
	if (LanternData)
	{
		// Binary Search를 통해 인벤토리 내부에 영혼 랜턴이 있는지 체크.
		int left = 0, right = InventorySize;
		int mid = 0;

		while (left <= right)
		{
			mid = left + (right - left) / 2;

			if (Inventory[mid].ItemNumber == LanternData->ItemNumber)
			{
				break;
			}
			else if (Inventory[mid].ItemNumber > LanternData->ItemNumber)
			{
				right = mid - 1;
			}
			else
			{
				left = mid + 1;
			}
		}

		if (Inventory[mid].ItemNumber == LanternData->ItemNumber)
		{
			ItemErrorText = NSLOCTEXT("UPlayerItemComponent", "Already_Get_Lantern", "You can get ONLY 1 Lantern");
			PlayerCharacter->SetErrorText(ItemErrorText, 3);
			return false;
		}

		// 인벤토리에 저장할 수 있는 최대 수를 넘었을 경우, 못 얻음
		if (InventorySize >= 9)
		{
			ItemErrorText = NSLOCTEXT("UPlayerItemComponent", "Full_Inventory", "Your Inventory is FULL!. You CANNOT get more items");
			PlayerCharacter->SetErrorText(ItemErrorText, 3);
			return false;
		}

		Inventory[++InventorySize].ItemName = LanternData->ItemName;
		Inventory[InventorySize].ItemNumber = LanternData->ItemNumber;
		Inventory[InventorySize].ItemIcon = LanternData->ItemIcon;
		Inventory[InventorySize].Type = LanternData->Type;
		Inventory[InventorySize].ItemCount = 1;

		// 인벤토리 내부에 아이템이 없었다면 선택한 인벤토리 인덱스를 0으로 설정함.
		if (CurrentSelectedSlot < 0)
		{
			CurrentSelectedSlot = 0;
		}

		InventorySorting();

		return true;
	}

	return false;
}

bool UPlayerItemComponent::AddGlowStick()
{
	// 라이터 아이템의 데이터 가져옴.
	FHorrorGameItemData* GlowstickData = ItemTable->FindRow<FHorrorGameItemData>(*FString::FromInt(11), TEXT(""));

	// 인벤토리에 있는지 체크해 내부에 존재할 경우
	if (GlowstickData)
	{
		// Binary Search를 통해 인벤토리 내부에 야광봉이 있는지 체크.
		int left = 0, right = InventorySize;
		int mid = 0;

		while (left <= right)
		{
			mid = left + (right - left) / 2;

			if (Inventory[mid].ItemNumber == GlowstickData->ItemNumber)
			{
				break;
			}
			else if (Inventory[mid].ItemNumber > GlowstickData->ItemNumber)
			{
				right = mid - 1;
			}
			else
			{
				left = mid + 1;
			}
		}

		if (Inventory[mid].ItemNumber == GlowstickData->ItemNumber)
		{
			Inventory[mid].ItemCount++;

			return true;
		}

		// 인벤토리에 저장할 수 있는 최대 수를 넘었을 경우, 못 얻음
		if (InventorySize >= 9)
		{
			ItemErrorText = NSLOCTEXT("UPlayerItemComponent", "Full_Inventory", "Your Inventory is FULL!. You CANNOT get more items");
			PlayerCharacter->SetErrorText(ItemErrorText, 3);
			return false;
		}

		Inventory[++InventorySize].ItemName = GlowstickData->ItemName;
		Inventory[InventorySize].ItemNumber = GlowstickData->ItemNumber;
		Inventory[InventorySize].ItemIcon = GlowstickData->ItemIcon;
		Inventory[InventorySize].Type = GlowstickData->Type;
		Inventory[InventorySize].ItemCount = 1;

		// 인벤토리 내부에 아이템이 없었다면 선택한 인벤토리 인덱스를 0으로 설정함.
		if (CurrentSelectedSlot < 0)
		{
			CurrentSelectedSlot = 0;
		}

		InventorySorting();

		return true;
	}

	return false;
}

bool UPlayerItemComponent::UseItem()
{
	
	return true;
}

bool UPlayerItemComponent::UseCigarLight()
{
	// 라이터는 소모성 아이템이 아니기 때문에 이 함수에서는 할 일이 없음.	
	return true;
}

bool UPlayerItemComponent::UseFlashLight()
{
	// 배터리 관련 함수로 써야할 듯 함.
	return true;
}

bool UPlayerItemComponent::UseKey()
{

	return true;
}

bool UPlayerItemComponent::UseTimer()
{

	return true;
}

bool UPlayerItemComponent::UseSword()
{

	return true;
}

bool UPlayerItemComponent::UseBell()
{

	return true;
}

bool UPlayerItemComponent::UseMirror()
{

	return true;
}

bool UPlayerItemComponent::UseExtinguisher()
{

	return true;
}

bool UPlayerItemComponent::UseCutter()
{

	return true;
}

bool UPlayerItemComponent::UseLantern()
{

	return true;
}

bool UPlayerItemComponent::UseGlowStick()
{

	return true;
}

int32 UPlayerItemComponent::GetSwordNumbers()
{
	return SwordCount;
}

int32 UPlayerItemComponent::GetMirrorNumbers()
{
	return MirrorCount;
}

int32 UPlayerItemComponent::GetBellNumbers()
{
	return BellCount;
}

int32 UPlayerItemComponent::GetCurrentItemNumber()
{
	return CurrentSelectedItemNumber;
}

void UPlayerItemComponent::InventorySorting()
{
	int r[9] = { 0, };
	bool b[11] = { false, };

	for (int i = 0; i < 9;)
	{
		r[i] = FMath::RandRange(1, 11);

		if (b[r[i] - 1])
		{
			continue;
		}
		else
		{
			b[r[i] - 1] = true;
			i++;
		}
	}

	for (int i = 0; i < 9; ++i)
	{
		FHorrorGameItemData* ItemData = ItemTable->FindRow<FHorrorGameItemData>(*FString::FromInt(r[i]), TEXT(""));

		Inventory[i].ItemName = ItemData->ItemName;
		Inventory[i].ItemNumber = ItemData->ItemNumber;
		Inventory[i].ItemIcon = ItemData->ItemIcon;
		Inventory[i].Type = ItemData->Type;
		Inventory[i].ItemCount = 1;
	}

	UE_LOG(LogTemp, Warning, TEXT("Before Inventory Sorting"));
	for (int i = 0; i < 9; ++i)
	{
		UE_LOG(LogTemp, Warning, TEXT("Inventory[%d]: %s"), i, *Inventory[i].ItemName.ToString());
	}


	TFunction<bool(FHorrorGameItemData, FHorrorGameItemData)> Compare = [](FHorrorGameItemData a, FHorrorGameItemData b)
	{

		return  a.ItemNumber < b.ItemNumber;

	};

	Inventory.Sort(Compare);

	UE_LOG(LogTemp, Warning, TEXT("After Inventory Sorting"));
	for (int i = 0; i < 9; ++i)
	{
		UE_LOG(LogTemp, Warning, TEXT("Inventory[%d]: %s"), i, *Inventory[i].ItemName.ToString());
	}
}