// CopyrightNotice=0 2023 Sunggon Kim kimdave205@gmail.com

#include "AI/Runner_cpp.h"
#include "Components/InputComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/PointLightComponent.h"
#include "Components/SpotLightComponent.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "Sound/SoundCue.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AI/AIController_Runner.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "PatrolPoint_cpp.h"
#include "EngineUtils.h"
#include "Wardrobe_cpp.h"
#include "Cabinet_cpp.h"
#include "HorrorGame/HorrorGameCharacter.h"
#include "HorrorGamePlayerController.h"
#include "ClassroomDoorActor_cpp.h"
#include "Door_cpp.h"
#include "MetalDoor_cpp.h"
#include "Alarm.h"
#include "Animation/AnimSequence.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
ARunner_cpp::ARunner_cpp()
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
	BoxCollision->OnComponentBeginOverlap.AddDynamic(this, &ARunner_cpp::SoundBeginOverlap);
	BoxCollision->OnComponentEndOverlap.AddDynamic(this, &ARunner_cpp::SoundEndOverlap);

	InteractBox = CreateDefaultSubobject<UBoxComponent>(TEXT("InteractBox"));
	InteractBox->SetupAttachment(GetMesh());
	InteractBox->SetBoxExtent(FVector(5.f, 5.f, 5.f));
	InteractBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	//InteractBox->OnComponentBeginOverlap.AddDynamic(this, &ARunner_cpp::DoorBeginOverlap);

	KillSphere = CreateDefaultSubobject<USphereComponent>(TEXT("KillBox"));
	KillSphere->SetupAttachment(GetMesh());
	KillSphere->SetRelativeLocation(FVector(0.f, 0.f, 130.f));
	KillSphere->SetRelativeScale3D(FVector(3.5f, 3.5f, 4.f));
	KillSphere->OnComponentBeginOverlap.AddDynamic(this, &ARunner_cpp::CatchBeginOverlap);

	AIControllerClass = AAIController_Runner::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	//GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &ARunner_cpp::CheckBoxBeginOverlap);
	GetMesh()->SetCollisionProfileName("OverlapAllDynamic");
	bIsCollectMode = true;
}

// Called when the game starts or when spawned
void ARunner_cpp::BeginPlay()
{
	Super::BeginPlay();

	if (bIsCollectMode)
	{
		UWorld* world = GetWorld();
		for (TActorIterator<APatrolPoint_cpp> entity(world); entity; ++entity)
		{
			PatrolPointLists.Add(*entity);
		}
	}
}

// Called every frame
void ARunner_cpp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsStunned)
	{
		CurrentStunnedTime += DeltaTime;
		if (CurrentStunnedTime >= stunTime)
		{
			bIsStunned = false;
			AAIController_Runner* controller = Cast<AAIController_Runner>(GetController());
			controller->SetStunned(bIsStunned);
			CurrentStunnedTime = 0;
			KillSphere->OnComponentBeginOverlap.AddDynamic(this, &ARunner_cpp::CatchBeginOverlap);
		}
	}

	Cast<AAIController_Runner>(GetController())->SetControlRotation(GetActorRotation());
}

// Called to bind functionality to input
void ARunner_cpp::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ARunner_cpp::Move(const FInputActionValue& Value)
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

void ARunner_cpp::Look(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void ARunner_cpp::PossessedBy(AController* NewController)
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
		if (bIsChase) GetCharacterMovement()->MaxWalkSpeed = 330.f;
		else GetCharacterMovement()->MaxWalkSpeed = 100.f;
	}
}

void ARunner_cpp::StartChase()
{
	if (!bIsChase)
	{
		UE_LOG(LogTemp, Warning, TEXT("SetTrace: Start Chase Function called"));
		bIsChase = true;
		// ReaperSound->SetSound(DetectedSound);
		// ReaperSound->Play();
		GetCharacterMovement()->MaxWalkSpeed = 330.f;
		AHorrorGameCharacter* Player = Cast<AHorrorGameCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());

		if (Player)
		{
			Player->SetCameraComponentNoise(2);
			if (!Player->bIsCooldown)
			{
				Player->AddPatience(15);
			}
		}
	}
}

