// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/CreatureClass.h"
//#include "GameFramework/Character.h"
//#include "InputActionValue.h"
#include "Runner_cpp.generated.h"

UCLASS()
class HIDEANDSEEK_API ARunner_cpp : public ACreatureClass
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputAction* LookAction;

	// class UAnimSequence* Anim;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Light")
		TObjectPtr<class UPointLightComponent> DetectSight;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Sound")
		TObjectPtr<class UBoxComponent> BoxCollision;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Sound")
		TObjectPtr<class UAudioComponent> RunnerSound;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Sound")
		TObjectPtr<class USoundCue> DetectedSound;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Sound")
		TObjectPtr<class USoundCue> PatrolSound;

	/*UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Interact")
		TObjectPtr<class UBoxComponent> InteractBox;*/

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Interact")
		TObjectPtr<class USphereComponent> KillSphere;

public:
	// Sets default values for this character's properties
	ARunner_cpp();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	void Move(const FInputActionValue& Value);

	void Look(const FInputActionValue& Value);

	bool bIsStunned = false;
	bool bIsDied = false;
	// 현재 향하고 있는 순찰 지점
	class APatrolPoint_cpp* CurrentPatrolPoint;

	// 플레이어가 숨어있는 캐비닛/옷장
	class AActor* PlayerHidingObject;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PatrolPointList")
		TArray<class APatrolPoint_cpp*> PatrolPointList;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PatrolPointList")
		bool bIsCollectMode; // Level1처럼 오브젝트를 모으는 챕터인가

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void PossessedBy(AController* NewController) override;

	virtual void ChangeMaterialInstance(float inValue) override;

	virtual void DissolveFinish() override;

	UFUNCTION(BlueprintCallable)
	void OpenDoor();

public:
	// 패트롤 성공 여부
	UPROPERTY()
		bool bIsPatrolSuccess = false;

	// 추격 중인지 여부
	UPROPERTY()
	bool bIsChase = false;

	// 플레이어를 잡았는지 여부
	UPROPERTY()
	bool bIsCatch = false;

	// 플레이어가 숨은 곳을 잡았는지 여부
	UPROPERTY()
	bool bIsHidingCatch = false;

	// 공격 애니메이션 종료 여부
	UPROPERTY()
	bool bAnimFinish = false;

	// 플레이어가 청동 거울을 사용했는지 여부
	UPROPERTY()
	bool bIsTimeStop = false; // 시간 정지

	// 행동 불능이 유지되는 시간
	UPROPERTY()
	float stunTime = 0.0f;

	// 행동 불능된 동안 흐르는 시간
	float CurrentStunnedTime = 0;

	// 시간 정지된 동안 흐르는 시간
	float TimeStopElapsedTime = 0;

public:
	/* Called for Sprint Input */
	UFUNCTION(BlueprintCallable)
		void StartChase();

	UFUNCTION(BlueprintCallable)
		void EndChase();

	UFUNCTION(BlueprintCallable)
		void SetIsCatch(bool Value);

	UFUNCTION(BlueprintCallable)
		void SetAnimFinish(bool Value);

	UFUNCTION(BlueprintCallable)
		void SoundBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(BlueprintCallable)
		void SoundEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION(BlueprintCallable)
		FVector GetPatrolPoint();

	UFUNCTION(BlueprintCallable)
	void DetectPlayerHidingObject(AActor* DetectedObject);

	UFUNCTION(BlueprintCallable)
		bool GetIsStunned();

	UFUNCTION(BlueprintCallable)
		void Exorcism();

	UFUNCTION(BlueprintCallable)
		void SetPatrolSuccess(bool value);

	UFUNCTION(BlueprintCallable)
		void Stunning(float dist);

	UFUNCTION(BlueprintCallable)
		void SetStun();

	UFUNCTION(BlueprintCallable)
		bool GetIsDied();

	UFUNCTION(BlueprintCallable)
		bool GetIsCatch();

	UFUNCTION(BlueprintCallable)
		bool GetAnimFinish();

	UFUNCTION(BlueprintCallable)
		void CatchBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherIndexBody, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(BlueprintCallable)
		bool GetPatrolSuccess();

	UFUNCTION(BlueprintCallable)
		void SetCreatureCollision(bool value);

	UFUNCTION(BlueprintCallable)
		void SetIsStop(bool inIsStop);

	UFUNCTION(BlueprintCallable)
		bool GetIsStop();

private:
	UPROPERTY()
		bool bIsStop = false; // 문을 여는 동작으로 인하여 멈추는지 확인
};