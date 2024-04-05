// Fill out your copyright notice in the Description page of Project Settings.


#include "Wardrobe_cpp.h"
#include "Camera/CameraComponent.h"
#include "InputActionValue.h"
#include "Components/PointLightComponent.h"
#include "Components/SpotLightComponent.h"
#include "Components/AudioComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "HideAndSeek/HorrorGameCharacter.h"
#include "Kismet/KismetMathLibrary.h"
#include "Sound/SoundCue.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "AI/CreatureAI.h"

// Sets default values
AWardrobe_cpp::AWardrobe_cpp()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	DefaultSceneRoot->SetWorldLocation(FVector(0.f, 0.f, 0.f));
	DefaultSceneRoot->SetWorldScale3D(FVector(1.f, 1.f, 1.f));

	RootComponent = DefaultSceneRoot;

	Wardrobe = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Wardrobe"));
	Wardrobe->SetupAttachment(DefaultSceneRoot);
	static ConstructorHelpers::FObjectFinder<UStaticMesh>WardrobeMesh(TEXT("/Game/Assets/Furniture/wardrobeDoor"));
	if (WardrobeMesh.Succeeded())
	{
		Wardrobe->SetStaticMesh(WardrobeMesh.Object);
		Wardrobe->SetRelativeLocation(FVector(0.f, 0.f, 0.f));
		Wardrobe->SetRelativeScale3D(FVector(2.f, 2.f, 1.7f));
	}

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(DefaultSceneRoot);
	Camera->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));
	Camera->bUsePawnControlRotation = true;

	CigarLight = CreateDefaultSubobject<UPointLightComponent>(TEXT("CigarLight"));
	CigarLight->SetupAttachment(Camera);
	CigarLight->Mobility = EComponentMobility::Movable;
	CigarLight->SetRelativeLocation(FVector(5.f, 0.f, 0.f));
	CigarLight->SetVisibility(false);
	CigarLight->SetIntensity(800.f);
	CigarLight->SetLightFColor(FColor(255.f, 188.f, 124.f));

	FlashLight = CreateDefaultSubobject<USpotLightComponent>(TEXT("FlashLight"));
	FlashLight->SetupAttachment(Camera);
	FlashLight->Mobility = EComponentMobility::Movable;
	FlashLight->SetRelativeLocation(FVector(5.f, 0.f, 0.f));
	FlashLight->SetVisibility(false);
	FlashLight->SetIntensity(800.f);
	FlashLight->SetInnerConeAngle(15.f);
	FlashLight->SetOuterConeAngle(30.f);

	Sound = CreateDefaultSubobject<UAudioComponent>(TEXT("HidingSound"));
	Sound->SetupAttachment(Camera);
	static ConstructorHelpers::FObjectFinder<USoundCue> HidingSound(TEXT("/Game/Assets/Sounds/SoundCues/WardrobeHide"));
	if (HidingSound.Succeeded())
	{
		Sound->SetSound(HidingSound.Object);
	}
	Sound->SetAutoActivate(false);

}

// Called when the game starts or when spawned
void AWardrobe_cpp::BeginPlay()
{
	Super::BeginPlay();

	if (CurveFloat)
	{
		FOnTimelineFloat TimelineProgress;
		TimelineProgress.BindDynamic(this, &AWardrobe_cpp::LightFlicker);
		FlickeringLight.AddInterpFloat(CurveFloat, TimelineProgress);
		FlickeringLight.SetLooping(true);
	}
	
	/*auto pc = GetWorld()->GetFirstPlayerController();

	auto player = Cast<AHorrorGameCharacter>(pc->GetPawn());


	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, FString::Printf(TEXT("Player Location: %s"), *player->GetActorLocation().ToString()));
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, FString::Printf(TEXT("%f"), FVector::Distance(GetActorLocation(), player->GetActorLocation())));
	}*/
}

// Called every frame
void AWardrobe_cpp::Tick(float DeltaTime)
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

void AWardrobe_cpp::OnInteract(class AHorrorGameCharacter* PlayerCharacter)
{
	ToggleHide(PlayerCharacter);
	Sound->Play();
}

