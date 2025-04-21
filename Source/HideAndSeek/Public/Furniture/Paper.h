// CopyrightNotice 2023 Sunggon Kim kimdave205@gmail.com. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Furniture/InteractableActor.h"
#include "Paper.generated.h"

UCLASS()
class HIDEANDSEEK_API APaper : public AInteractableActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APaper();

	UPROPERTY(VisibleAnywhere, Category = Defaults)
	TObjectPtr<USceneComponent> RootComp;

	UPROPERTY(VisibleAnywhere, Category = Mesh)
	TObjectPtr<UStaticMeshComponent> PaperMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Text, meta = (MultiLine = true))
	FText text;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Text)
	int DocumentIndex;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Text)
	class UDataTable* ArchiveData;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	virtual void OnInteract(class AHorrorGameCharacter* Player) override;

};
