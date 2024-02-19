// CopyrightNotice=0 2023 Sunggon Kim kimdave205@gmail.com

#include "AI/Reaper_cpp.h"
#include "Components/InputComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/PointLightComponent.h"
#include "Components/SpotLightComponent.h"
#include "Components/AudioComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/BoxComponent.h"
#include "Sound/SoundCue.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubSystems.h"
#include "AI/CreatureAI.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "PatrolPoint_cpp.h"
#include "EngineUtils.h"
#include "AI/ReaperAnim.h"
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
AReaper_cpp::AReaper_cpp()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<USkeletalMesh>MeshAsset(TEXT("/Game/ParagonSevarog/Characters/Heroes/Sevarog/Meshes/Sevarog"));
	if (MeshAsset.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(MeshAsset.Object);
	}

	static ConstructorHelpers::FClassFinder<UReaperAnim>AnimAsset(TEXT("/Game/Assets/AI/Reaper/BP_ReaperAnim"));
	if (AnimAsset.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(AnimAsset.Class);
	}

	DetectSight = CreateDefaultSubobject<UPointLightComponent>(TEXT("DetectSight"));
	DetectSight->SetupAttachment(GetMesh());
	DetectSight->SetRelativeLocation(FVector(0.f, 50.f, 230.f));
	DetectSight->SetLightFColor(FColor(255.f, 10.f, 10.f));
	DetectSight->SetAttenuationRadius(100.f);

	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("PatrolSoundBox"));
	BoxCollision->SetupAttachment(GetMesh());
	BoxCollision->SetBoxExtent(FVector(20.f, 20.f, 20.f));
	BoxCollision->OnComponentBeginOverlap.AddDynamic(this, &AReaper_cpp::SoundBeginOverlap);
	BoxCollision->OnComponentEndOverlap.AddDynamic(this, &AReaper_cpp::SoundEndOverlap);

	DissolveParticleSystem = CreateDefaultSubobject<UNiagaraComponent>(TEXT("DissolveParticleSystem"));
	DissolveParticleSystem->SetupAttachment(GetMesh());
	DissolveParticleSystem->SetActive(false);
	/*InteractBox = CreateDefaultSubobject<UBoxComponent>(TEXT("InteractBox"));
	InteractBox->SetupAttachment(GetMesh());
	InteractBox->SetBoxExtent(FVector(5.f, 5.f, 5.f));*/
	//InteractBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	//InteractBox->OnComponentBeginOverlap.AddDynamic(this, &AReaper_cpp::DoorBeginOverlap);

	KillBox = CreateDefaultSubobject<UBoxComponent>(TEXT("KillBox"));
	KillBox->SetupAttachment(GetMesh());
	KillBox->SetRelativeLocation(FVector(0.f, 0.f, 130.f));
	KillBox->SetRelativeScale3D(FVector(3.5f, 3.5f, 4.f));
	KillBox->OnComponentBeginOverlap.AddDynamic(this, &AReaper_cpp::CatchBeginOverlap);

	ReaperSound = CreateDefaultSubobject<UAudioComponent>(TEXT("ChasedSound"));
	ReaperSound->SetupAttachment(GetMesh());
	static ConstructorHelpers::FObjectFinder<USoundCue>DetectedCue(TEXT("/Game/Assets/Sounds/SoundCues/DetecedCue"));
	if (DetectedCue.Succeeded())
	{
		DetectedSound = DetectedCue.Object;
	}

	static ConstructorHelpers::FObjectFinder<USoundCue>PatrolCue(TEXT("/Game/Assets/Sounds/SoundCues/GhostSound"));
	if (PatrolCue.Succeeded())
	{
		PatrolSound = PatrolCue.Object;
	}
	ReaperSound->SetAutoActivate(false);

	GetMesh()->SetRelativeLocation(FVector(0.f, 0.f, 0.f));
	GetMesh()->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));

	AIControllerClass = ACreatureAI::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	// GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &AReaper_cpp::CheckBoxBeginOverlap);
	GetCapsuleComponent()->SetWorldLocation(FVector(0.f, 0.f, 70.f));
//	GetMesh()->SetCollisionProfileName("OverlapAllDynamic");
	
	static ConstructorHelpers::FObjectFinder<UAnimSequence>anim(TEXT("/Game/ParagonSevarog/Characters/Heroes/Sevarog/Animations/LevelStart"));
	if (anim.Succeeded())
	{
		Anim = anim.Object;
	}

	bIsCollectMode = true;
	UnSealedItemNumber = 0;
}

