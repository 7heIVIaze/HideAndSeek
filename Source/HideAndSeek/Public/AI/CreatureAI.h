// CopyrightNotice=0 2023 Sunggon Kim

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionTypes.h"
#include "CreatureAI.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class Sealed : uint8
{
	Sealed UMETA(DisplayName = "Sealed"),
	OneUnsealed UMETA(DisplayName = "One_Unsealed"),
	TwoUnsealed UMETA(DisplayName = "Two_Unsealed"),
	Unsealed UMETA(DisplayName = "All_Unsealed"),
	SealedButChase UMETA(DisplayName = "SealedButChase"), // 프롤로그 용 상태
};
UCLASS()
class HIDEANDSEEK_API ACreatureAI : public AAIController
{
	GENERATED_BODY()

public:
	ACreatureAI();

	void BeginPlay() override;
	//virtual void Tick(float DeltaSeconds) override;
	void OnPossess(APawn* InPawn) override;
	
	UFUNCTION(BlueprintCallable)
		void StopAI();
		// virtual void OnUnPossess() override;
	UFUNCTION(BlueprintCallable)
	class UBlackboardComponent* GetBlackboard() const;

	/*UFUNCTION(BlueprintCallable)
		class UBehaviorTreeComponent* GetBlackboard() const;*/

	UFUNCTION()
		void SetPerception();

	UFUNCTION()
		void OnTargetDetected(AActor* Actor, FAIStimulus const Stimulus);

	//UFUNCTION(BlueprintCallable)
	//void DetectedBySight(AActor* Actor, FAIStimulus const Stimulus);

	//UFUNCTION(BlueprintCallable)
	//void DetectedByHearing(AActor* Actor, FAIStimulus const Stimulus);

	UFUNCTION()
		void SetStunned(bool value);

	UFUNCTION()
		Sealed GetCurrentSealStatus();

	UFUNCTION()
		void SetCurrentSealStatus(Sealed Status);
public:
	static const FName PatrolTargetKey;
	static const FName NoiseTargetKey;
	static const FName HomePosKey;
	static const FName PatrolPosKey;
	static const FName TargetKey;
	static const FName TargetLocation;
	static const FName CanSeePlayer;
	static const FName NoiseDetected;
	static const FName Stunned;
	static const FName LockerLighting;
	static const FName SealStatus;
	static const FName LockerTargetKey;

	// AI Perception Variables
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float AISightRadius = 1600.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float AISightAge = 0.5f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float AILoseSightRadius = 1700.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float AIFieldOfView = 90.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float AILastSeenLocation = 1200.0f;
	/*UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float AIHearingRange = 1600.0f;*/


private:
	FTimerHandle RepeatTimerHandle;
	float RepeatInterval;

	class UBlackboardComponent* BlackboardComp;
	class UAISenseConfig_Sight* SightConfig;
	//class UAISenseConfig_Hearing* HearingConfig;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
		class UBehaviorTreeComponent* Behavior_Tree_Component;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
		class UBehaviorTree* BTAsset;

	UPROPERTY()
		class UBlackboardData* BBAsset;

	UPROPERTY()
		Sealed CurrentStatus;

	UPROPERTY()
		bool bIsStunned;
};
