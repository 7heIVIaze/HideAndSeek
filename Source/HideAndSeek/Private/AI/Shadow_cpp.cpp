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
#include "PatrolPoint_cpp.h"
#include "EngineUtils.h"
#include "Furniture/HideObject.h"
#include "HideAndSeek/HorrorGameCharacter.h"
#include "HorrorGamePlayerController.h"
#include "ClassroomDoorActor_cpp.h"
#include "Door_cpp.h"
#include "MetalDoor_cpp.h"
#include "Alarm.h"
#include "Kismet/KismetMathLibrary.h"
#include "NiagaraComponent.h"
#include "ComponentAction/HorrorGameSaveGame.h"

// Constructor
AShadow_cpp::AShadow_cpp()
{
	// 시간 정지 시간을 계산해야 하므로 Tick 활성화. AI Controller의 로테이션도 여기서 관리함
	PrimaryActorTick.bCanEverTick = true;

	// 액터 얼굴에 부착할 붉은색 포인트라이트 생성
	DetectSight = CreateDefaultSubobject<UPointLightComponent>(TEXT("DetectSight"));
	DetectSight->SetupAttachment(GetMesh());
	DetectSight->SetRelativeLocation(FVector(0.f, 50.f, 230.f));
	DetectSight->SetLightFColor(FColor(255.f, 10.f, 10.f));
	DetectSight->SetAttenuationRadius(100.f);

	// 플레이어 근처에 있으면 플레이어의 카메라에 노이즈를 걸 박스 콜라이더 생성
	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("PatrolSoundBox"));
	BoxCollision->SetupAttachment(GetMesh());
	BoxCollision->SetBoxExtent(FVector(20.f, 20.f, 20.f));
	BoxCollision->OnComponentBeginOverlap.AddDynamic(this, &AShadow_cpp::SoundBeginOverlap);
	BoxCollision->OnComponentEndOverlap.AddDynamic(this, &AShadow_cpp::SoundEndOverlap);

	// 처치될 때 이펙트를 생성할 나이아가라 컴포넌트 생성
	DissolveParticleSystem = CreateDefaultSubobject<UNiagaraComponent>(TEXT("DissolveParticleSystem"));
	DissolveParticleSystem->SetupAttachment(GetMesh());
	DissolveParticleSystem->SetActive(false);

	// 플레이어와 충돌 시 잡았는지 여부를 판단할 스피어 콜라이더 생성
	KillSphere = CreateDefaultSubobject<USphereComponent>(TEXT("KillBox"));
	KillSphere->SetupAttachment(GetMesh());
	KillSphere->SetRelativeLocation(FVector(0.f, 0.f, 130.f));
	KillSphere->SetRelativeScale3D(FVector(3.5f, 3.5f, 4.f));
	KillSphere->OnComponentBeginOverlap.AddDynamic(this, &AShadow_cpp::CatchBeginOverlap);

	// 플레이어가 사망할 때 카메라가 바라볼 지점 생성
	WatchPoint = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PlayerWatchPoint"));

	// AI Controller 설정
	AIControllerClass = AAIController_Shadow::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	// 콜리전 설정
	GetCapsuleComponent()->SetWorldLocation(FVector(0.f, 0.f, 90.f));
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel9, ECollisionResponse::ECR_Ignore);
}

// 게임 시작 또는 생성 시 호출됨.
void AShadow_cpp::BeginPlay()
{
	Super::BeginPlay();

	UWorld* world = GetWorld();
	
	// 순찰 지점을 배열에 추가
	for (TActorIterator<APatrolPoint_cpp>entity(world); entity; ++entity)
	{
		PatrolPointList.Add(*entity);
	}

	// Dissolve Curve가 존재하면 Dissolve Effect를 위한 타임라인 설정
	if (DissolveCurveFloat)
	{
		FOnTimelineFloat TimelineProgress;
		FOnTimelineEvent TimelineFinish;
		TimelineProgress.BindDynamic(this, &AShadow_cpp::ChangeMaterialInstance);
		DissolveTimeline.AddInterpFloat(DissolveCurveFloat, TimelineProgress);

		TimelineFinish.BindDynamic(this, &AShadow_cpp::DissolveFinish);
		DissolveTimeline.SetTimelineFinishedFunc(TimelineFinish);
		DissolveTimeline.SetLooping(false);
	}
}

