// CopyrightNotice=0 2023 Sunggon Kim kimdave205@gmail.com

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlayerStatComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class HIDEANDSEEK_API UPlayerStatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPlayerStatComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	virtual void InitializeComponent() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	UPROPERTY(EditInstanceOnly, Category = Timer, Meta = (AllowPrivateAccess = true))
		int32 Miliseconds;

	UPROPERTY(EditInstanceOnly, Category = Timer, Meta = (AllowPrivateAccess = true))
		int32 Seconds;

	UPROPERTY(EditInstanceOnly, Category = Timer, Meta = (AllowPrivateAccess = true))
		int32 Minutes;

	UPROPERTY(EditInstanceOnly, Category = Timer, Meta = (AllowPrivateAccess = true))
		int32 Hours;
};
