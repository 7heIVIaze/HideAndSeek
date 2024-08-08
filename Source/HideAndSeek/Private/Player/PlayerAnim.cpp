// CopyrightNotice 2023 Sunggon Kim kimdave205@gmail.com. All Rights Reserved.

#include "Player/PlayerAnim.h"
//#include "Sound/SoundCue.h"
#include "Player/HorrorGameCharacter.h"
#include "Items/PlayerSword_cpp.h"
//#include "Kismet/GameplayStatics.h"

// 플레이어 애니메이션 생성자.
UPlayerAnim::UPlayerAnim()
{
	static ConstructorHelpers::FObjectFinder<UAnimMontage> ATTACK_MONTAGE(TEXT("/Game/Characters/Mannequins/Animations/Attack"));
	if (ATTACK_MONTAGE.Succeeded())
	{
		AttackMontage = ATTACK_MONTAGE.Object;
	}
}

void UPlayerAnim::NativeInitializeAnimation()
{
	// 플레이어 정보를 가져옴.
	Player = Cast<AHorrorGameCharacter>(TryGetPawnOwner());
}

void UPlayerAnim::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (IsValid(Player))
	{
		Speed = Player->GetVelocity().Size();
		Direction = CalculateDirection(Player->GetVelocity(), Player->GetActorRotation());
		bShouldAttack = Player->GetNotifyAttackStart(); // 공격 모션을 재생했는지 확인.
		bIsSprinting = Player->GetIsSprinting();
		bIsCrouching = Player->bIsCrouch;
		// 적을 맞췄는지는 칼이 알아서 체크함
	}
}

// 청동 검을 사용할 때 모션이 시작 시 작동할 함수.
void UPlayerAnim::AnimNotify_SwordSlashStart()
{
	// 플레이어의 공격이 시작되었다고 알리고, 검의 충돌체 설정을 켜도록 함.
	if (IsValid(Player))
	{
		bShouldAttack = true;
		Player->AttackCheck(bShouldAttack);
	}
}

// 청동 검을 사용할 때 모션이 끝날 때 작동할 함수.
void UPlayerAnim::AnimNotify_SwordSlashEnd()
{
	// 플레이어의 공격이 끝났다고 알리고, 검의 충돌체 설정을 끄도록 함.
	if (IsValid(Player))
	{
		bShouldAttack = false;
		Player->AttackCheck(bShouldAttack);
	}
}

void UPlayerAnim::PlayAttackMontage()
{
	if (!Montage_IsPlaying(AttackMontage))
	{
		Montage_Play(AttackMontage, 1.0f);
	}
}