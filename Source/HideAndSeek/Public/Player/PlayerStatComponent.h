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

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHealthChanged, float, CurrentHealthPoint);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDeath);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnStaminaChanged, int32, CurrentStaminaPoint);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnStaminaOver);

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

	// 플레이어의 상태 설정.
	UFUNCTION(BlueprintCallable)
	void SetPlayerStatus(EPlayerStatus inPlayerStatus);

	// 착란 게이지 변경. inValue is plus or minus.
	UFUNCTION(BlueprintCallable)
	float AddConfusionPoint(float inValue);

	// 데미지를 입었을 때 HP 변화.
	UFUNCTION(BlueprintCallable)
	void TakeDamage(float DamageAmount);

	// 힐을 했을 때의 HP 변화.
	UFUNCTION(BlueprintCallable)
	void Heal(float HealAmount);

	// 플레이어 사망.
	UFUNCTION(BlueprintCallable)
	bool IsDead() const;

	UFUNCTION(BlueprintCallable)
	void SetCurrentPlayerStates(EPlayerStatus inPlayerStatus);

	UFUNCTION(BlueprintCallable)
	EPlayerStatus GetCurrentPlayerStates() const;

	UFUNCTION(BlueprintCallable)
	void DecreaseStamina();

	UFUNCTION(BlueprintCallable)
	void IncreaseStamina();

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Status")
	EPlayerStatus PlayerStatus;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Player)
	TObjectPtr<class AHorrorGameCharacter> OwnerCharacter;

	// 스태미너
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Sprint", meta = (AllowPrivateAccess = "true"))
	int32 Stamina;

	// 플레이어의 현재 HP. 0 ~ 200의 범위.
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = HP)
	float CurrentHP;

	// 플레이어의 최대 HP. 200임.
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = HP)
	float MaxHP;

	// 플레이어의 착란 게이지. 0~100의 범위이며, 100까지 쌓을 경우 2초간 행동 불능 상태가 됨.
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Stress)
	float ConfusionPoint;

	// HP Change Event
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Event)
	FOnHealthChanged OnHealthPointChanged;

	// Death Event
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Event)
	FOnDeath OnDeath;

	// 스태미너 변화 이벤트
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Event)
	FOnStaminaChanged OnStaminaChanged;

	// 스테미너 끝난 이벤트
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Event)
	FOnStaminaOver OnStaminaOver;

};
