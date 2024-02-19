// CopyrightNotice=0 2023 Sunggon Kim kimdave205@gmail.com

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/DataTable.h"
#include "CreatureType.generated.h"

UENUM(BlueprintType)
enum ECreatureType // 크리쳐 문서용 ENUM 클래스
{
	CT_None UMETA(DisplayName = "None"), // defaults
	CT_Creature UMETA(DisplayName = "Creature"), // 크리쳐
	CT_Tip UMETA(DisplayName = "Tip"), // 도움말
	CT_Devil UMETA(DisplayName = "Devil"), // 악마
};
USTRUCT(BlueprintType)
struct FCreatureData : public FTableRowBase
{
	GENERATED_BODY()
		
public:
	FCreatureData() : Type(ECreatureType::CT_None), Title(TEXT("")), Image(nullptr), Detail(TEXT("")) {}

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		TEnumAsByte<ECreatureType> Type;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		FString Title;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		UTexture2D* Image;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		FString Detail;
};
UCLASS()
class HIDEANDSEEK_API ACreatureType : public AActor // 데이터 테이블용 더미 액터
{
	GENERATED_BODY()
};
