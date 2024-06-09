// CopyrightNotice 2023 Sunggon Kim kimdave205@gmail.com. All Rights Reserved.

#include "AI/Brute_cpp.h"
#include "Components/InputComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/PointLightComponent.h"
#include "Components/SpotLightComponent.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "Sound/SoundCue.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AI/AIController_Brute.h"
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
#include "Animation/AnimSequence.h"
#include "Kismet/KismetMathLibrary.h"
#include "NiagaraComponent.h"
#include "ComponentAction/HorrorGameSaveGame.h"

// Sets default values
ABrute_cpp::ABrute_cpp()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	DetectSight = CreateDefaultSubobject<UPointLightComponent>(TEXT("DetectSight"));
	DetectSight->SetupAttachment(GetMesh());
	DetectSight->SetRelativeLocation(FVector(0.f, 50.f, 230.f));
	DetectSight->SetLightFColor(FColor(255.f, 10.f, 10.f));
	DetectSight->SetAttenuationRadius(100.f);

	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("PatrolSoundBox"));
	BoxCollision->SetupAttachment(GetMesh());
	BoxCollision->SetBoxExtent(FVector(20.f, 20.f, 20.f));
	BoxCollision->OnComponentBeginOverlap.AddDynamic(this, &ABrute_cpp::SoundBeginOverlap);
	BoxCollision->OnComponentEndOverlap.AddDynamic(this, &ABrute_cpp::SoundEndOverlap);

	DissolveParticleSystem = CreateDefaultSubobject<UNiagaraComponent>(TEXT("DissolveParticleSystem"));
	DissolveParticleSystem->SetupAttachment(GetMesh());
	DissolveParticleSystem->SetActive(false);
	
	KillSphere = CreateDefaultSubobject<USphereComponent>(TEXT("KillBox"));
	KillSphere->SetupAttachment(GetMesh());
	KillSphere->SetRelativeLocation(FVector(0.f, 0.f, 130.f));
	KillSphere->SetRelativeScale3D(FVector(3.5f, 3.5f, 4.f));
	KillSphere->OnComponentBeginOverlap.AddDynamic(this, &ABrute_cpp::CatchBeginOverlap);

	WatchPoint = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PlayerWatchPoint"));

	AIControllerClass = AAIController_Brute::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	//GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &ABrute_cpp::CheckBoxBeginOverlap);
	GetCapsuleComponent()->SetWorldLocation(FVector(0.f, 0.f, 90.f));
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel9, ECollisionResponse::ECR_Ignore);
	// GetMesh()->SetCollisionProfileName("OverlapAllDynamic");

	bIsCollectMode = true;
}

// Called when the game starts or when spawned
void ABrute_cpp::BeginPlay()
{
	Super::BeginPlay();
	
	// 순찰 지점을 배열에 추가
	if (bIsCollectMode)
	{
		UWorld* world = GetWorld();
		for (TActorIterator<APatrolPoint_cpp> entity(world); entity; ++entity)
		{
			PatrolPointList.Add(*entity);
		}
	}

	// Dissolve Curve가 존재하면 Dissolve Effect를 위한 타임라인 설정
	if (DissolveCurveFloat)
	{
		FOnTimelineFloat TimelineProgress;
		FOnTimelineEvent TimelineFinish;
		TimelineProgress.BindDynamic(this, &ABrute_cpp::ChangeMaterialInstance);
		DissolveTimeline.AddInterpFloat(DissolveCurveFloat, TimelineProgress);

		TimelineFinish.BindDynamic(this, &ABrute_cpp::DissolveFinish);
		DissolveTimeline.SetTimelineFinishedFunc(TimelineFinish);
		DissolveTimeline.SetLooping(false);
	}
}

