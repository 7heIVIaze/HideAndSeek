// CopyrightNotice 2023 Sunggon Kim kimdave205@gmail.com. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LevelManager.generated.h"

// 레벨이 시작되었다고 알릴 델리게이트 이벤트.
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnLevelStartDelegate);

UCLASS()
class HIDEANDSEEK_API ALevelManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALevelManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	USceneComponent* RootComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TArray<bool> PatrolPoint;

	UPROPERTY(EditAnywhere, Category = "Default")
	TArray<class ACreatureSpawner*> Spawners;

	UPROPERTY(EditAnywhere, Category = "Default")
		TObjectPtr<class AAltar_cpp> Altar;

	UPROPERTY(BlueprintAssignable, Category = Event)
	FOnLevelStartDelegate LevelStartDelegate;

private:
	UWorld* World;

	bool Check();

	bool bIsCalled = false;
	int count = 0;
};
