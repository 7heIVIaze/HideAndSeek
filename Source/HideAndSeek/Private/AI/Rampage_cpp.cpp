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

	/*BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("PatrolSoundBox"));
	BoxCollision->SetupAttachment(GetMesh());
	BoxCollision->SetBoxExtent(FVector(20.f, 20.f, 20.f));
	BoxCollision->OnComponentBeginOverlap.AddDynamic(this, &ARampage_cpp::SoundBeginOverlap);
	BoxCollision->OnComponentEndOverlap.AddDynamic(this, &ARampage_cpp::SoundEndOverlap);

	DissolveParticleSystem = CreateDefaultSubobject<UNiagaraComponent>(TEXT("DissolveParticleSystem"));
	DissolveParticleSystem->SetupAttachment(GetMesh());
	DissolveParticleSystem->SetActive(false);

	KillBox = CreateDefaultSubobject<UBoxComponent>(TEXT("KillBox"));
	KillBox->SetupAttachment(GetMesh());
	KillBox->OnComponentBeginOverlap.AddDynamic(this, &ARampage_cpp::CatchBeginOverlap);

	WatchPoint = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PlayerWatchPoint"));*/

	AIControllerClass = AAIController_Rampage::StaticClass();
	//AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	GetCapsuleComponent()->SetWorldLocation(FVector(0.f, 0.f, 70.f));

}

// Called when the game starts or when spawned
void ARampage_cpp::BeginPlay()
{
	Super::BeginPlay();
		
	// 현재 변수들을 다 초기화 해줌,
	/*Index = 0;
	MapName = UGameplayStatics::GetCurrentLevelName(GetWorld());*/
	RampageController = Cast<AAIController_Rampage>(GetController());
}

// Called every frame
void ARampage_cpp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 현재는 Rampage는 처치도, 기절도, 시간 정지도 통하지 않는 캐릭터이지만, 나중의 챕터에 등장할 수도 있기에 주석 처리로 남겨둠.
	
	//DissolveTimeline.TickTimeline(DeltaTime);

	//if (bIsStunned)
	//{
	//	CurrentStunnedTime += DeltaTime;
	//	if (CurrentStunnedTime >= stunTime)
	//	{
	//		bIsStunned = false;
	//		GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel3, ECollisionResponse::ECR_Block);
	//		KillBox->OnComponentBeginOverlap.AddDynamic(this, &AReaper_cpp::CatchBeginOverlap);
	//		//ACreatureAI* controller = Cast<ACreatureAI>(GetController());
	//		ReaperController->SetStunned(bIsStunned);
	//		CurrentStunnedTime = 0;
	//	}
	//}

	//if (bIsTimeStop)
	//{
	//	TimeStopElapsedTime += DeltaTime;
	//	if (TimeStopElapsedTime >= 25.f * CustomTimeDilation) // 정지 시간은 25초
	//	{
	//		bIsTimeStop = false;
	//		CustomTimeDilation = 1.0f;
	//		GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel3, ECollisionResponse::ECR_Block);
	//		KillBox->OnComponentBeginOverlap.AddDynamic(this, &AReaper_cpp::CatchBeginOverlap);
	//		//ACreatureAI* controller = Cast<ACreatureAI>(GetController());
	//		ReaperController->SetStunned(bIsTimeStop);
	//		TimeStopElapsedTime = 0;
	//	}
	//}

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

//// 추격 상태로 만들 함수
//void ARampage_cpp::StartChase()
//{
//	// 추격 상태가 아닐 경우에만 작동하도록 설정함.
//	if (!bIsChase)
//	{
//		// 추격 상태로 만들고 플레이어의 상태를 추격 중으로 바꿈
//		bIsChase = true;
//		if (Player)
//		{
//			Player->SetPlayerStatus(EPlayerStatus::Chased);
//		}
//	}
//}

