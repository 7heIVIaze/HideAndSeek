// CopyrightNotice 2023 Sunggon Kim kimdave205@gmail.com. All Rights Reserved.

#include "Items/GlowStick_cpp.h"
#include "Components/PointLightComponent.h"
#include "Player/HorrorGameCharacter.h"
#include "ComponentAction/HorrorGameSaveGame.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"

// 생성자
AGlowStick_cpp::AGlowStick_cpp()
{
	// 메시들의 기본 설정을 해줌. (세세한 설정은 블루프린트 클래스에서 수행)
	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = RootComp;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	MeshComp->SetupAttachment(RootComp);

}

// 플레이어가 야광봉과 상호작용할 때 작동할 함수.
void AGlowStick_cpp::OnInteract(class AHorrorGameCharacter* Player)
{
	Super::OnInteract(Player);

	// 플레이어의 야광봉을 얻는 메서드를 호출함.
	Player->AddGlowStick();

	// 위 메서드를 통해 플레이어가 아이템을 얻을 수 있는 상태이면
	if (Player->bCanItemGet)
	{
		// 야광봉을 처음 얻는 상태라면 야광봉 문서를 세이브 데이터에 영구히 저장함.
		if (UHorrorGameSaveGame* SaveData = UHorrorGameSaveGame::LoadObject(this, TEXT("Player"), 0))
		{
			if (!SaveData->CollectArchives.Item11_GlowStick)
			{
				SaveData->CollectArchives.Item11_GlowStick = true;
				Player->SetArchiveGetText(NSLOCTEXT("AGlowStick_cpp", "Get_GlowStick", "Glow Stick\nis added in archive"));
				SaveData->SaveData();
			}
		}

		// 그 후 배치된 이 액터를 제거함.
		Destroy();
	}
}

// 플레이어가 야광봉을 사용할 때 작동할 함수.
void AGlowStick_cpp::UseInteract(class AHorrorGameCharacter* Player)
{
	Super::UseInteract(Player);

	// 사용 효과 음을 재생함,
	if (UseSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, UseSound, GetActorLocation());
	}

	// 스폰된 야광봉 액터의 머티리얼 중 몸통 부분을 발광 형태의 머티리얼로 변경함.
	UMaterialInterface* Material = MeshComp->GetMaterial(1);
	MaterialInstance = MeshComp->CreateDynamicMaterialInstance(1, Material);

	MaterialInstance->SetScalarParameterValue(TEXT("Glow"), 1.0f);
}