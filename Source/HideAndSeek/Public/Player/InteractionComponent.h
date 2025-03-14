// CopyrightNotice 2023 Sunggon Kim kimdave205@gmail.com. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InteractionComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class HIDEANDSEEK_API UInteractionComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInteractionComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	AActor* GetTracedActor();

	// Variables
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Trace)
	AActor* HitActor;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Trace)
	FVector TraceStartVector;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Trace)
	FVector TraceForwardVector;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Trace)
	FVector TraceEndVector;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Trace)
	float TraceLength;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Trace)
	TObjectPtr<class AHorrorGameCharacter> OwnerPlayer;
		
};
