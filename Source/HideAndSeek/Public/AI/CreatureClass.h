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
	ACreatureClass();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Light)
	class UPointLightComponent* DetectSight;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Sound)
	class UBoxComponent* BoxCollision;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Sound)
	class UAudioComponent* YokaiSound;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Sound)
	class USoundCue* DetectedSound;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Sound)
	class USoundCue* PatrolSound;

	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Interact)
	//	class UBoxComponent* InteractBox;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Interact")
	TObjectPtr<class USphereComponent> KillSphere;

	UPROPERTY(VisibleAnywhere, Category = "Dissolve")
	FTimeline DissolveTimeline; // Exorcism 시 사라지는 이펙트를 위한 타임라인 생성

	UPROPERTY(EditAnywhere, Category = "Dissolve")
	UCurveFloat* DissolveCurveFloat; // Timeline Curve -> Dissolve 타임라인을 위한 float 변수

	/*UPROPERTY(VisibleAnywhere, Category = "Dissolve")
	UMaterialInstanceDynamic* MaterialInstance;*/

	UPROPERTY(VisibleAnywhere, Category = "Dissolve")
	TArray<UMaterialInstanceDynamic*> MaterialInstances;

	UPROPERTY(VisibleAnywhere, Category = "Dissolve")
	class UNiagaraComponent* DissolveParticleSystem;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dissolve")
	class UTexture* Texture;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "PlayerWatchPoint")
	class UStaticMeshComponent* WatchPoint;

public:
	UFUNCTION(BlueprintCallable)
	virtual void ChangeMaterialInstance(float inValue) {}

	UFUNCTION(BlueprintCallable)
	virtual void DissolveFinish() {}

	UFUNCTION(BlueprintCallable)
	virtual void OpenDoor();

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	//virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	//virtual void PossessedBy(AController* NewController) override;

	/* Called for Sprint Input */
	UFUNCTION(BlueprintCallable)
	virtual void  StartChase();

	UFUNCTION(BlueprintCallable)
	virtual void  EndChase();

	UFUNCTION(BlueprintCallable)
	virtual void  SetIsCatch(bool Value);

	UFUNCTION(BlueprintCallable)
	virtual void  SetAnimFinish(bool Value);

	UFUNCTION(BlueprintCallable)
	virtual void  SoundBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(BlueprintCallable)
	virtual void  SoundEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION(BlueprintCallable)
	virtual FVector GetPatrolPoint();

	UFUNCTION(BlueprintCallable)
	virtual void  DetectPlayerHidingObject(class AHideObject* DetectedObject);

	UFUNCTION(BlueprintCallable)
	virtual bool GetIsStunned();

	UFUNCTION(BlueprintCallable)
	virtual void  Exorcism();

	UFUNCTION(BlueprintCallable)
	virtual void  SetPatrolSuccess(bool value);

	UFUNCTION(BlueprintCallable)
	virtual void  Stunning(float dist);

	UFUNCTION(BlueprintCallable)
	virtual void  SetStun();

	UFUNCTION(BlueprintCallable)
	virtual bool GetIsDied();

	UFUNCTION(BlueprintCallable)
	virtual bool GetIsCatch();

	UFUNCTION(BlueprintCallable)
	virtual bool GetAnimFinish();

	UFUNCTION(BlueprintCallable)
	virtual void  CatchBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherIndexBody, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(BlueprintCallable)
	virtual bool GetPatrolSuccess();

	UFUNCTION(BlueprintCallable)
	virtual void  SetCreatureCollision(bool value);

	UFUNCTION(BlueprintCallable)
	virtual void  SetIsStop(bool inIsStop);

	UFUNCTION(BlueprintCallable)
	virtual bool GetIsStop();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PatrolPointList")
	TArray<class APatrolPoint_cpp*> PatrolPointList;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PatrolPointList")
	bool bIsCollectMode; // Level1처럼 오브젝트를 모으는 챕터인가

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player")
	class AHorrorGameCharacter* Player;

	// 패트롤 성공 여부
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Boolean)
	bool bIsPatrolSuccess = false;

	// 추격 중인지 여부
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Boolean)
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
	UPROPERTY()
	float CurrentStunnedTime = 0;

	// 시간 정지된 동안 흐르는 시간
	UPROPERTY()
	float TimeStopElapsedTime = 0;

	UPROPERTY()
	bool bIsStunned = false;
	bool bIsDied = false;
	// 현재 향하고 있는 순찰 지점
	class APatrolPoint_cpp* CurrentPatrolPoint;

	// 플레이어가 숨어있는 캐비닛/옷장
	class AHideObject* PlayerHidingObject;
private:
	UPROPERTY()
	bool bIsStop = false; // 문을 여는 동작으로 인하여 멈추는지 확인
};