// Called every frame
void ABrute_cpp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 타임라인에 틱 배정
	DissolveTimeline.TickTimeline(DeltaTime);

	// 문을 바라볼 때 문을 열 수 있는 함수 수행. Raycast를 통해 수행되기 때문에 반드시 Tick에서 실행.
	OpenDoor();

	// 행동 불능 되었을 때 수행
	if (bIsStunned)
	{
		CurrentStunnedTime += DeltaTime;
		// 행동 불능 유지 시간 이상으로 시간이 흘렀으면
		if (CurrentStunnedTime >= stunTime)
		{
			// 행동 불능 종료
			bIsStunned = false;
			// 플레이어가 통과 불가능하게 설정하고 충돌시 바로 사망하도록 콜백 함수 바인딩.
			GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel3, ECollisionResponse::ECR_Block);
			KillSphere->OnComponentBeginOverlap.AddDynamic(this, &ABrute_cpp::CatchBeginOverlap);
			
			// AI Controller가 다시 감각을 사용할 수 있도록 설정 
			AAIController_Brute* controller = Cast<AAIController_Brute>(GetController());
			controller->SetStunned(bIsStunned);
			CurrentStunnedTime = 0;
		}
	}
	
	// 시간 정지되었을 때 수행
	if (bIsTimeStop)
	{
		TimeStopElapsedTime += DeltaTime;
		// 청동 거울 효과가 유지 되는 시간보다 더 시간이 흘렀을 경우
		// 유지 시간은 25초이지만 TimeDilation을 감소시켰기 때문에 이에 대한 보정을 위해 TimeDilation을 곱합.
		if (TimeStopElapsedTime >= 25.f * CustomTimeDilation) // 정지 시간은 25초
		{
			// 시간 정지 종료
			bIsTimeStop = false;
			CustomTimeDilation = 1.0f;
			// 플레이어가 통과 불가능하게 설정하고 충돌 시 바로 사망하도록 콜백 함수 바인딩
			GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel3, ECollisionResponse::ECR_Block);
			KillSphere->OnComponentBeginOverlap.AddDynamic(this, &ABrute_cpp::CatchBeginOverlap);
			
			// AI Controller가 다시 감각을 사용할 수 있도록 설정
			AAIController_Brute* controller = Cast<AAIController_Brute>(GetController());
			controller->SetStunned(bIsTimeStop);
			TimeStopElapsedTime = 0;
		}
	}

	if (AAIController_Brute* BruteController = Cast<AAIController_Brute>(GetController()))
	{
		BruteController->SetControlRotation(GetActorRotation());
	}
}

// Called to bind functionality to input
void ABrute_cpp::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ABrute_cpp::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get Foward Vector
		const FVector FowardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		// get Right Vector
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		AddMovementInput(FowardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void ABrute_cpp::Look(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void ABrute_cpp::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (IsPlayerControlled())
	{
		//	SetControlMode();
		GetCharacterMovement()->MaxWalkSpeed = 180.f;
	}
	else
	{
		//	SetControlMode();
		if (bIsChase) GetCharacterMovement()->MaxWalkSpeed = 300.f;
		else GetCharacterMovement()->MaxWalkSpeed = 160.f;
	}
}

// 추격 시작을 알리는 함수
void ABrute_cpp::StartChase()
{
	// 추격 중이 아닌 경우에 실행되도록 함.
	if (!bIsChase)
	{
		// 추격 중임을 설정하고, 속도를 300으로 올림.
		UE_LOG(LogTemp, Warning, TEXT("SetTrace: Start Chase Function called"));
		bIsChase = true;
		ChangeNoiseRange(false);
		BroadCastChangeNoiseRange(false);
		GetCharacterMovement()->MaxWalkSpeed = 300.f;

		// 플레이어의 상태를 추격 중으로 설정
		AHorrorGameCharacter* PlayerCharacter = Cast<AHorrorGameCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());

		if (PlayerCharacter)
		{
			PlayerCharacter->SetPlayerStatus(Player_Status::Chased);
		}
	}
}