void AWardrobe_cpp::UseInteract() // 안 씀
{
	if (Player == nullptr)
	{
		auto playerController = GetWorld()->GetFirstPlayerController();
		// AHorrorGameCharacter* Player = nullptr;

		if (nullptr == playerController) return;

		Player = Cast<AHorrorGameCharacter>(playerController->GetPawn());

	}

//	if (Player->GetCurrentItemName() == TEXT("FlashLight"))
	if (Player->GetCurrentItemNumber() == 2)
	{
		SetFlashLightOn();
	}
	//else if (Player->GetCurrentItemName() == TEXT("CigarLight"))
	else if (Player->GetCurrentItemNumber() == 1)
	{
		SetCigarLightOn();
	}
}

void AWardrobe_cpp::ToggleHide(class AHorrorGameCharacter* PlayerCharacter)
{
	if (Player == nullptr)
	{
		Player = PlayerCharacter;
	}
	APlayerController* PlayerController = Cast<APlayerController>(Player->GetController());

	if (bIsHiding) // 숨어있다 나올 때
	{
		bIsHiding = false;
		PlayerCharacter->bIsHiding = false;
		PlayerCharacter->SetActorHiddenInGame(false);
		//PlayerCharacter->SetActorEnableCollision(true);
		SetCameraComponentNoise(0);
		PlayerController->SetViewTargetWithBlend(Player, 0.2f);
		Camera->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));
		FlashLight->SetVisibility(false);
		CigarLight->SetVisibility(false);
		FlickeringLight.Stop();
		bIsFlashLightOn = false;
		bIsCigarLightOn = false;
		Player = nullptr;
	}
	else
	{
		bIsHiding = true;
		if (!PlayerCharacter->bIsPatienceReduce)
		{
			if (PlayerCharacter->Patience > 0) // 패닉값이 0보다 클 경우에만 작동함
			{
				int32 ReduceValue = PlayerCharacter->Patience / 2;
				if (ReduceValue < 1) // 만약 감소값이 1 미만으로 떨어지면(즉 0.xxxx 라면)
				{
					ReduceValue = 1;
				}
				PlayerCharacter->PatienceToReduce = ReduceValue; // 1로 고정
			}
			PlayerCharacter->bIsPatienceReduce = true;
		}
	
		PlayerCharacter->bIsHiding = true;
		PlayerCharacter->GetCharacterMovement()->StopMovementImmediately();
		Camera->PostProcessSettings = PlayerCharacter->GetFirstPersonCameraComponent()->PostProcessSettings; // 노이즈 상황 등을 동일화
		PlayerCharacter->SetActorHiddenInGame(true);
		
		//PlayerCharacter->SetActorEnableCollision(false);
		PlayerController->SetViewTargetWithBlend(this, 0.2f);
	}
}

void AWardrobe_cpp::LookUp(float AxisValue)
{
	Pitch += AxisValue * -1.0f;
}

void AWardrobe_cpp::Turn(float AxisValue)
{
	Yaw += AxisValue;
}

void AWardrobe_cpp::SetFlashLightOn()
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

void AWardrobe_cpp::SetCigarLightOn()
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

void AWardrobe_cpp::MoveCamera()
{
	if (bIsHiding)
	{
		Camera->SetRelativeRotation(UKismetMathLibrary::MakeRotator(0, Pitch, Yaw));
		GetWorld()->GetTimerManager().SetTimer(_loopCameraTimerHandle, this, &AWardrobe_cpp::MoveCamera, 0.1f, false);
		GetWorld()->GetTimerManager().ClearTimer(_loopCameraTimerHandle);
	}
}

void AWardrobe_cpp::CigarLightOnFunc() // Deprecated
{
	CigarLight->SetVisibility(true);
	if (CreatureNum > 0)
		FlickeringLight.Play();
	GetWorld()->GetTimerManager().ClearTimer(_CigarLightTimerHandle);
}

void AWardrobe_cpp::BreakWardrobe()
{
	if (Player != nullptr)
	{
		ToggleHide(Player);
		Destroy();
	}
}

void AWardrobe_cpp::LightFlicker(float value)
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

void AWardrobe_cpp::SetCameraComponentNoise(int32 WhichStatus)
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