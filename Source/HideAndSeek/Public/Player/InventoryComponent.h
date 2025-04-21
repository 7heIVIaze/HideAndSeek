// CopyrightNotice 2023 Sunggon Kim kimdave205@gmail.com. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Resource/CustomDataTables.h"
#include "InventoryComponent.generated.h"

// 파라미터: 현재 선택 중인 아이템 데이터, 현재 선택 중인 인덱스.
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnItemAcquired, const FHorrorGameItemData&, NewItemData, int32, ItemIndex);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnItemSwitch, const FHorrorGameItemData&, ChangedItemData, int32, ItemIndex);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnItemConsumed, const FHorrorGameItemData&, ChangedItemData, int32, ItemIndex);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnDurabilityChanged, const FHorrorGameItemData&, ItemData, int32, ItemIndex);

// 오브젝트 아이템(환혼석) 수집 및 사용 이벤트
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnObjectAcquired, int32, ObjectCount);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnObjectConsumed, int32, ObjectCount);

// 아이템 수집 불가 이벤트
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnItemMessage, FText, Message);


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class HIDEANDSEEK_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UInventoryComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// 인벤토리에 아이템을 추가하는 함수 : 이 함수는 새로운 아이템의 개수를 필요로 하기 때문에, 파라미터로 아이템 객체를 받음.
	UFUNCTION(BlueprintCallable)
	bool AddToInventory(class AItemClass* Item, class AHorrorGameCharacter* Player);

	// 인벤토리에 아이템을 변경하는 함수 : 이 함수는 새로운 아이템의 개수를 필요로 하기 때문에, 파라미터로 아이템 객체를 받음.
	UFUNCTION(BlueprintCallable)
	bool ChangeCurrentItem(class AItemClass* ChangeItem, class AHorrorGameCharacter* Player);

	// 조명 아이템을 사용하는 함수.
	UFUNCTION(BlueprintCallable)
	bool UseLightingItem(class AHorrorGameCharacter* Player);

	// 인벤토리 내의 다른 조명 아이템을 사용하는 함수.
	UFUNCTION(BlueprintCallable)
	bool ChangeAnotherLightingItem(class AHorrorGameCharacter* Player);

	// 소비형 아이템을 사용하는 함수.
	UFUNCTION(BlueprintCallable)
	bool UseConsumableItem(class AHorrorGameCharacter* Player);

	// 라이터를 슬롯에 추가하는 함수 : 이 함수는 플레이어의 위젯을 변경시키므로, 플레이어 포인터를 파라미터로 받음. return 0: 실패, 1: 성공, 2: 이미 인벤토리 내에 존재함.
	UFUNCTION(BlueprintCallable)
	int AddCigarLighter(class AItemClass* Item, int IndexToInsert, class AHorrorGameCharacter* Player);

	// 라이터를 슬롯에서 변경하는 함수 : 이 함수는 플레이어의 위젯을 변경시키므로, 플레이어 포인터를 파라미터로 받음. return 0: 실패, 1: 성공, 2: 이미 인벤토리 내에 존재함.
	UFUNCTION(BlueprintCallable)
	int ChangeCigarLighter(class AItemClass* ChangedItem, int IndexToInsert, class AHorrorGameCharacter* Player);

	// 라이터를 사용하는 함수.
	UFUNCTION(BlueprintCallable)
	bool TurnonCigarlighter(class AHorrorGameCharacter* Player);

	// 손전등을 슬롯에 추가하는 함수 : 이 함수는 플레이어의 위젯을 변경시키므로, 플레이어 포인터를 파라미터로 받음. return 0: 실패, 1: 성공, 2: 이미 인벤토리 내에 존재함.
	UFUNCTION(BlueprintCallable)
	int AddFlashlight(class AItemClass* Item, int IndexToInsert, class AHorrorGameCharacter* Player);

	// 손전등을 슬롯에서 변경하는 함수 : 이 함수는 플레이어의 위젯을 변경시키므로, 플레이어 포인터를 파라미터로 받음. return 0: 실패, 1: 성공, 2: 이미 인벤토리 내에 존재함.
	UFUNCTION(BlueprintCallable)
	int ChangeFlashlight(class AItemClass* ChangedItem, int IndexToInsert, class AHorrorGameCharacter* Player);

	// 손전등을 사용하는 함수.
	UFUNCTION(BlueprintCallable)
	bool TurnonFlashlight(class AHorrorGameCharacter* Player);

	// 손전등의 배터리가 다 했을 때 호출할 함수.
	UFUNCTION(BlueprintCallable)
	void FlashlightDischarge();

	// 열쇠를 슬롯에 추가하는 함수 : 이 함수는 플레이어의 위젯을 변경시키므로, 플레이어 포인터를 파라미터로 받음. return 0: 실패, 1: 성공, 2: 이미 인벤토리 내에 존재함.
	UFUNCTION(BlueprintCallable)
	int AddKey(class AItemClass* Item, int IndexToInsert, class AHorrorGameCharacter* Player);

	// 열쇠를 슬롯에서 변경하는 함수 : 이 함수는 플레이어의 위젯을 변경시키므로, 플레이어 포인터를 파라미터로 받음. return 0: 실패, 1: 성공, 2: 이미 인벤토리 내에 존재함.
	UFUNCTION(BlueprintCallable)
	int ChangeKey(class AItemClass* ChangedItem, int IndexToInsert, class AHorrorGameCharacter* Player);

	// 열쇠를 사용하는 함수.
	UFUNCTION(BlueprintCallable)
	bool UseKey(class AHorrorGameCharacter* Player);

	// 소음을 유발하는 아이템(타이머)을 슬롯에 추가하는 함수 : 이 함수는 플레이어의 위젯을 변경시키므로, 플레이어 포인터를 파라미터로 받음. return 0: 실패, 1: 성공, 2: 이미 인벤토리 내에 존재함.
	UFUNCTION(BlueprintCallable)
	int AddTimer(class AItemClass* Item, int IndexToInsert, class AHorrorGameCharacter* Player);

	// 소음을 유발하는 아이템(타이머)을 슬롯에서 변경하는 함수 : 이 함수는 플레이어의 위젯을 변경시키므로, 플레이어 포인터를 파라미터로 받음. return 0: 실패, 1: 성공, 2: 이미 인벤토리 내에 존재함.
	UFUNCTION(BlueprintCallable)
	int ChangeTimer(class AItemClass* ChangedItem, int IndexToInsert, class AHorrorGameCharacter* Player);

	// 소음을 유발하는 아이템(타이머)을 사용하는 함수.
	UFUNCTION(BlueprintCallable)
	bool UseTimer(class AHorrorGameCharacter* Player);

	// 청동 검을 슬롯에 추가하는 함수 : 이 함수는 플레이어의 위젯을 변경시키므로, 플레이어 포인터를 파라미터로 받음. return 0: 실패, 1: 성공, 2: 이미 인벤토리 내에 존재함.
	UFUNCTION(BlueprintCallable)
	int AddSword(class AItemClass* Item, int IndexToInsert, class AHorrorGameCharacter* Player);

	// 청동 검을 슬롯에서 변경하는 함수 : 이 함수는 플레이어의 위젯을 변경시키므로, 플레이어 포인터를 파라미터로 받음. return 0: 실패, 1: 성공, 2: 이미 인벤토리 내에 존재함.
	UFUNCTION(BlueprintCallable)
	int ChangeSword(class AItemClass* ChangedItem, int IndexToInsert, class AHorrorGameCharacter* Player);

	// 청동 검을 사용하는 함수.
	UFUNCTION(BlueprintCallable)
	bool UseSword(class AHorrorGameCharacter* Player);

	// 청동 방울을 슬롯에 추가하는 함수 : 이 함수는 플레이어의 위젯을 변경시키므로, 플레이어 포인터를 파라미터로 받음. return 0: 실패, 1: 성공, 2: 이미 인벤토리 내에 존재함.
	UFUNCTION(BlueprintCallable)
	int AddBell(class AItemClass* Item, int IndexToInsert, class AHorrorGameCharacter* Player);

	// 청동 방울을 슬롯에서 변경하는 함수 : 이 함수는 플레이어의 위젯을 변경시키므로, 플레이어 포인터를 파라미터로 받음. return 0: 실패, 1: 성공, 2: 이미 인벤토리 내에 존재함.
	UFUNCTION(BlueprintCallable)
	int ChangeBell(class AItemClass* ChangedItem, int IndexToInsert, class AHorrorGameCharacter* Player);

	// 청동 방울을 사용하는 함수.
	UFUNCTION(BlueprintCallable)
	bool UseBell(class AHorrorGameCharacter* Player);

	// 청동 거울을 슬롯에 추가하는 함수 : 이 함수는 플레이어의 위젯을 변경시키므로, 플레이어 포인터를 파라미터로 받음. return 0: 실패, 1: 성공, 2: 이미 인벤토리 내에 존재함.
	UFUNCTION(BlueprintCallable)
	int AddMirror(class AItemClass* Item, int IndexToInsert, class AHorrorGameCharacter* Player);

	// 청동 거울을 슬롯에서 변경하는 함수 : 이 함수는 플레이어의 위젯을 변경시키므로, 플레이어 포인터를 파라미터로 받음. return 0: 실패, 1: 성공, 2: 이미 인벤토리 내에 존재함.
	UFUNCTION(BlueprintCallable)
	int ChangeMirror(class AItemClass* ChangedItem, int IndexToInsert, class AHorrorGameCharacter* Player);

	// 청동 거울을 사용하는 함수.
	UFUNCTION(BlueprintCallable)
	bool UseMirror(class AHorrorGameCharacter* Player);

	// 절단기를 슬롯에 추가하는 함수 : 이 함수는 플레이어의 위젯을 변경시키므로, 플레이어 포인터를 파라미터로 받음. return 0: 실패, 1: 성공, 2: 이미 인벤토리 내에 존재함.
	UFUNCTION(BlueprintCallable)
	int AddCutter(class AItemClass* Item, int IndexToInsert, class AHorrorGameCharacter* Player);

	// 절단기를 슬롯에서 변경하는 함수 : 이 함수는 플레이어의 위젯을 변경시키므로, 플레이어 포인터를 파라미터로 받음. return 0: 실패, 1: 성공, 2: 이미 인벤토리 내에 존재함.
	UFUNCTION(BlueprintCallable)
	int ChangeCutter(class AItemClass* ChangedItem, int IndexToInsert, class AHorrorGameCharacter* Player);

	// 절단기를 사용하는 함수.
	UFUNCTION(BlueprintCallable)
	bool UseCutter(class AHorrorGameCharacter* Player);

	// 소화기를 슬롯에 추가하는 함수 : 이 함수는 플레이어의 위젯을 변경시키므로, 플레이어 포인터를 파라미터로 받음. return 0: 실패, 1: 성공, 2: 이미 인벤토리 내에 존재함.
	UFUNCTION(BlueprintCallable)
	int AddExtinguisher(class AItemClass* Item, int IndexToInsert, class AHorrorGameCharacter* Player);

	// 소화기를 슬롯에서 변경하는 함수 : 이 함수는 플레이어의 위젯을 변경시키므로, 플레이어 포인터를 파라미터로 받음. return 0: 실패, 1: 성공, 2: 이미 인벤토리 내에 존재함.
	UFUNCTION(BlueprintCallable)
	int ChangeExtinguisher(class AItemClass* ChangedItem, int IndexToInsert, class AHorrorGameCharacter* Player);

	// 소화기를 사용하는 함수
	UFUNCTION(BlueprintCallable)
	bool UseExtinguisher();

	// 랜턴을 슬롯에 추가하는 함수 : 이 함수는 플레이어의 위젯을 변경시키므로, 플레이어 포인터를 파라미터로 받음. return 0: 실패, 1: 성공, 2: 이미 인벤토리 내에 존재함.
	UFUNCTION(BlueprintCallable)
	int AddLantern(class AItemClass* Item, int IndexToInsert, class AHorrorGameCharacter* Player);

	// 랜턴을 슬롯에서 변경하는 함수 : 이 함수는 플레이어의 위젯을 변경시키므로, 플레이어 포인터를 파라미터로 받음. return 0: 실패, 1: 성공, 2: 이미 인벤토리 내에 존재함.
	UFUNCTION(BlueprintCallable)
	int ChangeLantern(class AItemClass* ChangedItem, int IndexToInsert, class AHorrorGameCharacter* Player);

	// 랜턴을 사용하는 함수.
	UFUNCTION(BlueprintCallable)
	bool UseLantern(class AHorrorGameCharacter* Player);

	// 야광봉을 슬롯에 추가하는 함수 : 이 함수는 플레이어의 위젯을 변경시키므로, 플레이어 포인터를 파라미터로 받음. return 0: 실패, 1: 성공, 2: 이미 인벤토리 내에 존재함.
	UFUNCTION(BlueprintCallable)
	int AddGlowstick(class AItemClass* Item, int IndexToInsert, class AHorrorGameCharacter* Player);

	// 야광봉을 슬롯에서 변경하는 함수 : 이 함수는 플레이어의 위젯을 변경시키므로, 플레이어 포인터를 파라미터로 받음. return 0: 실패, 1: 성공, 2: 이미 인벤토리 내에 존재함.
	UFUNCTION(BlueprintCallable)
	int ChangeGlowstick(class AItemClass* ChangedItem, int IndexToInsert, class AHorrorGameCharacter* Player);

	// 야광봉을 사용하는 함수.
	UFUNCTION(BlueprintCallable)
	bool UseGlowstick(class AHorrorGameCharacter* Player);

	// 나침반을 슬롯에 추가하는 함수 : 이 함수는 플레이어의 위젯을 변경시키므로, 플레이어 포인터를 파라미터로 받음. return 0: 실패, 1: 성공, 2: 이미 인벤토리 내에 존재함.
	UFUNCTION(BlueprintCallable)
	int AddCompass(class AItemClass* Item, class AHorrorGameCharacter* Player);

	// 나침반을 사용하는 함수.
	UFUNCTION(BlueprintCallable)
	bool UseCompass(class AHorrorGameCharacter* Player);

	// 환혼석을 슬롯에 추가하는 함수 : 이 함수는 플레이어의 위젯을 변경시키므로, 플레이어 포인터를 파라미터로 받음. return 0: 실패, 1: 성공, 2: 이미 인벤토리 내에 존재함.
	UFUNCTION(BlueprintCallable)
	int AddStone(class AItemClass* Item, class AHorrorGameCharacter* Player);

	// 환혼석을 사용하는 함수.
	UFUNCTION(BlueprintCallable)
	bool UseStone(class AHorrorGameCharacter* Player);

	// 현재 선택 중인 아이템 변경하는 함수.
	UFUNCTION(BlueprintCallable)
	void MoveItemIndex(int AddValue);

	// 내구도를 감소시키는 함수.
	UFUNCTION(BlueprintCallable)
	void DecreaseDurability(EItemType InventoryType, EItemNumber ItemNumber);

