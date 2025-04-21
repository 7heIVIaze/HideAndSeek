// CopyrightNotice 2023 Sunggon Kim kimdave205@gmail.com. All Rights Reserved.

#pragma once

#include "EngineMinimal.h"
#include "GameFramework/Actor.h"
#include "Resource/InteractInterface.h"
#include "Resource/CustomDataTables.h"
#include "ItemClass.generated.h"

UCLASS()
class HIDEANDSEEK_API AItemClass : public AActor, public IInteractInterface
{
	GENERATED_BODY()

public:
	AItemClass();

public:
	UFUNCTION(BlueprintCallable)
	virtual void OnInteract(class AHorrorGameCharacter* Player) override;

	UFUNCTION(BlueprintCallable)
	virtual bool UseInteract(class AHorrorGameCharacter* Player) override;

	UFUNCTION(BlueprintCallable)
	virtual void HoldInteract(class AHorrorGameCharacter* Player);

	UFUNCTION(BlueprintCallable)
	void MoveFinish();

	UFUNCTION(BlueprintCallable)
	virtual FHorrorGameItemData GetItemData() { return ItemData; };

	UFUNCTION(BlueprintCallable)
	virtual void SetItemData(int inItemIndex);

	// Properties.
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Mesh)
	TObjectPtr<USceneComponent>RootComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Mesh)
	TObjectPtr<UStaticMeshComponent>ItemMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Data)
	FHorrorGameItemData ItemData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data)
	TObjectPtr<UDataTable> ItemDataTable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Sound)
	TObjectPtr<class USoundCue> ItemAcquireSoundCue;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data)
	int ItemIndex;
};
