// CopyrightNotice 2023 Sunggon Kim kimdave205@gmail.com. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AI/CreatureClass.h"
#include "Brute_cpp.generated.h"

UCLASS()
class HIDEANDSEEK_API ABrute_cpp : public ACreatureClass
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputAction* MoveAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputAction* LookAction;

public:
	// Sets default values for this character's properties
	ABrute_cpp();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	void Move(const FInputActionValue& Value);

	void Look(const FInputActionValue& Value);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void PossessedBy(AController* NewController) override;

	virtual void ChangeMaterialInstance(float inValue) override;

	virtual void DissolveFinish() override;

public:
	// 소리 감지 범위
	UPROPERTY()
	float NoiseDetectRange = 2400.f;

	// 소리 감지 범위 증가 여부
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool bCalledRangeChange = false;
	
	// 범위가 증가되었는지 여부
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool bIsRangeChange = false;

public:
	virtual void Exorcism() override;

	UFUNCTION(BlueprintCallable)
		void BroadCastChangeNoiseRange(const bool value);

	UFUNCTION(BlueprintCallable)
		void ChangeNoiseRange(const bool value);

	UFUNCTION(BlueprintCallable)
		float GetNoiseRange();
};