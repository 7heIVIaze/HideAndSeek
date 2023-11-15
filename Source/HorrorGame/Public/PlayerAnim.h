// CopyrightNotice=0 2023 Sunggon Kim kimdave205@gmail.com

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "PlayerAnim.generated.h"

/**
 * 
 */
UCLASS()
class HORRORGAME_API UPlayerAnim : public UAnimInstance
{
	GENERATED_BODY()


public:
	UPlayerAnim();

	virtual void NativeInitializeAnimation() override;

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

public:
	UFUNCTION(BlueprintCallable)
		void AnimNotify_SwordSlashStart();

	UFUNCTION(BlueprintCallable)
		void AnimNotify_SwordSlashEnd();

	/*UFUNCTION(BlueprintCallable)
		void AnimNotify_SprintStart();

	UFUNCTION(BlueprintCallable)
		void AnimNotify_SprintEnd();
	
	UFUNCTION(BlueprintCallable)
		void AnimNotify_WhenSprintEndBegin();

	UFUNCTION(BlueprintCallable)
		void AnimNotify_WhenSprintEndFinish();*/

private:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		UAnimMontage* AttackMontage;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		float Speed;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		float Direction;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		bool bShouldAttack;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		bool bIsSprinting; // 플레이어가 뛰고 있는지 확인

	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	//	bool bCheckIsSprinting; // bIsSprint과 비교용

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		bool bIsCrouching;

	void PlayAttackMontage();

	UPROPERTY()
		class AHorrorGameCharacter* Player;
};
