// CopyrightNotice 2023 Sunggon Kim kimdave205@gmail.com. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AI/CreatureClass.h"
#include "Rampage_cpp.generated.h"

/**
 * 
 */
UCLASS()
class HIDEANDSEEK_API ARampage_cpp : public ACreatureClass
{
	GENERATED_BODY()
	
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Light)
		class UPointLightComponent* DetectSight;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Sound)
		class UBoxComponent* BoxCollision;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Interact)
		class UBoxComponent* KillBox;

public:
	ARampage_cpp();

protected:
	virtual void BeginPlay() override;

private:
	bool bIsStunned = false;
	bool bIsDied = false;
	class APatrolPoint_cpp* CurrentPatrolPoint;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Cinematic", meta = (AllowPrivateAccess = "true"))
	bool bIsCinematic = true;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player")
		class AHorrorGameCharacter* Player;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Controller")
		class AAIController_Rampage* RampageController;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PatrolPointList")
		TArray<class APatrolPoint_cpp*> PatrolPointLists;

	/*UPROPERTY(VisibleAnywhere, Category = "Dissolve")
		TArray<UMaterialInstanceDynamic*> MaterialInstances;*/
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	//virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void PossessedBy(AController* NewController) override;

	//virtual void ChangeMaterialInstance(float inValue) override;

	//virtual void DissolveFinish() override;

public:
	UPROPERTY()
		bool bIsPatrolSuccess = false;

	int Index = 0;

	UPROPERTY()
		bool bIsChase = false;

	UPROPERTY()
		bool bIsCatch = false;

	UPROPERTY()
		bool bAnimFinish = false;

	UPROPERTY()
		bool bIsPlayerWatch = false;

	UPROPERTY()
		bool bIsCooldown = false;

	UPROPERTY()
		float stunTime = 0.0f;

	UPROPERTY()
		float CastingTime = 0.0f;

	UPROPERTY()
		float SkillCooldown = 0.0f;

	UPROPERTY()
		bool bIsTimeStop = false; // 시간 정지

	UPROPERTY()
		bool bSealedButChase = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool IsBerith = false; // 베리스 호출로 불렸는지

	UPROPERTY()
		FString MapName;

	float CurrentStunnedTime = 0;

	float TimeStopElapsedTime = 0;

	/* Called for Sprint Input */
	UFUNCTION(BlueprintCallable)
		void StartChase();

	UFUNCTION(BlueprintCallable)
		void EndChase();

	UFUNCTION(BlueprintCallable)
		void SetCreatureCollision(bool value);

	UFUNCTION(BlueprintCallable)
		void CalledByBerith();

	/*void EndChase();*/

	UFUNCTION(BlueprintCallable)
		void SetIsCatch(bool Value);

	UFUNCTION(BlueprintCallable)
		void SetAnimFinish(bool Value);

	UFUNCTION(BlueprintCallable)
		void SoundBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(BlueprintCallable)
		void SoundEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION(BlueprintCallable)
		void CheckBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(BlueprintCallable)
		FVector GetPatrolPoint();

	UFUNCTION(BlueprintCallable)
		bool GetIsStunned();

	UFUNCTION(BlueprintCallable)
		void SetIsCinematic(bool inIsCinematic);

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
		void SetIsStop(bool inIsStop);

	UFUNCTION(BlueprintCallable)
		bool GetIsStop();

	//UFUNCTION(BlueprintCallable)
	//	void CatchBeginOverlap(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	UFUNCTION(BlueprintCallable)
		void CatchBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(BlueprintCallable)
		bool GetPatrolSuccess();

	//UFUNCTION(BlueprintCallable)
	//	void SetCurrentMode(int Status); // 0: Cinematic, 1: OneUnsealed, 2: TwoUnsealed, 3: Unsealed, 4: SealedButChase

private:
	UPROPERTY()
		bool bIsStop = false; // 문을 여는 동작으로 인하여 멈추는지 확인

};
