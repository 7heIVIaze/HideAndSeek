// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotify_FootStepSound.generated.h"

/**
 * 
 */
UCLASS(const, hidecategories = Object, collapsecategories, meta = (DisplayName = "Play FootStep Sound"))
class HIDEANDSEEK_API UAnimNotify_FootStepSound : public UAnimNotify
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AnimNotify", meta = (DisplayName = "PMB Sound List"))
		TSubclassOf<class UFootStepSoundList> SoundList;

	UPROPERTY()
		class UFootStepSoundList* PMB_SoundList;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AnimNotify", meta = (ExposeOnSpawn = true))
		float VolumeMultiplier = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AnimNotify", meta = (ExposeOnSpawn = true))
		float PitchMultiplier = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AnimNotify")
		bool SoundAttach = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AnimNotify")
		FVector LocationOffset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AnimNotify")
		float CheckDistance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AnimNotify")
		FName SocketName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AnimNotify")
		bool ShowDebugLine = true;

public:
	UAnimNotify_FootStepSound();
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

public:
	void FootStep(USkeletalMeshComponent* MeshComp);
};
