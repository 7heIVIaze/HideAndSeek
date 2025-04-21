// CopyrightNotice 2023 Sunggon Kim kimdave205@gmail.com. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
//#include "GameFramework/Actor.h"
#include "Furniture/InteractableActor.h"
#include "Altar_cpp.generated.h"

// 클리어 델리게이트 이벤트
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnClear);

UCLASS()
class HIDEANDSEEK_API AAltar_cpp : public AInteractableActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAltar_cpp();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Altar")
		TObjectPtr<USceneComponent> RootComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Altar")
		TObjectPtr<UStaticMeshComponent> Altar;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "SpawnPoint")
		TObjectPtr<UStaticMeshComponent> SpawnPoint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Object")
		TSubclassOf<class AReaper_cpp> Reaper;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Object")
		class AReaper_cpp* CurrentReaper;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Object")
		class AHorrorGameCharacter* PlayerCharacter;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Object")
		class AEnd_Mirror* EndingMirror;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Sound")
		TObjectPtr<class UAudioComponent> InteractSound;

	UPROPERTY(VisibleAnywhere, Category = Object)
		int32 UnSealedItemNum; // 플레이어가 봉인 해제한 오브젝트 아이템의 개수

	// 클리어하기 위해 필요한 오브젝트 수.
	UPROPERTY(VisibleAnywhere, Category = Object)
	int32 ObjectAmountNeeded;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Object")
		float RespawnTimer;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Object")
		bool bIsLevelStart;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Object")
	bool bHasLevelGimmick;

	// 클리어 델리게이트 이벤트.
	UPROPERTY(BlueprintAssignable, Category = Event)
	FOnClear OnClear;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	 virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
		void OnInteract(class AHorrorGameCharacter* Player);

	UFUNCTION(BlueprintCallable)
		void ReaperSpawn();

	UFUNCTION(BlueprintCallable)
		void UnSealedObjectNumber(int32 value);
};
