// CopyrightNotice 2023 Sunggon Kim kimdave205@gmail.com. All Rights Reserved.

/***
* 브루트의 애니메이션과 관련된 동작을 수행할 코드
***/

#include "AI/BruteAnimInstance.h"
#include "AI/Brute_cpp.h"
#include "GameFramework/PawnMovementComponent.h"

// BeginPlay의 애니메이션 버전
void UBruteAnimInstance::NativeInitializeAnimation()
{
	// Pawn에 해당하는 소유자 인스턴스가 있는지 없는지 상관 없이 해당 객체를 가져오고 Brute_cpp로 변환
	Brute = Cast<ABrute_cpp>(TryGetPawnOwner());

}

// Tick의 애니메이션 버전
void UBruteAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	if (IsValid(Brute)) // Brute가 NULL인지 체크
	{
		// Brute의 속도와 방향을 가져옴. 그리고 낙하 중인지 체크함
		Speed = Brute->GetVelocity().Size();
		bIsFalling = Brute->GetMovementComponent()->IsFalling();
		Direction = CalculateDirection(Brute->GetVelocity(), Brute->GetActorRotation()); // 방향도
		
		// Brute의 상태 역시 가져옴
		bIsChase = Brute->bIsChase;
		bIsCatch = Brute->GetIsCatch();
		bIsStunned = Brute->GetIsStunned();
		bIsRangeChange = Brute->bIsRangeChange;
		bIsCalledRangeChange = Brute->bCalledRangeChange;
	}
}

// 공격 모션이 시작되었다는 것을 알릴 Delegate 함수
void UBruteAnimInstance::AnimNotify_BruteAttackStart()
{
	UE_LOG(LogTemp, Warning, TEXT("AttackStart Notify Called"));
	if (IsValid(Brute))
	{
		// 공격을 하면 Catch 상태를 true로 바꿈.
		UE_LOG(LogTemp, Warning, TEXT("Start IsValid Called"));
		Brute->SetIsCatch(true);
		bIsCatch = true;
		UE_LOG(LogTemp, Warning, TEXT("IsCatch = %s"), (bIsCatch == true) ? TEXT("true") : TEXT("false"));
	}
}

// 공격 모션이 끝났다는 것을 알릴 Delegate 함수
void UBruteAnimInstance::AnimNotify_BruteAttackEnd()
{
	UE_LOG(LogTemp, Warning, TEXT("AttackEnd Notify Called"));
	if (IsValid(Brute))
	{
		UE_LOG(LogTemp, Warning, TEXT("End IsValid Called"));
		// Catch 상태를 false로 설정하고 애니메이션을 소유한 폰(Brute)에게 애니메이션이 끝났다는 것을 알림.
		Brute->SetIsCatch(false);
		Brute->SetAnimFinish(true);
		bIsCatch = false;
		UE_LOG(LogTemp, Warning, TEXT("IsCatch = %s"), (bIsCatch == true) ? TEXT("true") : TEXT("false"));
	}
}

// 브루트의 청각 범위가 변경이 될 때 재생할 애니메이션이 시작되었다는 것을 알리는 코드
void UBruteAnimInstance::AnimNotify_BruteChangeRangeStart()
{
	UE_LOG(LogTemp, Warning, TEXT("Change Range Notify Called"));
	if (IsValid(Brute))
	{
		UE_LOG(LogTemp, Warning, TEXT("Start IsValid Called"));
		//Brute->SetIsCatch(true);
		//bIsCatch = true;
		//UE_LOG(LogTemp, Warning, TEXT("IsCatch = %s"), (bIsCatch == true) ? TEXT("true") : TEXT("false"));
	}
}

// 브루트의 청각 범위가 변경이 될 때 재생할 애니메이션이 끝났다는 것을 알리는 코드
void UBruteAnimInstance::AnimNotify_BruteChangeRangeEnd()
{
	UE_LOG(LogTemp, Warning, TEXT("Change Range Notify Called"));
	if (IsValid(Brute))
	{
		UE_LOG(LogTemp, Warning, TEXT("End IsValid Called"));
	
		// 추격 중이 아니면, Brute의 범위 증가 애니메이션이 종료되었다는 것을 알리고 경계모드로 들어감을 알림
		if (!bIsChase)
		{
			bIsRangeChange = true;
			Brute->BroadCastChangeNoiseRange(true);
		}
		// 추격 중이라면, 아무것도 하지 않음.
		else
		{
			bIsRangeChange = false;
			Brute->ChangeNoiseRange(false);
		}
		
	}
}