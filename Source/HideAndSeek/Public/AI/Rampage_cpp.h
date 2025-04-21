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
	ARampage_cpp();

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Cinematic", meta = (AllowPrivateAccess = "true"))
	bool bIsCinematic = true;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Controller")
		class AAIController_Rampage* RampageController;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void PossessedBy(AController* NewController) override;

public:
	// 베리스 호출로 불렸는지
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool IsBerith = false;

	virtual	void EndChase() override;

	UFUNCTION(BlueprintCallable)
	void CalledByBerith();

	virtual	void SetAnimFinish(bool Value) override;

	UFUNCTION(BlueprintCallable)
	void SetIsCinematic(bool inIsCinematic);

	virtual void  CatchBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherIndexBody, bool bFromSweep, const FHitResult& SweepResult) override;

};
