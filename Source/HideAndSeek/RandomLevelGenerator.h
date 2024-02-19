// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RandomLevelGenerator.generated.h"

UCLASS()
class HIDEANDSEEK_API ARandomLevelGenerator : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARandomLevelGenerator();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	UPROPERTY(VisibleAnywhere)
	class UBillboardComponent* Center;
public:
	FName QuadNames[11] = { TEXT("/Game/Levels/Level1/Quad1"), TEXT("/Game/Levels/Level1/Quad2") , TEXT("/Game/Levels/Level1/Quad3") , TEXT("/Game/Levels/Level1/Quad4"), TEXT("/Game/Levels/Level1/Quad5"),
	TEXT("/Game/Levels/Level1/Quad6"), TEXT("/Game/Levels/Level1/Quad7"), TEXT("/Game/Levels/Level1/Quad8"), TEXT("/Game/Levels/Level1/Quad9"), TEXT("/Game/Levels/Level1/Quad10"), TEXT("/Game/Levels/Level1/Quad11") };
	FName TripleNames[10] = { TEXT("/Game/Levels/Level1/Triple1"), TEXT("/Game/Levels/Level1/Triple2") , TEXT("/Game/Levels/Level1/Triple3"), TEXT("/Game/Levels/Level1/Triple4"), TEXT("/Game/Levels/Level1/Triple5"),
	TEXT("/Game/Levels/Level1/Triple6"), TEXT("/Game/Levels/Level1/Triple7"), TEXT("/Game/Levels/Level1/Triple8"), TEXT("/Game/Levels/Level1/Triple9"), TEXT("/Game/Levels/Level1/Triple10") };
	FName DoubleNames[4] = { TEXT("/Game/Levels/Level1/Double1"), TEXT("/Game/Levels/Level1/Double2"), TEXT("/Game/Levels/Level1/Double3"), TEXT("/Game/Levels/Level1/Double4") };
	
	
	/*UPROPERTY(EditAnywhere, Category = "Levels")
	FName QuadNames[11];
	UPROPERTY(EditAnywhere, Category = "Levels")
	FName TripleNames[10];
	UPROPERTY(EditAnywhere, Category = "Levels")
	FName DoubleNames[4];*/
	
	float LevelSize = 5200.0f;
	int numX = 5;
	int numY = 5;

public:
	int map[5][5] = { {2, 3, 4, 3, 2}, {3, 4, 4, 4, 3}, {3, 4, 4, 4, 3}, {3, 4, 4, 4, 3}, {2, 3, 4, 3, 2} };
	bool QuadUse[11] = { false };
	bool TripleUse[10] = { false };
	bool DoubleUse[4] = { false };
public:
	int LevelRotate[5][5] = { {1, 0, -1, 0, 0}, {1, -1, -1, -1, 3}, {1, -1, -1, -1, 3}, {1, -1, -1, -1, 3}, {2, 2, -1, 2, 3} };
};
