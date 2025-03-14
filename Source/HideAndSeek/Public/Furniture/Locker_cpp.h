// CopyrightNotice 2023 Sunggon Kim kimdave205@gmail.com. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
//#include "GameFramework/Actor.h"
#include "Furniture/InteractableActor.h"
#include "Locker_cpp.generated.h"

UCLASS()
class HIDEANDSEEK_API ALocker_cpp : public AInteractableActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALocker_cpp();

public: // Properties
	/** Please add a variable description */
	UPROPERTY(VisibleAnywhere, Category = "Locker")
	TObjectPtr<USceneComponent> RootComp;

	UPROPERTY(VisibleAnywhere, Category = "Locker")
	TObjectPtr<UStaticMeshComponent> LockerMesh;

	UPROPERTY(VisibleAnywhere, Category = "Locker")
	TObjectPtr<UChildActorComponent> Item;

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
