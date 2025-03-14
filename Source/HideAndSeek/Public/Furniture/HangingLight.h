// CopyrightNotice 2023 Sunggon Kim kimdave205@gmail.com. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
//#include "GameFramework/Actor.h"
#include "Components/TimelineComponent.h"
#include "Furniture/LightingClass.h"
#include "HangingLight.generated.h"

UCLASS()
class HIDEANDSEEK_API AHangingLight : public ALightingClass //public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHangingLight();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "LightMesh")
	//TObjectPtr<USceneComponent> RootComp;

	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "LightMesh")
	//TObjectPtr<UStaticMeshComponent> LightComp;

	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "LightMesh")
	//TObjectPtr<class UPointLightComponent> PointLight;

	//// 요괴들과 충돌 시 불을 깜빡이게 하기 위한 충돌체
	//UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "LightMesh")
	//TObjectPtr<class UBoxComponent> BoxCollision;

	//// 요괴들과 충돌 시 불이 꺼지게 하기 위한 충돌체.
	//UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "LightMesh")
	//TObjectPtr<class USphereComponent> SphereCollision;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LightBoolean")
	//bool bIsLightOn;

	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "LightBoolean")
	//bool bIsPlayerIn;

	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "LightBoolean")
	//bool bIsLightBlink = false;

	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "LightBoolean")
	//int32 CreatureNearNum;

	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "LightBoolean")
	//int32 CreatureNum; // Creature의 개수(불 깜박임을 위해서)

	//UPROPERTY(VisibleAnywhere, Category = "LightMesh")
	//FTimeline FlickeringLightTimeline; // Create Timeline

	//UPROPERTY(EditAnywhere, Category = "LightMesh")
	//UCurveFloat* FlickeringCurveFloat; // Timeline Curve -> 불빛 깜빡이는거 효과 줄 변수

	//UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Sound")
	//class USoundCue* LightSound;

	//UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "LightMesh")
	//float Intensity = 8000.f;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	//virtual void OnInteract(class AHorrorGameCharacter* Player) override;

	/*UFUNCTION(BlueprintCallable)
	void BoxOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherIndexBody, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(BlueprintCallable)
	void BoxOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherIndexBody);

	UFUNCTION(BlueprintCallable)
	void SphereOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherIndexBody, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(BlueprintCallable)
	void SphereOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherIndexBody);*/

	virtual void LightFlicker(float value) override;
};
