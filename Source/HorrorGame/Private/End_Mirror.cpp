// CopyrightNotice=0 2023 Sunggon Kim kimdave205@gmail.com

#include "End_Mirror.h"
#include "HorrorGame/HorrorGameCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "HorrorGamePlayerController.h"

// Sets default values
AEnd_Mirror::AEnd_Mirror()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = RootComp;

	MirrorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("EndMirror"));
	MirrorMesh->SetupAttachment(RootComp);
}

// Called when the game starts or when spawned
void AEnd_Mirror::BeginPlay()
{
	Super::BeginPlay();

	bIsCleared = false;
}

void AEnd_Mirror::OnInteract(class AHorrorGameCharacter* Player)
{
	if (bIsCleared) // 게임 클리어 시 Clear Widget 표시
	{
		Cast<AHorrorGamePlayerController>(GetWorld()->GetFirstPlayerController())->OnGameClear();
		//FString levelName = TEXT("/Game/Levels/GameLevel/Start");
		///*if(GameInstance)
		//	GameInstance->StopSound();*/
		//UGameplayStatics::OpenLevel(GetWorld(), *levelName);
	}
}

void AEnd_Mirror::SetIsCleared(const bool value)
{
	bIsCleared = value;
}