// CopyrightNotice 2023 Sunggon Kim kimdave205@gmail.com. All Rights Reserved.

// ED용 요괴 - 장산범으로 하면 좋을 거 같음.
#include "AI/Rampage_cpp.h"
#include "Components/PointLightComponent.h"
#include "Components/SpotLightComponent.h"
#include "Components/AudioComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/BoxComponent.h"
#include "Sound/SoundCue.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubSystems.h"
#include "AI/AIController_Rampage.h"
#include "AI/RampageAnimInstance.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "LevelManager/PatrolPoint_cpp.h"
#include "EngineUtils.h"
#include "Player/HorrorGameCharacter.h"
#include "Player/HorrorGamePlayerController.h"
#include "Furniture/DoorClass.h"
#include "Furniture/Alarm.h"
#include "Animation/AnimSequence.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraComponent.h"
#include "Materials/MaterialParameterCollection.h"
#include "Materials/MaterialParameterCollectionInstance.h"
#include "ComponentAction/HorrorGameSaveGame.h"

// Sets default values
ARampage_cpp::ARampage_cpp()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<USkeletalMesh>MeshAsset(TEXT("/Game/ParagonRampage/Characters/Heroes/Rampage/Meshes/Rampage"));
	if (MeshAsset.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(MeshAsset.Object);
	}

	AIControllerClass = AAIController_Rampage::StaticClass();

	GetCapsuleComponent()->SetWorldLocation(FVector(0.f, 0.f, 70.f));

}

// Called when the game starts or when spawned
void ARampage_cpp::BeginPlay()
{
	Super::BeginPlay();
		
	// 현재 변수들을 다 초기화 해줌,
	RampageController = Cast<AAIController_Rampage>(GetController());
}

// Called every frame
void ARampage_cpp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	RampageController->SetControlRotation(GetActorRotation());
}

void ARampage_cpp::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (IsPlayerControlled())
	{
		GetCharacterMovement()->MaxWalkSpeed = 160.f;
	}
	else
	{
		GetCharacterMovement()->MaxWalkSpeed = 280.f;
	}
}

// 추격 종료 상태로 만들 함수
void ARampage_cpp::EndChase()
{
	// 추격 상태인 경우에만 작동하도록 설정함.
	// 
	if (bIsChase)
	{
		// 추격 상태가 아님으로 설정하고, 플레이어의 상태도 생존으로 바꿈.
		
		if (RampageController != nullptr)
		{
			// AIController->EndChase();
		}
		bIsChase = false;
		//	ReaperSound->SetSound(PatrolSound);

		if (Player)
		{
			Player->SetPlayerStatus(EPlayerStatus::Survive);
		}
	}
}


// 공격 애니메이션 종료 시 작동할 함수
void ARampage_cpp::SetAnimFinish(bool Value)
{
	bAnimFinish = Value;

	if (bIsCinematic) // 시네마틱을 위한 씬에서 죽을 경우엔 노말 엔딩 출력
	{
		AHorrorGamePlayerController* PlayerController = Cast<AHorrorGamePlayerController>(Player->GetController());
		Player->SetPlayerStatus(EPlayerStatus::Clear);
	}
	// 시네마틱이 아닌 경우 작동할 것이지만, 현재는 이 것을 사용할 일 없음.
	else
	{
		if (Player->GetIsHiding())
		{
			Player->SetPlayerStatus(EPlayerStatus::Survive);
		}
		else
		{
			Player->SetPlayerStatus(EPlayerStatus::Died);
		}
	}
}

void ARampage_cpp::CatchBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherIndexBody, bool bFromSweep, const FHitResult& SweepResult)
{
	bool HideCatch = false;
	if (RampageController != nullptr)
	{
		HideCatch = RampageController->GetBlackboardComponent()->GetValueAsBool(AAIController_Rampage::LockerLighting);
	}

	if (!IsBerith)
	{
		if (OtherActor != this && OtherActor != nullptr && OtherComp != nullptr)
		{
			if (auto Character = Cast<AHorrorGameCharacter>(OtherActor))
			{
				if ((Character->GetPlayerStatus() == EPlayerStatus::Survive || Character->GetPlayerStatus() == EPlayerStatus::Chased) && !Character->GetIsHiding())
				{
					Character->SetPlayerStatus(EPlayerStatus::Catched);

					Character->OnFocus(WatchPoint->GetComponentLocation());

					SetIsCatch(true);
				}
			}

			if (auto Alarm = Cast<AAlarm>(OtherActor))
			{
				Alarm->AIInteract();
			}
		}
	}
	else
	{
		if (auto Character = Cast<AHorrorGameCharacter>(OtherActor))
		{
			AHorrorGamePlayerController* PlayerController = Cast<AHorrorGamePlayerController>(Character->GetController());
			Character->SetPlayerStatus(EPlayerStatus::Clear); // 엔딩을 보기 위해 클리어 상태로 설정
		}
	}
}

void ARampage_cpp::CalledByBerith()
{
	IsBerith = true;
	RampageController->GetBlackboardComponent()->SetValueAsBool(AAIController_Rampage::CalledByBerith, IsBerith);
	if (IsBerith)
	{
		BoxCollision->OnComponentBeginOverlap.RemoveDynamic(this, &ARampage_cpp::SoundBeginOverlap);
		BoxCollision->OnComponentEndOverlap.RemoveDynamic(this, &ARampage_cpp::SoundEndOverlap);
	}
}

void ARampage_cpp::SetIsCinematic(bool inIsCinematic)
{
	bIsCinematic = inIsCinematic;

	// 시네마틱 모드인 경우(챕터 2 클리어 후 등장한 경우)
	if (bIsCinematic)
	{
		RampageController->GetBlackboardComponent()->SetValueAsBool(AAIController_Rampage::IsCinematic, bIsCinematic);
		Player = Cast<AHorrorGameCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
		RampageController->GetBlackboardComponent()->SetValueAsObject(AAIController_Rampage::TargetKey, Player);
	}
}