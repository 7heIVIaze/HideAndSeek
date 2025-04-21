// CopyrightNotice 2023 Sunggon Kim kimdave205@gmail.com. All Rights Reserved.

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
	// Constructor
	AShadow_cpp();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void PossessedBy(AController* NewController) override;

	// 청동 검으로 사망 시 Dissolve Material Effect를 구현하기 위한 함수
	virtual void ChangeMaterialInstance(float inValue) override;
	
	virtual void DissolveFinish() override;

};
