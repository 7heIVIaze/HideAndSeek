// CopyrightNotice=0 2023 Sunggon Kim kimdave205@gmail.com

#pragma once

#include "CoreMinimal.h"
#include "Items/Items.h"
#include "Mirror_cpp.generated.h"

UCLASS()
class HIDEANDSEEK_API AMirror_cpp : public AItems
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMirror_cpp();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
//	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, Category = "Root")
		TObjectPtr<USceneComponent> DefaultSceneRoot;

	UPROPERTY(VisibleAnywhere, Category = "Mirror")
		TObjectPtr<class UStaticMeshComponent> MirrorMesh;

	UPROPERTY(VisibleAnywhere, Category = "Mirror")
		class AAltar_cpp* Altar;

public:
	void OnInteract(class AHorrorGameCharacter* Player) override;

	void UseInteract(class AHorrorGameCharacter* Player) override;
};
