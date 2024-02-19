// CopyrightNotice=0 2023 Sunggon Kim kimdave205@gmail.com

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "RunnerAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class HIDEANDSEEK_API URunnerAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	virtual void NativeInitializeAnimation() override;

	void NativeUpdateAnimation(float DeltaSeconds) override;

	UFUNCTION(BlueprintCallable)
		void AnimNotify_RunnerAttackStart();

	UFUNCTION(BlueprintCallable)
		void AnimNotify_RunnerAttackEnd();

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = "true"))
		float Speed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = "true"))
		bool bIsFalling;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = "true"))
		bool bIsChase;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = "true"))
		bool bIsCatch;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = "true"))
		bool bIsStunned;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = "true"))
		float Direction;

public:
	UPROPERTY()
		class ARunner_cpp* Runner;
};