// 추격 끝을 알리는 함수
void ABrute_cpp::EndChase()
{
	// 추격 중일 경우에만 실행되도록 함.
	if (bIsChase)
	{
		// 추격이 끝났음을 설정하고, 속도를 160(default)로 줄임.
		UE_LOG(LogTemp, Warning, TEXT("SetTrace: End Chase Function called"));
		bIsChase = false;
		GetCharacterMovement()->MaxWalkSpeed = 160.f;

		// 플레이어의 상태를 추격 종료로 설정함.
		AHorrorGameCharacter* PlayerCharacter = Cast<AHorrorGameCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());

		if (PlayerCharacter)
		{
			PlayerCharacter->SetPlayerStatus(Player_Status::Survive);
		}
	}
}

// 플레이어를 잡았는지 설정하는 함수
void ABrute_cpp::SetIsCatch(bool Value)
{
	bIsCatch = Value;
}

// 공격 애니메이션이 끝났는지 설정하는 함수
void ABrute_cpp::SetAnimFinish(bool Value)
{
	// 애니메이션이 끝났음을 설정
	bAnimFinish = Value;
	
	// 플레이어의 상태가 숨은 상태라면 생존으로, 아니면 사망으로 설정함.
	UWorld* World = GetWorld();
	AHorrorGamePlayerController* PlayerController = Cast<AHorrorGamePlayerController>(World->GetFirstPlayerController());
	AHorrorGameCharacter* PlayerCharacter = Cast<AHorrorGameCharacter>(PlayerController->GetPawn());

	// 이 때, 플레이어가 숨어있는 곳을 발견해 공격 모션을 취했을 경우 
	if (bIsHidingCatch)
	{
		// 그 숨은 장소를 부숨
		PlayerHidingObject->BreakHideObject();

		// 초기화 후 해당 함수에서 나감
		bIsHidingCatch = false;
		return;
	}
	// 그냥 플레이어를 잡았을 경우
	else
	{
		if (PlayerCharacter->GetIsHiding())
		{
			PlayerCharacter->SetPlayerStatus(Player_Status::Survive);
		}
		else
		{
			PlayerCharacter->SetPlayerStatus(Player_Status::Died);
		}
	}
}

// Box 충돌체가 플레이어와 충돌 시 카메라에 노이즈를 발생시킬 콜백 함수
void ABrute_cpp::SoundBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// 충돌체에 충돌한 액터가 존재할 경우
	if (OtherActor != nullptr && OtherActor != this && OtherComp != nullptr)
	{
		// 그 액터가 플레이어면
		if (auto PlayerCharacter = Cast<AHorrorGameCharacter>(OtherActor))
		{
			// 플레이어 근처의 요괴의 수를 증가시키고 그 수가 0 이상이면 노이즈 단계 1로 설정하고 조명을 깜빡이게 함.
			PlayerCharacter->CreatureNum++;
			if (PlayerCharacter->CreatureNum > 0)
			{
				PlayerCharacter->FlickeringLight.Play();
				PlayerCharacter->SetCameraComponentNoise(1);
			}
			
			// 패닉 상승 쿨 타임이 아니라면 게이지 1을 증가시킴.
			if (!PlayerCharacter->bIsCooldown)
			{
				PlayerCharacter->AddPatience(1);
			}

			// 캡슐 콜라이더의 충돌 채널을 문을 기준으로 Block으로 설정해, Brute가 문을 통과하지 못하도록 설정함.
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
		// ** 캐비닛/옷장의 경우, 숨지 않은 경우일때 충돌했더라도 플레이어의 카메라가 이미 노이즈가 발생했다면, 그 값을 그대로 가져오므로 상관없음.
	}
}

