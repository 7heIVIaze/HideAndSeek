// CopyrightNotice 2023 Sunggon Kim kimdave205@gmail.com. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_Detect.generated.h"

/**
 * 
 */
UCLASS()
class HIDEANDSEEK_API UBTService_Detect : public UBTService
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Debug)
	bool bIsDebug;

	// 감지된 이후 확장된 감지 범위.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Radius)
	float ExtendedRadius;

	// 기본 감지 범위
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Radius)
	float BaseRadius;

public:
	UBTService_Detect();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
