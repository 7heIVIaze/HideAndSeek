// CopyrightNotice=0 2023 Sunggon Kim kimdave205@gmail.com

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DistributionBox.generated.h"

UCLASS()
class HIDEANDSEEK_API ADistributionBox : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADistributionBox();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Mesh)
		TObjectPtr<USceneComponent> RootComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Mesh)
		TObjectPtr<UStaticMeshComponent> BoxMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = SpawnPoint)
		TObjectPtr<UStaticMeshComponent> SpawnPoint;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Mesh)
		bool bIsPowered;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Defaults)
		TArray<AActor*> BasementDoor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Defaults)
		AActor* Wall;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Defaults)
		TSubclassOf<class AReaper_cpp> ReaperClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Switch")
		class ADBox_Door* BoxDoor;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	// virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
		void OnInteract(class AHorrorGameCharacter* Player);
};