// 매 프레임마다 호출되는 함수.
void AShadow_cpp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 타임라인에 틱 추가
	DissolveTimeline.TickTimeline(DeltaTime);

	// 문을 바라볼 때 문을 열 수 있는 함수 수행. RayCast를 통해 수행하기 때문에 반드시 tick에서 실행
	OpenDoor();

	// 시간 정지 되었을 때 수행
	if (bIsTimeStop)
	{
		TimeStopElapsedTime += DeltaTime;
		if (TimeStopElapsedTime >= 25.f * CustomTimeDilation) // 정지 시간은 25초
		{
			// 25초가 지난 경우, 시간 정지 종료
			bIsTimeStop = false;
			CustomTimeDilation = 1.0f;
			// 통과 불가능하게 설정. 충돌 시 바로 사망하게 설정
			GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel3, ECollisionResponse::ECR_Block);
			KillSphere->OnComponentBeginOverlap.AddDynamic(this, &AShadow_cpp::CatchBeginOverlap);
			
			// 다시 controller가 감각을 사용할 수 있게 설정
			AAIController_Shadow* controller = Cast<AAIController_Shadow>(GetController());
			controller->SetStunned(bIsTimeStop);

			TimeStopElapsedTime = 0;
		}
	}

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
		//	SetControlMode();
		if (bIsChase) GetCharacterMovement()->MaxWalkSpeed = 300.f;
		else GetCharacterMovement()->MaxWalkSpeed = 160.f;
	}
}

// 추격 시작을 알리는 함수
void AShadow_cpp::StartChase()
{
	// 추격 중이 아닌 경우에 실행되도록 함.
	if (!bIsChase)
	{
		// 추격 중임을 설정하고, 속도를 300으로 올림
		UE_LOG(LogTemp, Warning, TEXT("SetTrace: Shadow Start Chase Function Called"));
		bIsChase = true;
		GetCharacterMovement()->MaxWalkSpeed = 300.0f;

		// 플레이어의 상태를 추격 중으로 설정함
		AHorrorGameCharacter* PlayerActor = Cast<AHorrorGameCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());

		if (PlayerActor)
		{
			PlayerActor->SetPlayerStatus(Player_Status::Chased);
		}
	}
}

// 추격 끝을 알리는 함수
void AShadow_cpp::EndChase()
{
	// 추격 중일 경우에만 실행되도록 함.
	if (bIsChase)
	{
		// 추격이 끝났음을 설정하고, 속도를 160으로 줄임
		UE_LOG(LogTemp, Warning, TEXT("SetTrace: Shadow End Chase Function Called"));

		bIsChase = false;
		GetCharacterMovement()->MaxWalkSpeed = 160.0f;

		// 플레이어의 상태를 추격 종료로 설정함. 이 때, 플레이어가 다른 요괴에게 추격 당할 수도 있기 때문에 그것도 체크해서 해제하도록 해야 함.
		AHorrorGameCharacter* PlayerActor = Cast<AHorrorGameCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
		
		if (PlayerActor)
		{
			PlayerActor->SetPlayerStatus(Player_Status::Survive);
		}
	}
}

// 플레이어를 잡았는지 설정하는 함수
void AShadow_cpp::SetIsCatch(bool inIsCatch)
{
	bIsCatch = inIsCatch;
}