// Box 충돌체와 플레이어 사이의 충돌이 끝났을 때 노이즈 제거용 콜백 함수
void ABrute_cpp::SoundEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	// 충돌체에 충돌이 끝난 액터가 존재할 경우
	if (OtherActor != nullptr && OtherActor != this && OtherComp != nullptr)
	{
		// 그 액터가 플레이어면
		if (auto PlayerCharacter = Cast<AHorrorGameCharacter>(OtherActor))
		{
			// 플레이어 근처의 요괴 수를 1 감소시키고, 그 수가 0 이하라면 깜빡임과 불빛의 세기, 카메라 노이즈 단계를 default로 설정함.
			PlayerCharacter->CreatureNum--;
			if (PlayerCharacter->CreatureNum <= 0)
			{
				PlayerCharacter->FlickeringLight.Stop();
				PlayerCharacter->CigarLight->SetIntensity(PlayerCharacter->CigarIntensity);
				PlayerCharacter->FlashLight->SetIntensity(PlayerCharacter->FlashIntensity);
				PlayerCharacter->SetCameraComponentNoise(0);
			}

			// 캡슐 콜라이더의 충돌 설정을 문을 기준으로 Ignore로 설정해, Brute가 문을 통과하도록 설정함.
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
FVector ABrute_cpp::GetPatrolPoint()
{
	AAIController_Brute* AIController = Cast<AAIController_Brute>(GetController());
	
	 // 0에서 PatrolPointList의 마지막 인덱스까지 중 랜덤 숫자 하나 뽑기
	int randIdx = FMath::RandRange(0, PatrolPointList.Num() - 1); 

	// 뽑아온 랜덤 인덱스의 순찰 지역을 목표 지점으로 설정하고 해당 지점의 위치를 리턴함
	CurrentPatrolPoint = PatrolPointList[randIdx];
	FVector ResultLocation = CurrentPatrolPoint->GetActorLocation();
	AIController->GetBlackboard()->SetValueAsObject(AAIController_Brute::PatrolTargetKey, CurrentPatrolPoint);

	return ResultLocation;
}

// 플레이어가 숨은 오브젝트를 감지했을 때, 컨트롤러에서 제어할 함수
void ABrute_cpp::DetectPlayerHidingObject(class AHideObject* DetectedObject)
{
	PlayerHidingObject = DetectedObject;
}

bool ABrute_cpp::GetIsStunned()
{
	return bIsStunned;
}

// 청동 검을 통해 처치될 때 호출할 함수
void ABrute_cpp::Exorcism()
{
	// 처치될 때 효과음이 있으면 좋을 듯 하다.


	// 사망한 것으로 설정
	bIsDied = true;

	// 나이아가라 시스템을 활성화하고, Dissolve 머티리얼 효과를 줘서 서서히 사라지는 듯한 연출을 함 
	DissolveParticleSystem->Activate(true);
	UMaterialInterface* Material = GetMesh()->GetMaterial(0);
	MaterialInstance = GetMesh()->CreateDynamicMaterialInstance(0, Material);
	MaterialInstance->SetTextureParameterValue(TEXT("Texture"), Texture);
	DissolveParticleSystem->SetVariableTexture(TEXT("Texture"), Texture);
	DissolveTimeline.PlayFromStart();
	
}

// 순찰 성공 여부를 설정하는 함수
void ABrute_cpp::SetPatrolSuccess(bool value)
{
	bIsPatrolSuccess = value;
}

// 행동 불능 상태를 설정하는 함수
void ABrute_cpp::Stunning(float dist)
{
	// 상태가 중첩되지 않게 행동 불능 상태가 아닌 경우에만 수행
	if (!bIsStunned)
	{
		// 이동을 즉시 멈추고 행동 불능 상태로 설정함
		GetMovementComponent()->StopMovementImmediately();
		float LongestStunTime = 18.f; // 최대 행동 불능 시간은 18초
		bIsStunned = true;

		// 플레이어와의 거리가 400 미만이면 18초 그대로 설정
		if (dist < 400.f)
		{
			stunTime = LongestStunTime;
		}
		// 800 초과면 9초로 설정
		else if (dist > 800.f)
		{
			stunTime = LongestStunTime * 0.5f;
		}
		// 400 이상 800 이하일 경우 13.5초로 설정
		else
		{
			stunTime = LongestStunTime * 0.75f;
		}

		// 플레이어가 메시를 통과할 수 있도록 충돌 설정을 Ignore로 설정하고 충돌하더라도 사망하지 않게 콜백 함수를 언바인딩함
		GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel3, ECollisionResponse::ECR_Ignore);
		KillSphere->OnComponentBeginOverlap.RemoveDynamic(this, &ABrute_cpp::CatchBeginOverlap);

		// AI Controller가 행동 불능된 동안에는 감각을 사용하지 못하게 알림.
		AAIController_Brute* controller = Cast<AAIController_Brute>(GetController());
		controller->SetStunned(bIsStunned);
	}
}

// 플레이어가 청동 거울을 사용할 때 호출할 함수
void ABrute_cpp::SetStun()
{
	// 즉시 이동을 멈추고 플레이어와의 충돌을 Ignore로 만들어 통과 가능하게 하면서, 시간 정지로 설정함.
	GetMovementComponent()->StopMovementImmediately();
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel3, ECollisionResponse::ECR_Ignore);
	bIsTimeStop = true; // Stop Movement and Stun!
	CustomTimeDilation = 0.0001f; // 브루트의 시간은 0.0001로 만들어 속도 줄이고 멈춘 것처럼 표현

	// 플레이어를 죽이는 트리거가 있는 Sphere 콜라이더에 부착된 콜백 함수를 제거함
	KillSphere->OnComponentBeginOverlap.RemoveDynamic(this, &ABrute_cpp::CatchBeginOverlap);
	
	// 그동안 컨트롤러가 아무것도 감지하지 못하게 하기 위해 시간 정지 장태로 알림.
	AAIController_Brute* controller = Cast<AAIController_Brute>(GetController());
	controller->SetStunned(bIsTimeStop);
}

