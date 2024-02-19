// CopyrightNotice=0 2023 Sunggon Kim kimdave205@gmail.com

#include "AI/RunnerAnimInstance.h"
#include "AI/Runner_cpp.h"
#include "GameFramework/PawnMovementComponent.h"

void URunnerAnimInstance::NativeInitializeAnimation()
{
	Runner = Cast<ARunner_cpp>(TryGetPawnOwner());
	// Pawn에 해당하는 인스턴스가 있는지 없는지 상관 없이 해당 객체를 가져와 Runner_cpp로 변환
}

void URunnerAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	// auto Pawn = TryGetPawnOwner();
	if (IsValid(Runner)) // Runner가 NULL인지 체크
	{
		Speed = Runner->GetVelocity().Size(); // Runner의 속도 가져옴
		bIsFalling = Runner->GetMovementComponent()->IsFalling();
		Direction = CalculateDirection(Runner->GetVelocity(), Runner->GetActorRotation());
		bIsChase = Runner->bIsChase;
		bIsCatch = Runner->GetIsCatch();
		bIsStunned = Runner->GetIsStunned();
	}
}

void URunnerAnimInstance::AnimNotify_RunnerAttackStart()
{
	UE_LOG(LogTemp, Warning, TEXT("AttackStart Notify Called"));
	if (IsValid(Runner))
	{
		UE_LOG(LogTemp, Warning, TEXT("Start IsValid Called"));
		Runner->SetIsCatch(true);
		bIsCatch = true;
		UE_LOG(LogTemp, Warning, TEXT("IsCatch = %s"), (bIsCatch == true) ? TEXT("true") : TEXT("false"));
	}
}

void URunnerAnimInstance::AnimNotify_RunnerAttackEnd()
{
	UE_LOG(LogTemp, Warning, TEXT("AttackEnd Notify Called"));
	if (IsValid(Runner))
	{
		UE_LOG(LogTemp, Warning, TEXT("End IsValid Called"));
		Runner->SetIsCatch(false);
		Runner->SetAnimFinish(true);
		bIsCatch = false;
		UE_LOG(LogTemp, Warning, TEXT("IsCatch = %s"), (bIsCatch == true) ? TEXT("true") : TEXT("false"));
	}
}