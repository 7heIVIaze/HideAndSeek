// CopyrightNotice 2023 Sunggon Kim kimdave205@gmail.com. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
//#include "GameFramework/Actor.h"
//#include "Resource/InteractInterface.h"
#include "Furniture/InteractableActor.h"
#include "Paper.generated.h"

// 문서가 추가되었다는 델리게이트 이벤트.
//DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDocumentMessage, FText, Message);

UCLASS()
class HIDEANDSEEK_API APaper : public AInteractableActor //public AActor, public IInteractInterface
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

	/*UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Event)
	FOnDocumentMessage OnInteractionMessage;*/

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	virtual void OnInteract(class AHorrorGameCharacter* Player) override;

};
