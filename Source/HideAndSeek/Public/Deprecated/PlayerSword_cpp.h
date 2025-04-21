// CopyrightNotice 2023 Sunggon Kim kimdave205@gmail.com. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PlayerSword_cpp.generated.h"

UCLASS()
class HIDEANDSEEK_API APlayerSword_cpp : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APlayerSword_cpp();

public:
	UPROPERTY(VisibleAnywhere, Category = "Root")
		class USceneComponent* DefaultSceneRoot;

	UPROPERTY(VisibleAnywhere, Category = "Sword")
		TObjectPtr<UStaticMeshComponent> Sword;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HitBox")
		TObjectPtr<class UCapsuleComponent> CapsuleComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		bool bShouldExorcism = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		bool bHadExorcism = false;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class AHorrorGameCharacter* OwnerPlayer;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Sound")
		TObjectPtr<class USoundCue> AttackSound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Sound")
		TObjectPtr<class USoundCue> ExorcismSound;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	void UseInteract(class AHorrorGameCharacter* Player);

	UFUNCTION(BlueprintCallable)
		void OnSwordBeginOverlap(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(BlueprintCallable)
		void SetOnCollision(const bool value);

	UFUNCTION(BlueprintCallable)
		void SetShouldExorcism(const bool value);

	UFUNCTION(BlueprintCallable)
		bool GetShouldExorcism();
};
