// CopyrightNotice=0 2023 Sunggon Kim kimdave205@gmail.com

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
#include "PatrolPoint_cpp.h"
#include "EngineUtils.h"
#include "Wardrobe_cpp.h"
#include "Cabinet_cpp.h"
#include "HideAndSeek/HorrorGameCharacter.h"
#include "HorrorGamePlayerController.h"
#include "ClassroomDoorActor_cpp.h"
#include "Door_cpp.h"
#include "MetalDoor_cpp.h"
#include "Alarm.h"
#include "Animation/AnimSequence.h"
#include "Kismet/KismetMathLibrary.h"
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

	/*static ConstructorHelpers::FClassFinder<UReaperAnim>AnimAsset(TEXT("/Game/Assets/AI/Reaper/BP_ReaperAnim"));
	if (AnimAsset.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(AnimAsset.Class);
	}*/

	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("PatrolSoundBox"));
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

	/*GetMesh()->SetRelativeLocation(FVector(0.f, 0.f, 0.f));
	GetMesh()->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));*/

	AIControllerClass = AAIController_Rampage::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	GetCapsuleComponent()->SetWorldLocation(FVector(0.f, 0.f, 70.f));

	/*static ConstructorHelpers::FObjectFinder<UAnimSequence>anim(TEXT("/Game/ParagonSevarog/Characters/Heroes/Sevarog/Animations/LevelStart"));
	if (anim.Succeeded())
	{
		Anim = anim.Object;
	}*/

}

// Called when the game starts or when spawned
void ARampage_cpp::BeginPlay()
{
	Super::BeginPlay();

	// GetCapsuleComponent()->OnComponentHit.AddDynamic(this, &AReaper_cpp::CatchBeginOverlap);

	UWorld* world = GetWorld();
	
	Index = 0;
	MapName = UGameplayStatics::GetCurrentLevelName(GetWorld());
	RampageController = Cast<AAIController_Rampage>(GetController());

	if (bIsCinematic) // 시네마틱 모드인 경우(챕터 2 클리어 후 등장한 경우)
	{
		RampageController->GetBlackboard()->SetValueAsBool(AAIController_Rampage::IsCinematic, bIsCinematic);
		Player = Cast<AHorrorGameCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
		RampageController->GetBlackboard()->SetValueAsObject(AAIController_Rampage::TargetKey, Player);
	}
	/*if (DissolveCurveFloat)
	{
		FOnTimelineFloat TimelineProgress;
		FOnTimelineEvent TimelineFinish;
		TimelineProgress.BindDynamic(this, &AReaper_cpp::ChangeMaterialInstance);
		DissolveTimeline.AddInterpFloat(DissolveCurveFloat, TimelineProgress);

		TimelineFinish.BindDynamic(this, &AReaper_cpp::DissolveFinish);
		DissolveTimeline.SetTimelineFinishedFunc(TimelineFinish);
		DissolveTimeline.SetLooping(false);
	}*/
}

// Called every frame
void ARampage_cpp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

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

	//if (bIsCollectMode) // 오브젝트 수집 모드이라면
	//{
	//	switch (UnSealedItemNumber)
	//	{
	//	case 0: // 아직 아무런 사신의 물품을 얻지 못했을 때
	//		ReaperController->SetCurrentSealStatus(Sealed::Sealed);
	//		break;
	//	case 1: // 사신의 물품을 하나 얻었을 때
	//		ReaperController->SetCurrentSealStatus(Sealed::OneUnsealed);
	//		break;
	//	case 2: // 사신의 물품을 두 개 얻었을 때
	//		ReaperController->SetCurrentSealStatus(Sealed::TwoUnsealed);
	//		ReaperController->GetBlackboard()->SetValueAsObject(ACreatureAI::TargetKey, Player);
	//		break;
	//	default: // 사신의 물품을 세 개 이상 얻었을 때
	//		ReaperController->SetCurrentSealStatus(Sealed::Unsealed);
	//		ReaperController->GetBlackboard()->SetValueAsObject(ACreatureAI::TargetKey, Player);
	//	}
	//}
	//else // 그 외 모드라면
	//{
	//	if (bSealedButChase)
	//	{
	//		ReaperController->GetBlackboard()->SetValueAsObject(ACreatureAI::TargetKey, Player);
	//	}
	//}

	//if (bIsPlayerWatch && !bIsCooldown && !bIsStunned)
	//{
	//	CastingTime += DeltaTime;

	//	if (CastingTime >= 3)
	//	{
	//		bIsCooldown = true;
	//		CastingTime = 0.f;
	//	}
	//}

	//if (bIsCooldown)
	//{
	//	SkillCooldown += DeltaTime;

	//	if (SkillCooldown >= 10)
	//	{
	//		SkillCooldown = 0.f;
	//		bIsCooldown = false;
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
	/*ACreatureAI* Controller = Cast<ACreatureAI>(GetController());
	Controller->GetBlackboard()->SetValueAsBool(ACreatureAI::Stunned, bIsStunned);*/
}

