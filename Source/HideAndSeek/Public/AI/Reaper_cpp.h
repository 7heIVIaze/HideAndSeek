// CopyrightNotice=0 2023 Sunggon Kim kimdave205@gmail.com

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

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputAction* LookAction;

	class UAnimSequence* Anim;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Light)
		class UPointLightComponent* DetectSight;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Sound)
		class UBoxComponent* BoxCollision;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Sound)
		class UAudioComponent* ReaperSound;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Sound)
		class USoundCue* DetectedSound;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Sound)
		class USoundCue* PatrolSound;

	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Interact)
	//	class UBoxComponent* InteractBox;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Interact)
		class UBoxComponent* KillBox;

public:
	// Sets default values for this character's properties
	AReaper_cpp();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	void Move(const FInputActionValue& Value);

	void Look(const FInputActionValue& Value);

	bool bIsStunned = false;
	bool bIsDied = false;
	class APatrolPoint_cpp* CurrentPatrolPoint;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PatrolPointList")
		TArray<FName>PatrolPointList = { TEXT("BP_PatrolPoint"), TEXT("BP_PatrolPoint2"), TEXT("BP_PatrolPoint3"), TEXT("BP_PatrolPoint4"), TEXT("BP_PatrolPoint5"),
		TEXT("BP_PatrolPoint6"), TEXT("BP_PatrolPoint7"), TEXT("BP_PatrolPoint8"), TEXT("BP_PatrolPoint9"), TEXT("BP_PatrolPoint10"),
		TEXT("BP_PatrolPoint11"), TEXT("BP_PatrolPoint12"), TEXT("BP_PatrolPoint13"), TEXT("BP_PatrolPoint14"), TEXT("BP_PatrolPoint15"),
		TEXT("BP_PatrolPoint16"), TEXT("BP_PatrolPoint17"), TEXT("BP_PatrolPoint18"), TEXT("BP_PatrolPoint19"), TEXT("BP_PatrolPoint20"),
		TEXT("BP_PatrolPoint21"), TEXT("BP_PatrolPoint22"), TEXT("BP_PatrolPoint23"), TEXT("BP_PatrolPoint24"), TEXT("BP_PatrolPoint25") };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PatrolPointList")
		TArray<class APatrolPoint_cpp*> PatrolPointLists;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PatrolPointList")
		bool bIsCollectMode; // Level1처럼 오브젝트를 모으는 챕터인가

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player")
		class AHorrorGameCharacter* Player;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Controller")
		class ACreatureAI* ReaperController;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Controller")
		int32 UnSealedItemNumber;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dissolve")
		class UMaterialParameterCollection* MPC_Reaper;

	UPROPERTY(VisibleAnywhere, Category = "Dissolve")
		TArray<UMaterialInstanceDynamic*> MaterialInstances;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void PossessedBy(AController* NewController) override;

	virtual void ChangeMaterialInstance(float inValue) override;

	virtual void DissolveFinish() override;

	/*void Move(const FInputActionValue& Value);

	void Look(const FInputActionValue& Value);*/

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
		void SetPlayerWatch(bool value);

	/*void EndChase();*/

	UFUNCTION(BlueprintCallable)
		void SetIsCatch(bool Value);

	UFUNCTION(BlueprintCallable)
		void SetAnimFinish(bool Value);

	UFUNCTION(BlueprintCallable)
	void SoundBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(BlueprintCallable)
	void SoundEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	/*UFUNCTION(BlueprintCallable)
	void DoorBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);*/

	UFUNCTION(BlueprintCallable)
		void CheckBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UFUNCTION(BlueprintCallable)
	FVector GetPatrolPoint();

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
		void SetIsStop(bool inIsStop);

	UFUNCTION(BlueprintCallable)
		bool GetIsStop();

	//UFUNCTION(BlueprintCallable)
	//	void CatchBeginOverlap(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	UFUNCTION(BlueprintCallable)
		void CatchBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(BlueprintCallable)
		bool GetPatrolSuccess();

	UFUNCTION(BlueprintCallable)
		void SetCurrentStatus(int32 Status); // 0: Sealed, 1: OneUnsealed, 2: TwoUnsealed, 3: Unsealed, 4: SealedButChase

private:
	UPROPERTY()
		bool bIsStop = false; // 문을 여는 동작으로 인하여 멈추는지 확인
};