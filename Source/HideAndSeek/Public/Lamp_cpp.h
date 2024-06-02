// CopyrightNotice 2023 Sunggon Kim kimdave205@gmail.com. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/TimelineComponent.h"
#include "Lamp_cpp.generated.h"

UCLASS()
class HIDEANDSEEK_API ALamp_cpp : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALamp_cpp();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
		void OnInteract();

	UFUNCTION(BlueprintCallable)
		void BoxOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherIndexBody, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(BlueprintCallable)
		void BoxOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherIndexBody);

	UFUNCTION(BlueprintCallable)
		void NearBoxOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherIndexBody, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(BlueprintCallable)
		void NearBoxOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherIndexBody);

	UFUNCTION(BlueprintCallable)
		void LightFlicker(float value);


public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "LightMesh")
		TObjectPtr<USceneComponent> RootComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "LightMesh")
		TObjectPtr<UStaticMeshComponent> LightComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "LightMesh")
		TObjectPtr<class UPointLightComponent> PointLight;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "LightMesh")
		TObjectPtr<class UBoxComponent> BoxCollision;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "LightMesh")
		TObjectPtr<class UBoxComponent> NearBoxCollision;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LightBoolean")
		bool bIsLightOn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LightBoolean")
		bool bIsPlayerIn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LightBoolean")
		bool bIsLightBlink;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LightBoolean")
		bool bIsNaturalFlick;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LightBoolean")
		int32 CreatureNearNum;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LightBoolean")
		int32 CreatureNum;

	UPROPERTY(VisibleAnywhere, Category = "LightAnim")
		FTimeline FlickeringLight;

	UPROPERTY(EditAnywhere, Category = "LightAnim")
		UCurveFloat* CurveFloat;

	UPROPERTY(EditAnywhere, Category = "LightAnim")
		UCurveFloat* NaturalCurveFloat;

	float Intensity = 8000.f;
};
