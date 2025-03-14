// CopyrightNotice 2023 Sunggon Kim kimdave205@gmail.com. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AI/CreatureClass.h"
//#include "GameFramework/Character.h"
//#include "InputActionValue.h"
#include "Reaper_cpp.generated.h"

UCLASS()
class HIDEANDSEEK_API AReaper_cpp : public ACreatureClass // ACharacter
{
	GENERATED_BODY()

	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	//	class UInputAction* MoveAction;

	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	//	class UInputAction* LookAction;

	class UAnimSequence* Anim;

public:
	/*UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Light)
		class UPointLightComponent* DetectSight;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Sound)
		class UBoxComponent* BoxCollision;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Sound)
		class UAudioComponent* ReaperSound;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Sound)
		class USoundCue* DetectedSound;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Sound)
		class USoundCue* PatrolSound;*/

	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Interact)
	//	class UBoxComponent* InteractBox;

	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Interact")
	//TObjectPtr<class USphereComponent> KillSphere;

public:
	// Sets default values for this character's properties
	AReaper_cpp();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	/*void Move(const FInputActionValue& Value);

	void Look(const FInputActionValue& Value);*/

	//bool bIsStunned = false;
	//bool bIsDied = false;
	//// 현재 향하고 있는 순찰 지점
	//class APatrolPoint_cpp* CurrentPatrolPoint;

	//// 플레이어가 숨어있는 캐비닛/옷장
	//class AHideObject* PlayerHidingObject;

public:
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PatrolPointList")
	//	TArray<class APatrolPoint_cpp*> PatrolPointList;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PatrolPointList")
	//	bool bIsCollectMode; // Level1처럼 오브젝트를 모으는 챕터인가

	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player")
	//	class AHorrorGameCharacter* Player;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Controller")
		class ACreatureAI* ReaperController;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Controller")
		int32 UnSealedItemNumber;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dissolve")
		class UMaterialParameterCollection* MPC_Reaper;

	//UPROPERTY(VisibleAnywhere, Category = "Dissolve")
	//TArray<UMaterialInstanceDynamic*> MaterialInstances;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	//virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void PossessedBy(class AController* NewController) override;

	virtual void ChangeMaterialInstance(float inValue) override;

	virtual void DissolveFinish() override;

	/*virtual void OpenDoor() override;*/

public:
	// 패트롤 성공 여부
	//UPROPERTY()
	//bool bIsPatrolSuccess = false;

	//int Index = 0;

	//// 추격 중인지 여부
	//UPROPERTY()
	//bool bIsChase = false;

	//// 플레이어를 잡았는지 여부
	//UPROPERTY()
	//bool bIsCatch = false;

	//// 플레이어가 숨은 곳을 잡았는지 여부
	//UPROPERTY()
	//bool bIsHidingCatch = false;

	//// 공격 애니메이션 종료 여부
	//UPROPERTY()
	//bool bAnimFinish = false;

	// 플레이어를 보고 있는지 여부
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool bIsPlayerWatch = false;

	// 플레이어를 행동 불능시키는 능력의 쿨타임이 돌고 있는지 여부
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool bIsCooldown = false;

	//// 행동 불능이 유지되는 시간
	//UPROPERTY()
	//float stunTime = 0.0f;

	// 능력의 캐스팅 시간
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float CastingTime = 0.0f;

	// 능력의 쿨타임
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float SkillCooldown = 0.0f;

	// 플레이어가 청동 거울을 사용했는지 여부
	//UPROPERTY()
	//bool bIsTimeStop = false; // 시간 정지

	// 프롤로그를 위한 변수임
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool bSealedButChase = false;

	// 맵 이름을 감지하기 위한 변수
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	FString MapName;

	//// 행동 불능된 동안 흐르는 시간
	//float CurrentStunnedTime = 0;

	//// 시간 정지된 동안 흐르는 시간
	//float TimeStopElapsedTime = 0;

public:
	/* Called for Sprint Input */
	/*virtual void StartChase() override;

	UFUNCTION(BlueprintCallable)
	virtual void EndChase() override;*/

	/*UFUNCTION(BlueprintCallable)
	virtual void SetCreatureCollision(bool value);*/

	UFUNCTION(BlueprintCallable)
	virtual	void SetPlayerWatch(bool value);

	/*UFUNCTION(BlueprintCallable)
		void SetIsCatch(bool Value);*/

	/*UFUNCTION(BlueprintCallable)
		void SetAnimFinish(bool Value);*/

	/*UFUNCTION(BlueprintCallable)
	virtual void SoundBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	UFUNCTION(BlueprintCallable)
	virtual void SoundEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;

	UFUNCTION(BlueprintCallable)
	FVector GetPatrolPoint();*/

	/*UFUNCTION(BlueprintCallable)
	void DetectPlayerHidingObject(class AHideObject* DetectedObject);

	UFUNCTION(BlueprintCallable)
		bool GetIsStunned();*/

	virtual void Exorcism() override;
	
	/*
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
		bool GetAnimFinish();*/

	/*UFUNCTION(BlueprintCallable)
	void SetIsStop(bool inIsStop);

	UFUNCTION(BlueprintCallable)
	bool GetIsStop();*/

	/*UFUNCTION(BlueprintCallable)
	void CatchBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(BlueprintCallable)
	bool GetPatrolSuccess();*/

	UFUNCTION(BlueprintCallable)
	virtual void SetCurrentStatus(int32 Status); // 0: Sealed, 1: OneUnsealed, 2: TwoUnsealed, 3: Unsealed, 4: SealedButChase

//private:
	//UPROPERTY()
	//	bool bIsStop = false; // 문을 여는 동작으로 인하여 멈추는지 확인
};