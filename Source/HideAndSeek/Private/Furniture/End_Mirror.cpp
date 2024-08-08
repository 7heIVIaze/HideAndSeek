// CopyrightNotice 2023 Sunggon Kim kimdave205@gmail.com. All Rights Reserved.

#include "Furniture/End_Mirror.h"
#include "Player/HorrorGameCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Player/HorrorGamePlayerController.h"
#include "LevelManager/HorrorGameGameMode.h"
#include "LevelManager/HorrorGameGameInstance.h"
#include "HUD/TimerWidget.h"
#include "Sound/SoundCue.h"
#include "HUD/GameUI.h"

// Sets default values
AEnd_Mirror::AEnd_Mirror()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// 메시들의 기본 설정을 해줌. (세세한 설정은 블루프린트 클래스에서 수행)
	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = RootComp;

	MirrorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("EndMirror"));
	MirrorMesh->SetupAttachment(RootComp);

	CommonTeleportPosition = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CommonTeleportPosition"));
	CommonTeleportPosition->SetupAttachment(RootComp);
	
	HiddenTeleportPosition = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HiddenTeleportPosition"));
	HiddenTeleportPosition->SetupAttachment(RootComp);
}

// Called when the game starts or when spawned
void AEnd_Mirror::BeginPlay()
{
	Super::BeginPlay();

	bIsCleared = false;
}

// 플레이어가 상호작용할 때 작동할 함수.
void AEnd_Mirror::OnInteract(class AHorrorGameCharacter* Player)
{
	// 게임 클리어 시
	if (bIsCleared)
	{
		// 순간이동 소리가 있으면 재생시킴
		if (InteractSound != nullptr)
		{
			UGameplayStatics::PlaySound2D(this, InteractSound);
		}
		// 기본 BGM을 제거함.
		Cast<AHorrorGameGameMode>(UGameplayStatics::GetGameMode(GetWorld()))->StopBackGroundMusic();

		// 플레이어가 사신의 물품을 각각 1개 씩 더 챙긴 상태에서 인터랙트할 경우에,
		if (Player->GetSwordNumbers() >= 1 && Player->GetMirrorNumbers() >= 1 && Player->GetBellNumbers() >= 1)
		{
			// 다른 순간이동 위치로 순간이동시킴.
			if (HiddenTeleportPosition)
			{
				FVector TeleportPosition = HiddenTeleportPosition->GetComponentLocation();
				Player->SetActorLocation(TeleportPosition);
			}
		}
		// 그게 아닐 경우
		else
		{
			// 원래 순간 이동 위치로 순간이동 시킴.
			if (CommonTeleportPosition)
			{
				FVector TeleportPosition = CommonTeleportPosition->GetComponentLocation();
				Player->SetActorLocation(TeleportPosition);
			}
		}
	}
}

void AEnd_Mirror::SetIsCleared(const bool value)
{
	bIsCleared = value;
}