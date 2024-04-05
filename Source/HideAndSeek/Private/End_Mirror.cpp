// CopyrightNotice=0 2023 Sunggon Kim kimdave205@gmail.com

#include "End_Mirror.h"
#include "HideAndSeek/HorrorGameCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "HorrorGamePlayerController.h"
#include "HideAndSeek/HorrorGameGameMode.h"
#include "HorrorGameGameInstance.h"
#include "HUD/TimerWidget.h"
#include "Sound/SoundCue.h"
#include "GameUI.h"

// Sets default values
AEnd_Mirror::AEnd_Mirror()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

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

void AEnd_Mirror::OnInteract(class AHorrorGameCharacter* Player)
{
	if (bIsCleared) // 게임 클리어 시
	{
		if (InteractSound != nullptr) // 순간이동 소리가 있으면 재생시킴
		{
			UGameplayStatics::PlaySound2D(this, InteractSound);
		}
		Cast<AHorrorGameGameMode>(UGameplayStatics::GetGameMode(GetWorld()))->StopBackGroundMusic();

		if (Player->GetSwordNumbers() >= 1 && Player->GetMirrorNumbers() >= 1 && Player->GetBellNumbers() >= 1)
		{ // 플레이어가 사신의 물품을 각각 1개 씩 더 챙긴 상태에서 인터랙트할 경우에,
			if (HiddenTeleportPosition)
			{
				FVector TeleportPosition = HiddenTeleportPosition->GetComponentLocation();
				Player->SetActorLocation(TeleportPosition);
			}
		}
		else
		{
			if (CommonTeleportPosition)
			{
				FVector TeleportPosition = CommonTeleportPosition->GetComponentLocation();
				Player->SetActorLocation(TeleportPosition);
			}
		}
	}
	//if (bIsCleared) // 게임 클리어 시 Clear Widget 표시
	//{
	//	if (UHorrorGameGameInstance* GameInstance = Cast<UHorrorGameGameInstance>(UGameplayStatics::GetGameInstance(GetWorld())))
	//	{
	//		FString ClearTime = Player->GameUIWidget->TimerWidget->GetClearTime();
	//		if (ClearChapterNum != NULL)
	//		{
	//			if (GameInstance->ClearedChapterSaveLogic(ClearChapterNum))
	//			{
	//				if (GameInstance->ClearTimeSaveLogic(ClearChapterNum, ClearTime))
	//				{
	//					Cast<AHorrorGamePlayerController>(Player->GetController())->OnGameClear(ClearTime);
	//				}
	//			}
	//		}
	//		//FString ClearTime = GameInstance->GetCurrentClearTime();
	//	}
	//	//FString levelName = TEXT("/Game/Levels/GameLevel/Start");
	//	///*if(GameInstance)
	//	//	GameInstance->StopSound();*/
	//	//UGameplayStatics::OpenLevel(GetWorld(), *levelName);
	//}
}

void AEnd_Mirror::SetIsCleared(const bool value)
{
	bIsCleared = value;
}