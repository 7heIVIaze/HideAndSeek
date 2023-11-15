// CopyrightNotice=0 2023 Sunggon Kim kimdave205@gmail.com

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "HorrorGameSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class HORRORGAME_API UHorrorGameSaveGame : public USaveGame
{
	GENERATED_BODY()
	
public:
	UHorrorGameSaveGame();

public:
	UPROPERTY()
		TArray<bool> bLevelCleared; // 클리어한 레벨
	// [0]: 프롤로그, [1]: 스테이지 1, [2]: 엔딩(겸 스테이지2)

	// 아이템 설명용
	UPROPERTY()
		TArray<bool> bIsItemGet; // 아이템 획득 여부(아이템 설명용)
	// [0]: 라이터, [1]: 플래시, [2]: 열쇠, [3]: 타이머, [4]: 청동검
	// [5]: 청동방울, [6]: 청동거울, [7]: 소화기, [8]: 절단기, [9]: 영혼랜턴

	UPROPERTY()
		TArray<bool>  bIsCatchedByCreature; // 어떤 크리쳐에게 잡혔는지(종류는 더 추가될 수 있음)
	// [0]: Reaper, [1]: Runner, [2]: Brute

	UPROPERTY()
		TArray<float> ClearTime; // 각 레벨별 클리어 타임
	// [0]: 프롤로그, [1]: 스테이지 1, [2]: 엔딩(추후에 추가될 스테이지 2)
};