// 공격 애니메이션이 끝났는지를 설정하는 함수
void AShadow_cpp::SetAnimFinish(bool inIsAnimationFinished)
{
	// 애니메이션이 끝났다고 설정.
	bAnimFinish = inIsAnimationFinished;

	// 플레이어의 상태가 숨은 상태라면 생존으로 그렇지 않으면 사망으로 설정함.
	UWorld* World = GetWorld();
	AHorrorGameCharacter* PlayerActor = Cast<AHorrorGameCharacter>(World->GetFirstPlayerController()->GetPawn());

	// 만약 숨어있는 곳을 잡았는지 확인
	if (bIsHidingCatch)
	{
		// 그 숨은 장소를 부숨
		PlayerHidingObject->BreakHideObject();

		// 초기화 후 나가기
		bIsHidingCatch = false;
		return;
	}
	// 그냥 플레이어를 잡았을 경우
	else
	{
		if (PlayerActor->GetIsHiding())
		{
			PlayerActor->SetPlayerStatus(Player_Status::Survive);
		}
		else
		{
			PlayerActor->SetPlayerStatus(Player_Status::Died);
		}
	}
}

// 충돌체가 플레이어와 충돌 시 카메라에 노이즈를 발생시킬 콜백 함수
void AShadow_cpp::SoundBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// 충돌체에 충돌한 액터가 존재할 경우
	if (OtherActor != nullptr && OtherActor != this && OtherComp != nullptr)
	{
		// 그 액터가 플레이어이면
		if (auto PlayerActor = Cast<AHorrorGameCharacter>(OtherActor))
		{
			// 근처 요괴의 수를 증가시키고 괴물 개수가 0 이상이면 노이즈 단계 1로 설정함.
			PlayerActor->CreatureNum++;
			if (PlayerActor->CreatureNum > 0)
			{
				// PlayerActor->FlickeringLight.Play();
				// PlayerActor->SetCameraNoise(true);
				PlayerActor->SetCameraComponentNoise(1);
			}
			// 얘는 플레이어의 라이터나 플래시의 불을 꺼버림.
			if (PlayerActor->bIsCigarLightOn)
			{
				PlayerActor->UseCigarLight();
			}
			if (PlayerActor->bIsFlashLightOn)
			{
				PlayerActor->UseFlashLight();
			}

			// 패닉 상승 쿨타임이 아니면 게이지 1을 증가시킴.
			if (!PlayerActor->bIsCooldown)
			{
				PlayerActor->AddPatience(1);
			}
			
			// 캡슐 콜라이더의 충돌 채널을 문을 기준으로 Block으로 설정. 즉 문을 통과하지 못하게 설정
			GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel9, ECollisionResponse::ECR_Block);
		}

		// 충돌한 액터가 숨는 장소이면
		if (auto HideObject = Cast<AHideObject>(OtherActor))
		{
			// 근처의 요괴 수를 증가시키고, 플레이어가 숨어있는 경우에 카메라 노이즈 단계를 1로 설정함.
			HideObject->CreatureNum++;

			if (HideObject->bIsHiding)
			{
				HideObject->SetCameraComponentNoise(1);
			}
		}
	}
}

// 충돌체와 플레이어 사이의 충돌이 끝났을 때 노이즈 제거용 콜백 함수.
void AShadow_cpp::SoundEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	// 충돌체에 충돌이 끝난 액터가 존재할 경우
	if (OtherActor != nullptr && OtherActor != this && OtherComp != nullptr)
	{
		// 그 액터가 플레이어이면
		if (auto PlayerActor = Cast<AHorrorGameCharacter>(OtherActor))
		{
			// 플레이어 주변의 괴물 수를 1 감소시키고, 그 수가 0 이하라면 깜빡임도 멈추고, 노이즈 단계도 0으로 설정하고 빛의 세기를 원상복귀시킴.
			PlayerActor->CreatureNum--;
			if (PlayerActor->CreatureNum <= 0)
			{
				PlayerActor->FlickeringLight.Stop();
				PlayerActor->CigarLight->SetIntensity(PlayerActor->CigarIntensity);
				PlayerActor->FlashLight->SetIntensity(PlayerActor->FlashIntensity);
				PlayerActor->SetCameraComponentNoise(0);
			}

			// 캡슐 콜라이더의 충돌 설정을 문을 기준으로 Ignore로 설정. 즉 문을 통과할 수 있게 설정.
			GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel9, ECollisionResponse::ECR_Ignore);
		}
		// 그 액터가 숨는 장소이면
		if (auto HideObject = Cast<AHideObject>(OtherActor))
		{
			// 근처 요괴의 수를 1 감소시키고, 그 수가 0 이하라면 깜빡임, 불빛의 세기, 카메라 노이즈 단계를 default로 설정함.
			HideObject->CreatureNum--;
			if (HideObject->CreatureNum <= 0)
			{
				HideObject->FlickeringLight.Stop();
				HideObject->CigarLight->SetIntensity(HideObject->Intensity);
				HideObject->FlashLight->SetIntensity(HideObject->Intensity);
				HideObject->SetCameraComponentNoise(0);
			}
		}
	}
}

