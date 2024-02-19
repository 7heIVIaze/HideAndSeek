// CopyrightNotice=0 2023 Sunggon Kim kimdave205@gmail.com

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

public:
	UFUNCTION(BlueprintCallable)
		virtual void ChangeMaterialInstance(float inValue) {};

	UFUNCTION(BlueprintCallable)
		virtual void DissolveFinish() {};
//public:
//	// Sets default values for this character's properties
//	ACreatureClass();
//
//protected:
//	// Called when the game starts or when spawned
//	virtual void BeginPlay() override;
//
//public:	
//	// Called every frame
//	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
//	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
//
//public:
//
//	void Move(const FInputActionValue& Value);
//
//	void Look(const FInputActionValue& Value);
//
//	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PatrolPointList")
//		TArray<FName>PatrolPointList = { TEXT("BP_PatrolPoint"), TEXT("BP_PatrolPoint2"), TEXT("BP_PatrolPoint3"), TEXT("BP_PatrolPoint4"), TEXT("BP_PatrolPoint5"),
//		TEXT("BP_PatrolPoint6"), TEXT("BP_PatrolPoint7"), TEXT("BP_PatrolPoint8"), TEXT("BP_PatrolPoint9"), TEXT("BP_PatrolPoint10"),
//		TEXT("BP_PatrolPoint11"), TEXT("BP_PatrolPoint12"), TEXT("BP_PatrolPoint13"), TEXT("BP_PatrolPoint14"), TEXT("BP_PatrolPoint15"),
//		TEXT("BP_PatrolPoint16"), TEXT("BP_PatrolPoint17"), TEXT("BP_PatrolPoint18"), TEXT("BP_PatrolPoint19"), TEXT("BP_PatrolPoint20"),
//		TEXT("BP_PatrolPoint21"), TEXT("BP_PatrolPoint22"), TEXT("BP_PatrolPoint23"), TEXT("BP_PatrolPoint24"), TEXT("BP_PatrolPoint25") };
//
//	UPROPERTY()
//		bool bIsStunned = false;
//
//	UPROPERTY()
//		bool bIsDied = false;
//
//	UPROPERTY()
//		bool bIsChase = false;
//
//	UPROPERTY()
//		float baseStunTime = 10.0f;
//
//	UPROPERTY()
//		float StunTime = 0.0f;
//
//	UPROPERTY()
//		int32 cnt = 0;
//
//	UPROPERTY()
//		class APatrolPoint_cpp* PatrolPoint = nullptr;
//
//	/*UFUNCTION(BlueprintCallable)
//		void SoundBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
//
//	UFUNCTION(BlueprintCallable)
//		void SoundEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
//
//	UFUNCTION(BlueprintCallable)
//		void DoorBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);*/
//
//	UFUNCTION(BlueprintCallable)
//		FVector GetPatrolPoint();
//
//	UFUNCTION(BlueprintCallable)
//		void StartChase();
//
//	UFUNCTION(BlueprintCallable)
//		void EndChase();
//
//	UFUNCTION(BlueprintCallable)
//		bool GetIsStunned();
//
//	UFUNCTION(BlueprintCallable)
//		void Exorcism();
//
//	UFUNCTION(BlueprintCallable)
//		void Stunning(float dist);
};