// Called when the game starts or when spawned
void AReaper_cpp::BeginPlay()
{
	Super::BeginPlay();

	// GetCapsuleComponent()->OnComponentHit.AddDynamic(this, &AReaper_cpp::CatchBeginOverlap);

	UWorld* world = GetWorld();
	if (bIsCollectMode)
	{
		for (TActorIterator<APatrolPoint_cpp> entity(world); entity; ++entity)
		{
			PatrolPointLists.Add(*entity);
		}
		Player = Cast<AHorrorGameCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
	}

	ReaperSound->SetSound(PatrolSound);
	Index = 0;
	MapName = UGameplayStatics::GetCurrentLevelName(GetWorld());
	ReaperController = Cast<ACreatureAI>(GetController());
	ReaperController->SetCurrentSealStatus(Sealed::OneUnsealed);

	if (DissolveCurveFloat)
	{
		FOnTimelineFloat TimelineProgress;
		FOnTimelineEvent TimelineFinish;
		TimelineProgress.BindDynamic(this, &AReaper_cpp::ChangeMaterialInstance);
		DissolveTimeline.AddInterpFloat(DissolveCurveFloat, TimelineProgress);

		TimelineFinish.BindDynamic(this, &AReaper_cpp::DissolveFinish);
		DissolveTimeline.SetTimelineFinishedFunc(TimelineFinish);
		DissolveTimeline.SetLooping(false);
	}
}

// Called every frame
void AReaper_cpp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	DissolveTimeline.TickTimeline(DeltaTime);

	if (bIsStunned)
	{
		CurrentStunnedTime += DeltaTime;
		if (CurrentStunnedTime >= stunTime)
		{
			bIsStunned = false;
			GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel3, ECollisionResponse::ECR_Block);
			KillBox->OnComponentBeginOverlap.AddDynamic(this, &AReaper_cpp::CatchBeginOverlap);
			//ACreatureAI* controller = Cast<ACreatureAI>(GetController());
			ReaperController->SetStunned(bIsStunned);
			CurrentStunnedTime = 0;
		}
	}
	
	if (bIsCollectMode) // 오브젝트 수집 모드이라면
	{
		switch (UnSealedItemNumber)
		{
		case 0: // 아직 아무런 사신의 물품을 얻지 못했을 때
			ReaperController->SetCurrentSealStatus(Sealed::Sealed);
			break;
		case 1: // 사신의 물품을 하나 얻었을 때
			ReaperController->SetCurrentSealStatus(Sealed::OneUnsealed);
			break;
		case 2: // 사신의 물품을 두 개 얻었을 때
			ReaperController->SetCurrentSealStatus(Sealed::TwoUnsealed);
			ReaperController->GetBlackboard()->SetValueAsObject(ACreatureAI::TargetKey, Player);
			break;
		default: // 사신의 물품을 세 개 이상 얻었을 때
			ReaperController->SetCurrentSealStatus(Sealed::Unsealed);
			ReaperController->GetBlackboard()->SetValueAsObject(ACreatureAI::TargetKey, Player);
		}
	}
	else // 그 외 모드라면
	{
		if (bSealedButChase)
		{
			ReaperController->GetBlackboard()->SetValueAsObject(ACreatureAI::TargetKey, Player);
		}
	}

	if (bIsPlayerWatch && !bIsCooldown && !bIsStunned)
	{
		CastingTime += DeltaTime;

		if (CastingTime >= 3)
		{
			bIsCooldown = true;
			CastingTime = 0.f;
		}
	}

	if (bIsCooldown)
	{
		SkillCooldown += DeltaTime;

		if (SkillCooldown >= 10)
		{
			SkillCooldown = 0.f;
			bIsCooldown = false;
		}
	}

	if (bIsTimeStop)
	{
		TimeStopElapsedTime += DeltaTime;
		if (TimeStopElapsedTime >= 25.f * CustomTimeDilation) // 정지 시간은 25초
		{
			bIsTimeStop = false;
			CustomTimeDilation = 1.0f;
			GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel3, ECollisionResponse::ECR_Block);
			KillBox->OnComponentBeginOverlap.AddDynamic(this, &AReaper_cpp::CatchBeginOverlap);
			//ACreatureAI* controller = Cast<ACreatureAI>(GetController());
			ReaperController->SetStunned(bIsTimeStop);
			TimeStopElapsedTime = 0;
		}
	}

	/*if (ACreatureAI* ReaperAI = Cast<ACreatureAI>(GetController()))
	{
		ReaperAI->SetControlRotation(GetActorRotation());
	}*/
	ReaperController->SetControlRotation(GetActorRotation());
	/*ACreatureAI* Controller = Cast<ACreatureAI>(GetController());
	Controller->GetBlackboard()->SetValueAsBool(ACreatureAI::Stunned, bIsStunned);*/
}