bool ABrute_cpp::GetIsDied()
{
	return bIsDied;
}

bool ABrute_cpp::GetIsCatch()
{
	return bIsCatch;
}

bool ABrute_cpp::GetAnimFinish()
{
	return bAnimFinish;
}

// Sphere 충돌체에 플레이어가 충돌한 경우 호출할 콜백 함수.
void ABrute_cpp::CatchBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherIndexBody, bool bFromSweep, const FHitResult& SweepResult)
{
	/*UE_LOG(LogTemp, Warning, TEXT("Something Catch"));*/
	// 플레이어가 캐비닛이나 옷장에 숨어있는 걸 알고 있는지 파악
	AAIController_Brute* AIController = Cast<AAIController_Brute>(GetController());
	bool HideCatch = false;
	if (AIController != nullptr)
	{
		HideCatch = AIController->GetBlackboard()->GetValueAsBool(AAIController_Brute::LockerLighting);
	}
	
	// Brute가 행동 불능이거나 죽은 상태, 시간 정지 상태가 아니라면 실행함
	if (!bIsStunned && !bIsDied && !bIsTimeStop)
	{
		// 충돌한 액터가 존재할 경우
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
			// 그 액터가 플레이어면
			if (auto PlayerCharacter = Cast<AHorrorGameCharacter>(OtherActor))
			{
				// 플레이어의 상태가 Survive이거나, 추격 중(Chased)이면서 동시에 숨은 상태가 아닌 경우에만 수행
				if ((PlayerCharacter->GetPlayerStatus() == Player_Status::Survive || PlayerCharacter->GetPlayerStatus() == Player_Status::Chased) && !PlayerCharacter->GetIsHiding())
				{
					// 플레이어의 상태를 Catch로 설정함.
					PlayerCharacter->SetPlayerStatus(Player_Status::Catched);

					// 추가로 SaveGame 파일을 불러와서
					if (UHorrorGameSaveGame* SaveData = UHorrorGameSaveGame::LoadObject(this, TEXT("Player"), 0))
					{
						// 플레이어가 처음 Brute에게 잡힌 것이라면
						if (!SaveData->CollectArchives.CatchedByBrute)
						{
							// Brute에게 잡혔다고 설정하고, SetArchiveGetText 메서드를 통해 Brute의 문서가 추가되었다고 알림.
							SaveData->CollectArchives.CatchedByBrute = true;
							PlayerCharacter->SetArchiveGetText(NSLOCTEXT("ABrute_cpp", "Kill_By_Brute", "Brute\nis added in archive"));
							SaveData->SaveData();
						}
					}

					// 그 후 플레이어의 카메라를 강제로 워치 포인트를 향하도록 옮기고
					PlayerCharacter->OnFocus(WatchPoint->GetComponentLocation());

					// 잡았다고 알림.
					SetIsCatch(true);

				}
			}
			// 그 액터가 경보기일 경우
			if (auto Alarm = Cast<AAlarm>(OtherActor))
			{
				// 상호작용을 수행함.
				Alarm->AIInteract();
			}
		}
	}
}

