// CopyrightNotice 2023 Sunggon Kim kimdave205@gmail.com. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
//#include "KismetAnimationLibrary.h"
#include "ReaperAnim.generated.h"

/**
 * 
 */
UCLASS()
class HIDEANDSEEK_API UReaperAnim : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	virtual void NativeInitializeAnimation() override;

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

public:
	UFUNCTION(BlueprintCallable)
		void AnimNotify_ReaperAttackStart();

	UFUNCTION(BlueprintCallable)
		void AnimNotify_ReaperAttackEnd();


public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		float Speed;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		float Direction;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		bool IsCatch;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		bool bIsStunned;

	UPROPERTY()
		class AReaper_cpp* Reaper;
};
