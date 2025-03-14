// CopyrightNotice 2023 Sunggon Kim kimdave205@gmail.com. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TimerWidget.generated.h"

/**
 * 
 */
UCLASS()
class HIDEANDSEEK_API UTimerWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	// 게임 플레이 타이머 위젯 생성자.
	virtual void NativeConstruct() override;

	// 게임 플레이 타이머 위젯 소멸자.(Remove Fromt view port가 호출되거나, 뷰포트에서 제거될 때 호출됨)
	virtual void NativeDestruct() override;

public:
	// 게임 플레이 시간을 측정하는 함수.
	UFUNCTION(BlueprintCallable)
	void StartTimer();

	// 타이머 시작 함수.
	UFUNCTION(BlueprintCallable)
	void Init();

	// 타이머를 정지하는 함수로, 작동한 타이머 핸들을 제거함.
	UFUNCTION(BlueprintCallable)
	void StopTimer();

	// String 타입으로 클리어 시간을 가져올 함수
	UFUNCTION(BlueprintCallable)
	FString GetClearTime();

	// float 타입으로 클리어 시간을 가져올 함수.(함수 명에 int는 숫자 타입이라는 것으로 표기하기 위함임...)
	UFUNCTION(BlueprintCallable)
	float GetIntClearTime();

public:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* MiliSecond;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* Second;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* Minute;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* Hour;

	UPROPERTY(EditInstanceOnly, Category = Timer, meta = (AllowPrivateAccess = true))
	int32 Miliseconds;

	UPROPERTY(EditInstanceOnly, Category = Timer, meta = (AllowPrivateAccess = true))
	int32 Seconds;

	UPROPERTY(EditInstanceOnly, Category = Timer, meta = (AllowPrivateAccess = true))
	int32 Minutes;

	UPROPERTY(EditInstanceOnly, Category = Timer, meta = (AllowPrivateAccess = true))
	int32 Hours;

private:
	FTimerHandle TimerHandle;
};
