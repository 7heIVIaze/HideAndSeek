// CopyrightNotice 2023 Sunggon Kim kimdave205@gmail.com. All Rights Reserved.

#include "AnimNotify_FootStepSound.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Animation/AnimSequenceBase.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Sound/SoundCue.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "FootStepSoundList.h"

// 생성자
UAnimNotify_FootStepSound::UAnimNotify_FootStepSound() : Super()
{
	// 플레이어의 소켓을 기준으로 바닥까지의 거리는 10 정도. 디버깅 라인은 false로 설정함.
	CheckDistance = 10.0f;
	SoundAttach = false;
	ShowDebugLine = false;

#if WITH_EDITORONLY_DATA
	NotifyColor = FColor(192, 255, 99, 255);
#endif
}

// 노티파이 함수.
void UAnimNotify_FootStepSound::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	FootStep(MeshComp);
}

// 발소리를 낼 함수.
void UAnimNotify_FootStepSound::FootStep(USkeletalMeshComponent* MeshComp)
{
	// 소리 리스트가 없다면 리턴함.
	if (SoundList == nullptr)
	{
		return;
	}
	
	// 피지컬 머티리얼 기반 발소리가 없다면 소리 리스트로부터 가져옴,
	if (PMB_SoundList == nullptr)
	{
		PMB_SoundList = SoundList->GetDefaultObject<UFootStepSoundList>();
	}

	// 그럼에도 없다면 리턴함.
	if (PMB_SoundList == nullptr)
	{
		return;
	}

	// 플레이어의 위치는 소켓에서 가져옴.
	const FTransform MeshTransform = MeshComp->GetSocketTransform(SocketName);

	// 레이 캐스팅을 위한 준비
	FVector LineTraceStart = MeshTransform.GetLocation() + LocationOffset;
	FVector LineTraceEnd = MeshTransform.GetLocation() - FVector::UpVector * CheckDistance;

	FHitResult HitResult;
	TArray<AActor*> Ignore;
	Ignore.Add(MeshComp->GetOwner());

	bool bDebug = false;
	bDebug = ShowDebugLine;

	EDrawDebugTrace::Type eDebug = EDrawDebugTrace::None;
	if (bDebug) 
	{
		eDebug = EDrawDebugTrace::ForDuration;
	}

	// 플레이어의 소켓을 기준으로 아래로 10만큼 레이 캐스팅을 시작함.
	bool bResult = UKismetSystemLibrary::LineTraceSingle(MeshComp->GetWorld(), LineTraceStart, LineTraceEnd, UEngineTypes::ConvertToTraceType(ECC_WorldStatic), true, Ignore, eDebug, HitResult, true);

	// 충돌된 것이 존재할 경우
	if (bResult)
	{
		// 만약 충돌된 액터가 없다면 리턴함.
		if (HitResult.GetActor() == nullptr)
		{
			return;
		}

		// 피지컬 머티리얼을 기반으로한 발소리 사운드를 가져옴.
		USoundCue* Sound = PMB_SoundList->GetSound(HitResult.PhysMaterial->GetName());
		// 만약 그것이 없다면 리턴함.
		if (Sound == nullptr)
		{
			return;
		}

		// 사운드가 무한 루프될 수 있어도 리턴함.(우리는 단발 사운드를 원함.)
		if (Sound->IsLooping())
		{
			return;
		}

		// 발소리를 냄.
		if (SoundAttach)
		{
			UGameplayStatics::SpawnSoundAttached(Sound, MeshComp, SocketName, FVector(ForceInit), EAttachLocation::SnapToTarget, false, VolumeMultiplier, PitchMultiplier);
		}
		else
		{
			UGameplayStatics::PlaySoundAtLocation(MeshComp->GetWorld(), Sound, MeshComp->GetComponentLocation(), VolumeMultiplier, PitchMultiplier);
		}
	}
}