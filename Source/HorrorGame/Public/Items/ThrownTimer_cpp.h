// CopyrightNotice=0 2023 Sunggon Kim kimdave205@gmail.com

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ThrownTimer_cpp.generated.h"

UCLASS()
class HORRORGAME_API AThrownTimer_cpp : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AThrownTimer_cpp();

	UPROPERTY(VisibleAnywhere, Category = "Timer")
		USceneComponent* RootComp;

	UPROPERTY(VisibleAnywhere, Category = "Timer")
		UStaticMeshComponent* TimerMesh;

	UPROPERTY(VisibleAnywhere, Category = "Sound")
		UAudioComponent* TimerSound;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	UFUNCTION(BlueprintCallable)
		void RingingStart();

	UFUNCTION(BlueprintCallable)
		void RingingEnd();

	UFUNCTION(BlueprintCallable)
		void SetPlaySound(bool bValue);

	UFUNCTION(BlueprintCallable)
		bool GetPlaySound();

private:
	UPROPERTY()
		bool bIsSoundPlaying = false;

};
