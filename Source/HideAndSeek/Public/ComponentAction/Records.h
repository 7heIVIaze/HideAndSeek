// CopyrightNotice 2023 Sunggon Kim kimdave205@gmail.com. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Records.generated.h"

UENUM(BlueprintType)
enum ELanguage
{
	LANG_En UMETA(DisplayName = "English"),
	LANG_Ko UMETA(DisplayName = "Korean"),
};

UENUM(BlueprintType)
enum class EResolution : uint8
{
	Resolution1 UMETA(DisplayName = "Resolutoin1"),
	Resolution2 UMETA(DisplayName = "Resolutoin2"),
	Resolution3 UMETA(DisplayName = "Resolutoin3"),
	Resolution4 UMETA(DisplayName = "Resolutoin4"),
	Resolution5 UMETA(DisplayName = "Resolutoin5"),
	Resolution6 UMETA(DisplayName = "Resolutoin6"),
	Resolution7 UMETA(DisplayName = "Resolutoin7"),
	Resolution8 UMETA(DisplayName = "Resolutoin8"),
};

USTRUCT(Atomic, BlueprintType)
struct FClearData
{
	GENERATED_BODY()

public:
	FClearData() : bIsNormalCleared(false), ChapterNumber(0), NormalClearTime(359640000.f), bIsOpened(false) {}

public:
	//UPROPERTY(BlueprintReadWrite)
	//	bool bIsEasyCleared; // 쉬움 난이도를 클리어했는지 여부 확인

	UPROPERTY(BlueprintReadWrite)
		bool bIsNormalCleared; // 보통 난이도를 클리어했는지 여부 확인

	//UPROPERTY(BlueprintReadWrite)
	//	bool bIsHardCleared; // 어려움 난이도를 클리어했는지 여부 확인

	UPROPERTY(BlueprintReadWrite)
		int ChapterNumber; // 챕터 번호

	//UPROPERTY(BlueprintReadWrite)
	//	float EasyClearTime; // 쉬움 난이도 클리어 타임

	UPROPERTY(BlueprintReadWrite)
		float NormalClearTime; // 보통 난이도 클리어 타임

	//UPROPERTY(BlueprintReadWrite)
	//	float NormalClearTime; // 어려움 난이도 클리어 타임

	UPROPERTY(BlueprintReadWrite)
		bool bIsOpened;
};

USTRUCT(BlueprintType)
struct FOptionSettings
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite)
		float BrightGamma; // 밝기 감마

	UPROPERTY(BlueprintReadWrite)
		bool bMotionBlur; // 모션 블러

	UPROPERTY(BlueprintReadWrite)
		float Volume; // 볼륨 설정

	UPROPERTY(BlueprintReadWrite)
		float MouseSensitive; // 마우스 감도 설정

	UPROPERTY(BlueprintReadWrite)
		bool bIsCrossHairOn; // 크로스헤어 설정

	UPROPERTY(BlueprintReadWrite)
		bool bIsTimerOn; // 타이머 설정

	UPROPERTY(BlueprintReadWrite)
		TEnumAsByte<ELanguage> Language; // 언어 설정
};
UCLASS()
class HIDEANDSEEK_API ARecords : public AActor
{
	GENERATED_BODY()
};
