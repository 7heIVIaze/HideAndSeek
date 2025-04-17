// CopyrightNotice 2023 Sunggon Kim kimdave205@gmail.com. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LightItem.generated.h"

UCLASS()
class HIDEANDSEEK_API ALightItem : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ALightItem();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	virtual bool TurnOn();

	UFUNCTION(BlueprintCallable)
	virtual void LightOn();

	UFUNCTION(BlueprintCallable)
	virtual bool TurnOff();

	UFUNCTION(BlueprintCallable)
	bool GetIsLightOn() const { return bIsLightOn; }

	UFUNCTION(BlueprintCallable)
	virtual void SetupDurability(int inDurability);

	// visibility 설정(hidden in game).
	UFUNCTION(BlueprintCallable)
	virtual void SetItemVisibility(bool NewVisibility);


protected:
	// Set the lightcomponent to have specific light component.
	virtual void SetupLightComponent() PURE_VIRTUAL(ALightItem::LightComponent, );

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Light)
	TObjectPtr<USceneComponent> RootComp;

	// 조명 컴포넌트로 선언함 : 랜턴/라이터: PointLightComponent, 손전등: SpotLightComponent.
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Light)
	TObjectPtr<class ULightComponent> LightComponent;

	// 아이템 사용 효과음.
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Sound)
	TObjectPtr<class UAudioComponent> AudioComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Sound)
	TObjectPtr<class USoundCue> TurnoffSoundCue;

	// 조명이 켜져있는가?
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Light)
	bool bIsLightOn;

	// 조명의 세기
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Light)
	float LightIntensity;

	// Battery/Durability.
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Durability)
	int Durability;

	UPROPERTY(VisibleAnywhere, Category = Timer)
	float Timer = 0.0f;

//	
//public:	
//	// Sets default values for this actor's properties
//	ALightItem();
//
//protected:
//	// Called when the game starts or when spawned
//	virtual void BeginPlay() override;
//
//public:
//	UPROPERTY(VisibleAnywhere, Category = "Root")
//		class USceneComponent* DefaultSceneRoot;
//
//	UPROPERTY(VisibleAnywhere, Category = "Sword")
//		TObjectPtr<UStaticMeshComponent> Sword;
//
//	UPROPERTY(VisibleAnywhere, Category = "Altar")
//		class AAltar_cpp* Altar;
//
//	UPROPERTY(EditAnywhere, Category = "Sound")
//		class USoundCue* ObjectSound;
//
//
//public:	
//	void OnInteract(class AHorrorGameCharacter* Player) override;
//
//	void UseInteract(class AHorrorGameCharacter* Player) override;
};
