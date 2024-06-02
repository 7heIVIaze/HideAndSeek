// CopyrightNotice 2023 Sunggon Kim kimdave205@gmail.com. All Rights Reserved.

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
	FCollectArchivesData() : Type(EDocumentType::DOCS_Article), Detail_EN(NSLOCTEXT("FCollectArchivesData", "NULL", "")) {}
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		TEnumAsByte<EDocumentType> Type;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		int Number;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		FText Detail_EN;

	/*UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		FText Detail_KR;*/

	/*UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data", meta = (MultiLine = true))
		FText FirstGetText;*/
};

USTRUCT(BlueprintType)
struct FCollectedArchives : public FTableRowBase
{
	GENERATED_BODY()

public:
	// 아이템 관련 문서
	UPROPERTY(BlueprintReadWrite)
		bool Item1_CigarLighter = false; // 라이터

	UPROPERTY(BlueprintReadWrite)
		bool Item2_FlashLight = false; // 플래시라이터

	UPROPERTY(BlueprintReadWrite)
		bool Item3_Key = false; // 열쇠

	UPROPERTY(BlueprintReadWrite)
		bool Item4_Timer = false; // 타이머

	UPROPERTY(BlueprintReadWrite)
		bool Item5_BronzeSword = false; // 청동검

	UPROPERTY(BlueprintReadWrite)
		bool Item6_BronzeBell = false; // 청동 방울

	UPROPERTY(BlueprintReadWrite)
		bool Item7_BronzeMirror = false; // 청동 거울

	UPROPERTY(BlueprintReadWrite)
		bool Item8_Extinguisher = false; // 소화기

	UPROPERTY(BlueprintReadWrite)
		bool Item9_Cutter = false; // 절단기

	UPROPERTY(BlueprintReadWrite)
		bool Item10_SoulLantern = false; // 영혼 랜턴

	UPROPERTY(BlueprintReadWrite)
		bool Item11_GlowStick = false; // 야광봉

		// 신문 기사 문서
	UPROPERTY(BlueprintReadWrite)
		bool Article1 = false; // 1번 기사

	UPROPERTY(BlueprintReadWrite)
		bool Article2 = false; // 2번 기사

	UPROPERTY(BlueprintReadWrite)
		bool Article3 = false; // 3번 기사

	UPROPERTY(BlueprintReadWrite)
		bool Article4 = false; // 4번 기사

	UPROPERTY(BlueprintReadWrite)
		bool Article5 = false; // 5번 기사

	// 편지 문서
	UPROPERTY(BlueprintReadWrite)
		bool Letter1_Berith1 = false; // 베리스의 편지 1

	UPROPERTY(BlueprintReadWrite)
		bool Letter2_Berith2 = false; // 베리스의 편지 2

	UPROPERTY(BlueprintReadWrite)
		bool Letter3_Berith3 = false; // 베리스의 편지 3

	UPROPERTY(BlueprintReadWrite)
		bool Letter4_Berith4 = false; // 베리스의 편지 4

	UPROPERTY(BlueprintReadWrite)
		bool Letter5_Berith5 = false; // 베리스의 편지 5

	UPROPERTY(BlueprintReadWrite)
		bool Letter6_Berith6 = false; // 베리스의 편지 6

	UPROPERTY(BlueprintReadWrite)
		bool Letter7_Gamigin1 = false; // 가미긴의 편지 1 

	UPROPERTY(BlueprintReadWrite)
		bool Letter8_Gamigin2 = false; // 가미긴의 편지 2 

	UPROPERTY(BlueprintReadWrite)
		bool Letter9_Gamigin3 = false; // 가미긴의 편지 3 

	UPROPERTY(BlueprintReadWrite)
		bool Letter10_Gamigin4 = false; // 가미긴의 편지 4

	// 일기 문서
	UPROPERTY(BlueprintReadWrite)
		bool Diary1 = false; // 일기 1

	UPROPERTY(BlueprintReadWrite)
		bool Diary2 = false; // 일기 2

	UPROPERTY(BlueprintReadWrite)
		bool Diary3 = false; // 일기 3

	UPROPERTY(BlueprintReadWrite)
		bool Diary4 = false; // 일기 4

	UPROPERTY(BlueprintReadWrite)
		bool Diary5 = false; // 일기 5

	// 크리쳐에게 잡혔는지 여부
	UPROPERTY(BlueprintReadWrite)
		bool CatchedByReaper = false; // 리퍼에게 죽었는가

	UPROPERTY(BlueprintReadWrite)
		bool CatchedByRunner = false; // 러너에게 죽었는가

	UPROPERTY(BlueprintReadWrite)
		bool CatchedByBrute = false; // 브루트에게 죽었는가

	UPROPERTY(BlueprintReadWrite)
		bool CatchedByShadow = false; // 그슨새에게 죽었는가

	// 기타 도움말 여부
	UPROPERTY(BlueprintReadWrite)
		bool SeeCharacteristic = false; // 크리쳐의 특징을 보았는가

	UPROPERTY(BlueprintReadWrite)
		bool SeeHowToEscape = false; // 피하는 방법을 보았는가
};
UCLASS()
class HIDEANDSEEK_API ACollectArchives : public AActor
{
	GENERATED_BODY()

};
