// Fill out your copyright notice in the Description page of Project Settings.


#include "Cabinet_cpp.h"
#include "Camera/CameraComponent.h"
#include "InputActionValue.h"
#include "Components/PointLightComponent.h"
#include "Components/SpotLightComponent.h"
#include "Components/AudioComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Sound/SoundCue.h"
#include "HorrorGame/HorrorGameCharacter.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

// Sets default values
ACabinet_cpp::ACabinet_cpp()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	DefaultSceneRoot->SetWorldLocation(FVector(0.0f, 0.0f, 0.0f));
	DefaultSceneRoot->SetWorldScale3D(FVector(1.0f, 1.0f, 1.0f));

	RootComponent = DefaultSceneRoot;

	Cabinet = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Cabinet"));
	Cabinet->SetupAttachment(DefaultSceneRoot);
	static ConstructorHelpers::FObjectFinder<UStaticMesh>CabinetMesh(TEXT("/Game/Assets/Furniture/Cabinet_Cabinet"));
	if (CabinetMesh.Succeeded())
	{
		Cabinet->SetStaticMesh(CabinetMesh.Object);
		Cabinet->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
		Cabinet->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f)); // Y Z X
		Cabinet->SetRelativeScale3D(FVector(1.0f, 1.0f, 1.0f));
	}

	Cabinet_Door = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Cabinet_Door"));
	Cabinet_Door->SetupAttachment(DefaultSceneRoot);
	static ConstructorHelpers::FObjectFinder<UStaticMesh>CabinetDoorMesh(TEXT("/Game/Assets/Furniture/Cabinet_Cube"));
	if (CabinetDoorMesh.Succeeded())
	{
		Cabinet_Door->SetStaticMesh(CabinetDoorMesh.Object);
		Cabinet_Door->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
		Cabinet_Door->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f)); // Y Z X
	}

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(DefaultSceneRoot);
	Camera->SetRelativeLocation(FVector(0.f, 0.f, 65.f));
	Camera->bUsePawnControlRotation = true;

	CigarLight = CreateDefaultSubobject<UPointLightComponent>(TEXT("CigarLight"));
	CigarLight->SetupAttachment(Camera);
	CigarLight->Mobility = EComponentMobility::Movable;
	CigarLight->SetRelativeLocation(FVector(5.0f, 0.0f, 0.0f));
	CigarLight->SetVisibility(false);
	CigarLight->SetIntensity(800.0f);
	CigarLight->SetLightFColor(FColor(255.0f, 188.0f, 124.0f));

	FlashLight = CreateDefaultSubobject<USpotLightComponent>(TEXT("FlashLight"));
	FlashLight->SetupAttachment(Camera);
	FlashLight->Mobility = EComponentMobility::Movable;
	FlashLight->SetRelativeLocation(FVector(5.0f, 0.0f, 0.0f));
	FlashLight->SetVisibility(false);
	FlashLight->SetIntensity(800.0f);
	FlashLight->SetInnerConeAngle(15.0f);
	FlashLight->SetOuterConeAngle(30.0f);

	/*Turnon = CreateDefaultSubobject<UAudioComponent>(TEXT("FlashLightSound"));
	Turnon->SetupAttachment(Camera);
	static ConstructorHelpers::FObjectFinder<USoundBase> TurnonSound(TEXT("/Game/Assets/Sounds/Turnon"));
	if (TurnonSound.Succeeded())
	{
		Turnon->SetSound(TurnonSound.Object);
	}
	Turnon->SetAutoActivate(false);
	
	CigarOn = CreateDefaultSubobject<UAudioComponent>(TEXT("CigarOn"));
	CigarOn->SetupAttachment(Camera);
	static ConstructorHelpers::FObjectFinder<USoundBase> Ignite(TEXT("/Game/Assets/Sounds/SoundWaves/LighterIgnite"));
	if (Ignite.Succeeded())
	{
		CigarOn->SetSound(Ignite.Object);
	}
	CigarOn->SetAutoActivate(false);

	CigarOff = CreateDefaultSubobject<UAudioComponent>(TEXT("CigarOff"));
	CigarOff->SetupAttachment(Camera);
	static ConstructorHelpers::FObjectFinder<USoundBase> OffSound(TEXT("/Game/Assets/Sounds/LighterOff"));
	if (OffSound.Succeeded())
	{
		CigarOff->SetSound(OffSound.Object);
	}
	CigarOff->SetAutoActivate(false);*/

	Sound = CreateDefaultSubobject<UAudioComponent>(TEXT("HidingSound"));
	Sound->SetupAttachment(Camera);
	static ConstructorHelpers::FObjectFinder<USoundCue> HidingSound(TEXT("/Game/Assets/Sounds/SoundCues/CabinetHide"));
	if (HidingSound.Succeeded())
	{
		Sound->SetSound(HidingSound.Object);
	}
	Sound->SetAutoActivate(false);
}

