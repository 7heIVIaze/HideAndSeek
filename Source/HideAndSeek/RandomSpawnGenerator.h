// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RandomSpawnGenerator.generated.h"

UCLASS()
class HIDEANDSEEK_API ARandomSpawnGenerator : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARandomSpawnGenerator();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

//public:	
//	// Called every frame
//	virtual void Tick(float DeltaTime) override;
public:
	UPROPERTY(VisibleAnywhere, Category="Default")
	class UBillboardComponent* center;
public:
	UPROPERTY(EditDefaultsOnly, Category="Default")
	FName LevelNames[3] = { TEXT("Spawn1"), TEXT("Spawn2"), TEXT("Spawn3") };
	UPROPERTY(EditDefaultsOnly, Category="Default")
	float LevelSize = 5200.0f;
	UPROPERTY(EditDefaultsOnly, Category="Default")
	int numX = 0;
	UPROPERTY(EditDefaultsOnly, Category="Default")
	int numY = 0;
};