// 비헤이비어 트리의 태스크 노드의 PatrolMove와 FindPatrolPos에서 호출할 현재 목표 지점 설정 함수
FVector AShadow_cpp::GetPatrolPoint()
{
	AAIController_Shadow* AIController = Cast<AAIController_Shadow>(GetController());
	
	// 0에서 PatrolPointList의 마지막 인덱스까지 중 랜덤 숫자 하나 뽑기
	int randIdx = FMath::RandRange(0, PatrolPointList.Num() - 1);

	// 뽑아온 랜덤 인덱스의 순찰 지역을 목표 지점으로 설정하고 해당 지점의 위치를 리턴함
	CurrentPatrolPoint = PatrolPointList[randIdx];
	FVector ResultLocation = CurrentPatrolPoint->GetActorLocation();
	AIController->GetBlackboard()->SetValueAsObject(AAIController_Shadow::PatrolTargetKey, CurrentPatrolPoint);

	return ResultLocation;
}

// 플레이어가 숨은 오브젝트를 감지했을 때, 컨트롤러에서 제어할 함수
void AShadow_cpp::DetectPlayerHidingObject(class AHideObject* DetectedObject)
{
	PlayerHidingObject = DetectedObject;
}

// 청동 검을 통해 처치할 때 호출할 함수
void AShadow_cpp::Exorcism()
{
	// 사망했다는 것으로 설정
	bIsDied = true;
	
	// 나이아가라 시스템을 활성화하고, Dissolve 머티리얼 효과를 줘서 서서히 사라지는 듯한 효과를 줌.
	DissolveParticleSystem->Activate(true);
	UMaterialInterface* Material = GetMesh()->GetMaterial(0);
	MaterialInstance = GetMesh()->CreateDynamicMaterialInstance(0, Material);
	MaterialInstance->SetTextureParameterValue(TEXT("Texture"), Texture);
	DissolveParticleSystem->SetVariableTexture(TEXT("Texture"), Texture);
	DissolveTimeline.PlayFromStart();
}

// 순찰 성공 여부를 설정하는 함수
void AShadow_cpp::SetPatrolSuccess(bool value)
{
	bIsPatrolSuccess = value;
}

// 기절 상태를 설정하는 함수
void AShadow_cpp::Stunning(float dist)
{
	// CodeName: Shadow는 소화기로 기절시킬 수 없는 요괴. 플레이어가 불을 켜면 없어지는 요괴임
	 
	/*if (!bIsStunned)
	{
		GetMovementComponent()->StopMovementImmediately();
		float LongestStunTime = 18.f;
		bIsStunned = true;
		if (dist < 400.f)
		{
			stunTime = LongestStunTime;
		}
		else if (dist > 800.f)
		{
			stunTime = LongestStunTime * 0.5f;
		}
		else
		{
			stunTime = LongestStunTime * 0.75f;
		}
	
		GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel3, ECollisionResponse::ECR_Ignore);
		KillSphere->OnComponentBeginOverlap.RemoveDynamic(this, &AShadow_cpp::CatchBeginOverlap);
		AAIController_Shadow* controller = Cast<AAIController_Shadow>(GetController());
	}*/
}

