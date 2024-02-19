// CopyrightNotice=0 2023 Sunggon Kim kimdave205@gmail.com

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/DataTable.h"
#include "CollectArchives.generated.h"

UENUM(BlueprintType)
enum EDocumentType
{
	DOCS_None UMETA(DisplayName = "None"),
	DOCS_Article UMETA(DisplayName = "Article"),
	DOCS_Letter UMETA(DisplayName = "Letter"),
	DOCS_Diary UMETA(DisplayName = "Diary"),
};
USTRUCT(BlueprintType)
struct FCollectArchivesData : public FTableRowBase
{
	GENERATED_BODY()

public:
	FCollectArchivesData() : Type(EDocumentType::DOCS_Article), Detail_EN(TEXT("")), Detail_KR(TEXT("")) {}
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		TEnumAsByte<EDocumentType> Type;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		FString Detail_EN;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		FString Detail_KR;
};
UCLASS()
class HIDEANDSEEK_API ACollectArchives : public AActor
{
	GENERATED_BODY()

};