public:
	// 소비형 아이템 인벤토리 : 기본적으로 5개의 슬롯을 가지나 최대 8개의 슬롯을 가질 수 있음. 이로 인해 벡터(TArray) 컨테이너 사용..
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Inventory)
	TArray<struct FHorrorGameItemData> Consumable_Inventory;

	// 조명 아이템 인벤토리 : 기본적으로 2개의 슬롯을 가지나, 최대 3개의 슬롯을 가질 수 있음. 이로 인해 벡터(TArray) 컨테이너 사용.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Inventory)
	TArray<struct FHorrorGameItemData> Lighting_Inventory;

	// 패시브(장비형) 아이템 인벤토리 : 기본적으로 1개의 슬롯을 가지나, 최대 2개의 슬롯을 가질 수 있음. 이로 인해 벡터(TArray) 컨테이너 사용.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Inventory)
	TArray<struct FHorrorGameItemData> Passive_Inventory;

	// 특수 아이템(나침반용) 인벤토리 : 1개의 슬롯을 가짐.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Inventory)
	struct FHorrorGameItemData Unique_Inventory;

	// 현재까지 수집한 오브젝트 아이템(환혼석)의 개수.
	UPROPERTY(VisibleAnywhere, Category = Object)
	int ObjectCount;

	// 인벤토리 내에 존재하는 소비형 아이템의 개수.
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Inventory)
	int32 ConsumableInventoryNumber;

	// 현재 선택 중인 소비형 아이템 인벤토리의 인덱스.
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Inventory)
	int32 CurrentConsumableItemIndex;

	// 소비형 아이템 인벤토리의 최대 크기 : 기본은 5이나, 최대 8까지 증가가능.
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Inventory)
	int32 ConsumableInventoryMaxSize;

	// 인벤토리 내에 존재하는 조명 아이템의 개수.
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Inventory)
	int32 LightingInventoryNumber;

	// 현재 선택 중인 조명 아이템 인벤토리의 인덱스.
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Inventory)
	int32 CurrentLightingItemIndex;

	// 조명 아이템 인벤토리의 최대 크기 : 기본은 2이나, 최대 3까지 증가가능.
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Inventory)
	int32 LightingInventoryMaxSize;

	// 패시브(장비형) 아이템 인벤토리의 최대 크기 : 기본은 1이나, 최대 2까지 증가가능.
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Inventory)
	int32 PassiveInventoryMaxSize;

	// 인벤토리 내에 존재하는 패시브(장비형) 아이템의 개수.
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Inventory)
	int32 PassiveItemNumber;

	// This is for flashlight battery
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Boolean)
	bool bIsFlashlightOn;

	// This if for extinguisher spray
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Boolean)
	bool bIsExtinguisherSpraying;

	// This is for compass use.
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Boolean)
	bool bDoHaveCompass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item)
	UDataTable* ItemDataTable;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Trace)
	TObjectPtr<class AHorrorGameCharacter> OwnerPlayer;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Trace)
	TObjectPtr<class UGameUI> GameUIWidget;

	// Current selected inventory type : 0=Equipment, 1:Useable, 2:Lighting.
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Inventory)
	int CurrentInventoryType;

	UPROPERTY(VisibleAnywhere, Category = Timer)
	float FlashlightTimer = 0.0f;

	UPROPERTY(VisibleAnywhere, Category = Timer)
	float ExtinguisherTimer = 0.0f;

	// 게임 인스턴스(세이브 데이터 관련)
	UPROPERTY(VisibleAnywhere, Category = GameInstance)
	TObjectPtr<class UHorrorGameGameInstance> GameInstance;

	// Add the item to inventory. : Called when acquiring an item, discarding the currently selected item, and acquiring another item.
	UPROPERTY(BlueprintAssignable, Category = Event)
	FOnItemAcquired OnItemAcquired;

	// Switch the item. : Called when selecting another item inside the inventory.
	UPROPERTY(BlueprintAssignable, Category = Event)
	FOnItemSwitch OnItemSwitch;

	// Consume the item. : Called when the durability of the currently selected item runs out or when all items are consumed.
	UPROPERTY(BlueprintAssignable, Category = Event)
	FOnItemConsumed OnItemConsumed;

	// Reduce durability of the item. : Event called when durability changes.
	UPROPERTY(BlueprintAssignable, Category = Event)
	FOnDurabilityChanged OnDurabilityChanged;

	// Collect the object item.
	UPROPERTY(BlueprintAssignable, Category = Event)
	FOnObjectAcquired OnObjectAcquired;

	UPROPERTY(BlueprintAssignable, Category = Event)
	FOnObjectConsumed OnObjectConsumed;

	UPROPERTY(BlueprintAssignable, Category = Event)
	FOnItemMessage OnItemMessage;
};