// 플레이어가 청동 거울을 사용할 경우 호출할 함수
void AShadow_cpp::SetStun()
{
	// 시간이 정지된 듯한 효과를 주기 위해서 즉시 이동을 멈추고 플레이어와의 충돌을 Ignore로 만들어 통과 가능하게 하면서, 기절 중임을 설정함.
	GetMovementComponent()->StopMovementImmediately();
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel3, ECollisionResponse::ECR_Ignore);
	bIsTimeStop = true; // Stop Movement and Stun!
	CustomTimeDilation = 0.0001f; // Shadow의 시간을 0.0001로 만들어 속도 줄이고 멈춘 것처럼 표현
	//stunTime = 25.f; // 기절 시간은 25초로
	
	// 플레이어를 죽이는 트리거가 있는 Sphaer 콜라이더에 부착된 콜백 함수를 제거함
	KillSphere->OnComponentBeginOverlap.RemoveDynamic(this, &AShadow_cpp::CatchBeginOverlap);
	
	// 그리고 그 동안 컨트롤러가 아무것도 감지하지 못하게 하기 위해 스턴 상태임을 알림.
	AAIController_Shadow* controller = Cast<AAIController_Shadow>(GetController());
	controller->SetStunned(bIsTimeStop);
}

// 처치 되었는지 확인하는 함수
bool AShadow_cpp::GetIsDied()
{
	return bIsDied;
}

// 플레이어를 잡았는지 확인하는 함수
bool AShadow_cpp::GetIsCatch()
{
	return bIsCatch;
}

// 공격 애니메이션이 끝났는지 확인하는 함수
bool AShadow_cpp::GetAnimFinish()
{
	return bAnimFinish;
}

// Sphere 충돌체에 플레이어가 충돌할 경우 호출할 콜백 함수.
void AShadow_cpp::CatchBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherIndexBody, bool bFromSweep, const FHitResult& SweepResult)
{
	/*UE_LOG(LogTemp, Warning, TEXT("Something Catch"));*/

	// 플레이어가 어느 캐비닛에 숨어있는 것을 알고 있는지 파악
	AAIController_Shadow* AIController = Cast<AAIController_Shadow>(GetController());
	bool HideCatch = false;
	if (AIController != nullptr)
	{
		HideCatch = AIController->GetBlackboard()->GetValueAsBool(AAIController_Shadow::LockerLighting);
	}

	// Shadow가 죽은 상태나 시간 정지 상태가 아니라면 실행함
	if (!bIsDied && !bIsTimeStop)
	{
		// 충돌한 액터가 존재할 경우에
		if (OtherActor != this && OtherActor != nullptr && OtherComp != nullptr)
		{
			// 그 액터가 숨을 수 있는 액터이면
			if (auto HideObject = Cast<AHideObject>(OtherActor))
			{
				// 현재 추격 중이고 숨어있는 것을 알고 있는 상태에서 잡았다면
				if (bIsChase && HideCatch)
				{
					// Catch 했음을 알리고 옷장을 잡았다고 함수를 호출함
					SetIsCatch(true);
					bIsHidingCatch = true;
					DetectPlayerHidingObject(HideObject);
				}
			}
			// 그 액터가 플레이어라면
			if (auto Character = Cast<AHorrorGameCharacter>(OtherActor))
			{
				// 플레이어의 상태가 Survive이거나, 추격 중(Chased)이면서 동시에 숨은 상태가 아닌 경우에만
				if ((Character->GetPlayerStatus() == Player_Status::Survive || Character->GetPlayerStatus() == Player_Status::Chased) && !Character->GetIsHiding())
				{
					// 플레이어의 상태를 Catch로 업데이트함.
					Character->SetPlayerStatus(Player_Status::Catched);

					// 추가로 SaveGame 파일을 불러와서
					if (UHorrorGameSaveGame* SaveData = UHorrorGameSaveGame::LoadObject(this, TEXT("Player"), 0))
					{
						// 플레이어가 처음 Shadow에게 잡힌 것이라면
						if (!SaveData->CollectArchives.CatchedByShadow)
						{
							// Shadow에게 한 번 잡혔다고 변경하고 SetArchiveGetText 메서드로 Shadow의 문서가 추가되었다고 알림.
							SaveData->CollectArchives.CatchedByShadow = true;
							Character->SetArchiveGetText(NSLOCTEXT("AShadow_cpp", "Kill_By_Shadow", "Shadow\nis added in archive"));
							SaveData->SaveData();
						}
					}

					// 그 후 플레이어의 카메라를 강제로 워치 포인트로 옮기고
					Character->OnFocus(WatchPoint->GetComponentLocation());

					// 잡았다고 알림
					SetIsCatch(true);
				}
			}
			// 그 액터가 경보기일 경우 
			if (auto Alarm = Cast<AAlarm>(OtherActor))
			{
				// 경보기 상호 작용을 수행함
				Alarm->AIInteract();
			}
		}
	}
}