void ARampage_cpp::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (IsPlayerControlled())
	{
		//	SetControlMode();
		GetCharacterMovement()->MaxWalkSpeed = 160.f;
	}
	else
	{
		//	SetControlMode();
	//	if (bIsChase) GetCharacterMovement()->MaxWalkSpeed = 300.f;
		GetCharacterMovement()->MaxWalkSpeed = 280.f;
	}
}


void ARampage_cpp::StartChase()
{
	if (!bIsChase)
	{
		bIsChase = true;
		//ReaperSound->SetSound(DetectedSound);
		//ReaperSound->Play();
		//GetCharacterMovement()->MaxWalkSpeed = 160.f;
		// AHorrorGameCharacter* player = Cast<AHorrorGameCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());

		if (Player)
		{
			Player->SetPlayerStatus(Player_Status::Chased);
			/*Player->SetCameraComponentNoise(2);
			if (!Player->bIsCooldown)
			{
				Player->AddPatience(15);
			}*/
		}
	}

	/*ReaperSound->SetSound(DetectedSound);
	ReaperSound->Play();
	GetCharacterMovement()->MaxWalkSpeed = 300.f;*/
}

void ARampage_cpp::EndChase()
{
	if (bIsChase)
	{
		// ACreatureAI* AIController = Cast<ACreatureAI>(GetController());
		if (RampageController != nullptr)
		{
			// AIController->EndChase();
		}
		bIsChase = false;
		//	ReaperSound->SetSound(PatrolSound);

		if (Player)
		{
			Player->SetPlayerStatus(Player_Status::Survive);
		}
	}

	/*ACreatureAI* AIController = Cast<ACreatureAI>(GetController());
	if (AIController != nullptr)
	{
		AIController->EndChase();
	}
	ReaperSound->SetSound(PatrolSound);
	GetCharacterMovement()->MaxWalkSpeed = 160.f;*/
}

void ARampage_cpp::SetIsCatch(bool Value)
{
	bIsCatch = Value;
}

void ARampage_cpp::SetAnimFinish(bool Value)
{
	bAnimFinish = Value;
	//UWorld* World = GetWorld();
	//AHorrorGamePlayerController* PlayerController = Cast<AHorrorGamePlayerController>(World->GetFirstPlayerController());
	//AHorrorGameCharacter* Player = Cast<AHorrorGameCharacter>(PlayerController->GetPawn());

	if (Player->GetIsHiding())
		Player->SetPlayerStatus(Player_Status::Survive);
	else
		Player->SetPlayerStatus(Player_Status::Died);
}

void ARampage_cpp::SoundBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor != nullptr && OtherActor != this && OtherComp != nullptr)
	{
		if (auto PlayerActor = Cast<AHorrorGameCharacter>(OtherActor))
		{
			PlayerActor->CreatureNum++;
			if (PlayerActor->CreatureNum > 0)
			{
				PlayerActor->FlickeringLight.Play();
				PlayerActor->SetCameraComponentNoise(1);
			}
			//ReaperSound->Play();
			if (!PlayerActor->bIsCooldown)
			{
				PlayerActor->AddPatience(1);
			}
			GetMesh()->SetCollisionProfileName("AICharacters");
			//InteractBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
			//InteractBox->OnComponentBeginOverlap.AddDynamic(this, &AReaper_cpp::DoorBeginOverlap);
		}

		if (auto Cabinet = Cast<ACabinet_cpp>(OtherActor))
		{
			Cabinet->CreatureNum++;
			/*if (Cabinet->CreatureNum > 0 && Cabinet->bIsHiding)
				Cabinet->FlickeringLight.Play();*/
				//ReaperSound->Play();
			if (Cabinet->bIsHiding)
			{
				Cabinet->SetCameraComponentNoise(1);
			}
			/*if (Cabinet->Player != nullptr) // 원래는 숨어있을 때도 긴장도 증가였지만, 없애기로 함
				if (!Cabinet->Player->bIsCooldown)
					Cabinet->Player->Patience += 5;*/
		}

		if (auto Wardrobe = Cast<AWardrobe_cpp>(OtherActor))
		{
			Wardrobe->CreatureNum++;
			/*if (Wardrobe->CreatureNum > 0 && Wardrobe->bIsHiding)
				Wardrobe->FlickeringLight.Play();*/
				//ReaperSound->Play();
			if (Wardrobe->bIsHiding)
			{
				Wardrobe->SetCameraComponentNoise(1);
			}
			/*if (Wardrobe->Player != nullptr) // 원래는 숨어있을 때도 긴장도 증가였지만, 없애기로 함
				if (!Wardrobe->Player->bIsCooldown)
					Wardrobe->Player->Patience += 5;*/
		}
	}
}

