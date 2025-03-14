// CopyrightNotice 2023 Sunggon Kim kimdave205@gmail.com. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/DataTable.h"
#include "CustomDataTables.generated.h"

class AHorrorGameCharacter;

UENUM(BlueprintType)
enum class EItemType : uint8
{
	ITEM_NONE UMETA(DisplayName = "None"), // Nothing
	ITEM_Consumable UMETA(DisplayName = "Consumable"), // 소비형 아이템
	ITEM_Lighting UMETA(DisplayName = "Light"), // 조명 아이템
	ITEM_Passive UMETA(DisplayName = "Passive"), // 패시브(장비형) 아이템
	ITEM_Unique UMETA(DisplayName = "Unique"), // 특수 아이템(나침반, 환혼석 등)
};

// 소비형 아이템 번호
UENUM(BlueprintType)
enum class EItemNumber : uint8
{
	ITEM_None UMETA(DisplayName = "None"),
	ITEM_CigarLighter UMETA(DisplayName = "CigarLighter"), // 라이터
	ITEM_FlashLight UMETA(DisplayName = "FlashLight"), // 손전등
	ITEM_Key UMETA(DisplayName = "Key"), // 열쇠
	ITEM_Timer UMETA(DisplayName = "Timer"), // 어그로 아이템
	ITEM_Sword UMETA(DisplayName = "Sword"), // 청동 검
	ITEM_Bell UMETA(DisplayName = "Bell"), // 청동 방울
	ITEM_Mirror UMETA(DisplayName = "Mirror"), // 청동 거울
	ITEM_Extinguisher UMETA(DisplayName = "Extinguisher"), // 소화기
	ITEM_Cutter UMETA(DisplayName = "Cutter"), // 절단기
	ITEM_Lantern UMETA(DisplayName = "Lantern"), // 영혼 랜턴
	ITEM_Glowstick UMETA(DisplayName = "Glowstick"), // 야광봉
	ITEM_Compass UMETA(DisplayName = "Compass"), // 나침반
	ITEM_ResurrectionStone UMETA(DisplayName = "ResurrectionStone"), // 환혼석
};

USTRUCT(BlueprintType)
struct FHorrorGameItemData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:
	// 정렬을 하기 위해서 아이템 번호를 11보다 큰 12로 설정함.
	FHorrorGameItemData() : ItemNumber(EItemNumber::ITEM_None), ItemName(NSLOCTEXT("FHorrorGameItemData", "NULL", "")), ItemType(EItemType::ITEM_NONE), ItemCount(0), Durability(0), ItemIcon(nullptr), ItemBPClass(nullptr), ItemDescription(NSLOCTEXT("FHorrorGameItemData", "NULL", "")) {}

	void Clear();
	void Use(AHorrorGameCharacter* Player);
	//void Initialize();

public:
	// Item Number
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Data)
	EItemNumber ItemNumber;

	// Item Name
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Data)
	FText ItemName;

	// Item Type : Useable / Lighting / Equipment
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Data)
	EItemType ItemType;

	// Item Count(Amount) : For Inventory
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Data)
	int ItemCount;

	// Durability : For Inventory
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Data)
	int Durability;

	// Item Icon : For Inventory
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Data)
	UTexture2D* ItemIcon;

	// Item Path
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Data)
	TSubclassOf<class AItemClass> ItemBPClass;

	// Description of item : For Archive Widget 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data)
	FText ItemDescription;
};
UCLASS()
class HIDEANDSEEK_API ACustomDataTables : public AActor
{
	GENERATED_BODY()
};
