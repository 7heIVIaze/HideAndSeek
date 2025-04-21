// CopyrightNotice 2023 Sunggon Kim kimdave205@gmail.com. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Furniture/InteractableActor.h"
#include "Components/TimelineComponent.h"
#include "DrawerClass.generated.h"

UCLASS()
class HIDEANDSEEK_API ADrawerClass : public AInteractableActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADrawerClass();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	UPROPERTY(VisibleAnywhere, Category = "Drawer")
	TObjectPtr<UChildActorComponent> Item;

	UPROPERTY(VisibleAnywhere, Category = "Drawer")
	TObjectPtr<UStaticMeshComponent> Drawer;

	/** Please add a variable description */
	UPROPERTY(VisibleAnywhere, Category = "Drawer")
	TObjectPtr<USceneComponent> RootComp;

	UPROPERTY(EditAnywhere, Category = "Drawer")
	TArray<TObjectPtr<UStaticMesh>> DrawerMeshes;

	UPROPERTY(EditAnywhere, Category = "Drawer")
	FTimeline OpenAndCloseTimeline; // Create TimeLine 

	UPROPERTY(EditAnywhere, Category = Drawer)
	UCurveFloat* OpenAndCloseCurveFloat; // Timeline Curve

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Drawer")
	float DrawerOpenMove = 80.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Drawer")
	bool bIsDrawerClosed = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	bool bIsItemSpawned;

public:
	UFUNCTION(BlueprintCallable, Category = "Interaction")
	virtual void OnInteract(class AHorrorGameCharacter* Player) override;

	UFUNCTION(BlueprintCallable, Category = "Drawer")
	virtual void OpenDrawer(float Value);

	UFUNCTION(BlueprintCallable)
	virtual bool SetSpawnItem(TSubclassOf<AActor> inItem);

	UFUNCTION(BlueprintCallable)
	virtual bool GetIsItemSpawned();
};
