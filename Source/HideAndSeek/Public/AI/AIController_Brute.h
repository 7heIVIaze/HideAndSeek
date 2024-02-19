// CopyrightNotice=0 2023 Sunggon Kim kimdave205@gmail.com

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionTypes.h"
#include "AIController_Brute.generated.h"

/**
 * 
 */
UCLASS()
class HIDEANDSEEK_API AAIController_Brute : public AAIController
{
	GENERATED_BODY()
	
public:
	AAIController_Brute();

	void BeginPlay() override;
	void OnPossess(APawn* InPawn) override;

	UFUNCTION(BlueprintCallable)
		void StopAI();

	class UBlackboardComponent* GetBlackboard() const;

	UFUNCTION()
		void SetPerception();

	/*UFUNCTION()
		void UpdatePerception(const TArray<AActor*>& Actors);*/

	UFUNCTION()
		void OnTargetDetected(AActor* Actor, FAIStimulus const Stimulus);

	UFUNCTION()
		void SetStunned(bool value);

	//UFUNCTION()
	//	void EndChase();

public:
	static const FName PatrolTargetKey;
	static const FName NoiseTargetKey;
	static const FName HomePosKey;
	static const FName PatrolPosKey;
	static const FName TargetKey;
	static const FName TargetLocation;
	static const FName CanSeePlayer;
	static const FName NoiseDetected;
	//static const FName SprintDetected;
	static const FName Stunned;
	static const FName LockerLighting;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float AISightRadius = 1200.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float AISightAge = 0.5f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float AILoseSightRadius = 1700.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float AIFieldOfView = 60.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float AILastSeenLocation = 2000.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float NoLightSightRadius = 800.f;

private:
	/*void OnRepeatTimer();

	FTimerHandle RepeatTimerHandle;
	float RepeatInterval;*/

	class UBlackboardComponent* BlackboardComp;
	class UAISenseConfig_Sight* SightConfig;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
		class UBehaviorTreeComponent* Behavior_Tree_Component;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
		class UBehaviorTree* BTAsset;

	UPROPERTY()
		class UBlackboardData* BBAsset;

	UPROPERTY()
		bool bIsStunned;
};
