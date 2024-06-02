// CopyrightNotice 2023 Sunggon Kim kimdave205@gmail.com. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AI/CreatureClass.h"
//#include "GameFramework/Character.h"
//#include "InputActionValue.h"
#include "Brute_cpp.generated.h"

UCLASS()
class HIDEANDSEEK_API ABrute_cpp : public ACreatureClass
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputAction* MoveAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputAction* LookAction;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Light")
		TObjectPtr<class UPointLightComponent> DetectSight;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Sound")
		TObjectPtr<class UBoxComponent> BoxCollision;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Sound")
		TObjectPtr<class UAudioComponent> BruteSound;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Sound")
		TObjectPtr<class USoundCue> DetectedSound;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Sound")
		TObjectPtr<class USoundCue> PatrolSound;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Interact")
		TObjectPtr<class USphereComponent> KillSphere;

public:
	// Sets default values for this character's properties
	ABrute_cpp();

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
	class AHideObject* PlayerHidingObject;

public:
	// Deprecated
	/*UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PatrolPointList")
		TArray<FName>PatrolPointList = { TEXT("BP_PatrolPoint"), TEXT("BP_PatrolPoint2"), TEXT("BP_PatrolPoint3"), TEXT("BP_PatrolPoint4"), TEXT("BP_PatrolPoint5"),
		TEXT("BP_PatrolPoint6"), TEXT("BP_PatrolPoint7"), TEXT("BP_PatrolPoint8"), TEXT("BP_PatrolPoint9"), TEXT("BP_PatrolPoint10"),
		TEXT("BP_PatrolPoint11"), TEXT("BP_PatrolPoint12"), TEXT("BP_PatrolPoint13"), TEXT("BP_PatrolPoint14"), TEXT("BP_PatrolPoint15"),
		TEXT("BP_PatrolPoint16"), TEXT("BP_PatrolPoint17"), TEXT("BP_PatrolPoint18"), TEXT("BP_PatrolPoint19"), TEXT("BP_PatrolPoint20"),
		TEXT("BP_PatrolPoint21"), TEXT("BP_PatrolPoint22"), TEXT("BP_PatrolPoint23"), TEXT("BP_PatrolPoint24"), TEXT("BP_PatrolPoint25") };
	*/
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

	// ?
	UPROPERTY()
	float RangeChangeTime = 0.f;
	
	// 행동 불능이 유지되는 시간
	UPROPERTY()
	float stunTime = 0.f;

	// 소리 감지 범위
	UPROPERTY()
	float NoiseDetectRange = 2400.f;

	// 소리 감지 범위 증가 여부
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool bCalledRangeChange = false;
	
	// 범위가 증가되었는지 여부
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool bIsRangeChange = false;

	// 플레이어가 청동 거울을 사용했는지 여부
	UPROPERTY()
	bool bIsTimeStop = false; // 시간 정지

	// 행동 불능된 동안 흐르는 시간
	float CurrentStunnedTime = 0;

	// 시간 정지된 동안 흐르는 시간
	float TimeStopElapsedTime = 0;

public:
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
	void DetectPlayerHidingObject(class AHideObject* DetectedObject);

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
		void BroadCastChangeNoiseRange(const bool value);

	UFUNCTION(BlueprintCallable)
		void ChangeNoiseRange(const bool value);

	UFUNCTION(BlueprintCallable)
		float GetNoiseRange();

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