// CopyrightNotice 2023 Sunggon Kim kimdave205@gmail.com. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AI/CreatureClass.h"
#include "Reaper_cpp.generated.h"

UCLASS()
class HIDEANDSEEK_API AReaper_cpp : public ACreatureClass // ACharacter
{
	GENERATED_BODY()

	class UAnimSequence* Anim;

public:
	// Sets default values for this character's properties
	AReaper_cpp();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Controller")
		class AAIController_Reaper* ReaperController;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Controller")
		int32 UnSealedItemNumber;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dissolve")
		class UMaterialParameterCollection* MPC_Reaper;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void PossessedBy(class AController* NewController) override;

	virtual void ChangeMaterialInstance(float inValue) override;

	virtual void DissolveFinish() override;

public:
	// 플레이어를 보고 있는지 여부
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool bIsPlayerWatch = false;

	// 플레이어를 행동 불능시키는 능력의 쿨타임이 돌고 있는지 여부
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool bIsCooldown = false;

	// 능력의 캐스팅 시간
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float CastingTime = 0.0f;

	// 능력의 쿨타임
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float SkillCooldown = 0.0f;

	// 프롤로그를 위한 변수임
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool bSealedButChase = false;

	// 맵 이름을 감지하기 위한 변수
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	FString MapName;

public:
	UFUNCTION(BlueprintCallable)
	virtual	void SetPlayerWatch(bool value);

	virtual void Exorcism() override;
	
	virtual void CatchBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	UFUNCTION(BlueprintCallable)
	virtual void SetCurrentStatus(int32 Status); // 0: Sealed, 1: OneUnsealed, 2: TwoUnsealed, 3: Unsealed, 4: SealedButChase

};