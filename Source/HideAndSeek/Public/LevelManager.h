// CopyrightNotice=0 2023 Sunggon Kim kimdave205@gmail.com

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LevelManager.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnLevelStartDelegate);

UCLASS()
class HIDEANDSEEK_API ALevelManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALevelManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY()
	USceneComponent* RootComp;

	UPROPERTY()
		TArray<bool> PatrolPoint;

private:
	UWorld* World;
	// bool IsAllLoaded[25] = { false, };
	

	bool Check();

	bool bIsCalled = false;
	int count = 0;
	// TArray<class APatrolPoint_cpp> PatrolArray;

public:
	UPROPERTY(EditAnywhere, Category = "Default")
		TArray<class ACreatureSpawner*> Spawners;

	UPROPERTY(EditAnywhere, Category = "Default")
		TObjectPtr<class AAltar_cpp> Altar;

	/*UPROPERTY(EditAnywhere, Category = "Default")
		TObjectPtr<class APlayerSpawner> PlayerSpawner;*/

	FOnLevelStartDelegate LevelStartDelegate;
};