// 추격 종료 상태로 만들 함수
void ARampage_cpp::EndChase()
{
	// 추격 상태인 경우에만 작동하도록 설정함.
	// 
	if (bIsChase)
	{
		// 추격 상태가 아님으로 설정하고, 플레이어의 상태도 생존으로 바꿈.
		// ACreatureAI* AIController = Cast<ACreatureAI>(GetController());
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

//void ARampage_cpp::SetIsCatch(bool Value)
//{
//	bIsCatch = Value;
//}

// 공격 애니메이션 종료 시 작동할 함수
void ARampage_cpp::SetAnimFinish(bool Value)
{
	bAnimFinish = Value;

	if (bIsCinematic) // 시네마틱을 위한 씬에서 죽을 경우엔 노말 엔딩 출력
	{
		AHorrorGamePlayerController* PlayerController = Cast<AHorrorGamePlayerController>(Player->GetController());
		Player->SetPlayerStatus(EPlayerStatus::Clear);
		//PlayerController->ShowEnding(1);
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

//void ARampage_cpp::SoundBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
//{
//	if (OtherActor != nullptr && OtherActor != this && OtherComp != nullptr)
//	{
//		if (auto PlayerActor = Cast<AHorrorGameCharacter>(OtherActor))
//		{
//			if (bIsCinematic)
//			{
//				PlayerActor->SetCameraComponentNoise(3);
//			}
//			else
//			{
//				PlayerActor->CreatureNum++;
//				if (PlayerActor->CreatureNum > 0)
//				{
//					PlayerActor->FlickeringLight.Play();
//					PlayerActor->SetCameraComponentNoise(1);
//				}
//				//ReaperSound->Play();
//				if (!PlayerActor->bIsCooldown)
//				{
//					PlayerActor->AddPatience(1);
//				}
//				GetMesh()->SetCollisionProfileName("AICharacters");
//				//InteractBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
//				//InteractBox->OnComponentBeginOverlap.AddDynamic(this, &AReaper_cpp::DoorBeginOverlap);
//			}
//		}
//	}
//}

//void ARampage_cpp::SoundEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
//{
//	if (OtherActor != nullptr && OtherActor != this && OtherComp != nullptr)
//	{
//		if (auto PlayerActor = Cast<AHorrorGameCharacter>(OtherActor))
//		{
//			if (bIsCinematic)
//			{
//				PlayerActor->SetCameraComponentNoise(4);
//			}
//			else
//			{
//				//ReaperSound->Stop();
//				PlayerActor->CreatureNum--;
//				if (PlayerActor->CreatureNum <= 0)
//				{
//					PlayerActor->FlickeringLight.Stop();
//					PlayerActor->CigarLight->SetIntensity(PlayerActor->CigarIntensity);
//					PlayerActor->FlashLight->SetIntensity(PlayerActor->FlashIntensity);
//					PlayerActor->SetCameraComponentNoise(0);
//				}
//				//GetMesh()->SetCollisionProfileName("OverlapAllDynamic");
//				//InteractBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
//				//InteractBox->OnComponentBeginOverlap.RemoveDynamic(this, &AReaper_cpp::DoorBeginOverlap);
//			}
//		}
//	}
//}

//void ARampage_cpp::CheckBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
//{
//	if (OtherActor != nullptr && OtherActor != this && OtherComp != nullptr)
//	{
//		if (OtherActor == CurrentPatrolPoint)
//		{
//			SetPatrolSuccess(true);
//		}
//	}
//}
//
//FVector ARampage_cpp::GetPatrolPoint()
//{
//	/*UWorld* world = GetWorld();
//	FVector ResultLocation = GetActorLocation();*/
//	//AAIController_Rampage* AIController = Cast<ACreatureAI>(GetController());
//	FVector ResultLocation(0.f);
//	
//	CurrentPatrolPoint = PatrolPointLists[Index++]; // 현재 인덱스의 패트롤 포인트 인스턴스를 지정해주고 인덱스 증가
//	ResultLocation = CurrentPatrolPoint->GetActorLocation();
//	RampageController->GetBlackboard()->SetValueAsObject(AAIController_Rampage::PatrolTargetKey, CurrentPatrolPoint);
//	if (Index == PatrolPointLists.Num()) // 만약 인덱스가 PatrolPointLists의 최대 인덱스를 넘어가면
//	{
//		Index = 0; // 다시 0으로 초기화
//	}
//
//	return ResultLocation;
//}

//bool ARampage_cpp::GetIsStunned()
//{
//	return bIsStunned;
//}

//void ARampage_cpp::Exorcism()
//{
//	bIsDied = true;
//
//	DissolveParticleSystem->Activate(true);
//	TArray<UMaterialInterface*> Materials = GetMesh()->GetMaterials();
//	for (int i = 0; i < Materials.Num(); ++i)
//	{
//		MaterialInstances.Add(GetMesh()->CreateDynamicMaterialInstance(i, Materials[i]));
//		MaterialInstances[i]->SetTextureParameterValue(TEXT("Texture"), Texture);
//	}
//	//	MaterialInstance = GetMesh()->CreateDynamicMaterialInstance(0, Material);
//	//	MaterialInstance->SetTextureParameterValue(TEXT("Texture"), Texture);
//	DissolveParticleSystem->SetVariableTexture(TEXT("Texture"), Texture);
//	DissolveTimeline.PlayFromStart();
//
//	/*GetMesh()->PlayAnimation(Anim, false);
//	Destroy();*/
//}

//void ARampage_cpp::SetPatrolSuccess(bool value)
//{
//	bIsPatrolSuccess = value;
//}
//
//void ARampage_cpp::Stunning(float dist)
//{
//	if (!bIsStunned)
//	{
//		GetMovementComponent()->StopMovementImmediately();
//		float LongestStunTime = 18.f;
//		bIsStunned = true;
//		if (dist < 400.f)
//		{
//			stunTime = LongestStunTime;
//		}
//		else if (dist > 800.f)
//		{
//			stunTime = LongestStunTime * 0.5f;
//		}
//		else
//		{
//			stunTime = LongestStunTime * 0.75f;
//		}
//
//		//ACreatureAI* controller = Cast<ACreatureAI>(GetController());
//		GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel3, ECollisionResponse::ECR_Ignore);
//		KillBox->OnComponentBeginOverlap.RemoveDynamic(this, &ARampage_cpp::CatchBeginOverlap);
//		RampageController->SetStunned(bIsStunned);
//	}
//}
//
//void ARampage_cpp::SetStun()
//{
//	GetMovementComponent()->StopMovementImmediately();
//	bIsTimeStop = true; // Stop Movement and Stun!
//	CustomTimeDilation = 0.0001f; // 브루트의 시간은 0.0001로 만들어 속도 줄이고 멈춘 것처럼 표현
//	//stunTime = 25.f; // 기절 시간은 25초로
//	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel3, ECollisionResponse::ECR_Ignore);
//	KillBox->OnComponentBeginOverlap.RemoveDynamic(this, &ARampage_cpp::CatchBeginOverlap);
//	RampageController->SetStunned(bIsTimeStop);
//}
//
//bool ARampage_cpp::GetIsDied()
//{
//	return bIsDied;
//}
//
//bool ARampage_cpp::GetIsCatch()
//{
//	return bIsCatch;
//}
//
//bool ARampage_cpp::GetAnimFinish()
//{
//	return bAnimFinish;
//}

void ARampage_cpp::CatchBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherIndexBody, bool bFromSweep, const FHitResult& SweepResult)
{
	// ACreatureAI* AIController = Cast<ACreatureAI>(GetController());
	bool HideCatch = false;
	if (RampageController != nullptr)
	{
		HideCatch = RampageController->GetBlackboard()->GetValueAsBool(AAIController_Rampage::LockerLighting);
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
			//Character->GetCharacterMovement()->StopMovementImmediately();
			//Character->DisableInput(PlayerController);
			//PlayerController->ShowEnding(0); // 베리스 호출로 탄생 시 베드 엔딩 
		}
	}
}
//
//bool ARampage_cpp::GetPatrolSuccess()
//{
//	return bIsPatrolSuccess;
//}
//
//void ARampage_cpp::SetCreatureCollision(bool value)
//{
//	if (value) // true라면 플레이어가 근처에 있는 것이기 때문에 콜리전(물리적 충돌) 활성화
//	{
//		GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel9, ECollisionResponse::ECR_Block);
//	}
//	else // false라면 근처에 플레이어가 없는 것이기 때문에 콜리전(물리적 충돌) 비활성화
//	{
//		GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel9, ECollisionResponse::ECR_Ignore);
//	}
//}
//
//void ARampage_cpp::SetIsStop(bool inIsStop)
//{
//	bIsStop = inIsStop;
//}
//
//bool ARampage_cpp::GetIsStop()
//{
//	return bIsStop;
//}

void ARampage_cpp::CalledByBerith()
{
	IsBerith = true;
	RampageController->GetBlackboard()->SetValueAsBool(AAIController_Rampage::CalledByBerith, IsBerith);
	if (IsBerith)
	{
		//KillBox->OnComponentBeginOverlap.RemoveDynamic(this, &ARampage_cpp::CatchBeginOverlap);
		BoxCollision->OnComponentBeginOverlap.RemoveDynamic(this, &ARampage_cpp::SoundBeginOverlap);
		BoxCollision->OnComponentEndOverlap.RemoveDynamic(this, &ARampage_cpp::SoundEndOverlap);
	}
}

void ARampage_cpp::SetIsCinematic(bool inIsCinematic)
{
	bIsCinematic = inIsCinematic;

	if (bIsCinematic) // 시네마틱 모드인 경우(챕터 2 클리어 후 등장한 경우)
	{
		RampageController->GetBlackboard()->SetValueAsBool(AAIController_Rampage::IsCinematic, bIsCinematic);
		Player = Cast<AHorrorGameCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
		RampageController->GetBlackboard()->SetValueAsObject(AAIController_Rampage::TargetKey, Player);
	}
}