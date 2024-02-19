// CopyrightNotice=0 2023 Sunggon Kim kimdave205@gmail.com

#pragma once

#include "CoreMinimal.h"
#include "ComponentAction/HorrorGameSaveGameBase.h"
#include "ComponentAction/Records.h"
#include "HorrorGameSaveGame.generated.h"

/**
 * 
 */

UCLASS()
class HIDEANDSEEK_API UHorrorGameSaveGame : public UHorrorGameSaveGameBase
{
	GENERATED_BODY()
	
public:
	static UHorrorGameSaveGame* LoadObject(UObject* inObject, const FString& inUniqueID, int32 inUserIndex);

public:
	UPROPERTY()
		int32 ClearedChapter; // 클리어한 레벨

	UPROPERTY()
		TArray<FString>Article;

	UPROPERTY()
		TArray<FString>Diary;

	UPROPERTY()
		TArray<FString>Message;

	UPROPERTY()
		TArray<FString>ClearTime; // 클리어 타임

	UPROPERTY()
		TArray<FClearData>ClearChapter;

	UPROPERTY()
		float BrightGamma; // 밝기 감마

	UPROPERTY()
		bool bMotionBlur; // 모션 블러

	UPROPERTY()
		float Volume; // 볼륨 설정

	UPROPERTY()
		float MouseSensitive; // 마우스 감도 설정

	UPROPERTY()
		bool bIsCrossHairOn; // 크로스헤어 설정

	UPROPERTY()
		bool bIsTimerOn; // 타이머 설정

	// 아이템 관련 문서
	UPROPERTY()
		bool Item1_CigarLighter; // 라이터

	UPROPERTY()
		bool Item2_FlashLight; // 플래시라이터

	UPROPERTY()
		bool Item3_Key; // 열쇠

	UPROPERTY()
		bool Item4_Timer; // 타이머

	UPROPERTY()
		bool Item5_BronzeSword; // 청동검

	UPROPERTY()
		bool Item6_BronzeBell; // 청동 방울

	UPROPERTY()
		bool Item7_BronzeMirror; // 청동 거울

	UPROPERTY()
		bool Item8_Extinguisher; // 소화기

	UPROPERTY()
		bool Item9_Cutter; // 절단기

	UPROPERTY()
		bool Item10_SoulLantern; // 영혼 랜턴

		// 신문 기사 문서
	UPROPERTY()
		bool Article1; // 1번 기사

	UPROPERTY()
		bool Article2; // 2번 기사

	UPROPERTY()
		bool Article3; // 3번 기사

	UPROPERTY()
		bool Article4; // 4번 기사

	UPROPERTY()
		bool Article5; // 5번 기사

	// 편지 문서
	UPROPERTY()
		bool Letter1_Berith1; // 베리스의 편지 1

	UPROPERTY()
		bool Letter2_Berith2; // 베리스의 편지 2

	UPROPERTY()
		bool Letter3_Berith3; // 베리스의 편지 3

	UPROPERTY()
		bool Letter4_Berith4; // 베리스의 편지 4

	UPROPERTY()
		bool Letter5_Berith5; // 베리스의 편지 5

	UPROPERTY()
		bool Letter6_Berith6; // 베리스의 편지 6

	UPROPERTY()
		bool Letter7_Gamigin1; // 가미긴의 편지 1 

	UPROPERTY()
		bool Letter8_Gamigin2; // 가미긴의 편지 2 

	UPROPERTY()
		bool Letter9_Gamigin3; // 가미긴의 편지 3 

	UPROPERTY()
		bool Letter10_Gamigin4; // 가미긴의 편지 4

	// 일기 문서
	UPROPERTY()
		bool Diary1; // 일기 1

	UPROPERTY()
		bool Diary2; // 일기 2

	UPROPERTY()
		bool Diary3; // 일기 3

	UPROPERTY()
		bool Diary4; // 일기 4

	UPROPERTY()
		bool Diary5; // 일기 5

	// 크리쳐에게 잡혔는지 여부
	UPROPERTY()
		bool CatchedByReaper; // 리퍼에게 죽었는가

	UPROPERTY()
		bool CatchedByRunner; // 러너에게 죽었는가

	UPROPERTY()
		bool CatchedByBrute; // 브루트에게 죽었는가

	UPROPERTY()
		bool CatchedByShadow; // 그슨새에게 죽었는가

	// 기타 도움말 여부
	UPROPERTY()
		bool SeeCharacteristic; // 크리쳐의 특징을 보았는가

	UPROPERTY()
		bool SeeHowToEscape; // 피하는 방법을 보았는가
};