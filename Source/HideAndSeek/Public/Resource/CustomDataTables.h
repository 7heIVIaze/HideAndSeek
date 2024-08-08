// CopyrightNotice 2023 Sunggon Kim kimdave205@gmail.com. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/DataTable.h"
#include "CustomDataTables.generated.h"

class AHorrorGameCharacter;

UENUM(BlueprintType)
enum EItemType
{
	ITEM_None UMETA(DisplayName = "None"),
	ITEM_Useable UMETA(DisplayName = "Useable"),
	ITEM_Passive UMETA(DisplayName = "Passive"),
};

// 사용형 아이템 번호
UENUM(BlueprintType)
enum class Active_Item_Num : uint8
{
	CigetLighter UMETA(DisplayName = "CigarLighter"),
	FlashLight UMETA(DisplayName = "FlashLight"),
	Key UMETA(DisplayName = "Key"),
	Timer UMETA(DisplayName = "Timer"),
	Sword UMETA(DisplayName = "Sword"),
	Bell UMETA(DisplayName = "Bell"),
	Mirror UMETA(DisplayName = "Mirror"),
	Extinguisher UMETA(DisplayName = "Extinguisher"),
	Cutter UMETA(DisplayName = "Cutter"),
	SoulLantern UMETA(DisplayName = "SoulLantern"),
	GlowStick UMETA(DisplayName = "GlowStick")
};

USTRUCT(BlueprintType)
struct FHorrorGameItemData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:
	// 정렬을 하기 위해서 아이템 번호를 11보다 큰 12로 설정함.
	FHorrorGameItemData() : ItemNumber(12), ItemName(NSLOCTEXT("FHorrorGameItemData", "NULL", "")), ItemCount(0), ItemIcon(nullptr), Type(EItemType::ITEM_None), ItemPath(TEXT("")), ItemDetail(NSLOCTEXT("FHorrorGameItemData", "NULL", "")) {}

	void Clear();
	void Use(AHorrorGameCharacter* Player);
	//void Initialize();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	int32 ItemNumber;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	FText ItemName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	int32 ItemCount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	UTexture2D* ItemIcon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	TEnumAsByte<EItemType> Type;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	FString ItemPath;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	FText ItemDetail;

};
UCLASS()
class HIDEANDSEEK_API ACustomDataTables : public AActor
{
	GENERATED_BODY()
};