// Called when the game starts or when spawned
void ACabinet_cpp::BeginPlay()
{
	Super::BeginPlay();

	if (CurveFloat)
	{
		FOnTimelineFloat TimelineProgress;
		TimelineProgress.BindDynamic(this, &ACabinet_cpp::LightFlicker);
		FlickeringLight.AddInterpFloat(CurveFloat, TimelineProgress);
		FlickeringLight.SetLooping(true);
	}
}

// Called every frame
void ACabinet_cpp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FlickeringLight.TickTimeline(DeltaTime);

	if (Player != nullptr)
	{
		if (bIsHiding)
		{
			if (bIsCigarLightOn != Player->bIsCigarLightOn)
			{
				SetCigarLightOn();
			}
			
			if (bIsFlashLightOn != Player->bIsFlashLightOn)
			{
				SetFlashLightOn();
			}

			if (CreatureNum > 0)
			{
				FlickeringLight.Play();
			}
			else
			{
				FlickeringLight.Stop();
			}
		}
	}
}

void ACabinet_cpp::OnInteract(class AHorrorGameCharacter* PlayerCharacter)
{
	ToggleHide(PlayerCharacter);
	Sound->Play();
}

void ACabinet_cpp::UseInteract() // 안 씀
{
	if (Player == nullptr)
	{
		APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
		// AHorrorGameCharacter* Player = nullptr;
		if (nullptr == PlayerController) return;

		Player = Cast<AHorrorGameCharacter>(PlayerController->GetPawn());
		
		if (Player == nullptr)
			return;
	}

	if (Player->GetCurrentItemName() == TEXT("FlashLight"))
	{
		SetFlashLightOn();
	}
	else if (Player->GetCurrentItemName() == TEXT("CigarLight"))
	{
		SetCigarLightOn();
		//Turnon->Play();
	}
	
}

void ACabinet_cpp::ToggleHide(class AHorrorGameCharacter* PlayerCharacter)
{
	if (Player == nullptr)
	{
		Player = PlayerCharacter;
	}
	APlayerController* PlayerController = Cast<APlayerController>(Player->GetController());
	// if (nullptr == PlayerCharacter) return;
	/*if (nullptr == Player) return;*/
	
	if (bIsHiding) // Hide on Cabinet
	{	// True
		bIsHiding = false;
		/*PlayerCharacter->bIsHiding = false;
		PlayerCharacter->SetActorHiddenInGame(false);
		PlayerCharacter->SetActorEnableCollision(true);
		PlayerController->SetViewTargetWithBlend(PlayerCharacter, 0.2f);*/
		Player->bIsHiding = false;
		Player->SetActorHiddenInGame(false);
		Player->SetActorEnableCollision(true);
		PlayerController->SetViewTargetWithBlend(Player, 0.2f);
		Camera->SetRelativeRotation(FRotator(0.f, 0.f, 0.f));
		FlickeringLight.Stop();
		FlashLight->SetVisibility(false);
		CigarLight->SetVisibility(false);
		bIsFlashLightOn = false;
		bIsCigarLightOn = false;
		Player = nullptr;
	}
	else
	{	// False
		bIsHiding = true;
		if (!PlayerCharacter->bIsPatienceReduce)
		{
			if (PlayerCharacter->Patience > 0) // 패닉값이 0보다 클 경우에만 작동함
			{
				PlayerCharacter->PatienceToReduce = PlayerCharacter->Patience / 2;
				if (PlayerCharacter->PatienceToReduce < 1) // 만약 감소값이 1 미만으로 떨어지면(즉 0.xxxx 라면)
				{
					PlayerCharacter->PatienceToReduce = 1; // 1로 고정
				}
			}
			PlayerCharacter->bIsPatienceReduce = true;
		}
		/*PlayerCharacter->bIsHiding = true;
		PlayerCharacter->GetCharacterMovement()->StopMovementImmediately();
		PlayerCharacter->SetActorHiddenInGame(true);
		PlayerCharacter->SetActorEnableCollision(false);*/
		Player->bIsHiding = true;
		Player->GetCharacterMovement()->StopMovementImmediately();
		Player->SetActorHiddenInGame(true);
		Player->SetActorEnableCollision(false);
		PlayerController->SetViewTargetWithBlend(this, 0.2f);
	}
}

