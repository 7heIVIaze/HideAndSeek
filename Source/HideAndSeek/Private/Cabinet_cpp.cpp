// CopyrightNotice 2023 Sunggon Kim kimdave205@gmail.com. All Rights Reserved.

/***
* Deprecated! Furniture/HideObject 클래스가 이 클래스를 대체함.
***/

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
#include "HideAndSeek/HorrorGameCharacter.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

// Sets default values
ACabinet_cpp::ACabinet_cpp()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	// 불빛 깜빡임 등을 관리해야하므로 active시킴.
	PrimaryActorTick.bCanEverTick = true;

	// Root Component 설정용 컴포넌트임.
	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	/*DefaultSceneRoot->SetWorldLocation(FVector(0.0f, 0.0f, 0.0f));
	DefaultSceneRoot->SetWorldScale3D(FVector(1.0f, 1.0f, 1.0f));*/
	RootComponent = DefaultSceneRoot;

	//Cabinet = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Cabinet"));
	//Cabinet->SetupAttachment(DefaultSceneRoot);
	//static ConstructorHelpers::FObjectFinder<UStaticMesh>CabinetMesh(TEXT("/Game/Assets/Furniture/Cabinet_Cabinet"));
	//if (CabinetMesh.Succeeded())
	//{
	//	Cabinet->SetStaticMesh(CabinetMesh.Object);
	//	Cabinet->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	//	Cabinet->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f)); // Y Z X
	//	Cabinet->SetRelativeScale3D(FVector(1.0f, 1.0f, 1.0f));
	//}
	//
	//Cabinet_Door = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Cabinet_Door"));
	//Cabinet_Door->SetupAttachment(DefaultSceneRoot);
	//static ConstructorHelpers::FObjectFinder<UStaticMesh>CabinetDoorMesh(TEXT("/Game/Assets/Furniture/Cabinet_Cube"));
	//if (CabinetDoorMesh.Succeeded())
	//{
	//	Cabinet_Door->SetStaticMesh(CabinetDoorMesh.Object);
	//	Cabinet_Door->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	//	Cabinet_Door->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f)); // Y Z X
	//}

	// 캐비닛 메시임. 뒤에서 못 열게 하기 위해 설정함.
	Cabinet = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Cabinet_Door"));
	Cabinet->SetupAttachment(DefaultSceneRoot);
	static ConstructorHelpers::FObjectFinder<UStaticMesh>CabinetDoorMesh(TEXT("/Game/Assets/Furniture/Cabinet_Cube"));
	if (CabinetDoorMesh.Succeeded())
	{
		Cabinet->SetStaticMesh(CabinetDoorMesh.Object);
		Cabinet->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
		Cabinet->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f)); // Y Z X
	}

	// 플레이어처럼 행동할 수 있게 하기 위한 카메라 컴포넌트임. 플레이어를 넣을 수 있으면 넣어보고 싶긴 함.
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(DefaultSceneRoot);
	Camera->SetRelativeLocation(FVector(0.f, 0.f, 65.f));
	Camera->bUsePawnControlRotation = true;

	// 플레이어처럼 라이터 켜기용 컴포넌트
	CigarLight = CreateDefaultSubobject<UPointLightComponent>(TEXT("CigarLight"));
	CigarLight->SetupAttachment(Camera);
	CigarLight->Mobility = EComponentMobility::Movable;
	CigarLight->SetRelativeLocation(FVector(5.0f, 0.0f, 0.0f));
	CigarLight->SetVisibility(false);
	CigarLight->SetIntensity(800.0f);
	CigarLight->SetLightFColor(FColor(255.0f, 188.0f, 124.0f));

	// 플레이어처럼 플래시 켜기 용 컴포넌트
	FlashLight = CreateDefaultSubobject<USpotLightComponent>(TEXT("FlashLight"));
	FlashLight->SetupAttachment(Camera);
	FlashLight->Mobility = EComponentMobility::Movable;
	FlashLight->SetRelativeLocation(FVector(5.0f, 0.0f, 0.0f));
	FlashLight->SetVisibility(false);
	FlashLight->SetIntensity(800.0f);
	FlashLight->SetInnerConeAngle(15.0f);
	FlashLight->SetOuterConeAngle(30.0f);

	// 숨을 때 낼 소리
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

	// 근처 요괴가 있을 때, 불빛이 깜빡이는 것을 구현하기 위한 타임 라인
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

	// FlickeringLight라는 타임 라인에 틱을 동기화함.
	FlickeringLight.TickTimeline(DeltaTime);

	// Player는 숨어있는 플레이어에 대한 정보를 담은 변수
	if (Player != nullptr)
	{
		// 플레이어에 대한 정보도 있고, 숨어있는 것으로 판별될 때
		if (bIsHiding)
		{
			// 플레이어가 라이터를 켰는데 라이터를 켠 상태가 아닐 경우 라이터를 켬
			if (bIsCigarLightOn != Player->bIsCigarLightOn)
			{
				SetCigarLightOn();
			}
			
			// 플레이어가 플래시를 켰는데 플래시를 켠 상태가 아닐 경우, 플래시를 켬
			if (bIsFlashLightOn != Player->bIsFlashLightOn)
			{
				SetFlashLightOn();
			}

			// 근처의 요괴가 1 개체 이상 존재하면 타임라인 재생
			if (CreatureNum > 0)
			{
				FlickeringLight.Play();
			}
			// 그게 아니면 타임라인 종료
			else
			{
				FlickeringLight.Stop();
			}
		}
	}
}

