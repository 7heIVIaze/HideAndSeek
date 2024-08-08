// CopyrightNotice 2023 Sunggon Kim kimdave205@gmail.com. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Resource/CustomDataTables.h"
#include "PlayerItemComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class HIDEANDSEEK_API UPlayerItemComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPlayerItemComponent();

	UPROPERTY(BlueprintReadWrite)
		TArray<struct FHorrorGameItemData> Inventory;
	/*UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Collider")
		TObjectPtr<class USphereComponent> SphereCollider;*/

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Data)
	UDataTable* ItemTable;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	//virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	UFUNCTION(BlueprintCallable)
	void AddPassiveItem(int inPassiveItemNumber); // inPassiveItemNumber is PassiveItemNumber. It starts from 0.

	// A function that puts the collected items into the inventory.
	UFUNCTION(BlueprintCallable)
	bool AddToInventory(int ItemNumber);

	// A function that removes all consumed items from the inventory
	UFUNCTION(BlueprintCallable)
	bool RemoveFromInventory();

	// Add the cigarlighter into the inventory.
	UFUNCTION(BlueprintCallable)
	bool AddCigarLight();

	// Add the flashlight into the inventory.
	UFUNCTION(BlueprintCallable)
	bool AddFlashLight();

	// Add the key into the inventory.
	UFUNCTION(BlueprintCallable)
	bool AddKey();

	// Add the timer into the inventory.
	UFUNCTION(BlueprintCallable)
	bool AddTimer();

	// Add the bronze sword into the inventory.
	UFUNCTION(BlueprintCallable)
	bool AddSword();

	// Add the bronze bell into the inventory.
	UFUNCTION(BlueprintCallable)
	bool AddBell();

	// Add the bronze mirror into the inventory.
	UFUNCTION(BlueprintCallable)
	bool AddMirror();

	// Add the extinguisher into the inventory.
	UFUNCTION(BlueprintCallable)
	bool AddExtinguisher();

	// Add the cutter into the inventory.
	UFUNCTION(BlueprintCallable)
	bool AddCutter();

	// Add the lantern into the inventory.
	UFUNCTION(BlueprintCallable)
	bool AddLantern();

	// Add the glowstick into the inventory.
	UFUNCTION(BlueprintCallable)
	bool AddGlowStick();

	// A function to use item.
	UFUNCTION(BlueprintCallable)
	bool UseItem();

	// Use the cigarlighter.
	UFUNCTION(BlueprintCallable)
	bool UseCigarLight();

	// Use the flashlight.
	UFUNCTION(BlueprintCallable)
	bool UseFlashLight();

	// Add the key into the inventory.
	UFUNCTION(BlueprintCallable)
	bool UseKey();

	// Add the timer into the inventory.
	UFUNCTION(BlueprintCallable)
	bool UseTimer();

	// Add the bronze sword into the inventory.
	UFUNCTION(BlueprintCallable)
	bool UseSword();

	// Add the bronze bell into the inventory.
	UFUNCTION(BlueprintCallable)
	bool UseBell();

	// Add the bronze mirror into the inventory.
	UFUNCTION(BlueprintCallable)
	bool UseMirror();

	// Add the extinguisher into the inventory.
	UFUNCTION(BlueprintCallable)
	bool UseExtinguisher();

	// Add the cutter into the inventory.
	UFUNCTION(BlueprintCallable)
	bool UseCutter();

	// Add the lantern into the inventory.
	UFUNCTION(BlueprintCallable)
	bool UseLantern();

	// Add the glowstick into the inventory.
	UFUNCTION(BlueprintCallable)
	bool UseGlowStick();

	UFUNCTION(BlueprintCallable)
	void InventorySorting();

	// 현재 소유한 청동 검의 개수를 알려줄 함수
	UFUNCTION(BlueprintCallable, Category = "Object")
	int32 GetSwordNumbers();

	// 현재 소유한 청동 거울의 개수를 알려줄 함수
	UFUNCTION(BlueprintCallable, Category = "Object")
	int32 GetMirrorNumbers();

	// 현재 소유한 청동 방울의 개수를 알려줄 함수
	UFUNCTION(BlueprintCallable, Category = "Object")
	int32 GetBellNumbers();

	// 현재 선택한 아이템의 번호를 알려줄 함수.
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	int32 GetCurrentItemNumber();

private:
	// 현재 선택중인 인벤토리 슬롯
	UPROPERTY(VisibleAnywhere, Category = Inventory, meta = (AllowPrivateAccess = "true"))
	int CurrentSelectedSlot;

	// 현재 선택중인 아이템의 번호
	UPROPERTY(VisibleAnywhere, Category = Inventory, meta = (AllowPrivateAccess = "true"))
	int CurrentSelectedItemNumber;

	// 인벤토리에 저장된 아이템 개수
	UPROPERTY(VisibleAnywhere, Category = Inventory, meta = (AllowPrivateAccess = "true"))
	int InventorySize;

	// 인벤토리 내부에 있는 칼의 개수
	UPROPERTY(VisibleAnywhere, Category = Object, meta = (AllowPrivateAccess = "true"))
	int32 SwordCount;

	// 인벤토리 내부에 있는 거울의 개수
	UPROPERTY(VisibleAnywhere, Category = Object, meta = (AllowPrivateAccess = "true"))
	int32 MirrorCount;

	// 인벤토리 내부에 있는 방울의 개수
	UPROPERTY(VisibleAnywhere, Category = Object, meta = (AllowPrivateAccess = "true"))
	int32 BellCount;

	// 인벤토리 내부에 있는 플래시 라이트 배터리 잔량
	UPROPERTY(VisibleAnywhere, Category = Item, meta = (AllowPrivateAccess = "true"))
	int32 FlashLightBattery;

	// 인벤토리 내부에 있는 소화기 분말 잔량
	UPROPERTY(VisibleAnywhere, Category = Item, meta = (AllowPrivateAccess = "true"))
	int32 ExtinguisherLeft;

	// 인벤토리 내부에 있는 절단기 내구도
	UPROPERTY(VisibleAnywhere, Category = Item, meta = (AllowPrivateAccess = "true"))
	int32 CutterDurability;

	UPROPERTY(VisibleAnywhere, Category = Text, meta = (AllowPrivateAccess = "true"))
	FText ItemErrorText;

	// 플레이어
	UPROPERTY()
	TObjectPtr<class AHorrorGameCharacter> PlayerCharacter;
};
