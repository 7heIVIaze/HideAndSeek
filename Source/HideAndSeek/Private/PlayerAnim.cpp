// CopyrightNotice=0 2023 Sunggon Kim kimdave205@gmail.com

#include "PlayerAnim.h"
//#include "Sound/SoundCue.h"
#include "HideAndSeek/HorrorGameCharacter.h"
#include "Items/PlayerSword_cpp.h"
//#include "Kismet/GameplayStatics.h"

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

void UPlayerAnim::AnimNotify_SwordSlashStart()
{
	if (IsValid(Player))
	{
		bShouldAttack = true;
		Player->AttackCheck(bShouldAttack);
	}
}

void UPlayerAnim::AnimNotify_SwordSlashEnd()
{
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