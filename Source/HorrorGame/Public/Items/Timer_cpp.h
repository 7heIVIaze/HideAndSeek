// CopyrightNotice=0 2023 Sunggon Kim kimdave205@gmail.com

#pragma once

#include "CoreMinimal.h"
#include "Items/Items.h"
#include "Timer_cpp.generated.h"

class USceneComponent;
class UStaticMeshComponent;
UCLASS()
class HORRORGAME_API ATimer_cpp : public AItems
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATimer_cpp();

public: // Properties
	UPROPERTY(VisibleAnywhere, Category = "Timer")
		USceneComponent* DefaultSceneRoot;

	UPROPERTY(VisibleAnywhere, Category = "Timer")
		UStaticMeshComponent* TimerMesh;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	// Functions
	void OnInteract(class AHorrorGameCharacter* Player) override;
	void UseInteract(class AHorrorGameCharacter* Player) override;
};
