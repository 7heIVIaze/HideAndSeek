// CopyrightNotice 2023 Sunggon Kim kimdave205@gmail.com. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "Components/TimelineComponent.h"
#include "CreatureClass.generated.h"

UENUM(BlueprintType)
enum class Creature_Status : uint8
{
	Survive UMETA(DisplayName = "Survive"),
	Stunned UMETA(DisplayName = "Stunned"),
	Catched UMETA(DisplayName = "Catched"),
	Died UMETA(DisplayName = "Died"),
};

UCLASS()
class HIDEANDSEEK_API ACreatureClass : public ACharacter
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, Category = "Dissolve")
	FTimeline DissolveTimeline; // Exorcism 시 사라지는 이펙트를 위한 타임라인 생성

	UPROPERTY(EditAnywhere, Category = "Dissolve")
	UCurveFloat* DissolveCurveFloat; // Timeline Curve -> Dissolve 타임라인을 위한 float 변수

	UPROPERTY(VisibleAnywhere, Category = "Dissolve")
	UMaterialInstanceDynamic* MaterialInstance;

	UPROPERTY(VisibleAnywhere, Category = "Dissolve")
	class UNiagaraComponent* DissolveParticleSystem;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dissolve")
	class UTexture* Texture;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "PlayerWatchPoint")
	class UStaticMeshComponent* WatchPoint;

public:
	UFUNCTION(BlueprintCallable)
	virtual void ChangeMaterialInstance(float inValue) {};

	UFUNCTION(BlueprintCallable)
	virtual void DissolveFinish() {};
};