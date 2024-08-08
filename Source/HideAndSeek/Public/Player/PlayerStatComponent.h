// CopyrightNotice 2023 Sunggon Kim kimdave205@gmail.com. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlayerStatComponent.generated.h"

UENUM(BlueprintType)
enum class EPlayerStatus : uint8
{
	Loading UMETA(DisplayName = "Loading"),
	Survive UMETA(DisplayName = "Survive"),
	Chased UMETA(DisplayName = "Chased"),
	Catched UMETA(DisplayName = "Catched"),
	Stunned UMETA(DisplayName = "Stunned"),
	Hiding UMETA(DisplayName = "Hiding"), // 이 상태는 숨은 상태를 나타내는 것이 아닌, 숨은 오브젝트에 넣을 상태임.
	Died UMETA(DisplayName = "Died"),
	Clear UMETA(DisplayName = "Clear"),
	Ending UMETA(DisplayName = "Ending"),
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class HIDEANDSEEK_API UPlayerStatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPlayerStatComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	virtual void InitializeComponent() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	void SetPlayerStatus(EPlayerStatus inPlayerStatus);

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Status")
	EPlayerStatus PlayerStatus;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Player)
	TObjectPtr<class AHorrorGameCharacter> OwnerCharacter;
};
