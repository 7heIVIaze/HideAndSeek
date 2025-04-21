// CopyrightNotice 2023 Sunggon Kim kimdave205@gmail.com. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "YokaiAIController.h"
#include "AIController_Reaper.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class Sealed : uint8
{
	Sealed UMETA(DisplayName = "Sealed"),
	OneUnsealed UMETA(DisplayName = "One_Unsealed"),
	TwoUnsealed UMETA(DisplayName = "Two_Unsealed"),
	Unsealed UMETA(DisplayName = "All_Unsealed"),
	SealedButChase UMETA(DisplayName = "SealedButChase"), // 프롤로그 용 상태
};
UCLASS()
class HIDEANDSEEK_API AAIController_Reaper : public AYokaiAIController //AAIController
{
	GENERATED_BODY()

public:
	AAIController_Reaper();

	virtual void SetupBlackboardAndBehaviorTree() override;

	virtual void OnTargetDetected(AActor* Actor, FAIStimulus const Stimulus) override;

	UFUNCTION(BlueprintCallable)
	Sealed GetCurrentSealStatus();

	UFUNCTION(BlueprintCallable)
	void SetCurrentSealStatus(Sealed Status);

public:
	static const FName SealStatus;

private:
	UPROPERTY()
		Sealed CurrentStatus;

};
