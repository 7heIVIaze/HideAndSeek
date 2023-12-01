// CopyrightNotice=0 2023 Sunggon Kim kimdave205@gmail.com

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/TimelineComponent.h"
#include "SwitchLever.generated.h"

UCLASS()
class HORRORGAME_API ASwitchLever : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASwitchLever();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Mesh)
		TObjectPtr<USceneComponent> RootComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Mesh)
		TObjectPtr<UStaticMeshComponent> LeverBody;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Mesh)
		TObjectPtr<UStaticMeshComponent> Lever;

	UPROPERTY(VisibleAnywhere, Category = Lever)
		FTimeline UpAndDown; // Create TimeLine 

	UPROPERTY(EditAnywhere)
		UCurveFloat* CurveFloat;

	UPROPERTY()
		float LeverRotateAngle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Lever)
		bool bIsLeverOn;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
		void OnInteract(class AHorrorGameCharacter* Player);

	UFUNCTION(BlueprintCallable)
		void ChangeLeverRotation(float Value);
};
