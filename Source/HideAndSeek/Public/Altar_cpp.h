// CopyrightNotice=0 2023 Sunggon Kim kimdave205@gmail.com

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Altar_cpp.generated.h"

UCLASS()
class HIDEANDSEEK_API AAltar_cpp : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAltar_cpp();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Altar")
		TObjectPtr<USceneComponent> RootComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Altar")
		TObjectPtr<UStaticMeshComponent> Altar;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Object")
		TObjectPtr<UStaticMeshComponent> Sword;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Object")
		TObjectPtr<UStaticMeshComponent> Mirror;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Object")
		TObjectPtr<UStaticMeshComponent> Bell;

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

	UPROPERTY()
		float RespawnTimer;

	UPROPERTY()
		bool bIsLevelStart;

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
