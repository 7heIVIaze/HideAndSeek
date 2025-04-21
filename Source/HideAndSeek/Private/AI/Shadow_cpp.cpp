// CopyrightNotice 2023 Sunggon Kim kimdave205@gmail.com. All Rights Reserved.

#include "AI/Shadow_cpp.h"
#include "Components/PointLightComponent.h"
#include "Components/SpotLightComponent.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "Components/CapsuleComponent.h"
#include "Sound/SoundCue.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AI/AIController_Shadow.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "LevelManager/PatrolPoint_cpp.h"
#include "EngineUtils.h"
#include "Furniture/HideObject.h"
#include "Player/HorrorGameCharacter.h"
#include "Player/HorrorGamePlayerController.h"
#include "Furniture/DoorClass.h"
#include "Furniture/Alarm.h"
#include "Kismet/KismetMathLibrary.h"
#include "NiagaraComponent.h"
#include "ComponentAction/HorrorGameSaveGame.h"

// Constructor
AShadow_cpp::AShadow_cpp()
{
	// 시간 정지 시간을 계산해야 하므로 Tick 활성화. AI Controller의 로테이션도 여기서 관리함
	PrimaryActorTick.bCanEverTick = true;

	// AI Controller 설정
	AIControllerClass = AAIController_Shadow::StaticClass();

	// 콜리전 설정
	GetCapsuleComponent()->SetWorldLocation(FVector(0.f, 0.f, 90.f));
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel9, ECollisionResponse::ECR_Ignore);
}

// 게임 시작 또는 생성 시 호출됨.
void AShadow_cpp::BeginPlay()
{
	Super::BeginPlay();

}

// 매 프레임마다 호출되는 함수.
void AShadow_cpp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (AAIController_Shadow* ShadowController = Cast<AAIController_Shadow>(GetController()))
	{
		ShadowController->SetControlRotation(GetActorRotation());
	}
}

//소유한 컨트롤러에 따라 다른 속도를 가짐
void AShadow_cpp::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	// 플레이어 소유일 시(이럴 경우 없음) 속도는 180으로
	if (IsPlayerControlled())
	{
		GetCharacterMovement()->MaxWalkSpeed = 180.f;
	}
	// 그게 아니면 추격 중이면 300으로, 노멀 상태면 160으로 설정
	else
	{
		if (bIsChase) GetCharacterMovement()->MaxWalkSpeed = 300.f;
		else GetCharacterMovement()->MaxWalkSpeed = 160.f;
	}
}

// Exorcism될 때 Dissolve Material을 구현하기 위한 타임라인 콜백 함수.
void AShadow_cpp::ChangeMaterialInstance(float inValue)
{
	Super::ChangeMaterialInstance(inValue);

	// 1에서 0으로 서서히 Curve Float에 따라 감소하면서 머티리얼의 파라미터와 나이아가라 파티클의 파라미터를 변경함.
	float Amount = FMath::Lerp(1.0f, 0.0f, inValue);

	MaterialInstances[0]->SetScalarParameterValue(TEXT("Amount"), Amount);

	DissolveParticleSystem->SetVariableFloat(TEXT("Amount"), Amount);
}

// 타임라인 종료 후 호출할 콜백 함수.
void AShadow_cpp::DissolveFinish()
{
	Super::DissolveFinish();

	// 파티클을 비활성화시키고 해당 캐릭터를 제거함
	DissolveParticleSystem->SetVariableFloat(TEXT("Width"), 0.0f);

	DissolveParticleSystem->Deactivate();

	Destroy();
}