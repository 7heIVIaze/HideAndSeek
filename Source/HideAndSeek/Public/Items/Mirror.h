// CopyrightNotice 2023 Sunggon Kim kimdave205@gmail.com. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Items/ActiveItem.h"
#include "Mirror.generated.h"

UCLASS()
class HIDEANDSEEK_API AMirror : public AActiveItem
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMirror();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	// virtual void Tick(float DeltaTime) override;

	// Use Interaction: by right click.
	/*UFUNCTION(BlueprintCallable)
	bool UseInteract(class APlayerCharacter* Player);*/

	virtual bool UseItem() override;

	////////////////
	// PROPERTIES //
	////////////////
public:
	/*UPROPERTY(VisibleAnywhere, Category = Mesh)
	TObjectPtr<USceneComponent> RootComp;

	UPROPERTY(VisibleAnywhere, Category = Mesh)
	TObjectPtr<UStaticMeshComponent> MirrorMesh;*/

	// Mirror use sound
	UPROPERTY(VisibleAnywhere, Category = Light)
	class USoundCue* MirrorCue;

//protected:
//	// Called when the game starts or when spawned
//	virtual void BeginPlay() override;
//
//public:	
//	// Called every frame
////	virtual void Tick(float DeltaTime) override;
//
//	UPROPERTY(VisibleAnywhere, Category = "Root")
//		TObjectPtr<USceneComponent> DefaultSceneRoot;
//
//	UPROPERTY(VisibleAnywhere, Category = "Mirror")
//		TObjectPtr<class UStaticMeshComponent> MirrorMesh;
//
//	UPROPERTY(VisibleAnywhere, Category = "Altar")
//		class AAltar_cpp* Altar;
//
//	UPROPERTY(EditAnywhere, Category = "Sound")
//		class USoundCue* ObjectSound;
//
//
//public:
//	void OnInteract(class AHorrorGameCharacter* Player) override;
//
//	void UseInteract(class AHorrorGameCharacter* Player) override;
};