// 플레이어가 클릭을 해 상호작용 시
void ACabinet_cpp::OnInteract(class AHorrorGameCharacter* PlayerCharacter)
{
	// 숨거나 나오기를 하고 사운드를 재생함.
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

	//if (Player->GetCurrentItemName() == TEXT("FlashLight"))
	if (Player->GetCurrentItemNumber() == 2)
	{
		SetFlashLightOn();
	}
	//else if (Player->GetCurrentItemName() == TEXT("CigarLight"))
	else if (Player->GetCurrentItemNumber() == 1)
	{
		SetCigarLightOn();
		//Turnon->Play();
	}
	
}

// 플레이어가 캐비닛에 숨거나 나오는 로직을 구현한 함수
void ACabinet_cpp::ToggleHide(class AHorrorGameCharacter* PlayerCharacter)
{
	// 플레이어의 데이터를 얻기 위해 null 포인터일 경우 플레이어를 지정해줌.
	if (Player == nullptr)
	{
		Player = PlayerCharacter;
	}
	APlayerController* PlayerController = Cast<APlayerController>(Player->GetController());

	// 숨어있다가 나올 때
	if (bIsHiding) // Hide on Cabinet
	{	// True
		// 숨었다는 정보를 없애고, 숨었을 때의 로직을 반대로 구현함.
		bIsHiding = false;
		Player->bIsHiding = false;
		Player->SetActorHiddenInGame(false);
		
		// 메인 카메라를 캐비닛 카메라에서 플레이어의 카메라로 전환함.
		PlayerController->SetViewTargetWithBlend(Player, 0.2f);
		Camera->SetRelativeRotation(FRotator(0.f, 0.f, 0.f));
		
		// 모든 변수와 컴포넌트의 상태를 다 초기화함
		FlickeringLight.Stop();
		FlashLight->SetVisibility(false);
		CigarLight->SetVisibility(false);
		bIsFlashLightOn = false;
		bIsCigarLightOn = false;
		Player = nullptr;
	}
	// 숨는 경우
	else
	{	// False
		// 숨었다는 상태로 만듦.
		bIsHiding = true;
		
		Player->bIsHiding = true;
		Player->GetCharacterMovement()->StopMovementImmediately();
		Camera->PostProcessSettings = PlayerCharacter->GetFirstPersonCameraComponent()->PostProcessSettings; // 
		Player->SetActorHiddenInGame(true);

		// 동시에 패닉 게이지를 감소시킴.
		if (!PlayerCharacter->bIsPatienceReduce)
		{
			// 패닉값이 0보다 클 경우에만 작동함
			if (PlayerCharacter->Patience > 0) 
			{
				int32 ReduceValue = PlayerCharacter->Patience / 2;
				if (ReduceValue < 1) // 만약 감소값이 1 미만으로 떨어지면(즉 0.xxxx 라면)
				{
					ReduceValue = 1; // 1로 고정
				}
				PlayerCharacter->PatienceToReduce = ReduceValue;
			}
			// 그 후 더이상 감소되지 않게 true로 설정함
			PlayerCharacter->bIsPatienceReduce = true;
		}

		// 이 후 메인 카메라를 플레이어의 카메라에서 캐비닛 카메라로 변경함.
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

// 플래시를 켜는 함수
void ACabinet_cpp::SetFlashLightOn()
{
	// 플레이어 변수가 null 포인터가 아닐 때 == 플레이어가 숨어있는 상태일 때
	if (Player)
	{
		// 플레이어가 플래시를 켠 상태면 똑같이 켬
		if (Player->bIsFlashLightOn)
		{
			bIsFlashLightOn = true;
			FlashLight->SetVisibility(true);
		}
		// 아니라면 똑같이 끔
		else
		{
			bIsFlashLightOn = false;
			FlashLight->SetVisibility(false);
		}
	}
}

// 라이터를 켜는 함수
void ACabinet_cpp::SetCigarLightOn()
{
	// 플레이어 변수가 null 포인터가 아닐 때 == 플레이어가 숨어있는 상태일 때
	if (Player)
	{
		// 플레이어가 라이터를 켠 상태면 똑같이 켬
		if (Player->bIsCigarLightOn)
		{
			bIsCigarLightOn = true;
			CigarLight->SetVisibility(true);
		}
		// 아니라면 똑같이 끔
		else
		{
			bIsCigarLightOn = false;
			CigarLight->SetVisibility(false);
		}
	}
}

// 카메라를 움직이게 하는 함수. 플레이어를 child actor로 박아볼까 생각 중. 실험이 성공하면 deprecate할 것.
void ACabinet_cpp::MoveCamera()
{
	if (bIsHiding)
	{
		Camera->SetRelativeRotation(UKismetMathLibrary::MakeRotator(0, Pitch, Yaw));
		/*GetWorld()->GetTimerManager().SetTimer(_loopCameraTimerHandle, this, &ACabinet_cpp::MoveCamera, 0.1f, false);
		GetWorld()->GetTimerManager().ClearTimer(_loopCameraTimerHandle);*/
	}
}

void ACabinet_cpp::CigarLightOnFunc() // 안 씀
{
	CigarLight->SetVisibility(true);
	if (CreatureNum > 0)
		FlickeringLight.Play();
	GetWorld()->GetTimerManager().ClearTimer(_CigarLightTimerHandle);
}

// 요괴들로 인해 캐비닛이 부서지는 로직
void ACabinet_cpp::BreakCabinet()
{
	// 플레이어가 숨은 상태가 아니면 작동 안되게 함.
	if (Player != nullptr)
	{
		// 플레이어를 내보내고
		ToggleHide(Player);
		// 부서짐. -> 소리도 재생되게 해야 함.
		Destroy();
	}
}

// 불빛을 깜빡이게 할 타임라인의 바인딩될 함수
void ACabinet_cpp::LightFlicker(float value)
{
	// 빛 세기는 원 밝기 세기와 타임 라인의 CurveFloat의 곱임. 범위는 0에서 원 밝기 세기 사이로 잡아줌.
	float LightIntense = FMath::Clamp(Intensity * value, 0.0f, Intensity);

	// 라이터가 켜진 경우엔 라이터 불을 깜빡이게 함.
	if (bIsCigarLightOn)
	{
		/*if (LightIntense < 0.f)
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
		}*/
		CigarLight->SetIntensity(LightIntense);
	}

	// 플래시가 켜진 경우엔 플레시 불을 깜빡이게 함.
	if (bIsFlashLightOn)
	{
		/*if (LightIntense < 0.f)
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
		}*/
		FlashLight->SetIntensity(LightIntense);
	}
}

// 카메라 노이즈를 관리할 함수.
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
		Camera->PostProcessSettings.FilmGrainIntensity = 1.f;
		break;
	case 2: // 추격 판정이 뜰 경우 노이즈 크게 함
		Camera->PostProcessSettings.bOverride_VignetteIntensity = true;
		Camera->PostProcessSettings.VignetteIntensity = 3.f;
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