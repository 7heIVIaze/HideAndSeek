// CopyrightNotice 2023 Sunggon Kim kimdave205@gmail.com. All Rights Reserved.

#include "Items/Soul_Lantern_cpp.h"
#include "Sound/SoundCue.h"
#include "Components/PointLightComponent.h"
#include "Kismet/GameplayStatics.h"
#include "ComponentAction/HorrorGameSaveGame.h"
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "EngineUtils.h"
#include "Altar_cpp.h"
#include "HideAndSeek/HorrorGameCharacter.h"

ASoul_Lantern_cpp::ASoul_Lantern_cpp()
{
	PrimaryActorTick.bCanEverTick = true;

	// 메시들의 기본 설정을 해줌. (세세한 설정은 블루프린트 클래스에서 수행)
	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = RootComp;

	LanternMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Lantern"));
	LanternMesh->SetupAttachment(RootComp);
	static ConstructorHelpers::FObjectFinder<UStaticMesh>LanternObj(TEXT("/Game/Assets/Items/Lantern"));
	if (LanternObj.Succeeded())
	{
		LanternMesh->SetStaticMesh(LanternObj.Object);
	}
	LanternMesh->SetCollisionProfileName("ItemObjects");

	Light = CreateDefaultSubobject<UPointLightComponent>(TEXT("SoulLight"));
	Light->SetupAttachment(LanternMesh);

	LightNiagara = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Soul"));
	LightNiagara->SetupAttachment(LanternMesh);
	static ConstructorHelpers::FObjectFinder<UNiagaraSystem>Soul_NS(TEXT("/Game/Assets/Material/NS_Soul"));
	if (Soul_NS.Succeeded())
	{
		LightNiagara->SetAsset(Soul_NS.Object);
	}

	bIsLightOn = false;
	AltarLocation = FVector(0, 0, 0);
}

void ASoul_Lantern_cpp::BeginPlay()
{
	Super::BeginPlay();

	// 빛은 처음에 안 켜지게 설정하고, 도착지를 나타낼 나이아가라 이펙트는 비활성화함.
	Light->SetVisibility(bIsLightOn);
	LightNiagara->Deactivate();

	// 나이아가라 이펙트를 위해 제단의 위치를 가져옴.
	for (TActorIterator<AAltar_cpp>entity(GetWorld()); entity; ++entity)
	{
		AAltar_cpp* Altar = *entity;
		if (Altar != nullptr) {
			AltarLocation = Altar->GetActorLocation();
			AltarLocation.Z = 0.f; // 정확한 Z축을 구하기 위한 Cross Product 계산을 위해 0으로 설정
		}
	}
}

void ASoul_Lantern_cpp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	// 나이아가라 이펙트의 회전을 위해 틱마다 제단의 위치와 액터의 위치를 비교해 도착지 방향을 가리키도록 함.
	if (bIsLightOn)
	{
		FVector ToTarget = AltarLocation - LightNiagara->GetComponentLocation();
		FRotator LookAtRotation = FRotator(0.f, ToTarget.Rotation().Yaw, 0.f);
		LightNiagara->SetWorldRotation(LookAtRotation);
	}
}

// 플레이어가 영혼 랜턴 아이템을 습득하려 할 때 작동할 함수.
void ASoul_Lantern_cpp::OnInteract(class AHorrorGameCharacter* Player)
{
	Super::OnInteract(Player);

	// 플레이어의 랜턴을 얻는 메서드를 호출함.
	Player->AddLantern();

	// 위 메서드를 통해 플레이어가 아이템을 얻을 수 있는 상태이면
	if (Player->bCanItemGet)
	{
		// 영혼 랜턴을 처음 얻은 상태라면 영혼 랜턴 문서를 세이브 데이터에 영구히 저장함.
		if (UHorrorGameSaveGame* SaveData = UHorrorGameSaveGame::LoadObject(this, TEXT("Player"), 0))
		{
			if (!SaveData->CollectArchives.Item10_SoulLantern)
			{
				SaveData->CollectArchives.Item10_SoulLantern = true;
				Player->SetArchiveGetText(NSLOCTEXT("ASoul_Lantern_cpp", "Get_SoulLantern", "Soul Lantern\nis added in archive"));
				SaveData->SaveData();
			}
		}

		// 그 후 배치된 이 액터를 제거함.
		Destroy();
	}
}

// 플레이어가 영혼 랜턴 아이템을 사용할 때 작동할 함수.
void ASoul_Lantern_cpp::UseInteract(class AHorrorGameCharacter* Player)
{
	Super::UseInteract(Player);
	
	// 빛이 나는 지를 현재 값을 반전시켜 저장하고, 플레이어에게도 저장함.
	bIsLightOn = !bIsLightOn;
	Player->bLanternOn = bIsLightOn;
	UE_LOG(LogTemp, Warning, TEXT("Player: LanternOn : %s"), bIsLightOn ? TEXT("true") : TEXT("false"));
	
	// 빛을 낼 경우, 소리를 재생하고 나이아가라 시스템을 활성화함.
	if (bIsLightOn)
	{
		if (LanternCue)
		{
			UGameplayStatics::PlaySoundAtLocation(this, LanternCue, GetActorLocation());
		}
		LightNiagara->Activate(true);
	}
	// 빛을 안 낼 경우, 나이아가라 시스템을 비활성화함.
	else
	{
		LightNiagara->Activate(false);
		LightNiagara->Deactivate();
	}

	// 그 후 라이트 컴포넌트를 켜거나 끔
	Light->SetVisibility(bIsLightOn);
}

// 플레이어가 숨어있는 경우, 숨어있는 오브젝트 안에서 호출할 함수.
void ASoul_Lantern_cpp::HideInteract(class AHorrorGameCharacter* Player)
{
	// 빛이 나는 지를 현재 값을 반전시켜 저장하고, 플레이어에게도 저장함.
	bIsLightOn = !bIsLightOn;
	UE_LOG(LogTemp, Warning, TEXT("Player: LanternOn : %s"), bIsLightOn ? TEXT("true") : TEXT("false"));

	// 빛을 낼 경우, 소리를 재생하고 나이아가라 시스템을 활성화함.
	if (bIsLightOn)
	{
		//USoundCue* LanternCue = LoadObject<USoundCue>(nullptr, TEXT("/Game/Assets/Sounds/SoundCues/LanternSoundCue"));
		/*if (LanternCue)
		{
			UGameplayStatics::PlaySoundAtLocation(this, LanternCue, GetActorLocation());
		}*/
		LightNiagara->Activate(true);
	}
	// 빛을 안 낼 경우, 나이아가라 시스템을 비활성화함.
	else
	{
		LightNiagara->Activate(false);
		LightNiagara->Deactivate();
	}

	// 그 후 라이트 컴포넌트를 켜거나 끔
	Light->SetVisibility(bIsLightOn);
}