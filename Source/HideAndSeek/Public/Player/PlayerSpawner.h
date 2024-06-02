// CopyrightNotice 2023 Sunggon Kim kimdave205@gmail.com. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PlayerSpawner.generated.h"

UCLASS()
class HIDEANDSEEK_API APlayerSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APlayerSpawner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	//virtual void Tick(float DeltaTime) override;
	UFUNCTION(BlueprintCallable)
		void BindFunction(class ALevelManager* LevelManager);

	UFUNCTION(BlueprintCallable)
		void SpawnPlayer();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = PlayerToSpawn)
		TObjectPtr<class AHorrorGameCharacter>Player;

	/*UFUNCTION(BlueprintCallable)
		void TeleportPlayer(AActor* Player);*/
};