void ARunner_cpp::EndChase()
{
	if (bIsChase)
	{
		UE_LOG(LogTemp, Warning, TEXT("SetTrace: End Chase Function called"));
		AAIController_Runner* AIController = Cast<AAIController_Runner>(GetController());
		if (AIController != nullptr)
		{
			AIController->EndChase();
		}
		bIsChase = false;
		// ReaperSound->SetSound(PatrolSound);
		GetCharacterMovement()->MaxWalkSpeed = 100.f;
	}
}

void ARunner_cpp::SetIsCatch(bool Value)
{
	bIsCatch = Value;
}

void ARunner_cpp::SetAnimFinish(bool Value)
{
	bAnimFinish = Value;
	UWorld* World = GetWorld();
	AHorrorGamePlayerController* PlayerController = Cast<AHorrorGamePlayerController>(World->GetFirstPlayerController());
	AHorrorGameCharacter* Player = Cast<AHorrorGameCharacter>(PlayerController->GetPawn());

	if (Player->GetIsHiding())
		Player->SetPlayerStatus(Player_Status::Survive);
	else
		Player->SetPlayerStatus(Player_Status::Died);
}

void ARunner_cpp::SoundBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
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
			InteractBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
			InteractBox->OnComponentBeginOverlap.AddDynamic(this, &ARunner_cpp::DoorBeginOverlap);
		}

		if (auto Cabinet = Cast<ACabinet_cpp>(OtherActor))
		{
			Cabinet->CreatureNum++;
			/*if (Cabinet->CreatureNum > 0 && Cabinet->bIsHiding)
				Cabinet->FlickeringLight.Play();*/
			//ReaperSound->Play();
			/*if (Cabinet->Player != nullptr) // 원래는 숨어있을 때도 긴장도 증가였지만, 없애기로 함
				if (!Cabinet->Player->bIsCooldown)
					Cabinet->Player->Patience += 5;*/
			if (Cabinet->bIsHiding)
			{
				Cabinet->SetCameraComponentNoise(1);
			}
		}

		if (auto Wardrobe = Cast<AWardrobe_cpp>(OtherActor))
		{
			Wardrobe->CreatureNum++;
			/*if (Wardrobe->CreatureNum > 0 && Wardrobe->bIsHiding)
			{
				Wardrobe->FlickeringLight.Play();

			}*/
			if (Wardrobe->bIsHiding)
			{
				Wardrobe->SetCameraComponentNoise(1);
			}
			//ReaperSound->Play(); 
			/*if (Wardrobe->Player != nullptr) // 원래는 숨어있을 때도 긴장도 증가였지만, 없애기로 함
				if (!Wardrobe->Player->bIsCooldown)
					Wardrobe->Player->Patience += 5;*/
		}
	}
}

void ARunner_cpp::SoundEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
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
		GetMesh()->SetCollisionProfileName("OverlapAllDynamic");
		InteractBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		InteractBox->OnComponentBeginOverlap.RemoveDynamic(this, &ARunner_cpp::DoorBeginOverlap);
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

// AI Interact Box
void ARunner_cpp::DoorBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor != nullptr && OtherActor != this && OtherComp != nullptr)
	{
		if (auto Classroom = Cast<AClassroomDoorActor_cpp>(OtherActor))
		{
			if (Classroom->bIsDoorClosed)
			{
				Classroom->AIInteract();
			}
		}
		else if (auto Door = Cast<ADoor_cpp>(OtherActor))
		{
			if (Door->bIsDoorClosed)
			{
				Door->AIInteract();
			}
		}
		else if (auto MetalDoor = Cast<AMetalDoor_cpp>(OtherActor))
		{
			if (MetalDoor->bIsDoorClosed)
			{
				MetalDoor->AIInteract();
			}
		}
	}
}

void ARunner_cpp::CheckBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor != nullptr && OtherActor != this && OtherComp != nullptr)
	{
		if (OtherActor == CurrentPatrolPoint)
		{
			SetPatrolSuccess(true);
		}
	}
}

