// CopyrightNotice 2023 Sunggon Kim kimdave205@gmail.com. All Rights Reserved.

#include "AnimNotify_FootStepSound.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Animation/AnimSequenceBase.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Sound/SoundCue.h"
#include "PhysicalMaterials/PhysicalMaterial.h"

#include "FootStepSoundList.h"

UAnimNotify_FootStepSound::UAnimNotify_FootStepSound() : Super()
{
	CheckDistance = 10.0f;
	SoundAttach = false;
	ShowDebugLine = false;

#if WITH_EDITORONLY_DATA
	NotifyColor = FColor(192, 255, 99, 255);
#endif
}

void UAnimNotify_FootStepSound::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	FootStep(MeshComp);
}

void UAnimNotify_FootStepSound::FootStep(USkeletalMeshComponent* MeshComp)
{
	if (SoundList == nullptr) return;
	if (PMB_SoundList == nullptr) PMB_SoundList = SoundList->GetDefaultObject<UFootStepSoundList>();
	if (PMB_SoundList == nullptr) return;

	const FTransform MeshTransform = MeshComp->GetSocketTransform(SocketName);

	FVector LineTraceStart = MeshTransform.GetLocation() + LocationOffset;
	FVector LineTraceEnd = MeshTransform.GetLocation() - FVector::UpVector * CheckDistance;

	FHitResult HitResult;
	TArray<AActor*> Ignore;
	Ignore.Add(MeshComp->GetOwner());

	bool bDebug = false;
	bDebug = ShowDebugLine;

	EDrawDebugTrace::Type eDebug = EDrawDebugTrace::None;
	if (bDebug) eDebug = EDrawDebugTrace::ForDuration;

	bool bResult = UKismetSystemLibrary::LineTraceSingle(MeshComp->GetWorld(), LineTraceStart, LineTraceEnd, UEngineTypes::ConvertToTraceType(ECC_WorldStatic), true, Ignore, eDebug, HitResult, true);

	if (bResult)
	{
		if (HitResult.GetActor() == nullptr) return;

		USoundCue* Sound = PMB_SoundList->GetSound(HitResult.PhysMaterial->GetName());
		if (Sound == nullptr) return;

		if (Sound->IsLooping())
		{
			return;
		}

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