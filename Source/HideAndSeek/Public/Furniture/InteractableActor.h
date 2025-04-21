// CopyrightNotice 2023 Sunggon Kim kimdave205@gmail.com. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Resource/InteractInterface.h"
#include "InteractableActor.generated.h"

// 상호작용을 할 수 없다는 것을 알릴 델리게이트 이벤트.
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInteractionMessage, FText, Message);

UCLASS()
class HIDEANDSEEK_API AInteractableActor : public AActor, public IInteractInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AInteractableActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void OnInteract(class AHorrorGameCharacter* Player) override;
	
	virtual bool UseInteract(class AHorrorGameCharacter* Player) override;


	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Event)
	FOnInteractionMessage OnInteractionMessage;
};
