// CopyrightNotice 2023 Sunggon Kim kimdave205@gmail.com. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Resource/ItemInterface.h"
#include "Shelf.generated.h"

UCLASS()
class HIDEANDSEEK_API AShelf : public AActor, public IItemInterface
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
	TObjectPtr<UChildActorComponent> Upper_Deco_Left;
	
	UPROPERTY(VisibleAnywhere, Category = "Shelf")
	TObjectPtr<UChildActorComponent> Upper_Deco_Right;

	UPROPERTY(VisibleAnywhere, Category = "Shelf")
	TObjectPtr<UChildActorComponent> Middle_Item;

	UPROPERTY(VisibleAnywhere, Category = "Shelf")
	TObjectPtr<UChildActorComponent> Middle_Deco_Left;

	UPROPERTY(VisibleAnywhere, Category = "Shelf")
	TObjectPtr<UChildActorComponent> Middle_Deco_Right;

	UPROPERTY(VisibleAnywhere, Category = "Shelf")
	TObjectPtr<UChildActorComponent> Lower_Item;

	UPROPERTY(VisibleAnywhere, Category = "Shelf")
	TObjectPtr<UChildActorComponent> Lower_Deco_Left;

	UPROPERTY(VisibleAnywhere, Category = "Shelf")
	TObjectPtr<UChildActorComponent> Lower_Deco_Right;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	TArray<TSubclassOf<AActor>> Decorators;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	bool bIsUpperItemSpawned;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	bool bIsMiddleItemSpawned;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	bool bIsLowerItemSpawned;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	UFUNCTION(BlueprintCallable)
		virtual bool SetSpawnItem(TSubclassOf<AActor> inItem) override;

	UFUNCTION(BlueprintCallable)
		bool GetIsUpperItemSpawned();

	UFUNCTION(BlueprintCallable)
		bool GetIsMiddleItemSpawned();

	UFUNCTION(BlueprintCallable)
		bool GetIsLowerItemSpawned();

};