// Called to bind functionality to input
void AReaper_cpp::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Move
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AReaper_cpp::Move);
		// Look
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AReaper_cpp::Look);
	}
}

void AReaper_cpp::Move(const FInputActionValue& Value)
{
	// Super::Move(Value);
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

void AReaper_cpp::Look(const FInputActionValue& Value)
{
	// Super::Look(Value);
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void AReaper_cpp::PossessedBy(AController* NewController)
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
		if (bIsChase) GetCharacterMovement()->MaxWalkSpeed = 300.f;
		else GetCharacterMovement()->MaxWalkSpeed = 160.f;
	}
}

void AReaper_cpp::StartChase()
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

void AReaper_cpp::EndChase()
{
	if (bIsChase)
	{
		// ACreatureAI* AIController = Cast<ACreatureAI>(GetController());
		if (ReaperController != nullptr)
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

void AReaper_cpp::SetIsCatch(bool Value)
{
	bIsCatch = Value;
}

void AReaper_cpp::SetAnimFinish(bool Value)
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

void AReaper_cpp::SoundBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
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

void AReaper_cpp::SoundEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
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

// AI Interaction Box
//void AReaper_cpp::DoorBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
//{
//	if (OtherActor != nullptr && OtherActor != this && OtherComp != nullptr)
//	{
//		if (auto Classroom = Cast<AClassroomDoorActor_cpp>(OtherActor))
//		{
//			Classroom->AIInteract(this);
//		}
//		else if (auto Door = Cast<ADoor_cpp>(OtherActor))
//		{
//			Door->AIInteract(this);
//		}
//		else if (auto MetalDoor = Cast<AMetalDoor_cpp>(OtherActor))
//		{
//			MetalDoor->AIInteract(this);
//		}
//	}
//}

void AReaper_cpp::CheckBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor != nullptr && OtherActor != this && OtherComp != nullptr)
	{
		if (OtherActor == CurrentPatrolPoint)
		{
			SetPatrolSuccess(true);
		}
	}
}

FVector AReaper_cpp::GetPatrolPoint()
{
	/*UWorld* world = GetWorld();
	FVector ResultLocation = GetActorLocation();*/
	ACreatureAI* AIController = Cast<ACreatureAI>(GetController());
	FVector ResultLocation(0.f);

	if (bIsCollectMode) // 현재 레벨이 오브젝트 수집 레벨일 때
	{
		int randIdx = FMath::RandRange(0, PatrolPointLists.Num() - 1); // 0에서 PatrolPointLists의 마지막 인덱스까지 중 랜덤 숫자 하나 뽑기
		CurrentPatrolPoint = PatrolPointLists[randIdx];
		ResultLocation = CurrentPatrolPoint->GetActorLocation();
		ReaperController->GetBlackboard()->SetValueAsObject(ACreatureAI::PatrolTargetKey, CurrentPatrolPoint);
	}
	else // 정해진 길을 따라 가는 레벨일 때
	{
		CurrentPatrolPoint = PatrolPointLists[Index++]; // 현재 인덱스의 패트롤 포인트 인스턴스를 지정해주고 인덱스 증가
		ResultLocation = CurrentPatrolPoint->GetActorLocation();
		ReaperController->GetBlackboard()->SetValueAsObject(ACreatureAI::PatrolTargetKey, CurrentPatrolPoint);
		if (Index == PatrolPointLists.Num()) // 만약 인덱스가 PatrolPointLists의 최대 인덱스를 넘어가면
		{
			Index = 0; // 다시 0으로 초기화
		}
	}

	return ResultLocation;
}

bool AReaper_cpp::GetIsStunned()
{
	return bIsStunned;
}

