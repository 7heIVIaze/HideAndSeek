// CopyrightNotice 2023 Sunggon Kim kimdave205@gmail.com. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ItemManager.generated.h"

//DECLARE_MULTICAST_DELEGATE_OneParam(FItemProbabilityDelegate, int32, SpawnedItemProbability);
UCLASS()
class HIDEANDSEEK_API AItemManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItemManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	// virtual void Tick(float DeltaTime) override;

	//FItemProbabilityDelegate ItemProbabilityDelegate;

	bool bIsBinded = false;

	UFUNCTION(BlueprintCallable, Category = "ItemProbability")
		void BindItemSetting(class ALevelManager* LevelManager);

	UFUNCTION(BlueprintCallable, Category = "ItemProbability")
		void ItemSetting();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
		TArray<TSubclassOf<class AItems>> Items; // 배치될 아이템들(블루프린트에서 담을 것)

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
		TArray<int32> ItemNumbers; // 배치될 아이템의 개수(Items와 동일한 인덱스임)

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemBoxes")
		TArray<AActor*> Objects; // 레벨에 배치해놓을 거
};