FVector ARunner_cpp::GetPatrolPoint()
{
	AAIController_Runner* AIController = Cast<AAIController_Runner>(GetController());
	int randIdx = FMath::RandRange(0, PatrolPointLists.Num() - 1); // 0에서 PatrolPointLists의 마지막 인덱스까지 중 랜덤 숫자 하나 뽑기
	CurrentPatrolPoint = PatrolPointLists[randIdx];
	FVector ResultLocation = CurrentPatrolPoint->GetActorLocation();
	AIController->GetBlackboard()->SetValueAsObject(AAIController_Runner::PatrolTargetKey, CurrentPatrolPoint);
	//int randIdx = FMath::RandRange(0, 24);
	//UWorld* world = GetWorld();
	//FVector ResultLocation = GetActorLocation();
	//for (TActorIterator<APatrolPoint_cpp> entity(world); entity; ++entity)
	//{
	//	// TArray<UObject*>components;

	//	if (entity->GetActorLabel() == PatrolPointList[randIdx].ToString()) {
	//		ResultLocation = entity->GetActorLocation();
	//		CurrentPatrolPoint = *entity;
	//		AIController->GetBlackboard()->SetValueAsObject(AAIController_Runner::PatrolTargetKey, CurrentPatrolPoint);
	//		break;
	//	}
	//}
	return ResultLocation;
}

bool ARunner_cpp::GetIsStunned()
{
	return bIsStunned;
}

void ARunner_cpp::Exorcism()
{
	bIsDied = true;
	// GetMesh()->PlayAnimation(Anim, false);
	Destroy();
}

void ARunner_cpp::SetPatrolSuccess(bool value)
{
	bIsPatrolSuccess = value;
}

void ARunner_cpp::Stunning(float dist)
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
		
		KillSphere->OnComponentBeginOverlap.RemoveDynamic(this, &ARunner_cpp::CatchBeginOverlap);
		AAIController_Runner* controller = Cast<AAIController_Runner>(GetController());
		controller->SetStunned(bIsStunned);
	}
	/*GetCharacterMovement()->StopMovementImmediately();
	GetCharacterMovement()->DisableMovement();*/
}

void ARunner_cpp::SetStun()
{
	GetMovementComponent()->StopMovementImmediately();
	bIsStunned = true; // Stop Movement and Stun!

	stunTime = 25.f; // 기절 시간은 25초로
	KillSphere->OnComponentBeginOverlap.RemoveDynamic(this, &ARunner_cpp::CatchBeginOverlap);
	AAIController_Runner* controller = Cast<AAIController_Runner>(GetController());
	controller->SetStunned(bIsStunned);
}

bool ARunner_cpp::GetIsDied()
{
	return bIsDied;
}

bool ARunner_cpp::GetIsCatch()
{
	return bIsCatch;
}

bool ARunner_cpp::GetAnimFinish()
{
	return bAnimFinish;
}

void ARunner_cpp::CatchBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherIndexBody, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("Something Catch"));
	AAIController_Runner* AIController = Cast<AAIController_Runner>(GetController());
	bool HideCatch = false;
	if (AIController != nullptr)
	{
		HideCatch = AIController->GetBlackboard()->GetValueAsBool(AAIController_Runner::LockerLighting);
	}

	if (OtherActor != this && OtherActor != nullptr && OtherComp != nullptr)
	{
		if (!bIsStunned)
		{
			if (auto Cabinet = Cast<ACabinet_cpp>(OtherActor))
			{
				if (bIsChase && HideCatch)
				{
					if (GEngine)
						GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Cabinet Catch")));
					SetIsCatch(true);
					if (GetAnimFinish())
						Cabinet->BreakCabinet();
					//SetIsCatch(false);
				}
			}
			if (auto Wardrobe = Cast<AWardrobe_cpp>(OtherActor))
			{
				if (bIsChase && HideCatch)
				{
					if (GEngine)
						GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Wardrobe Catch")));
					SetIsCatch(true);
					if (GetAnimFinish())
						Wardrobe->BreakWardrobe();
					//SetIsCatch(false);
				}
			}
			if (auto Character = Cast<AHorrorGameCharacter>(OtherActor))
			{
				if (Character->GetPlayerStatus() == Player_Status::Survive && !Character->GetIsHiding())
				{
					if (GEngine)
						GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Player Catch")));
					Character->SetPlayerStatus(Player_Status::Catched);
					
					FVector CreatureLocation = GetActorLocation();
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
					}
					// Cast<AHorrorGamePlayerController>(Character->GetController())->SetControlRotation(NewRotation);
					Character->SetActorRotation(NewRotation);

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

bool ARunner_cpp::GetPatrolSuccess()
{
	return bIsPatrolSuccess;
}

void ARunner_cpp::SetCreatureCollision(bool value)
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