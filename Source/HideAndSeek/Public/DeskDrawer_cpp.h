// CopyrightNotice 2023 Sunggon Kim kimdave205@gmail.com. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
//#include "Components/TimelineComponent.h"
#include "DrawerClass.h"
#include "DeskDrawer_cpp.generated.h"

UCLASS()
class HIDEANDSEEK_API ADeskDrawer_cpp : public ADrawerClass
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADeskDrawer_cpp();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	//UPROPERTY(VisibleAnywhere, Category = "Drawer")
	//	TObjectPtr<UStaticMeshComponent> Drawer;

	///** Please add a variable description */
	//UPROPERTY(VisibleAnywhere, Category = "Drawer")
	//	TObjectPtr<USceneComponent> DefaultSceneRoot;
	///** Please add a variable description */
	//UPROPERTY(VisibleAnywhere, Category = "Drawer")
	//	FTimeline OpenAndClose; // Create TimeLine 

	//UPROPERTY(EditAnywhere)
	//	UCurveFloat* CurveFloat; // Timeline Curve


	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Drawer")
	//	float DrawerOpenMove;

	//bool bIsDrawerClosed = true;

	UPROPERTY(EditDefaultsOnly, Category = "Sound")
		class USoundCue* DrawerOpenSound;

	UPROPERTY(EditDefaultsOnly, Category = "Sound")
		class USoundCue* DrawerCloseSound;
public:
	//UFUNCTION(BlueprintCallable, Category = "Interaction")
		virtual void OnInteract() override;

	//UFUNCTION(BlueprintCallable, Category = "Drawer")
		virtual void OpenDrawer(float Value) override;
};
