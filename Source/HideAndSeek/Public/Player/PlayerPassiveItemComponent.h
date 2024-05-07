// CopyrightNotice=0 2023 Sunggon Kim kimdave205@gmail.com

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlayerPassiveItemComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class HIDEANDSEEK_API UPlayerPassiveItemComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPlayerPassiveItemComponent();

	UPROPERTY()
		struct FHorrorGameItemData PassiveItem;
	/*UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Collider")
		TObjectPtr<class USphereComponent> SphereCollider;*/

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	//virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	UFUNCTION(BlueprintCallable)
		void GetPassiveItem(int inPassiveItemNumber); // inPassiveItemNumber is PassiveItemNumber. It starts from 0.
};
