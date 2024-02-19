// CopyrightNotice=0 2023 Sunggon Kim kimdave205@gmail.com

#include "AI/ReaperAnim.h"
#include "AI/Reaper_cpp.h"

void UReaperAnim::NativeInitializeAnimation()
{
	Reaper = Cast<AReaper_cpp>(TryGetPawnOwner());
}

void UReaperAnim::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (IsValid(Reaper))
	{
		Speed = Reaper->GetVelocity().Size();
		Direction = CalculateDirection(Reaper->GetVelocity(), Reaper->GetActorRotation());
		IsCatch = Reaper->GetIsCatch();
		bIsStunned = Reaper->GetIsStunned();
	}

}

void UReaperAnim::AnimNotify_ReaperAttackStart()
{
	UE_LOG(LogTemp, Warning, TEXT("AttackStart Notify Called"));
	if (IsValid(Reaper))
	{
		UE_LOG(LogTemp, Warning, TEXT("Start IsValid Called"));
		Reaper->SetIsCatch(true);
		IsCatch = true;
		UE_LOG(LogTemp, Warning, TEXT("IsCatch = %s"), (IsCatch == true) ? TEXT("true") : TEXT("false"));
	}
}

void UReaperAnim::AnimNotify_ReaperAttackEnd()
{
	UE_LOG(LogTemp, Warning, TEXT("AttackEnd Notify Called"));
	if (IsValid(Reaper))
	{
		UE_LOG(LogTemp, Warning, TEXT("End IsValid Called"));
		Reaper->SetIsCatch(false);
		Reaper->SetAnimFinish(true);
		IsCatch = false;
		UE_LOG(LogTemp, Warning, TEXT("IsCatch = %s"), (IsCatch == true) ? TEXT("true") : TEXT("false"));
	}
}