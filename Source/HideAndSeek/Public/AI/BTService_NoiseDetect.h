// CopyrightNotice 2023 Sunggon Kim kimdave205@gmail.com. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_NoiseDetect.generated.h"

/**
 * 
 */
UCLASS()
class HIDEANDSEEK_API UBTService_NoiseDetect : public UBTService
{
	GENERATED_BODY()
	
public:
	UBTService_NoiseDetect();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Debug)
	bool bIsDebug;

	// 감지된 이후 확장된 감지 범위.(Brute만)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Radius)
	float ExtendedRadius;

	// 기본 감지 범위
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Radius)
	float BaseRadius;


protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
