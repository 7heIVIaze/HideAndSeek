// CopyrightNotice=0 2023 Sunggon Kim kimdave205@gmail.com

#include "AI/BruteAnimInstance.h"
#include "AI/Brute_cpp.h"
#include "GameFramework/PawnMovementComponent.h"

void UBruteAnimInstance::NativeInitializeAnimation()
{
	Brute = Cast<ABrute_cpp>(TryGetPawnOwner());
	// Pawn에 해당하는 인스턴스가 있는지 없는지 상관 없이 해당 객체를 가져오고 Brute_cpp로 변환
}

void UBruteAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	if (IsValid(Brute)) // Brute가 NULL인지 체크
	{
		Speed = Brute->GetVelocity().Size(); // Brute의 속도 가져옴
		bIsFalling = Brute->GetMovementComponent()->IsFalling();
		Direction = CalculateDirection(Brute->GetVelocity(), Brute->GetActorRotation());
		bIsChase = Brute->bIsChase;
		bIsCatch = Brute->GetIsCatch();
	}
}

void UBruteAnimInstance::AnimNotify_BruteAttackStart()
{
	UE_LOG(LogTemp, Warning, TEXT("AttackStart Notify Called"));
	if (IsValid(Brute))
	{
		UE_LOG(LogTemp, Warning, TEXT("Start IsValid Called"));
		Brute->SetIsCatch(true);
		bIsCatch = true;
		UE_LOG(LogTemp, Warning, TEXT("IsCatch = %s"), (bIsCatch == true) ? TEXT("true") : TEXT("false"));
	}
}

void UBruteAnimInstance::AnimNotify_BruteAttackEnd()
{
	UE_LOG(LogTemp, Warning, TEXT("AttackEnd Notify Called"));
	if (IsValid(Brute))
	{
		UE_LOG(LogTemp, Warning, TEXT("End IsValid Called"));
		Brute->SetIsCatch(false);
		Brute->SetAnimFinish(true);
		bIsCatch = false;
		UE_LOG(LogTemp, Warning, TEXT("IsCatch = %s"), (bIsCatch == true) ? TEXT("true") : TEXT("false"));
	}
}