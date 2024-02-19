// CopyrightNotice=0 2023 Sunggon Kim kimdave205@gmail.com

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Records.generated.h"

USTRUCT(Atomic, BlueprintType)
struct FClearData
{
	GENERATED_BODY()

public:
	UPROPERTY()
		bool bIsCleared; // 클리어했는지 여부 확인

	UPROPERTY()
		FString ClearTime; // 클리어 타임
};

UCLASS()
class HIDEANDSEEK_API ARecords : public AActor
{
	GENERATED_BODY()
};
