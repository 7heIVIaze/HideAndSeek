// CopyrightNotice=0 2023 Sunggon Kim kimdave205@gmail.com

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
};

USTRUCT(BlueprintType)
struct FHorrorGameItemData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:
	FHorrorGameItemData() : ItemNumber(0), ItemName(TEXT("")), ItemCount(0), ItemIcon(nullptr), Type(EItemType::ITEM_None), ItemPath(TEXT("")), ItemDetail(TEXT("")), DetailKorean(TEXT("")) {}

	void Clear();
	void Use(AHorrorGameCharacter* Player);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	int32 ItemNumber;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	FString ItemName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	int32 ItemCount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	UTexture2D* ItemIcon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	TEnumAsByte<EItemType> Type;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	FString ItemPath;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	FString ItemDetail;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		FString DetailKorean;
};
UCLASS()
class HIDEANDSEEK_API ACustomDataTables : public AActor
{
	GENERATED_BODY()
};
