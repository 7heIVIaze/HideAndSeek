// CopyrightNotice 2023 Sunggon Kim kimdave205@gmail.com. All Rights Reserved.

#pragma once

#include "EngineMinimal.h"
#include "GameFramework/Actor.h"
#include "ActiveItem.generated.h"

UCLASS()
class HIDEANDSEEK_API AActiveItem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AActiveItem();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Item Use.
	UFUNCTION(BlueprintCallable)
	virtual bool UseItem();

	UFUNCTION(BlueprintCallable)
	virtual void SetupDurability(int inDurability);

	// Set visibility(hidden in game).
	UFUNCTION(BlueprintCallable)
	virtual void SetItemVisibility(bool NewVisibility);

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Mesh)
	TObjectPtr<USceneComponent> RootComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Mesh)
	TObjectPtr<UStaticMeshComponent> MeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Durabiility)
	int Durability;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Timer)
	float Timer = 0.0f;

};
