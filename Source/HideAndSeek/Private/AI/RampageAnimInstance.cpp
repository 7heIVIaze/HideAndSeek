// CopyrightNotice 2023 Sunggon Kim kimdave205@gmail.com. All Rights Reserved.

#include "AI/RampageAnimInstance.h"
#include "AI/Rampage_cpp.h"
#include "GameFramework/PawnMovementComponent.h"

void URampageAnimInstance::NativeInitializeAnimation()
{
	Rampage = Cast<ARampage_cpp>(TryGetPawnOwner());
	// Pawn에 해당하는 인스턴스가 있는지 없는지 상관 없이 해당 객체를 가져와 Rampage_cpp로 변환
}

void URampageAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	// auto Pawn = TryGetPawnOwner();
	if (IsValid(Rampage)) // Rampage가 NULL인지 체크
	{
		Speed = Rampage->GetVelocity().Size(); // Rampage의 속도 가져옴
		bIsFalling = Rampage->GetMovementComponent()->IsFalling();
		Direction = CalculateDirection(Rampage->GetVelocity(), Rampage->GetActorRotation());
		bIsChase = Rampage->bIsChase;
		bIsCatch = Rampage->GetIsCatch();
		bIsStunned = Rampage->GetIsStunned();
	}
}

void URampageAnimInstance::AnimNotify_RampageAttackStart()
{
	UE_LOG(LogTemp, Warning, TEXT("AttackStart Notify Called"));
	if (IsValid(Rampage))
	{
		UE_LOG(LogTemp, Warning, TEXT("Start IsValid Called"));
		Rampage->SetIsCatch(true);
		bIsCatch = true;
		UE_LOG(LogTemp, Warning, TEXT("IsCatch = %s"), (bIsCatch == true) ? TEXT("true") : TEXT("false"));
	}
}

void URampageAnimInstance::AnimNotify_RampageAttackEnd()
{
	UE_LOG(LogTemp, Warning, TEXT("AttackEnd Notify Called"));
	if (IsValid(Rampage))
	{
		UE_LOG(LogTemp, Warning, TEXT("End IsValid Called"));
		Rampage->SetIsCatch(false);
		Rampage->SetAnimFinish(true);
		bIsCatch = false;
		UE_LOG(LogTemp, Warning, TEXT("IsCatch = %s"), (bIsCatch == true) ? TEXT("true") : TEXT("false"));
	}
}

