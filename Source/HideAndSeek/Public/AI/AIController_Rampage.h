// CopyrightNotice=0 2023 Sunggon Kim kimdave205@gmail.com

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionTypes.h"
#include "AIController_Rampage.generated.h"

/**
 * 
 */
UCLASS()
class HIDEANDSEEK_API AAIController_Rampage : public AAIController
{
	GENERATED_BODY()

public:
	AAIController_Rampage();

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

	UFUNCTION()
		void SetStunned(bool value);

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
	static const FName IsCinematic;
	static const FName CalledByBerith;

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


private:
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
