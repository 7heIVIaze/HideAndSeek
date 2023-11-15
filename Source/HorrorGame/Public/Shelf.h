// CopyrightNotice=0 2023 Sunggon Kim kimdave205@gmail.com

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Shelf.generated.h"

UCLASS()
class HORRORGAME_API AShelf : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AShelf();

	UPROPERTY(VisibleAnywhere, Category = "Shelf")
		TObjectPtr<USceneComponent> RootComp;

	UPROPERTY(VisibleAnywhere, Category = "Shelf")
		TObjectPtr<UStaticMeshComponent> ShelfMesh;

	UPROPERTY(VisibleAnywhere, Category = "Shelf")
		TObjectPtr<UChildActorComponent> Upper_Item;

	UPROPERTY(VisibleAnywhere, Category = "Shelf")
		TObjectPtr<UChildActorComponent> Middle_Item;

	UPROPERTY(VisibleAnywhere, Category = "Shelf")
		TObjectPtr<UChildActorComponent> Lower_Item;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
		TArray<TSubclassOf<class AItems>> Items;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
		TArray<float> ItemProbability;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
		float fNoItemProbability = 0.5f; // 아이템이 안 나올 확률
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	// virtual void Tick(float DeltaTime) override;
	UFUNCTION(BlueprintCallable)
		void SetUpperItem();

	UFUNCTION(BlueprintCallable)
		void SetMiddleItem();

	UFUNCTION(BlueprintCallable)
		void SetLowerItem();
};
