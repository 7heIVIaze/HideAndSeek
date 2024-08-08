// CopyrightNotice 2023 Sunggon Kim kimdave205@gmail.com. All Rights Reserved.

/***
* Deprecated! Furniture/HideObject 클래스가 이 클래스를 대체함.
***/

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"
#include "Components/TimelineComponent.h"
#include "Resource/HideInterface.h"
#include "Wardrobe_cpp.generated.h"

UCLASS()
class HIDEANDSEEK_API AWardrobe_cpp : public AActor, public IHideInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWardrobe_cpp();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	UPROPERTY(VisibleAnywhere, Category = "Wardrobe")
		TObjectPtr<USceneComponent> DefaultSceneRoot;

	UPROPERTY(VisibleAnywhere, Category = "Wardrobe")
		TObjectPtr<class UStaticMeshComponent> Wardrobe;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* Camera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CigarLight")
		TObjectPtr<class UPointLightComponent> CigarLight;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "FlashLight")
		TObjectPtr<class USpotLightComponent> FlashLight;

	/*UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Sound")
		TObjectPtr<class UAudioComponent> Turnon;*/

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Sound")
		TObjectPtr<class UAudioComponent> Sound;

	/*UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Sound")
		TObjectPtr<class UAudioComponent> CigarOn;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Sound")
		TObjectPtr<class UAudioComponent> CigarOff;*/

	UPROPERTY(VisibleAnywhere, Category = "PlayerCharacter")
		TObjectPtr<class AHorrorGameCharacter> Player = nullptr;

	/*UPROPERTY(VisibleAnywhere, Category = "Box Collision")
		TObjectPtr<UBoxComponent> BoxCollision;*/

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputAction* LookAction;

	UPROPERTY(VisibleAnywhere, Category = "LightMesh")
		FTimeline FlickeringLight; // Create Timeline

	UPROPERTY(EditAnywhere, Category = "LightMesh")
		UCurveFloat* CurveFloat; // Timeline Curve -> 불빛 깜빡이는거 효과 줄 변수

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "LightBoolean")
		int32 CreatureNum = 0; // Creature의 개수(불 깜박임을 위해서)

	float Intensity = 800.f;

public:
	bool bIsHiding = false;

	float Pitch = 0.f;
	float Yaw = 0.f;

	bool bIsFlashLightOn = false;
	bool bIsCigarLightOn = false;

public:
	UFUNCTION(BlueprintCallable, Category = "Interaction")
		virtual void OnInteract(class AHorrorGameCharacter* PlayerCharacter) override;

	UFUNCTION(BlueprintCallable, Category = "Interaction")
		void UseInteract();

	UFUNCTION(BlueprintCallable, Category = "Wardrobe")
		void ToggleHide(class AHorrorGameCharacter* PlayerCharacter);

	UFUNCTION(BlueprintCallable)
		void LightFlicker(float value);

	/*UFUNCTION(BlueprintCallable, Category = "BoxCollision")
		void OnComponentBeginOverlap(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(BlueprintCallable, Category = "BoxCollision")
		void OnComponentEndOverlap(class UPrimitiveComponent* OverlappedComponent, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);*/

	UFUNCTION()
		void LookUp(float AxisValue);

	UFUNCTION()
		void Turn(float AxisValue);

	UFUNCTION()
		void SetFlashLightOn();

	UFUNCTION()
		void SetCigarLightOn();

	UFUNCTION()
		void MoveCamera();

	UFUNCTION()
		void CigarLightOnFunc();

	UFUNCTION()
		void BreakWardrobe();

	UFUNCTION(BlueprintCallable, Category = "CameraNoise")
		void SetCameraComponentNoise(int32 WhichStatus); // 0: 노이즈 해제, 1: 근처에 있음, 2: 추격 시작

private:
	FTimerHandle _loopCameraTimerHandle, _CigarLightTimerHandle;
};
