// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
//#include "Components/TimelineComponent.h"
#include "DrawerClass.h"
#include "WardrobeDrawer_cpp.generated.h"

UCLASS()
class HORRORGAME_API AWardrobeDrawer_cpp : public ADrawerClass
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWardrobeDrawer_cpp();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	/*UPROPERTY(VisibleAnywhere, Category = "Wardrobe")
		TObjectPtr<class UStaticMeshComponent> Drawer;

	UPROPERTY(VisibleAnywhere, Category = "Wardrobe")
		TObjectPtr<USceneComponent> DefaultSceneRoot;
	
	UPROPERTY(VisibleAnywhere, Category = "Wardrobe")
		FTimeline OpenAndClose;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Wardrobe")
		TObjectPtr<class UAudioComponent> DrawerSound;

	UPROPERTY(EditAnywhere)
		UCurveFloat* CurveFloat;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wardrobe")
		float DrawerOpenMove = 80.f;

	bool bIsDrawerClosed = true;*/

public:
	//UFUNCTION(BlueprintCallable, Category = "Interaction")
		virtual void OnInteract() override;

	//UFUNCTION(BlueprintCallable, Category = "Drawer")
		virtual void OpenDrawer(float Value) override;
};
