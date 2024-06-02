// CopyrightNotice 2023 Sunggon Kim kimdave205@gmail.com. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TimerProjectile_cpp.generated.h"

class UBoxComponent;
class UStaticMeshComponent;
class UAudioComponent;
class UProjectileMovementComponent;
UCLASS()
class HIDEANDSEEK_API ATimerProjectile_cpp : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATimerProjectile_cpp();

public: // Properties
	UPROPERTY(VisibleDefaultsOnly, Category = "Projectile")
		UBoxComponent* Collision;

	UPROPERTY(VisibleAnywhere, Category = "Timer")
		UStaticMeshComponent* TimerMesh;

	/*UPROPERTY(VisibleAnywhere, Category = "Sound")
		UAudioComponent* TimerSound;*/

	UPROPERTY(VisibleAnywhere, Category = "Movement")
		UProjectileMovementComponent* ProjectileMovement;

	bool bIsHit;
	float count = 0;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	// Function
	/*UFUNCTION()
		void RangEnd();*/

	UFUNCTION()
		void FireInDirection(const FVector& ShootDirection);

	/*UFUNCTION()
		void SetPlaySound(bool bValue);

	UFUNCTION()
		bool GetPlaySound();*/

	UFUNCTION()
		void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

//private:
//	UPROPERTY()
//		bool bPlaySound = false;
	
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Timer)
		TSubclassOf<class AThrownTimer_cpp> Timer;
		//TSubclassOf<class ATimerProjectile_cpp> Timer;
};