void AReaper_cpp::Exorcism()
{
	bIsDied = true;

	DissolveParticleSystem->Activate(true);
	TArray<UMaterialInterface*> Materials = GetMesh()->GetMaterials();
	for (int i = 0; i < Materials.Num(); ++i)
	{
		MaterialInstances.Add(GetMesh()->CreateDynamicMaterialInstance(i, Materials[i]));
		MaterialInstances[i]->SetTextureParameterValue(TEXT("Texture"), Texture);
	}
//	MaterialInstance = GetMesh()->CreateDynamicMaterialInstance(0, Material);
//	MaterialInstance->SetTextureParameterValue(TEXT("Texture"), Texture);
	DissolveParticleSystem->SetVariableTexture(TEXT("Texture"), Texture);
	DissolveTimeline.PlayFromStart();

	/*GetMesh()->PlayAnimation(Anim, false);
	Destroy();*/
}

void AReaper_cpp::SetPatrolSuccess(bool value)
{
	bIsPatrolSuccess = value;
}

void AReaper_cpp::Stunning(float dist)
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
		KillBox->OnComponentBeginOverlap.RemoveDynamic(this, &AReaper_cpp::CatchBeginOverlap);
		ReaperController->SetStunned(bIsStunned);
	}
	/*GetCharacterMovement()->StopMovementImmediately();
	GetCharacterMovement()->DisableMovement();*/
}

void AReaper_cpp::SetStun()
{
	GetMovementComponent()->StopMovementImmediately();
	bIsTimeStop = true; // Stop Movement and Stun!
	CustomTimeDilation = 0.0001f; // 브루트의 시간은 0.0001로 만들어 속도 줄이고 멈춘 것처럼 표현
	//stunTime = 25.f; // 기절 시간은 25초로
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel3, ECollisionResponse::ECR_Ignore);
	KillBox->OnComponentBeginOverlap.RemoveDynamic(this, &AReaper_cpp::CatchBeginOverlap);
	ReaperController->SetStunned(bIsTimeStop);
}

bool AReaper_cpp::GetIsDied()
{
	return bIsDied;
}	

bool AReaper_cpp::GetIsCatch()
{
	return bIsCatch;
}

bool AReaper_cpp::GetAnimFinish()
{
	return bAnimFinish;
}

void AReaper_cpp::CatchBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherIndexBody, bool bFromSweep, const FHitResult& SweepResult)
{
	// ACreatureAI* AIController = Cast<ACreatureAI>(GetController());
	bool HideCatch = false;
	Sealed CurrentStatus = Sealed::Sealed;
	if (ReaperController != nullptr)
	{
		HideCatch = ReaperController->GetBlackboard()->GetValueAsBool(ACreatureAI::LockerLighting);
		CurrentStatus = ReaperController->GetCurrentSealStatus();
	}

	if (!bIsStunned)
	{
		if (OtherActor != this && OtherActor != nullptr && OtherComp != nullptr)
		{
		
			if (auto Cabinet = Cast<ACabinet_cpp>(OtherActor))
			{
				if (bIsChase && HideCatch)
				{
					if (CurrentStatus != Sealed::Sealed)
					{
						SetIsCatch(true);
						if (GetAnimFinish())
							Cabinet->BreakCabinet();
						//SetIsCatch(false);
					}
				}
			}
			if (auto Wardrobe = Cast<AWardrobe_cpp>(OtherActor))
			{
				if (bIsChase && HideCatch)
				{
					if (CurrentStatus != Sealed::Sealed)
					{
						SetIsCatch(true);
						if (GetAnimFinish())
							Wardrobe->BreakWardrobe();
						//SetIsCatch(false);
					}
				}
			}
			if (auto Character = Cast<AHorrorGameCharacter>(OtherActor))
			{
				if ((Character->GetPlayerStatus() == Player_Status::Survive || Character->GetPlayerStatus() == Player_Status::Chased) && !Character->GetIsHiding())
				{
					if (CurrentStatus == Sealed::Sealed) // 리퍼의 상태가 봉인된 상태면 패닉 게이지 증가시키고 소멸
					{
						Character->AddPatience(20);
						Destroy();
					}

					else
					{
						Character->SetPlayerStatus(Player_Status::Catched);
						// Character->GetFirstPersonCameraComponent()->SetRelativeRotation(FRotator(0.f, GetActorForwardVector().Rotation().Yaw, 0.f));
						/*FVector CreatureLocation = GetActorLocation();
						FVector PlayerLocation = Character->GetActorLocation();
						CreatureLocation.Z = 0.f;
						PlayerLocation.Z = 0.f;

						FVector Direction = (PlayerLocation - CreatureLocation).GetSafeNormal();
						FVector Forward = GetActorForwardVector();

						float Dot = FVector::DotProduct(Forward, Direction);
						float Theta = UKismetMathLibrary::DegAcos(Dot);

						FVector Cross = FVector::CrossProduct(Forward, Direction);
						FRotator NewRotation(0.f);

						if (Cross.Z > 0)
						{
							NewRotation = FRotator(0.f, Theta, 0.f);
						}
						else if (Cross.Z < 0)
						{
							NewRotation = FRotator(0.f, -Theta, 0.f);
						}*/
						
						if (UHorrorGameSaveGame* SaveData = UHorrorGameSaveGame::LoadObject(this, TEXT("Player"), 0))
						{
							if (!SaveData->CatchedByReaper)
							{
								SaveData->CatchedByReaper = true;
								SaveData->SaveData();
							}
						}

						/*Character->bUseControllerRotationYaw = false;
						Character->SetActorRotation(NewRotation);*/
						Character->OnFocus(GetActorLocation());
						Character->SetArchiveGetText(NSLOCTEXT("AReaper_cpp", "Kill_By_Reaper", "Reaper\nis added in archive"));

						SetIsCatch(true);
					}
				}
			}

			if (auto Alarm = Cast<AAlarm>(OtherActor))
			{
				Alarm->AIInteract();
			}
		}
	}
}

