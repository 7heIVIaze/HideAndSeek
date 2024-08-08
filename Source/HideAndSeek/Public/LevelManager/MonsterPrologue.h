// CopyrightNotice 2023 Sunggon Kim kimdave205@gmail.com. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MonsterPrologue.generated.h"

UCLASS()
class HIDEANDSEEK_API AMonsterPrologue : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMonsterPrologue();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	// virtual void Tick(float DeltaTime) override;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collide")
		TObjectPtr<class UBoxComponent> PlayerEnterBox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpawnPoint")
		TObjectPtr<class UStaticMeshComponent> SpawnPoint;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Default")
		TSubclassOf<class AReaper_cpp> ObjectToSpawn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PatrolPointList")
		TArray<class APatrolPoint_cpp*> PatrolPointLists;
	
	UPROPERTY()
		bool bIsPlayerIn = false;

public:
	UFUNCTION(BlueprintCallable)
		void PlayerEnterBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