void ARampage_cpp::SoundEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor != nullptr && OtherActor != this && OtherComp != nullptr)
	{
		if (auto PlayerActor = Cast<AHorrorGameCharacter>(OtherActor))
		{
			//ReaperSound->Stop();
			PlayerActor->CreatureNum--;
			if (PlayerActor->CreatureNum <= 0)
			{
				PlayerActor->FlickeringLight.Stop();
				PlayerActor->CigarLight->SetIntensity(PlayerActor->CigarIntensity);
				PlayerActor->FlashLight->SetIntensity(PlayerActor->FlashIntensity);
				PlayerActor->SetCameraComponentNoise(0);
			}
			//GetMesh()->SetCollisionProfileName("OverlapAllDynamic");
			//InteractBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			//InteractBox->OnComponentBeginOverlap.RemoveDynamic(this, &AReaper_cpp::DoorBeginOverlap);
		}

		if (auto Cabinet = Cast<ACabinet_cpp>(OtherActor))
		{
			//ReaperSound->Stop();
			Cabinet->CreatureNum--;
			if (Cabinet->CreatureNum <= 0)
			{
				Cabinet->FlickeringLight.Stop();
				Cabinet->CigarLight->SetIntensity(Cabinet->Intensity);
				Cabinet->FlashLight->SetIntensity(Cabinet->Intensity);
				Cabinet->SetCameraComponentNoise(0);
			}
		}

		if (auto Wardrobe = Cast<AWardrobe_cpp>(OtherActor))
		{
			//ReaperSound->Stop();
			Wardrobe->CreatureNum--;
			if (Wardrobe->CreatureNum <= 0)
			{
				Wardrobe->FlickeringLight.Stop();
				Wardrobe->CigarLight->SetIntensity(Wardrobe->Intensity);
				Wardrobe->FlashLight->SetIntensity(Wardrobe->Intensity);
				Wardrobe->SetCameraComponentNoise(0);
			}
		}
	}
}

void ARampage_cpp::CheckBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor != nullptr && OtherActor != this && OtherComp != nullptr)
	{
		if (OtherActor == CurrentPatrolPoint)
		{
			SetPatrolSuccess(true);
		}
	}
}

FVector ARampage_cpp::GetPatrolPoint()
{
	/*UWorld* world = GetWorld();
	FVector ResultLocation = GetActorLocation();*/
	//AAIController_Rampage* AIController = Cast<ACreatureAI>(GetController());
	FVector ResultLocation(0.f);
	
	CurrentPatrolPoint = PatrolPointLists[Index++]; // 현재 인덱스의 패트롤 포인트 인스턴스를 지정해주고 인덱스 증가
	ResultLocation = CurrentPatrolPoint->GetActorLocation();
	RampageController->GetBlackboard()->SetValueAsObject(AAIController_Rampage::PatrolTargetKey, CurrentPatrolPoint);
	if (Index == PatrolPointLists.Num()) // 만약 인덱스가 PatrolPointLists의 최대 인덱스를 넘어가면
	{
		Index = 0; // 다시 0으로 초기화
	}

	return ResultLocation;
}

bool ARampage_cpp::GetIsStunned()
{
	return bIsStunned;
}

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

void ARampage_cpp::SetPatrolSuccess(bool value)
{
	bIsPatrolSuccess = value;
}

void ARampage_cpp::Stunning(float dist)
{
	if (!bIsStunned)
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

		//ACreatureAI* controller = Cast<ACreatureAI>(GetController());
		GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel3, ECollisionResponse::ECR_Ignore);
		KillBox->OnComponentBeginOverlap.RemoveDynamic(this, &ARampage_cpp::CatchBeginOverlap);
		RampageController->SetStunned(bIsStunned);
	}
}

