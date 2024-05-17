// CopyrightNotice=0 2023 Sunggon Kim kimdave205@gmail.com

#pragma once

#include "CoreMinimal.h"
#include "AI/CreatureClass.h"
#include "Shadow_cpp.generated.h"

/**
 * 
 */
UCLASS()
class HIDEANDSEEK_API AShadow_cpp : public ACreatureClass
{
	GENERATED_BODY()
	
public:
	// 얼굴 부분에 빛을 낼 포인트라이트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Light")
	TObjectPtr<class UPointLightComponent> DetectSight;

	// 
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Sound")
	TObjectPtr<class UBoxComponent> BoxCollision;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Interact")
	TObjectPtr<class USphereComponent> KillSphere;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PatrolPointList")
	TArray<class APatrolPoint_cpp*> PatrolPointList;

public:
	// Constructor
	AShadow_cpp();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	bool bIsDied = false;
	// 현재 향하고 있는 순찰 지점
	class APatrolPoint_cpp* CurrentPatrolPoint;

	//// 플레이어를 추격 중인 요괴들 배열을 관리하기 위한 포인터 변수
	//class AHorrorGameCharacter* Player;

	// 플레이어가 숨어있는 캐비닛/옷장
	class AActor* PlayerHidingObject;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void PossessedBy(AController* NewController) override;

	// 청동 검으로 사망 시 Dissolve Material Effect를 구현하기 위한 함수
	virtual void ChangeMaterialInstance(float inValue) override;
	
	virtual void DissolveFinish() override;

	UFUNCTION(BlueprintCallable)
	void OpenDoor();

	UFUNCTION(BlueprintCallable)
	void StartChase();

	UFUNCTION(BlueprintCallable)
	void EndChase();

	UFUNCTION(BlueprintCallable)
	void SetIsCatch(bool inIsCatch);

	UFUNCTION(BlueprintCallable)
	void SetAnimFinish(bool inIsAnimationFinished);

	UFUNCTION(BlueprintCallable)
	void SoundBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(BlueprintCallable)
	void SoundEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION(BlueprintCallable)
	FVector GetPatrolPoint();

	UFUNCTION(BlueprintCallable)
	void DetectPlayerHidingObject(AActor* DetectedObject);

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

public:
	// 패트롤 여부
	UPROPERTY()
	bool bIsPatrolSuccess = false;

	// 추격 여부
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
	bool bIsTimeStop = false;

private:
	// 시간 정지된 동안 흐르는 시간
	float TimeStopElapsedTime = 0;

	UPROPERTY()
	bool bIsStop = false; // 문을 여는 동작으로 인하여 멈추는지 확인
};