// 소음 감지 범위를 늘리는 함수
void ABrute_cpp::BroadCastChangeNoiseRange(const bool value)
{
	if (value)
	{
		bCalledRangeChange = true;
	}
	else
	{
		bCalledRangeChange = false;
	}
}

// 소음 감지 범위를 늘리는 함수
void ABrute_cpp::ChangeNoiseRange(const bool value)
{
	if (bIsChase) // 현재 추격 중이라면
	{
		Cast<AAIController_Brute>(GetController())->GetBlackboard()->SetValueAsBool(AAIController_Brute::ChangeDetectRange, false);
		UE_LOG(LogTemp, Warning, TEXT("Change Noise Range false Called!"));
		NoiseDetectRange = 2400.f;
		bIsRangeChange = value;
	}
	// 현재 추격 중이 아닌 상태일 때만 범위가 변경되도록 설정함.
	else
	{
		if (value)
		{
			// 걷는 소음 감지 시 멈춰서 그르릉 거리도록 설정
			GetCharacterMovement()->StopMovementImmediately();
			Cast<AAIController_Brute>(GetController())->GetBlackboard()->SetValueAsBool(AAIController_Brute::ChangeDetectRange, true);
			UE_LOG(LogTemp, Warning, TEXT("Change Noise Range true Called!"));
			NoiseDetectRange = 2800.f;
			bIsRangeChange = value;
		}

		else
		{
			Cast<AAIController_Brute>(GetController())->GetBlackboard()->SetValueAsBool(AAIController_Brute::ChangeDetectRange, false);
			UE_LOG(LogTemp, Warning, TEXT("Change Noise Range false Called!"));
			NoiseDetectRange = 2400.f;
			bIsRangeChange = value;
		}
	}
}

float ABrute_cpp::GetNoiseRange()
{
	return NoiseDetectRange;
}

bool ABrute_cpp::GetPatrolSuccess()
{
	return bIsPatrolSuccess;
}

void ABrute_cpp::SetCreatureCollision(bool value)
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

void ABrute_cpp::SetIsStop(bool inIsStop)
{
	bIsStop = inIsStop;
}

bool ABrute_cpp::GetIsStop()
{
	return bIsStop;
}

// Exorcism될 때 Dissolve Material을 구현하기 위한 타임라인 콜백 함수.
void ABrute_cpp::ChangeMaterialInstance(float inValue)
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
void ABrute_cpp::DissolveFinish()
{
	Super::DissolveFinish();

	// 파티클을 비활성화시키고 해당 캐릭터를 제거함
	DissolveParticleSystem->SetVariableFloat(TEXT("Width"), 0.0f);
	
	DissolveParticleSystem->Deactivate();

	Destroy();
}

// 문을 여는 동작을 수행할 함수
void ABrute_cpp::OpenDoor()
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