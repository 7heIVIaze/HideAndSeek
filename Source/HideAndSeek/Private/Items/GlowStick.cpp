// CopyrightNotice 2023 Sunggon Kim kimdave205@gmail.com. All Rights Reserved.

#include "Items/GlowStick.h"
#include "Components/PointLightComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Player/HorrorGameCharacter.h"
#include "Sound/SoundCue.h"

// 생성자
AGlowStick::AGlowStick()
{
	// 메시들의 기본 설정을 해줌. (세세한 설정은 블루프린트 클래스에서 수행)
	GlowLightComp = CreateDefaultSubobject<UPointLightComponent>(TEXT("GlowLight"));
	GlowLightComp->SetupAttachment(RootComp);
}

// 플레이어가 야광봉을 사용할 때 작동할 함수.
void AGlowStick::SetLightOn()
{
	// 사용 효과 음을 재생함,
	if (UseSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, UseSound, GetActorLocation());
	}

	GlowLightComp->SetVisibility(true);
}

void AGlowStick::OnInteract(class AHorrorGameCharacter* Player)
{
	Super::OnInteract(Player);
	
}

bool AGlowStick::UseInteract(class AHorrorGameCharacter* Player)
{
	Super::UseInteract(Player);
	
	SetLightOn();

	return true;
}