void ARampage_cpp::SetStun()
{
	GetMovementComponent()->StopMovementImmediately();
	bIsTimeStop = true; // Stop Movement and Stun!
	CustomTimeDilation = 0.0001f; // 브루트의 시간은 0.0001로 만들어 속도 줄이고 멈춘 것처럼 표현
	//stunTime = 25.f; // 기절 시간은 25초로
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel3, ECollisionResponse::ECR_Ignore);
	KillBox->OnComponentBeginOverlap.RemoveDynamic(this, &ARampage_cpp::CatchBeginOverlap);
	RampageController->SetStunned(bIsTimeStop);
}

bool ARampage_cpp::GetIsDied()
{
	return bIsDied;
}

bool ARampage_cpp::GetIsCatch()
{
	return bIsCatch;
}

bool ARampage_cpp::GetAnimFinish()
{
	return bAnimFinish;
}

void ARampage_cpp::CatchBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherIndexBody, bool bFromSweep, const FHitResult& SweepResult)
{
	// ACreatureAI* AIController = Cast<ACreatureAI>(GetController());
	bool HideCatch = false;
	if (RampageController != nullptr)
	{
		HideCatch = RampageController->GetBlackboard()->GetValueAsBool(AAIController_Rampage::LockerLighting);
	}

	if (!bIsStunned)
	{
		if (OtherActor != this && OtherActor != nullptr && OtherComp != nullptr)
		{

			if (auto Cabinet = Cast<ACabinet_cpp>(OtherActor))
			{
				if (bIsChase && HideCatch)
				{
					SetIsCatch(true);
					if (GetAnimFinish())
					{
						Cabinet->BreakCabinet();
					}
				}
			}
			if (auto Wardrobe = Cast<AWardrobe_cpp>(OtherActor))
			{
				if (bIsChase && HideCatch)
				{
					SetIsCatch(true);
					if (GetAnimFinish())
					{
						Wardrobe->BreakWardrobe();
					}
				}
			}
			if (auto Character = Cast<AHorrorGameCharacter>(OtherActor))
			{
				if ((Character->GetPlayerStatus() == Player_Status::Survive || Character->GetPlayerStatus() == Player_Status::Chased) && !Character->GetIsHiding())
				{
					Character->SetPlayerStatus(Player_Status::Catched);

					/*if (UHorrorGameSaveGame* SaveData = UHorrorGameSaveGame::LoadObject(this, TEXT("Player"), 0))
					{
						if (!SaveData->CatchedByReaper)
						{
							SaveData->CatchedByReaper = true;
							SaveData->SaveData();
						}
					}*/

					Character->OnFocus(GetActorLocation());
					//Character->SetArchiveGetText(NSLOCTEXT("ARampage_cpp", "Kill_By_Rampage", "Rampage\nis added in archive"));

					SetIsCatch(true);
				}
			}

			if (auto Alarm = Cast<AAlarm>(OtherActor))
			{
				Alarm->AIInteract();
			}
		}
	}
}

bool ARampage_cpp::GetPatrolSuccess()
{
	return bIsPatrolSuccess;
}

void ARampage_cpp::SetCreatureCollision(bool value)
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

void ARampage_cpp::SetIsStop(bool inIsStop)
{
	bIsStop = inIsStop;
}

bool ARampage_cpp::GetIsStop()
{
	return bIsStop;
}

//void AReaper_cpp::ChangeMaterialInstance(float inValue)
//{
//	Super::ChangeMaterialInstance(inValue);
//
//	float Amount = FMath::Lerp(1.0f, 0.0f, inValue);
//	//float MI_Amount = FMath::Lerp(1.0f, 0.0f, inValue);
//
//	for (int i = 0; i < MaterialInstances.Num(); ++i)
//	{
//		MaterialInstances[i]->SetScalarParameterValue(TEXT("Amount"), Amount);
//	}
//
//	//MPC_Reaper->SetScalarParameterValue(TEXT("Amount"), Amount);
//	//UMaterialParameterCollectionInstance* PCI_Reaper = GetWorld()->GetParameterCollectionInstance(MPC_Reaper);
//	//PCI_Reaper->SetScalarParameterValue(TEXT("Amount"), Amount);
//	//float NC_Amount = FMath::Lerp(1.0f, .0f, inValue);
//	DissolveParticleSystem->SetVariableFloat(TEXT("Amount"), Amount);
//}

//void AReaper_cpp::DissolveFinish()
//{
//	Super::DissolveFinish();
//
//	DissolveParticleSystem->SetVariableFloat(TEXT("Width"), 0.0f);
//
//	DissolveParticleSystem->Deactivate();
//
//
//	Destroy();
//}