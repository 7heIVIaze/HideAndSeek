// CopyrightNotice=0 2023 Sunggon Kim kimdave205@gmail.com

#include "PlayerAnim.h"
//#include "Sound/SoundCue.h"
#include "HorrorGame/HorrorGameCharacter.h"
#include "Items/PlayerSword_cpp.h"
//#include "Kismet/GameplayStatics.h"

UPlayerAnim::UPlayerAnim()
{
	static ConstructorHelpers::FObjectFinder<UAnimMontage> ATTACK_MONTAGE(TEXT("/Game/Characters/Mannequins/Animations/Attack"));
	if (ATTACK_MONTAGE.Succeeded())
	{
		AttackMontage = ATTACK_MONTAGE.Object;
	}
	
	/*static ConstructorHelpers::FObjectFinder<USoundCue> BREATHE_SOUNDCUE(TEXT("/Game/Assets/Sounds/SoundCues/EndSprintBreathe_Cue"));
	if (BREATHE_SOUNDCUE.Succeeded())
	{
		BreatheSoundCue = BREATHE_SOUNDCUE.Object;
	}*/
}

void UPlayerAnim::NativeInitializeAnimation()
{
	Player = Cast<AHorrorGameCharacter>(TryGetPawnOwner());
	// bCheckSprinting = true;
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
		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Green, FString::Printf(TEXT("Current Anim Should Attack: %s"), bShouldAttack ? TEXT("true") : TEXT("false")));
		Player->AttackCheck(bShouldAttack);
	}
}

void UPlayerAnim::AnimNotify_SwordSlashEnd()
{
	if (IsValid(Player))
	{
		bShouldAttack = false;
		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Green, FString::Printf(TEXT("Current Anim Should Attack: %s"), bShouldAttack ? TEXT("true") : TEXT("false")));
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

//void UPlayerAnim::AnimNotify_SprintStart()
//{
//	// bIsSprinting = Player->GetIsSprinting();
//	bCheckIsSprinting = Player->GetIsSprinting();
//}
//
//void UPlayerAnim::AnimNotify_SprintEnd()
//{
//	// bIsSprinting = Player->GetIsSprinting();
//}
//
//void UPlayerAnim::AnimNotify_WhenSprintEndBegin()
//{
//	if (bCheckIsSprinting != bIsSprinting)
//	{
//		if (BreatheSoundCue)
//		{
//			UGameplayStatics::PlaySound2D(Player, BreatheSoundCue);
//		}
//	}
//	// bCheckIsSprinting = Player->GetIsSprinting();
//}
//
//void UPlayerAnim::AnimNotify_WhenSprintEndFinish()
//{
//	bCheckIsSprinting = Player->GetIsSprinting();
//}