// 순찰에 성공했는지 여부
bool AShadow_cpp::GetPatrolSuccess()
{
	return bIsPatrolSuccess;
}

// 요괴의 충돌체 설정 함수
void AShadow_cpp::SetCreatureCollision(bool value)
{
	if (value) // true라면 플레이어가 근처에 있는 것이기 때문에 콜리전(물리적 충돌) 활성화
	{
		GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel9, ECollisionResponse::ECR_Block);
	}
	else // false라면 근처에 플레이어가 없는 것이기 때문에 콜리전(물리적 충돌) 비활성화
	{
		GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel9, ECollisionResponse::ECR_Ignore);
	}
}

// 문을 부수기 위해 멈춘 상태인지 설정
void AShadow_cpp::SetIsStop(bool inIsStop)
{
	bIsStop = inIsStop;
}

bool AShadow_cpp::GetIsStop()
{
	return bIsStop;
}

// Exorcism될 때 Dissolve Material을 구현하기 위한 타임라인 콜백 함수.
void AShadow_cpp::ChangeMaterialInstance(float inValue)
{
	Super::ChangeMaterialInstance(inValue);

	// 1에서 0으로 서서히 Curve Float에 따라 감소하면서 머티리얼의 파라미터와 나이아가라 파티클의 파라미터를 변경함.
	float Amount = FMath::Lerp(1.0f, 0.0f, inValue);
	//float MI_Amount = FMath::Lerp(1.0f, 0.0f, inValue);

	MaterialInstance->SetScalarParameterValue(TEXT("Amount"), Amount);

	//float NC_Amount = FMath::Lerp(1.0f, .0f, inValue);
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

// 문을 여는 동작을 수행할 함수
void AShadow_cpp::OpenDoor()
{
	// RayCast를 통해서 문을 식별하고 열게 할 것이기 때문에 여러 변수 필요

	// Cast된 객체를 가져올 구조체
	FHitResult HitResult;

	// 시작지점과 끝 지점
	FVector Start = GetActorLocation();
	FVector ForwardVector = GetActorForwardVector();
	FVector End = (ForwardVector * 160.f) + Start;
	AActor* HitActor = nullptr;

	// Ray Cast를 수행하고, 그 결과를 Hit에 저장
	bool bIsHit = GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility);
	
	// Hit된 것이 존재하면
	if (bIsHit)
	{
		if (HitResult.GetActor())
		{
			// 그 액터가 존재하면 가져옴
			HitActor = HitResult.GetActor();

			// Cast된 액터가 문이면 문과 상호 작용을 수행함.
			if (ADoor_cpp* Door = Cast<ADoor_cpp>(HitActor))
			{
				Door->AIInteract(this);
			}
			// Cast된 액터가 교실문이면 교실문과 상호 작용을 수행함.
			else if (AClassroomDoorActor_cpp* ClassroomDoor = Cast<AClassroomDoorActor_cpp>(HitActor))
			{
				ClassroomDoor->AIInteract(this);
			}
			// Cast된 액터가 철문이면 철문과 상호 작용을 수행함.
			else if (AMetalDoor_cpp* MetalDoor = Cast <AMetalDoor_cpp>(HitActor))
			{
				MetalDoor->AIInteract(this);
			}
		}
	}
}