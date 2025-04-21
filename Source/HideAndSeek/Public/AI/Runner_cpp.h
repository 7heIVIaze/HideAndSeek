// CopyrightNotice 2023 Sunggon Kim kimdave205@gmail.com. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AI/CreatureClass.h"
#include "Runner_cpp.generated.h"

UCLASS()
class HIDEANDSEEK_API ARunner_cpp : public ACreatureClass
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputAction* LookAction;

public:
	// Sets default values for this character's properties
	ARunner_cpp();

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
	virtual void Exorcism() override;
	
};