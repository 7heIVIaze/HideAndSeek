// CopyrightNotice 2023 Sunggon Kim kimdave205@gmail.com. All Rights Reserved.

#include "Player/PlayerStatComponent.h"
#include "Player/HorrorGameCharacter.h"

// Sets default values for this component's properties
UPlayerStatComponent::UPlayerStatComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// Defaults
	PlayerStatus = EPlayerStatus::Loading;
	MaxHP = 200.0f;
	CurrentHP = MaxHP;
	Stamina = 400.0f;
	ConfusionPoint = 0.0f;
	// ...
}


// Called when the game starts
void UPlayerStatComponent::BeginPlay()
{
	Super::BeginPlay();

	// 소유자 설정
	OwnerCharacter = Cast<AHorrorGameCharacter>(GetOwner());
	
}

void UPlayerStatComponent::InitializeComponent()
{
	Super::InitializeComponent();
}


// Called every frame
void UPlayerStatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UPlayerStatComponent::SetPlayerStatus(EPlayerStatus inPlayerStatus)
{
	PlayerStatus = inPlayerStatus;
}

float UPlayerStatComponent::AddConfusionPoint(float inValue)
{
	ConfusionPoint += inValue;

	ConfusionPoint = FMath::Clamp(ConfusionPoint, 0.0f, 100.0f);

	if (ConfusionPoint == 100.0f)
	{
		OnPanicStart.Broadcast();
	}

	OnPanicChanged.Broadcast(ConfusionPoint);

	return ConfusionPoint;
}

void UPlayerStatComponent::TakeDamage(float DamageAmount)
{
	UE_LOG(LogTemp, Warning, TEXT("Take Damage Called"));
	// If the player is already dead or the damage is 0, escape.
	if (DamageAmount <= 0.0f || IsDead())
	{
		return;
	}

	CurrentHP = FMath::Clamp(CurrentHP - DamageAmount, 0.0f, MaxHP);
	UE_LOG(LogTemp, Warning, TEXT("Take Damage CurrentHP: %f"), CurrentHP);

	OnHealthPointChanged.Broadcast(CurrentHP);

	// If the player is dead, then notice the game is over.
	if (IsDead())
	{
		OnDeath.Broadcast();
	}
}

void UPlayerStatComponent::Heal(float HealAmount)
{
	// If the player is already dead or the heal amount is 0, escape.
	if (HealAmount <= 0.0f || IsDead())
	{
		return;
	}

	CurrentHP = FMath::Clamp(CurrentHP + HealAmount, 0.0f, MaxHP);

	OnHealthPointChanged.Broadcast(CurrentHP);
}

bool UPlayerStatComponent::IsDead() const
{
	return CurrentHP <= 0.0f;
}

void UPlayerStatComponent::SetCurrentPlayerStates(EPlayerStatus inPlayerStatus)
{
	PlayerStatus = inPlayerStatus;
}

EPlayerStatus UPlayerStatComponent::GetCurrentPlayerStates() const
{
	return PlayerStatus;
}

void UPlayerStatComponent::DecreaseStamina()
{
	// 스태미너가 모두 소모된 상태가 아니라면
	if (Stamina > 0)
	{
		// 스태미너를 2씩 감소시키고, 위젯에 스태미너 값을 업데이트 함.
		Stamina -= 2;
		
		OnStaminaChanged.Broadcast(Stamina);
	}
	// 스태미너가 모두 소모된 상태인 경우
	else
	{
		// 스태미너가 전부 소모되었다고 알림.
		OnStaminaOver.Broadcast();
	}
}

void UPlayerStatComponent::IncreaseStamina()
{
	// 스태미너가 소모된 상태라면
	if (Stamina <= 399)
	{
		// 스태미너를 1 씩 회복하고, 위젯에 스태미너 값을 업데이트 해줌.
		Stamina += 1;
		
		OnStaminaChanged.Broadcast(Stamina);
	}
}