void ACabinet_cpp::OnComponentBeginOverlap(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && (OtherActor != this))
	{
		APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
		this->EnableInput(PlayerController);
	}
}

void ACabinet_cpp::OnComponentEndOverlap(class UPrimitiveComponent* OverlappedComponent, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor && (OtherActor != this))
	{
		APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
		this->DisableInput(PlayerController);
	}
}

void ACabinet_cpp::LookUp(float AxisValue)
{
	//Pitch += AxisValue * -1.0f;
}

void ACabinet_cpp::Turn(float AxisValue)
{
	//Yaw += AxisValue;
}

void ACabinet_cpp::SetFlashLightOn()
{
	if (Player)
	{
		if (Player->bIsFlashLightOn)
		{
			bIsFlashLightOn = true;
			FlashLight->SetVisibility(true);
		}
		else
		{
			bIsFlashLightOn = false;
			FlashLight->SetVisibility(false);
		}
	}
}

void ACabinet_cpp::SetCigarLightOn()
{
	if (Player)
	{
		if (Player->bIsCigarLightOn)
		{
			bIsCigarLightOn = true;
			CigarLight->SetVisibility(true);
		}
		else
		{
			bIsCigarLightOn = false;
			CigarLight->SetVisibility(false);
		}
	}
}

void ACabinet_cpp::MoveCamera()
{
	if (bIsHiding)
	{
		Camera->SetRelativeRotation(UKismetMathLibrary::MakeRotator(0, Pitch, Yaw));
		GetWorld()->GetTimerManager().SetTimer(_loopCameraTimerHandle, this, &ACabinet_cpp::MoveCamera, 0.1f, false);
		GetWorld()->GetTimerManager().ClearTimer(_loopCameraTimerHandle);
	}
}

void ACabinet_cpp::CigarLightOnFunc() // 안 씀
{
	CigarLight->SetVisibility(true);
	if (CreatureNum > 0)
		FlickeringLight.Play();
	GetWorld()->GetTimerManager().ClearTimer(_CigarLightTimerHandle);
}

void ACabinet_cpp::BreakCabinet()
{
	if (Player != nullptr)
	{
		ToggleHide(Player);
		Destroy();
	}
}

void ACabinet_cpp::LightFlicker(float value)
{
	float LightIntense = Intensity * value;

	if (bIsCigarLightOn)
	{
		if (LightIntense < 0.f)
		{
			CigarLight->SetIntensity(0.0f);
		}
		else if (LightIntense > Intensity)
		{
			CigarLight->SetIntensity(Intensity);
		}
		else
		{
			CigarLight->SetIntensity(LightIntense);
		}
	}

	if (bIsFlashLightOn)
	{
		if (LightIntense < 0.f)
		{
			FlashLight->SetIntensity(0.0f);
		}
		else if (LightIntense > Intensity)
		{
			FlashLight->SetIntensity(Intensity);
		}
		else
		{
			FlashLight->SetIntensity(LightIntense);
		}
	}
}

void ACabinet_cpp::SetCameraComponentNoise(int32 WhichStatus)
{
	FTimerHandle NoiseTimer;
	GetWorldTimerManager().ClearTimer(NoiseTimer);
	switch (WhichStatus)
	{
	case 1: // 근처에 Creature가 있는 상황이면, 카메라에 노이즈 걸 것임
		Camera->PostProcessSettings.bOverride_VignetteIntensity = true;
		Camera->PostProcessSettings.VignetteIntensity = 1.f;
		Camera->PostProcessSettings.bOverride_FilmGrainIntensity = true;
		Camera->PostProcessSettings.FilmGrainIntensity = 4.f;
		break;
	case 2: // 추격 판정이 뜰 경우 노이즈 크게 함
		Camera->PostProcessSettings.bOverride_VignetteIntensity = true;
		Camera->PostProcessSettings.VignetteIntensity = 1.f;
		Camera->PostProcessSettings.bOverride_FilmGrainIntensity = true;
		Camera->PostProcessSettings.FilmGrainIntensity = 15.f;
		GetWorldTimerManager().SetTimer(NoiseTimer, FTimerDelegate::CreateLambda([&]() {
			SetCameraComponentNoise(1); // 0.5초동안 노이즈 크게 하고 다시 1번 상태로 돌릴 것임
		}), 0.5f, false);
		break;
	default: // Creature가 근처에 없다면 카메라에 노이즈 해제함
		Camera->PostProcessSettings.bOverride_VignetteIntensity = false;
		Camera->PostProcessSettings.bOverride_FilmGrainIntensity = false;
	}
}