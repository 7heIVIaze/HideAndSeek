// CopyrightNotice=0 2023 Sunggon Kim kimdave205@gmail.com

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
	virtual void NativeConstruct() override;

public:
	UFUNCTION()
		void StartTimer();

	UFUNCTION()
		void Init();

	UFUNCTION(BlueprintCallable)
		void StopTimer();

	UFUNCTION(BlueprintCallable)
		FString GetClearTime();
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