bool AReaper_cpp::GetPatrolSuccess()
{
	return bIsPatrolSuccess;
}

void AReaper_cpp::SetCreatureCollision(bool value)
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

void AReaper_cpp::SetPlayerWatch(bool value)
{
	bIsPlayerWatch = value;
	if (bIsPlayerWatch)
	{
		if (!bIsCooldown && !bIsStunned)
		{
			Player->SetReaperLookPlayer(true);
		}
		else
		{
			Player->SetReaperLookPlayer(false);
		}
	}
	else
	{
		Player->SetReaperLookPlayer(false);
	}
	
}

void AReaper_cpp::SetCurrentStatus(int32 Status)
{
	Sealed CurrentStatus = Sealed::Sealed;

	switch (Status)
	{
		case 0:// Sealed
		{
			CurrentStatus = Sealed::Sealed;
			break;
		}
		case 1: // OneUnsealed
		{
			CurrentStatus = Sealed::OneUnsealed;
			break;
		}
		case 2: // TwoUnsealed
		{
			CurrentStatus = Sealed::TwoUnsealed;
			break;
		}
		case 3: // Unsealed
		{
			CurrentStatus = Sealed::Unsealed;
			break;
		}
		case 4: // Sealed but chase
		{
			CurrentStatus = Sealed::SealedButChase;
			bSealedButChase = true;
			StartChase();
			ReaperController->GetBlackboard()->SetValueAsObject(ACreatureAI::TargetKey, Player);
			GetCharacterMovement()->MaxWalkSpeed = 220.f;
			break;
		}
	}
	ReaperController->SetCurrentSealStatus(CurrentStatus);
}

void AReaper_cpp::SetIsStop(bool inIsStop)
{
	bIsStop = inIsStop;
}

bool AReaper_cpp::GetIsStop()
{
	return bIsStop;
}

void AReaper_cpp::ChangeMaterialInstance(float inValue)
{
	Super::ChangeMaterialInstance(inValue);

	float Amount = FMath::Lerp(1.0f, 0.0f, inValue);
	//float MI_Amount = FMath::Lerp(1.0f, 0.0f, inValue);

	for (int i = 0; i < MaterialInstances.Num(); ++i)
	{
		MaterialInstances[i]->SetScalarParameterValue(TEXT("Amount"), Amount);
	}

	//MPC_Reaper->SetScalarParameterValue(TEXT("Amount"), Amount);
	//UMaterialParameterCollectionInstance* PCI_Reaper = GetWorld()->GetParameterCollectionInstance(MPC_Reaper);
	//PCI_Reaper->SetScalarParameterValue(TEXT("Amount"), Amount);
	//float NC_Amount = FMath::Lerp(1.0f, .0f, inValue);
	DissolveParticleSystem->SetVariableFloat(TEXT("Amount"), Amount);
}

void AReaper_cpp::DissolveFinish()
{
	Super::DissolveFinish();

	DissolveParticleSystem->SetVariableFloat(TEXT("Width"), 0.0f);

	DissolveParticleSystem->Deactivate();


	Destroy();
}