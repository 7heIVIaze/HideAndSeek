// CopyrightNotice 2023 Sunggon Kim kimdave205@gmail.com. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"
#include "HideInterface.h"
#include "Components/TimelineComponent.h"
#include "HideObject.generated.h"

UCLASS()
class HIDEANDSEEK_API AHideObject : public AActor, public IHideInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHideObject();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// PROPERTES
public:
	UPROPERTY(VisibleAnywhere, Category = HideObject)
	TObjectPtr<USceneComponent> RootComp;

	UPROPERTY(EditDefaultsOnly, Category = HideObject)
	TObjectPtr<UStaticMeshComponent> ObjectMesh;

	UPROPERTY(EditDefaultsOnly, Category = HiddenPlayer)
	TObjectPtr<class UCameraComponent> Camera;

	UPROPERTY(EditDefaultsOnly, Category = Light)
	TObjectPtr<class UPointLightComponent> CigarLight;

	UPROPERTY(EditDefaultsOnly, Category = Light)
	TObjectPtr<class USpotLightComponent> FlashLight;

	UPROPERTY(EditDefaultsOnly, Category = Light)
	TObjectPtr<UChildActorComponent> Lantern;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Sound)
	TObjectPtr<class UAudioComponent> Sound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Sound)
	TObjectPtr<class USoundCue> SoundCue;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Sound)
	TObjectPtr<class USoundCue> BreakSound;

	UPROPERTY(VisibleAnywhere, Category = PlayerCharacter)
	TObjectPtr<class AHorrorGameCharacter> Player = nullptr;

	UPROPERTY(VisibleAnywhere, Category = LightMesh)
	FTimeline FlickeringLight; // Create Timeline

	UPROPERTY(EditAnywhere, Category = LightMesh)
	UCurveFloat* CurveFloat; // Timeline Curve -> 불빛 깜빡이는거 효과 줄 변수

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = LightBoolean)
	int32 CreatureNum = 0; // Creature의 개수(불 깜박임을 위해서)

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = PlayerCharacter)
	TObjectPtr<UStaticMeshComponent> PlayerOutPoint;

	// Variable
public:
	UPROPERTY(BlueprintReadWrite)
	bool bIsHiding = false;

	float Intensity = 800.f;

	float Pitch = 0.f;

	float Yaw = 0.f;

	UPROPERTY(BlueprintReadWrite)
	bool bIsFlashLightOn = false;

	UPROPERTY(BlueprintReadWrite)
	bool bIsCigarLightOn = false;

	UPROPERTY(BlueprintReadWrite)
	bool bIsLanternOn = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera_Variable)
	FRotator OriginRotation;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Camera_Variable)
	FRotator PlayerRotation;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	virtual void OnInteract(class AHorrorGameCharacter* PlayerCharacter) override;

	UFUNCTION(BlueprintCallable)
	void ToggleHide(class AHorrorGameCharacter* PlayerCharacter);

	UFUNCTION()
	void SetFlashLightOn();

	UFUNCTION()
	void SetCigarLightOn();
	
	UFUNCTION()
	void SetLanternOn();

	UFUNCTION()
	void MoveCamera();

	UFUNCTION()
	void BreakHideObject();

	UFUNCTION(BlueprintCallable)
	void LightFlicker(float value);

	UFUNCTION(BlueprintCallable, Category = "CameraNoise")
	void SetCameraComponentNoise(int32 WhichStatus); // 0: 노이즈 해제, 1: 근처에 있음, 2: 추격 시작

};
