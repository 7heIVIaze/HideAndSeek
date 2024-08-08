// CopyrightNotice 2023 Sunggon Kim kimdave205@gmail.com. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Locker_cpp.generated.h"

class UStaticMeshComponent;
class USceneComponent;
class UChildActorComponent;
UCLASS()
class HIDEANDSEEK_API ALocker_cpp : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALocker_cpp();

public: // Properties
	/** Please add a variable description */
	UPROPERTY(VisibleAnywhere, Category = "Locker")
		USceneComponent* DefaultSceneRoot;

	UPROPERTY(VisibleAnywhere, Category = "Locker")
		UStaticMeshComponent* LockerMesh;

	UPROPERTY(VisibleAnywhere, Category = "Locker")
		UChildActorComponent* Item;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
		TArray<TSubclassOf<class AItems>> Items;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
		TArray<float> ItemProbability;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
		float fNoItemProbability = 0.5f; // 아이템이 안 나올 확률

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
		bool bIsItemSpawned;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	 // Functions
	// Called every frame
	//virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
		bool SetSpawnItem(TSubclassOf<AActor> inItem);

	UFUNCTION(BlueprintCallable)
		bool GetIsItemSpawned();
};
