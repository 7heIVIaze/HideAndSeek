// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CreatureSpawner.generated.h"

UCLASS()
class HIDEANDSEEK_API ACreatureSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACreatureSpawner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

private:
	bool Check();

private:
	// FTimerHandle _spawnTimerHandle;
	bool IsAllLoaded[25] = { false, };

	USceneComponent* RootComp;
	int reaper_cnt = 0;
	int count = 0;

	UWorld* world;
	FVector spawnLocation;
	FRotator rotator;
	FName path;

	float fPlayTime = 0.f;

public:
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Default")
	TArray<TSubclassOf<class ACharacter>> ObjectToSpawn;

	UFUNCTION(BlueprintCallable)
		void CreatureSpawn();
	
};
