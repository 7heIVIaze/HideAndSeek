// CopyrightNotice=0 2023 Sunggon Kim kimdave205@gmail.com

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "BruteAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class HIDEANDSEEK_API UBruteAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	virtual void NativeInitializeAnimation() override;

	void NativeUpdateAnimation(float DeltaSeconds) override;

	UFUNCTION(BlueprintCallable)
		void AnimNotify_BruteAttackStart();

	UFUNCTION(BlueprintCallable)
		void AnimNotify_BruteAttackEnd();

	UFUNCTION(BlueprintCallable)
	void AnimNotify_BruteChangeRangeStart();

	UFUNCTION(BlueprintCallable)
	void AnimNotify_BruteChangeRangeEnd();

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
		float Direction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = "true"))
		bool bIsStunned;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = "true"))
	bool bIsCalledRangeChange;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = "true"))
	bool bIsRangeChange;
public:
	class ABrute_cpp* Brute;
};
