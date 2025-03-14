// CopyrightNotice 2023 Sunggon Kim kimdave205@gmail.com. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
//#include "GameFramework/Actor.h"
//#include "Resource/InteractInterface.h"
#include "Furniture/InteractableActor.h"
#include "Alarm.generated.h"

UCLASS()
class HIDEANDSEEK_API AAlarm : public AInteractableActor //public AActor , public IInteractInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAlarm();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AlarmMesh")
		TObjectPtr<USceneComponent> RootComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AlarmMesh")
		TObjectPtr<UStaticMeshComponent> AlarmMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AlarmMesh")
		TObjectPtr<class UPointLightComponent> LightComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AlarmMesh")
		TObjectPtr<class UAudioComponent> AudioComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AlarmBoolean")
		bool bIsAlarmRing;

public:	
	// Called every frame
	// virtual void Tick(float DeltaTime) override;

	virtual void OnInteract(class AHorrorGameCharacter* Player) override;
	//virtual void OnInteract() override;

	virtual bool UseInteract(class AHorrorGameCharacter* Player) override;
	//virtual void UseInteract() override;

	UFUNCTION(BlueprintCallable)
	virtual void AIInteract();

	UFUNCTION(BlueprintCallable)
	void SetAlarmRing();
};
