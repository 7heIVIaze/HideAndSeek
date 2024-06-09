// CopyrightNotice 2023 Sunggon Kim kimdave205@gmail.com. All Rights Reserved.

#include "HorrorGameCharacter.h"
#include "HorrorGameGameMode.h"
#include "ComponentAction/PrologueGameMode.h"
#include "HorrorGameProjectile.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/BoxComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "HorrorGameGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"
#include "Blueprint/UserWidget.h"
#include "Components/PointLightComponent.h"
#include "Components/SpotLightComponent.h"
#include "InteractDot_cpp.h"
#include "AI/Reaper_cpp.h"
#include "AI/Runner_cpp.h"
#include "AI/Brute_cpp.h"
#include "HorrorGamePlayerController.h"
#include "GameUI.h"
#include "SwitchLever.h"
#include "PatrolPoint_cpp.h"
#include "WardrobeDrawer_cpp.h"
#include "Wardrobe_cpp.h"
#include "Drawer_cpp.h"
#include "Door_cpp.h"
#include "Cabinet_cpp.h"
#include "ClassroomDoorActor_cpp.h"
#include "ClassroomDoors_cpp.h"
#include "LockerDoorActor_cpp.h"
#include "Items/Key_cpp.h"
#include "Items/Mirror_cpp.h"
#include "Items/Timer_cpp.h"
#include "Items/TimerProjectile_cpp.h"
#include "Items/Cutter_cpp.h"
#include "Items/Sword_cpp.h"
#include "Items/Bell_cpp.h"
#include "Items/FlashLight_cpp.h"
#include "Items/CigarLighter_cpp.h"
#include "Items/Extinguisher_cpp.h"
#include "Items/Soul_Lantern_cpp.h"
#include "Items/GlowStick_cpp.h"
#include "Items/PlayerSword_cpp.h"
#include "Furniture/DistributionBox.h"
#include "Furniture/Paper.h"
#include "Furniture/HideObject.h"
#include "EngineUtils.h"
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "MetalDoor_cpp.h"
#include "DeskDrawer_cpp.h"
#include "End_Mirror.h"
#include "HangingLight.h"
#include "Alarm.h"
#include "DoorInterface_cpp.h"
#include "HideInterface.h"
#include "DrawerClass.h"
#include "LightSwitch.h"
#include "Altar_cpp.h"
#include "NiagaraFunctionLibrary.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Materials/MaterialInstance.h"
#include "PlayerAnim.h"
#include "Kismet/KismetMathLibrary.h"
//#include "Kismet/KismetMaterialLibrary.h"
//#include "Materials/MaterialParameterCollection.h"

//////////////////////////////////////////////////////////////////////////
// AHorrorGameCharacter

// Default Setting
AHorrorGameCharacter::AHorrorGameCharacter()
{
	// Character doesnt have a rifle at start
	bHasRifle = false;
	bIsSprint = false;
	bIsCrouch = false;
	bIsFlashLightOn = false;
	bIsCigarLightOn = false;
	bFLIntenseDown = false;
	bisSoundOn = false;
	Stamina = 400;
	FlashLightBattery = 200;
	bIsCleared = false;

	// 메시들의 기본 설정을 해줌. (세세한 설정은 블루프린트 클래스에서 수행)
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("AHorrorGameCharacter"));

	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-10.f, 0.f, 60.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(false);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	//Mesh1P->SetRelativeRotation(FRotator(0.9f, -19.19f, 5.2f));
	Mesh1P->SetRelativeLocation(FVector(-30.f, 0.f, -150.f));

	static ConstructorHelpers::FObjectFinder<UInputMappingContext>IMC_Default(TEXT("/Game/FirstPerson/Input/IMC_Default"));
	if (IMC_Default.Succeeded())
		DefaultMappingContext = IMC_Default.Object;

	static ConstructorHelpers::FObjectFinder<UInputAction>IA_Jump(TEXT("/Game/FirstPerson/Input/Actions/IA_Jump"));
	if (IA_Jump.Succeeded())
		JumpAction = IA_Jump.Object;

	static ConstructorHelpers::FObjectFinder<UInputAction>IA_Look(TEXT("/Game/FirstPerson/Input/Actions/IA_Look"));
	if (IA_Look.Succeeded())
		LookAction = IA_Look.Object;

	static ConstructorHelpers::FObjectFinder<UInputAction>IA_Move(TEXT("/Game/FirstPerson/Input/Actions/IA_Move"));
	if (IA_Move.Succeeded())
		MoveAction = IA_Move.Object;

	// SpotLight Component to implement FlashLight
	FlashLight = CreateDefaultSubobject<USpotLightComponent>(TEXT("FlashLight"));
	FlashLight->Mobility = EComponentMobility::Movable;
	FlashLight->SetupAttachment(FirstPersonCameraComponent);
	FlashLight->SetRelativeLocation(FVector(-20.f, 0.f, 0.f));
	FlashLight->SetVisibility(false);
	FlashLight->SetIntensity(15000.f);
	FlashLight->SetAttenuationRadius(3200.f);
	FlashLight->SetInnerConeAngle(25.f);
	FlashLight->SetOuterConeAngle(40.f);
	static ConstructorHelpers::FObjectFinder<UMaterialInterface> LightFunctionMaterial(TEXT("/Game/Assets/Material/Materials/FlashLightMat"));
	if (LightFunctionMaterial.Succeeded())
	{
		FlashLight->SetLightFunctionMaterial(LightFunctionMaterial.Object);
	}

	// PointLight Component to implement CigarLight
	CigarLight = CreateDefaultSubobject<UPointLightComponent>(TEXT("CigarLight"));
	CigarLight->Mobility = EComponentMobility::Movable;
	CigarLight->SetupAttachment(FirstPersonCameraComponent);
	CigarLight->SetRelativeLocation(FVector(-20.f, 0.f, 0.f));
	CigarLight->SetAttenuationRadius(1600.f);
	CigarLight->SetVisibility(false);
	CigarLight->SetIntensity(5000.f);
	CigarLight->SetLightFColor(FColor(255.f, 188.f, 124.f));

	// default character walk speed
	GetCharacterMovement()->MaxWalkSpeed = 180.f;

	// Sound when character stop running
	Sound = CreateDefaultSubobject<UAudioComponent>(TEXT("Sound"));
	Sound->SetupAttachment(FirstPersonCameraComponent);

	SprintSound = CreateDefaultSubobject<UAudioComponent>(TEXT("SprintSound"));
	SprintSound->SetupAttachment(FirstPersonCameraComponent);

	// Sound when character turn on or off a flashlight
	Turnon = CreateDefaultSubobject<UAudioComponent>(TEXT("TunronSound"));
	Turnon->SetupAttachment(FirstPersonCameraComponent);
	static ConstructorHelpers::FObjectFinder<USoundCue> TurnonSound(TEXT("/Game/Assets/Sounds/SoundCues/FlashLight"));
	if (TurnonSound.Succeeded())
	{
		TurnOnSoundCue = TurnonSound.Object;
	}

	PanicSound = CreateDefaultSubobject<UAudioComponent>(TEXT("PanicSound"));
	PanicSound->SetupAttachment(FirstPersonCameraComponent);

	CigarLightOnSound = CreateDefaultSubobject<UAudioComponent>(TEXT("CigarLightOnSound"));
	CigarLightOnSound->SetupAttachment(FirstPersonCameraComponent);
	static ConstructorHelpers::FObjectFinder<USoundCue> IgniteSound(TEXT("/Game/Assets/Sounds/SoundCues/CigarLightCue"));
	if (IgniteSound.Succeeded())
	{
		CigarLightOnSoundCue = IgniteSound.Object;
		CigarLightOnSound->OnAudioFinished.AddDynamic(this, &AHorrorGameCharacter::CigarLightOn);
	}

	CigarLightOffSound = CreateDefaultSubobject<UAudioComponent>(TEXT("CigarLightOffSound"));
	CigarLightOffSound->SetupAttachment(FirstPersonCameraComponent);
	static ConstructorHelpers::FObjectFinder<USoundCue> TurnOffSound(TEXT("/Game/Assets/Sounds/SoundCues/CigarLightOffCue"));
	if (TurnOffSound.Succeeded())
	{
		CigarLightOffSoundCue = TurnOffSound.Object;
	}

	BellSound = CreateDefaultSubobject<UAudioComponent>(TEXT("BellSound"));
	BellSound->SetupAttachment(FirstPersonCameraComponent);
	static ConstructorHelpers::FObjectFinder<USoundCue> BellSoundWav(TEXT("/Game/Assets/Sounds/SoundCues/BellSoundCue"));
	if (BellSoundWav.Succeeded())
	{
		BellSound->SetSound(BellSoundWav.Object);
		BellSound->SetAutoActivate(false);
		BellSound->OnAudioFinished.AddDynamic(this, &AHorrorGameCharacter::BellSoundFinish);
	}

	Lantern = CreateDefaultSubobject<UChildActorComponent>(TEXT("Lantern"));
	Lantern->SetupAttachment(FirstPersonCameraComponent);
	Lantern->SetHiddenInGame(true);

	Sword = CreateDefaultSubobject<UChildActorComponent>(TEXT("Sword"));
	Sword->SetupAttachment(GetMesh(), TEXT("hand_r_socket"));
	Sword->SetHiddenInGame(true);

	Mirror = CreateDefaultSubobject<UChildActorComponent>(TEXT("Mirror"));
	Mirror->SetupAttachment(FirstPersonCameraComponent);
	Mirror->SetHiddenInGame(true);

	// Prepare Inventory array
	Inventory.Init(FHorrorGameItemData::FHorrorGameItemData(), 9);

	static ConstructorHelpers::FObjectFinder<UDataTable> DT_ItemTable(TEXT("/Game/Assets/Items/Item"));
	if (DT_ItemTable.Succeeded())
	{
		ItemTable = DT_ItemTable.Object;
	}
	
	SmokeComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Smoke"));
	SmokeComponent->SetupAttachment(FirstPersonCameraComponent);
	static ConstructorHelpers::FObjectFinder<UNiagaraSystem>Extinguisher_NS(TEXT("/Game/Assets/Material/NS_Extinguisher"));
	if (Extinguisher_NS.Succeeded())
	{
		SmokeComponent->SetAsset(Extinguisher_NS.Object);
	}

	SwordCount = 0;
	MirrorCount = 0;
	BellCount = 0;
	DoorBoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("DoorBox"));
	DoorBoxComp->SetupAttachment(FirstPersonCameraComponent);
	DoorBoxComp->OnComponentBeginOverlap.AddDynamic(this, &AHorrorGameCharacter::OnDoorBoxBeginOverlap);
	DoorBoxComp->OnComponentEndOverlap.AddDynamic(this, &AHorrorGameCharacter::OnDoorBoxEndOverlap);
	
	PlayerStatus = Player_Status::Loading;
	HitActor = nullptr;
}

void AHorrorGameCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	HorrorGamePlayerController = Cast<AHorrorGamePlayerController>(GetController());

	//Add Input Mapping Context
	if (HorrorGamePlayerController != nullptr)
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(HorrorGamePlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}

	// 현재 선택한 아이템의 인덱스는 -1.
	CurrentItemNum = -1;

	// 각 오디오 컴포넌트의 자동 활성화를 비활성화시킴.
	if (Sound->IsValidLowLevelFast())
	{
		// Sound->SetSound(RunStop);
		Sound->SetAutoActivate(false);
	}
	if (Turnon->IsValidLowLevelFast())
	{
		Turnon->SetSound(TurnOnSoundCue);
		Turnon->SetAutoActivate(false);
	}

	if (CigarLightOnSound->IsValidLowLevelFast())
	{
		CigarLightOnSound->SetSound(CigarLightOnSoundCue);
		CigarLightOnSound->SetAutoActivate(false);
	}

	if (CigarLightOffSound->IsValidLowLevelFast())
	{
		CigarLightOffSound->SetSound(CigarLightOffSoundCue);
		CigarLightOffSound->SetAutoActivate(false);
	}

	// 불빛 타임라인 커브 값이 있다면 불 깜빡임 타임라인에 할당하고, 재생될 때 실행할 콜백 함수도 바인딩함.
	if (CurveFloat)
	{
		FOnTimelineFloat TimelineProgress;
		TimelineProgress.BindDynamic(this, &AHorrorGameCharacter::LightFlicker);
		FlickeringLight.AddInterpFloat(CurveFloat, TimelineProgress);
		FlickeringLight.SetLooping(true);
	}

	// 카메라 회전 타임라인 커브 값이 있다면 카메라 회전 타임라인에 할당하고, 재생될 때 실행할 콜백 함수도 바인딩함.
	if (RotateCurveFloat)
	{
		FOnTimelineFloat TimelineProgress;
		TimelineProgress.BindDynamic(this, &AHorrorGameCharacter::OnFocusRotation);
		RotateCameraTimeline.AddInterpFloat(RotateCurveFloat, TimelineProgress);

		FOnTimelineEvent TimelineFinishEvent;
		TimelineFinishEvent.BindDynamic(this, &AHorrorGameCharacter::OnFocusFinished);
		RotateCameraTimeline.SetTimelineFinishedFunc(TimelineFinishEvent);

	}

	// 달리기 타임라인 커브 값이 있다면 달리기 타임라인에 할당하고, 재생될 때 실행할 콜백 함수도 바인딩함.
	if (SprintCurveFloat)
	{
		FOnTimelineFloat TimelineProgress;
		TimelineProgress.BindDynamic(this, &AHorrorGameCharacter::OnSprintCameraView);
		SprintCameraTimeline.AddInterpFloat(SprintCurveFloat, TimelineProgress);
	}

	// 착란 시 작동할 소리를 자동 활성화를 끄고, 소리 설정을 한 후, 오디오가 끝날 때 작동할 함수를 바인딩해줌.
	if (PanicSoundCue)
	{
		PanicSound->SetAutoActivate(false);
		PanicSound->SetSound(PanicSoundCue);
		PanicSound->OnAudioFinished.AddDynamic(this, &AHorrorGameCharacter::SetPanicScreamEnd);
	}

	// 포스트 프로세싱 머티리얼을 생성함.
	PostProcessDynamicInstance = UMaterialInstanceDynamic::Create(PostProcessMaterialInstance, this);
}

void AHorrorGameCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//AActor* HitActor = nullptr;

	// Ray Cast를 통해 상호작용 가능한 객체인지를 확인함.
	GetLineTraceSingle(HitActor);

	// 플레이어가 숨은 상태라면 적 개체에 의해 밀려서 숨는 공간에서 나오지 못하는 상황을 막기 위한 로직임,
	if (bIsHiding) 
	{
		// 숨기 전의 트랜스폼을 계속 유지하게 설정
		SetActorLocation(BeforeHideLocation);
		SetActorRotation(BeforeHideRotation);
	}

	FlickeringLight.TickTimeline(DeltaTime);
	RotateCameraTimeline.TickTimeline(DeltaTime);
	SprintCameraTimeline.TickTimeline(DeltaTime);

	//// 플레이어가 죽은 상황일 경우
	//if (PlayerStatus == Player_Status::Died)
	//{
	//	if (HorrorGamePlayerController != nullptr)
	//	{
	//		// 마우스를 보이게 설정함.
	//		HorrorGamePlayerController->SetShowMouseCursor(true);
	//		HorrorGamePlayerController->bEnableClickEvents = true;
	//		HorrorGamePlayerController->bEnableMouseOverEvents = true;
	//	}
	//}

	// 플레이어가 행동 불능 상태인 경우
	if (PlayerStatus == Player_Status::Stunned)
	{
		// 행동 불능 타이머를 프레임 전환 시간만큼 계속 더함.
		StunTimer += DeltaTime;

		// 행동 불능이 된 지 1.5초 이상된 경우
		if (StunTimer >= 1.5f)
		{
			// 타이머를 0으로 설정하고, 상태를 생존으로 변경함.
			StunTimer = 0.f;
			SetPlayerStatus(Player_Status::Survive);
		}
	}

	// 플레이어가 이동 중(속도가 0 초과)인 경우, 이동 중으로 설정.
	if (GetCharacterMovement()->Velocity.Length() > 0.f)
	{
		bIsMove = true;
	}
	else
	{
		bIsMove = false;
	}
	
	// 설명 텍스트가 출력된 경우.
	if (TextTimer > 0)
	{
		// 설명 타이머를 프레임 전환 시간만큼 계속 더함.
		cnt += DeltaTime;

		// 타이머가 지정한 초 이상으로 지속된 경우
		if (cnt >= TextTimer)
		{
			// 설명 텍스트를 빈 칸("")으로 초기화해줌.
			SetExplainText(NSLOCTEXT("AHorrorGameCharacter", "None_Explain", ""), 0);
		}
	}

	// 상호 작용 실패 텍스트가 출력된 경우
	if (ErrorTextTimer > 0)
	{
		// 상호 작용 실패 타이머를 프레임 전환 시간만큼 계속 더함.
		ErrorTextCount += DeltaTime;

		// 타이머가 지정한 초 이상으로 지속된 경우
		if (ErrorTextCount >= ErrorTextTimer)
		{
			// 상호 작용 실패 텍스트를 빈 칸("")으로 초기화해줌.
			SetErrorText(NSLOCTEXT("AHorrorGameCharacter", "None_Error", ""), 0);
		}
	}

	// 문서 보관함에 문서가 추가된 경우
	if (bIsArchiveTextOn)
	{
		// 문서 보관함 텍스트 타이머를 프레임 전환 시간만큼 계속 더함.
		ArchiveTextTimer += DeltaTime;

		// 타이머가 지정한 10 초 이상으로 지속된 경우
		if (ArchiveTextTimer >= 10.f)
		{
			// 플레이어가 사망한 상태라면
			if (PlayerStatus == Player_Status::Died)
			{
				// 사망 UI의 문서 보관함 텍스트를 빈 칸("")으로 초기화해줌.
				HorrorGamePlayerController->SetDeadUIText(FText::FromString(TEXT("")));
			}
			// 플레이어가 사망한 상태가 아니라면
			else
			{
				// 메인(인게임) UI의 문서 보관함 텍스트를 빈 칸("")으로 초기화해줌.
				if (GameUIWidget)
				{
					GameUIWidget->SetArchiveGetText(FText::FromString(TEXT("")));
				}
			}

			// 문서를 획득했다는 것을 끄고, 타이머도 초기화함.
			bIsArchiveTextOn = false;
			ArchiveTextTimer = 0.f;
		}
	}

	// 물 속에 있는 경우
	if (bIsInWater)
	{
		// 달릴 경우, 350에서 감소된 수치만큼으로 최대 속력을 설정함.
		if (bIsSprint)
		{
			GetCharacterMovement()->MaxWalkSpeed = 350.f * InWaterSpeedDown;
		}
		// 걷는 경우, 180에서 감소된 수치만큼으로 최대 속력을 설정함.
		else
		{
			GetCharacterMovement()->MaxWalkSpeed = 180.f * InWaterSpeedDown;
		}

		// 웅크려 이동하기는 80에서 감소된 수치만큼으로 최대 속력을 설정함.
		GetCharacterMovement()->MaxWalkSpeedCrouched = 80.f * InWaterSpeedDown;
	}
	else
	{
		// 달릴 경우, 350으로 최대 속력을 설정함.
		if (bIsSprint)
		{
			GetCharacterMovement()->MaxWalkSpeed = 350.f;
		}
		// 걷는 경우, 180으로 최대 속력을 설정함.
		else
		{
			GetCharacterMovement()->MaxWalkSpeed = 180.f;
		}

		// 웅크려 이동하기는 80으로 최대 속력을 설정함.
		GetCharacterMovement()->MaxWalkSpeedCrouched = 80.f;
	}

	// 청동 거울을 사용한 상태라면
	if (bIsTimeStop)
	{
		// 시간 정지 타이머를 프레임 전환 시간만큼 계속 더함.
		TimeStopTimer += DeltaTime;

		// 거울 속 세계로 들어간 (== 시간 정지) 상태가 25초 이상 지속된 경우. 
		if (TimeStopTimer >= 25)
		{
			// 타이머를 초기화하고
			TimeStopTimer = 0;
			bIsTimeStop = false;

			// 포스트 프로세스 머티리얼을 카메라에서 제거하고, BGM을 교체함.
			FirstPersonCameraComponent->RemoveBlendable(PostProcessMaterial);
			Cast<AHorrorGameGameMode>(UGameplayStatics::GetGameMode(GetWorld()))->StopMirrorUseBackGroundMusic();
		}
	}

	// 캐비닛이나 옷장에 숨은 경우로 패닉 게이지가 감소하는 상태인 경우
	if (bIsPatienceReduce)
	{
		// 착란 게이지를 1씩 감소시킴.
		AddPatience(-1);
		
		// 패닉 게이지가 줄어들 양만큼 줄어들었으면 줄어들 양을 초기화
		if (Patience == PatienceToReduce) 
		{
			PatienceToReduce = -1;
			bIsPatienceReduce = false; // 감소 중이 아니라고 알려줌
		}
	}

	// 착란 게이지의 무제한 상승 버그를 막기 위한 로직.
	if (bIsCooldown)
	{
		// 게이지 상승 쿨타임 타이머를 프레임 전환 시간만큼 계속 더함.
		CooldownTimer += DeltaTime;

		// 게이지 상승 쿨타임이 5초 이상 지났을 경우
		if (CooldownTimer >= 5.f)
		{
			// 타이머 초기화.
			CooldownTimer = 0.f;
			bIsCooldown = false;
		}
	}

	// 패닉 게이지가 100까지 도달했을 경우엔 어그로를 끌게 됨
	// 패닉이 100일 때 계속 소리를 내는 것을 방지하기 위해, Screaming하는 지 체크하여 하지 않을 경우에만 발동하도록 함
	if (Patience == 100 && !bIsScreaming) 
	{
		// 플레이어의 이동을 즉시 멈추고, 컨트롤러 Input도 못하게 설정함.
		GetCharacterMovement()->StopMovementImmediately();
		DisableInput(HorrorGamePlayerController);

		// 그 후 비명 중이라고 설정하고, 패닉 음도 재생함.
		bIsScreaming = true;
		PanicSound->Play();
	}
	
	// 현재 웅크리기 중이라면
	if (bIsCrouch)
	{
		// 스태미너가 소모된 상태라면
		if (Stamina <= 399)
		{
			// 스태미너를 1 씩 회복하고, 위젯에 스태미너 값을 업데이트 해줌.
			Stamina += 1;
			if (GameUIWidget)
			{
				GameUIWidget->SetStaminaHUD(Stamina);
			}
		}
	}
	// 웅크리기 중이 아니라면
	else
	{
		// 현재 달리는 중이라면
		if (bIsSprint)
		{
			// 스태미너가 모두 소모된 상태가 아니라면
			if (Stamina > 0)
			{
				// 스태미너를 2씩 감소시키고, 위젯에 스태미너 값을 업데이트 함.
				Stamina -= 2;
				if (GameUIWidget)
				{
					GameUIWidget->SetStaminaHUD(Stamina);
				}
			}
			// 스태미너가 모두 소모된 상태라면, 달리기를 멈춤.
			else
			{
				EndSprint();
			}
		}
		// 달리는 중이 아니라면
		else
		{
			// 스태미너가 소모된 상태라면
			if (Stamina <= 399)
			{
				// 스태미너를 1 씩 회복하고, 위젯에 스태미너 값을 업데이트 해줌.
				Stamina += 1;
				if (GameUIWidget)
				{
					GameUIWidget->SetStaminaHUD(Stamina);
				}
			}
		}
	}

	// 리퍼가 능력을 사용하기 시작했다면
	if (bReaperWatchPlayer && PlayerStatus != Player_Status::Stunned) 
	{
		// 리퍼가 바라본 시간을 프레임 전환 시간만큼 계속 더함.
		ReaperWatchElapsedTime += DeltaTime;
		
		// 리퍼가 플레이어를 바라본지 3초의 시간이 안 지났을 경우
		if (ReaperWatchElapsedTime < 3.0f) 
		{
			// 스턴 효과 포스트 프로세스 머티리얼이 존재하면 카메라에 포스트 프로세싱 머티리얼을 추가함.
			if (PostProcessMaterialInstance)
			{
				// 서서히 일렁이는 효과를 주도록 함.
				PixelateIntensity = FMath::Lerp(0.0f, 1.0f, ReaperWatchElapsedTime / 3.0f);
				PostProcessDynamicInstance->SetScalarParameterValue(TEXT("Switch"), PixelateIntensity);
			}
		}
		// 리퍼가 플레이어를 바라본 지 3초의 시간이 흘렀을 경우
		else if (ReaperWatchElapsedTime >= 3.0f)
		{
			// 타이머를 초기화함.
			ReaperWatchElapsedTime = 0.f;

			// 스턴 소리를 재생함.
			if (StunSoundCue)
			{
				UGameplayStatics::PlaySound2D(this, StunSoundCue);
			}

			// 그 후 플레이어는 행동 불능 상태로 만듦.
			SetPlayerStatus(Player_Status::Stunned);
		}
	}
}

//////////////////////////////////////////////////////////////////////////// Input
void AHorrorGameCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		//Jumping
		//EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
		//EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		//Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AHorrorGameCharacter::Move);

		//Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AHorrorGameCharacter::Look);

	}
	// Crouch
	PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &AHorrorGameCharacter::BeginCrouch);
	PlayerInputComponent->BindAction("Crouch", IE_Released, this, &AHorrorGameCharacter::EndCrouch);

	// Sprint
	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &AHorrorGameCharacter::BeginSprint);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &AHorrorGameCharacter::EndSprint);

	// Interact
	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &AHorrorGameCharacter::Interact);

	// Item Use
	PlayerInputComponent->BindAction("ItemUse", IE_Pressed, this, &AHorrorGameCharacter::ItemUse);
	
	// Item Selection
	//PlayerInputComponent->BindAction("ItemChange", IE_Pressed, this, &AHorrorGameCharacter::SelectItem);
	PlayerInputComponent->BindAction("ItemLeftSelect", IE_Pressed, this, &AHorrorGameCharacter::ScrollUpItem);
	PlayerInputComponent->BindAction("ItemRightSelect", IE_Pressed, this, &AHorrorGameCharacter::ScrollDownItem);
}

// 이동 관련
void AHorrorGameCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (HorrorGamePlayerController != nullptr)
	{
		// add movement 
		if (!bIsHiding)
		{
			AddMovementInput(GetActorForwardVector(), MovementVector.Y);
			AddMovementInput(GetActorRightVector(), MovementVector.X);
		}
	}
}

// 카메라 회전(뷰포트 회전) 관련
void AHorrorGameCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>() * MouseSensitive;
	if (HorrorGamePlayerController != nullptr)
	{
		// 플레이어가 숨어있는 경우
		if (bIsHiding)
		{
			// 현재 RayCasting을 한 액터를 가져옴.
			// AActor* HitActor = nullptr;
			// bool isHit = GetLineTraceSingle(HitActor);

			//if (isHit)
			//{
				if (HitActor)
				{
					// 해당 액터가 숨을 수 있는 액터라면
					if (AHideObject* HideObject = Cast<AHideObject>(HitActor))
					{
						// 해당 액터 내부 카메라의 Pitch와 Yaw를 변경시켜 회전하는 것으로 함.
						HideObject->Pitch += LookAxisVector.Y * -1.0f;
						HideObject->Yaw += LookAxisVector.X;
						HideObject->MoveCamera(); // Call Cabinet_cpp Hide Function
					}

					// Deprecated
					/*
					if(ACabinet_cpp* Cabinet = Cast<ACabinet_cpp>(HitActor))
					{
						//ACabinet_cpp* Cabinet = Cast<ACabinet_cpp>(HitActor);
						Cabinet->Pitch += LookAxisVector.Y * -1.f;
						Cabinet->Yaw += LookAxisVector.X;
						Cabinet->MoveCamera(); // Call Cabinet_cpp Hide Function
					}

					//else if (HitActor->IsA<AWardrobe_cpp>())
					else if(AWardrobe_cpp* Wardrobe = Cast<AWardrobe_cpp>(HitActor))
					{
						//AWardrobe_cpp* Wardrobe = Cast<AWardrobe_cpp>(HitActor);
						Wardrobe->Pitch += LookAxisVector.Y * -1.0f;
						Wardrobe->Yaw += LookAxisVector.X;
						Wardrobe->MoveCamera(); // Call Cabinet_cpp Hide Function
					}
					*/
					
				}
			//}
		}
		// 숨어있는 상태가 아니면 플레이어 컨트롤러의 Pitch와 Yaw를 변경시킴.
		else
		{
			// add yaw and pitch input to controller
			AddControllerYawInput(LookAxisVector.X);
			AddControllerPitchInput(LookAxisVector.Y);
		}
	}
}

// Deprecated
void AHorrorGameCharacter::SetHasRifle(bool bNewHasRifle)
{
	bHasRifle = bNewHasRifle;
}

// Deprecated
bool AHorrorGameCharacter::GetHasRifle()
{
	return bHasRifle;
}

// Sprint Functions: BeginSprint(), EndSprint(), StaminaChange()
// 달리기를 시작하는 함수.
void AHorrorGameCharacter::BeginSprint()
{
	// 플레이어가 웅크리지 않은 상태여야 작동함.
	if (!bIsCrouch) 
	{
		// 스태미너가 전부 소모되지 않은 상태라면
		if (Stamina > 0)
		{
			// 달리기가 활성화됨.
			bIsSprint = true;

			// 달리는 중이라는 효과를 주기 위해 카메라의 FOV를 조절하는 타임라인을 재생함.
			SprintCameraTimeline.PlayFromStart();

			// 숨을 내쉬는 소리가 재생 중이면 멈추고, 달리는 소리를 재생함.
			if (Sound->GetPlayState() == EAudioComponentPlayState::Playing)
			{
				Sound->Stop();
			}

			SprintSound->Play();
		}
	}
}

// 달리기를 멈추는 함수.
void AHorrorGameCharacter::EndSprint()
{
	// 달리는 중이었다면, 숨을 내쉬는 소리를 재생하고 달리기를 비활성화함.
	if (bIsSprint)
	{
		Sound->Play();
	}
	bIsSprint = false;

	// 다시 원래 상태로 돌아가기 위해 카메라의 FOV를 조절하는 타임라인을 역재생함.
	SprintCameraTimeline.Reverse();

	// 달리는 소리가 재생 중이면, 멈춤.
	if (SprintSound->GetPlayState() == EAudioComponentPlayState::Playing)
	{
		SprintSound->Stop();
	}
}

// Deprecated. Tick에서 작동 중임.
void AHorrorGameCharacter::StaminaChange()
{
	if (bIsCrouch)
	{
		if (Stamina <= 199)
		{
			Stamina += 1;
		}
	}
	else
	{
		if (bIsSprint)
		{
			if (Stamina > 0)
			{
				Stamina -= 2;
			}
			else
			{
				EndSprint();
			}
		}
		else
		{
			if (Stamina <= 199)
			{
				Stamina += 1;
			}
		}
	}

	GameUIWidget->SetStaminaHUD(Stamina);
	// GetWorld()->GetTimerManager().ClearTimer(_loopStaminaTimerHandle);
	//GetWorld()->GetTimerManager().SetTimer(_loopStaminaTimerHandle, this, &AHorrorGameCharacter::StaminaChange, 0.05f, false);
}

// Courching Fucntion: BeginCrouch(), EndCrouch()
// 웅크리는 함수.
void AHorrorGameCharacter::BeginCrouch()
{
	// 웅크리기를 활성화함.
	Crouch();
	
	bIsCrouch = true;
}

// 일어서는 함수.
void AHorrorGameCharacter::EndCrouch()
{
	// 웅크리기를 비활성화함.
	UnCrouch();
	
	bIsCrouch = false;
}

// Interaction with another actors: Interact(), ItemUse() 
// 상호작용하는 함수.
void AHorrorGameCharacter::Interact()
{
	//AActor* HitActor = nullptr;
	//bool isHit = GetLineTraceSingle(HitActor);

	//if (isHit)
	//{
		// 캐스팅된 액터가 있다면
		if (HitActor)
		{
			// 그 액터가 아이템 류일 경우
			if (HitActor->GetClass()->ImplementsInterface(UInteractInterface::StaticClass()))
			{
				// 해당 아이템류와 상호 작용 함수를 호출함.
				auto InterfaceVariable = Cast<IInteractInterface>(HitActor);
				
				InterfaceVariable->OnInteract(this);

				// 아이템을 얻을 수 있는지 체크하는 변수를 초기화함.
				if(!bCanItemGet)
				{
					bCanItemGet = true;
				}
			}
			// 그 액터가 문 류일 경우
			else if (HitActor->GetClass()->ImplementsInterface(UDoorInterface_cpp::StaticClass()))
			{
				// 해당 문과의 상호 작용 함수를 호출함.
				auto InterfaceVariable = Cast<IDoorInterface_cpp>(HitActor);

				InterfaceVariable->OnInteract(this);
			}
			// 그 액터가 숨는 액터 류일 경우
			else if (HitActor->GetClass()->ImplementsInterface(UHideInterface::StaticClass()))
			{
				// 해당 액터와의 상호 작용 함수를 호출함.
				auto InterfaceVariable = Cast<IHideInterface>(HitActor);

				InterfaceVariable->OnInteract(this);
				
				 // 숨은 상태가 되면 숨기 전의 트랜스폼을 설정해줌.
				if (bIsHiding)
				{
					BeforeHideLocation = GetActorLocation();
					BeforeHideRotation = GetActorRotation();
				}
			}
			// 그 액터가 옷장 서랍일 경우
			else if (AWardrobeDrawer_cpp* WardDrawer = Cast<AWardrobeDrawer_cpp>(HitActor))
			{
				// 상호 작용 함수를 호출함.
				WardDrawer->OnInteract();
			}
			// 그 액터가 서랍일 경우
			else if (ADrawer_cpp* Drawer = Cast<ADrawer_cpp>(HitActor))
			{
				// 상호 작용 함수를 호출함.
				Drawer->OnInteract();
			}
			// 그 액터가 사물함 문일 경우
			else if(ALockerDoorActor_cpp* Locker = Cast<ALockerDoorActor_cpp>(HitActor))
			{
				// 상호 작용 함수를 호출함.
				Locker->OnInteract(this);
			}
			// 그 액터가 책상 서랍일 경우
			else if (ADeskDrawer_cpp* DeskDrawer = Cast<ADeskDrawer_cpp>(HitActor))
			{
				// 상호 작용 함수를 호출함.
				DeskDrawer->OnInteract();
			}
			// 그 액터가 전등일 경우
			else if (AHangingLight* HangingLight = Cast<AHangingLight>(HitActor))
			{
				// 상호 작용 함수를 호출함.
				HangingLight->OnInteract();
			}
			// 그 액터가 경보기일 경우
			else if (AAlarm* Alarm = Cast<AAlarm>(HitActor))
			{
				// 상호 작용 함수를 호출함.
				Alarm->OnInteract();
			}	
			// 그 액터가 전등 스위치일 경우
			else if (ALightSwitch* Switch = Cast<ALightSwitch>(HitActor))
			{
				// 상호 작용 함수를 호출함.
				Switch->OnInteract();
			}
			// 그 액터가 제단일 경우
			else if (AAltar_cpp* Altar = Cast<AAltar_cpp>(HitActor))
			{
				// 아직 클리어하지 않을 경우 상호 작용 함수를 호출함.
				if (!bIsCleared)
				{
					Altar->OnInteract(this);
				}
			}
			// 그 액터가 스위치 레버일 경우
			else if (ASwitchLever* Lever = Cast<ASwitchLever>(HitActor))
			{
				// 상호 작용 함수를 호출함.
				Lever->OnInteract(this);
			}
			// 그 액터가 분전함일 경우
			else if (ADistributionBox* DBox = Cast<ADistributionBox>(HitActor))
			{
				// 상호 작용 함수를 호출함.
				DBox->OnInteract(this);
			}
			// 그 액터가 종이(문서)일 경우
			else if (APaper* Paper = Cast<APaper>(HitActor))
			{
				// 상호 작용 함수를 호출함.
				Paper->OnInteract(this);
			}
			// 그 액터가 엔딩 거울일 경우
			else if (AEnd_Mirror* EndMirror = Cast<AEnd_Mirror>(HitActor))
			{
				// 상호 작용 함수를 호출함.
				EndMirror->OnInteract(this);
			}
		}
	//}
}

// 아이템을 사용하는 함수.
void AHorrorGameCharacter::ItemUse()
{
	// 인덱스 에러 대비
	if (CurrentItemNum >= 0 && 0 <= InventoryNum) // Avoid Index Error
	{
		// 현재 선택된 아이템을 가져옴.
		FHorrorGameItemData& CurrentItem = Inventory[CurrentItemNum];

		// 선택한 아이템이 라이터일 경우, 라이터 사용 함수를 호출함.
		if (CurrentItem.ItemNumber == 1) // CigarLighter
		{
			UseCigarLight();
		}
		// 선택한 아이템이 플래시 라이트일 경우, 플래시 라이트 사용 함수를 호출함.
		else if (CurrentItem.ItemNumber == 2) // FlashLight
		{
			UseFlashLight();
		}
		// 선택한 아이템이 열쇠일 경우, 열쇠 사용 함수를 호출함.
		else if (CurrentItem.ItemNumber == 3) // Key
		{
			UseKey();
		}
		// 선택한 아이템이 타이머일 경우, 타이머 사용 함수를 호출함.
		else if (CurrentItem.ItemNumber == 4) // Timer
		{
			UseTimer();
		}
		// 선택한 아이템이 청동 검일 경우, 청동 검 사용 함수를 호출함.
		else if (CurrentItem.ItemNumber == 5) // Sword
		{
			UseSword();
		}
		// 선택한 아이템이 청동 방울일 경우, 청동 방울 사용 함수를 호출함.
		else if (CurrentItem.ItemNumber == 6) // Bell
		{
			UseBell();
		}
		// 선택한 아이템이 청동 거울일 경우, 청동 거울 사용 함수를 호출함.
		else if (CurrentItem.ItemNumber == 7) // Mirror
		{
			UseMirror();
		}
		// 선택한 아이템이 소화기일 경우, 소화기 사용 함수를 호출함.
		else if (CurrentItem.ItemNumber == 8) // Extinguisher
		{
			if(bCanExtinguisherUse)
				UseExtinguisher();
		}
		// 선택한 아이템이 절단기일 경우, 절단기 사용 함수를 호출함.
		else if (CurrentItem.ItemNumber == 9) // Cutter
		{
			UseCutter();
		}
		// 선택한 아이템이 영혼 랜턴일 경우, 영혼 랜턴 사용 함수를 호출함.
		else if (CurrentItem.ItemNumber == 10) // Lantern
		{
			AActor* ChildActor = Lantern->GetChildActor();
			if (ASoul_Lantern_cpp* SoulLantern = Cast<ASoul_Lantern_cpp>(ChildActor))
			{
				SoulLantern->UseInteract(this);
			}
		}
		// 선택한 아이템이 야광봉일 경우, 야광봉 사용 함수를 호출함.
		else if (CurrentItem.ItemNumber == 11) // GlowStick
		{
			UseGlowStick();
		}

	}
}

// LineTrace Functions: GetLineTraceSingle(), GetLineTraceSingleForBP, GetLineTraceSingleForBPActor()
// Ray Casting을 하는 함수
bool AHorrorGameCharacter::GetLineTraceSingle(AActor* &inHitActor)
{
	if (FirstPersonCameraComponent == nullptr)
	{
		return false;
	}

	// Ray Cast를 위한 Ray의 시작 지점과 끝 지점을 설정해줌.
	FHitResult OutHit;
	FVector Start = FirstPersonCameraComponent->GetComponentLocation();
	FVector ForwardVector = FirstPersonCameraComponent->GetForwardVector();
	FVector End = (ForwardVector * TraceLength) + Start;

	// 해당 지점을 향해 보이는 모든 것들을 Ray cast함.
	bool isHit = GetWorld()->LineTraceSingleByChannel(OutHit, Start, End, ECC_Visibility);
	inHitActor = OutHit.GetActor();
	if (isHit)
	{
		if (OutHit.GetActor())
		{
			// 부딪힌 액터가 Item 클래스인 경우
			if (inHitActor->IsA<AItems>()) // Item 클래스 일 경우
			{
				// 상호 작용 조준점으로 조준점을 변경하고, "Take" 텍스트를 출력함.
				if (GameUIWidget)
				{
					GameUIWidget->SetInteractDotText(NSLOCTEXT("AHorrorGameCharacter","Look_Item","Take"));
					GameUIWidget->SetInteractDot(true);
				}
				return true;
			}
			// 부딪힌 액터가 서랍 클래스인 경우
			else if(inHitActor->IsA<ADrawerClass>()) // 서랍류(책상 서랍, 옷장 서랍, 일반 서랍)일 경우
			{
				// 상호 작용 조준점으로 조준점을 변경하고, "Open/Close" 텍스트를 출력함.
				if (GameUIWidget)
				{
					GameUIWidget->SetInteractDotText(NSLOCTEXT("AHorrorGameCharacter", "Look_Drawer", "Open/Close"));
					GameUIWidget->SetInteractDot(true);
				}
				return true;
			}
			// 부딪힌 액터가 문 클래스인 경우
			else if (inHitActor->GetClass()->ImplementsInterface(UDoorInterface_cpp::StaticClass())) // 문일 경우(교실문, 창살문, 방문)
			{
				// 상호 작용 조준점으로 조준점을 변경하고, "Open/Close" 텍스트를 출력함.
				if (GameUIWidget)
				{
					GameUIWidget->SetInteractDotText(NSLOCTEXT("AHorrorGameCharacter", "Look_Door", "Open/Close"));
					GameUIWidget->SetInteractDot(true);
				}
				return true;
			}
			// 부딪힌 액터가 숨는 액터 클래스인 경우
			else if (inHitActor->GetClass()->ImplementsInterface(UHideInterface::StaticClass())) // 숨는 포인트일 경우(옷장, 캐비닛)
			{
				// 상호 작용 조준점으로 조준점을 변경함
				if (GameUIWidget)
				{
					// 숨은 상태일 경우, "Exit" 텍스트를 출력함.
					if (bIsHiding)
					{
						GameUIWidget->SetInteractDotText(NSLOCTEXT("AHorrorGameCharacter", "Look_Cabinet_While_Hiding", "Exit"));
					}
					// 숨지 않은 상태일 경우, "Hide" 텍스트를 출력함.
					else
					{
						GameUIWidget->SetInteractDotText(NSLOCTEXT("AHorrorGameCharacter", "Look_Cabinet", "Hide"));
					}
					GameUIWidget->SetInteractDot(true);
				}
				return true;
			}
			// 부딪힌 액터가 사물함 문 액터 클래스인 경우
			else if(inHitActor->IsA<ALockerDoorActor_cpp>()) // 사물함 문일경우
			{
				// 상호 작용 조준점으로 조준점을 변경하고, "Open/Close" 텍스트를 출력함.
				if (GameUIWidget)
				{
					GameUIWidget->SetInteractDotText(NSLOCTEXT("AHorrorGameCharacter", "Look_Locker", "Open/Close"));
					GameUIWidget->SetInteractDot(true);
				}
				return true;
			}
			// 부딪힌 액터가 경보기 액터 클래스인 경우
			else if (inHitActor->IsA<AAlarm>()) // 경보기일 경우
			{
				// 상호 작용 조준점으로 조준점을 변경하고, "Ring" 텍스트를 출력함.
				if (GameUIWidget)
				{
					GameUIWidget->SetInteractDotText(NSLOCTEXT("AHorrorGameCharacter", "Look_Alarm", "Ring"));
					GameUIWidget->SetInteractDot(true);
				}
				return true;
			}
			// 부딪힌 액터가 전등 액터 클래스인 경우
			else if (AHangingLight* HangLight = Cast<AHangingLight>(inHitActor)) // 전등일 경우
			{
				// 불이 안 켜진 상태일 때만 상호 작용 조준점으로 조준점을 변경하고, "Turn on" 텍스트를 출력함.
				if (!HangLight->bIsLightOn)
				{
					if (GameUIWidget)
					{
						GameUIWidget->SetInteractDotText(NSLOCTEXT("AHorrorGameCharacter", "Look_Hanging_Light", "Turn On"));
						GameUIWidget->SetInteractDot(true);
					}
					return true;
				}
			}
			// 부딪힌 액터가 분전함 액터 클래스인 경우
			else if (ADistributionBox* DBox = Cast<ADistributionBox>(inHitActor)) // 전압기일 경우
			{
				// 분전함의 전원이 안 켜진 상태일 때만 상호 작용 조준점으로 조준점을 변경하고, "Power on" 텍스트를 출력함.
				if (!DBox->bIsPowered)
				{
					if (GameUIWidget)
					{
						GameUIWidget->SetInteractDotText(NSLOCTEXT("AHorrorGameCharacter", "Look_Distribution_Box", "Power On"));
						GameUIWidget->SetInteractDot(true);
					}
					return true;
				}
			}
			// 부딪힌 액터가 전등 스위치 액터 클래스인 경우
			else if (inHitActor->IsA<ALightSwitch>()) // 전등 스위치일 경우
			{
				// 상호 작용 조준점으로 조준점을 변경하고, "Turn on" 텍스트를 출력함.
				if (GameUIWidget)
				{
					GameUIWidget->SetInteractDotText(NSLOCTEXT("AHorrorGameCharacter", "Look_Light_Switch", "Turn On"));
					GameUIWidget->SetInteractDot(true);
				}
				return true;
			}
			// 부딪힌 액터가 제단 액터 클래스인 경우
			else if (inHitActor->IsA<AAltar_cpp>()) // 제단일 경우
			{
				// 아직 클리어한 상태가 아닌 경우에만 상호 작용 조준점으로 조준점을 변경하고, "Place the Reaper's Items" 텍스트를 출력함.
				if (!bIsCleared)
				{
					if (GameUIWidget)
					{
						GameUIWidget->SetInteractDotText(NSLOCTEXT("AHorrorGameCharacter", "Look_Altar", "Place the Reaper's Items"));
						GameUIWidget->SetInteractDot(true);
					}
					return true;
				}
			}
			// 부딪힌 액터가 스위치 레버 액터 클래스인 경우
			else if (inHitActor->IsA<ASwitchLever>()) // 레버일 경우
			{
				// 상호 작용 조준점으로 조준점을 변경하고, "Lift Up/Down" 텍스트를 출력함.
				if (GameUIWidget)
				{
					GameUIWidget->SetInteractDotText(NSLOCTEXT("AHorrorGameCharacter", "Look_Switch_Lever", "Lift Up/Down"));
					GameUIWidget->SetInteractDot(true);
				}
				return true;
			}
			// 부딪힌 액터가 종이 액터 클래스인 경우
			else if (inHitActor->IsA<APaper>()) // 종이일 경우
			{
				// 상호 작용 조준점으로 조준점을 변경하고, "Look" 텍스트를 출력함.
				if (GameUIWidget)
				{
					GameUIWidget->SetInteractDotText(NSLOCTEXT("AHorrorGameCharacter", "Look_Paper", "Look"));
					GameUIWidget->SetInteractDot(true);
				}
				return true;
			}
			// 부딪힌 액터가 엔딩 거울 액터 클래스인 경우
			else if (inHitActor->IsA<AEnd_Mirror>()) // 엔딩용 거울일 경우
			{
				// 상호 작용 조준점으로 조준점을 변경하고, "Go To Natty" 텍스트를 출력함.
				if (GameUIWidget)
				{
					GameUIWidget->SetInteractDotText(NSLOCTEXT("AHorrorGameCharacter", "Look_Mirror", "Go To Natty"));
					GameUIWidget->SetInteractDot(true);
				}
				return true;
			}
		}
	}
	// 부딪힌 액터가 없다면 일반 조준점으로 변경하고 텍스트를 빈 칸으로 설정함.
	if (GameUIWidget)
	{
		GameUIWidget->SetInteractDotText(NSLOCTEXT("AHorrorGameCharacter", "Nothing_Look", ""));
		GameUIWidget->SetInteractDot(false); // 에러
	}
	return false;
}

// 카메라 노이즈 설정하는 함수.
void AHorrorGameCharacter::SetCameraComponentNoise(int32 WhichStatus)
{
	FTimerHandle NoiseTimer;
	GetWorldTimerManager().ClearTimer(NoiseTimer);
	AGameModeBase* GameMode = UGameplayStatics::GetGameMode(this);
	
	switch (WhichStatus)
	{
		// 근처에 Creature가 있는 상황이면, 카메라에 노이즈 걸 것임
		case 1:
		{
			// 비네팅과 필름 그레인을 설정함.
			FirstPersonCameraComponent->PostProcessSettings.bOverride_VignetteIntensity = true;
			FirstPersonCameraComponent->PostProcessSettings.VignetteIntensity = 1.f;
			FirstPersonCameraComponent->PostProcessSettings.bOverride_FilmGrainIntensity = true;
			FirstPersonCameraComponent->PostProcessSettings.FilmGrainIntensity = 1.f;
			
			// 거울을 사용하지 않은 상태여야 재생안함
			if (!bIsTimeStop) 
			{
				if (AHorrorGameGameMode* HorrorGameGameMode = Cast<AHorrorGameGameMode>(GameMode))
				{
					HorrorGameGameMode->PlayNervousBackGroundMusic();
				}
				else if (APrologueGameMode* PrologueGameMode = Cast<APrologueGameMode>(GameMode))
				{
					PrologueGameMode->PlayNervousBackGroundMusic();
				}
			}

			return;
		}
		// 추격 판정이 뜰 경우 노이즈 크게 함
		case 2:
		{
			// 비네팅과 필름 그레인의 세기를 더 키워 추격 판정이 떴다고 알림.
			FirstPersonCameraComponent->PostProcessSettings.bOverride_VignetteIntensity = true;
			FirstPersonCameraComponent->PostProcessSettings.VignetteIntensity = 3.f;
			FirstPersonCameraComponent->PostProcessSettings.bOverride_FilmGrainIntensity = true;
			FirstPersonCameraComponent->PostProcessSettings.FilmGrainIntensity = 15.f;
		
			// 1초동안 노이즈 크게 하고 다시 1번 상태로 돌릴 것임
			GetWorldTimerManager().SetTimer(NoiseTimer, FTimerDelegate::CreateLambda([&]() {
				SetCameraComponentNoise(1); 
			}), 1.0f, false);
			return;
		}
		// 시네마틱 용 케이스임.
		case 3:
		{
			// 비네팅과 필름 그레인을 설정함.
			FirstPersonCameraComponent->PostProcessSettings.bOverride_VignetteIntensity = true;
			FirstPersonCameraComponent->PostProcessSettings.VignetteIntensity = 1.f;
			FirstPersonCameraComponent->PostProcessSettings.bOverride_FilmGrainIntensity = true;
			FirstPersonCameraComponent->PostProcessSettings.FilmGrainIntensity = 1.f;
			break;
		}
		// 엔딩 용 케이스임.
		case 4:
		{
			// 비네팅과 필름 그레인을 비활성화함.
			FirstPersonCameraComponent->PostProcessSettings.bOverride_VignetteIntensity = false;
			FirstPersonCameraComponent->PostProcessSettings.bOverride_FilmGrainIntensity = false;
			break;
		}
		// Creature가 근처에 없다면 카메라에 노이즈 해제함
		default: 
		{
			// 비네팅과 필름 그레인을 비활성화함.
			FirstPersonCameraComponent->PostProcessSettings.bOverride_VignetteIntensity = false;
			FirstPersonCameraComponent->PostProcessSettings.bOverride_FilmGrainIntensity = false;
			
			// 시간 정지 상태가 아닐 경우
			if (!bIsTimeStop)
			{
				// 플레이어가 생존하거나 행동 불능일 경우에만
				if (PlayerStatus == Player_Status::Survive || PlayerStatus == Player_Status::Stunned) // 생존 또는 스턴 상태일 때만, 작동하도록
				{
					// 긴장 BGM을 종료함.
					if (AHorrorGameGameMode* HorrorGameGameMode = Cast<AHorrorGameGameMode>(GameMode))
					{
						HorrorGameGameMode->StopNervousBackGroundMusic();
					}
					else if (APrologueGameMode* PrologueGameMode = Cast<APrologueGameMode>(GameMode))
					{
						PrologueGameMode->StopNervousBackGroundMusic();
					}
				}
				// 플레이어가 사망 시
				else if (PlayerStatus == Player_Status::Died)
				{
					// 사망 BGM을 재생함.
					if (AHorrorGameGameMode* HorrorGameGameMode = Cast<AHorrorGameGameMode>(GameMode))
					{
						HorrorGameGameMode->PlayDiedBackGroundMusic();
					}
					else if (APrologueGameMode* PrologueGameMode = Cast<APrologueGameMode>(GameMode))
					{
						PrologueGameMode->PlayDiedBackGroundMusic();
					}
				}
				// 클리어한 상태일 시
				else if (PlayerStatus == Player_Status::Clear)
				{
					// 클리어 BGM을 재생함.
					if (AHorrorGameGameMode* HorrorGameGameMode = Cast<AHorrorGameGameMode>(GameMode))
					{
						HorrorGameGameMode->PlayClearBackGroundMusic();
					}
					else if (APrologueGameMode* PrologueGameMode = Cast<APrologueGameMode>(GameMode))
					{
						PrologueGameMode->PlayClearBackGroundMusic();
					}
				}
				// 엔딩의 경우 엔딩 BGM을 재생함.
				else if (PlayerStatus == Player_Status::Ending)
				{
					if (AHorrorGameGameMode* HorrorGameGameMode = Cast<AHorrorGameGameMode>(GameMode))
					{
						HorrorGameGameMode->PlayEndingBackGroundMusic();
					}
				}
			}
		}
	}
}

// 옵션 설정한 후 적용하는 함수
void AHorrorGameCharacter::SetPlayerSetting()
{
	// 조준점과 타이머 및 민감도 설정을 적용함.
	if (UHorrorGameGameInstance* GameInstance = Cast<UHorrorGameGameInstance>(UGameplayStatics::GetGameInstance(GetWorld())))
	{
		if (GameUIWidget)
		{
			GameUIWidget->SetTimerWidget(GameInstance->GetIsTimerOn());
			GameUIWidget->SetCrossHairVisible(GameInstance->GetIsCrossHairOn());
		}
		MouseSensitive = GameInstance->GetMouseSensitive();
	}
}

// Deprecated. 현재는 사용 안 함.
void AHorrorGameCharacter::SelectItem(FKey fkey)
{
	FString KeyName = fkey.GetFName().ToString();
	int32 KeyNumber = FCString::Atoi(*KeyName);
	switch (KeyNumber)
	{
		case 1:
		{
			if (InventoryNum < 0) CurrentItemNum = InventoryNum;
			else CurrentItemNum = 0;
			break;
		}
		case 2:
		{
			if (InventoryNum < 1)  CurrentItemNum = InventoryNum;
			else CurrentItemNum = 1;
			break;
		}
		case 3:
		{
			if (InventoryNum < 2)  CurrentItemNum = InventoryNum;
			else CurrentItemNum = 2;
			break;
		}
		case 4:
		{
			if (InventoryNum < 3)  CurrentItemNum = InventoryNum;
			else CurrentItemNum = 3;
			break;
		}
		case 5:
		{
			if (InventoryNum < 4)  CurrentItemNum = InventoryNum;
			else CurrentItemNum = 4;
			break;
		}
		case 6:
		{
			if (InventoryNum < 5)  CurrentItemNum = InventoryNum;
			else CurrentItemNum = 5;
			break;
		}
		case 7:
		{
			if (InventoryNum < 6)  CurrentItemNum = InventoryNum;
			else CurrentItemNum = 6;
			break;
		}
		case 8:
		{
			if (InventoryNum < 7)  CurrentItemNum = InventoryNum;
			else CurrentItemNum = 7;
			break;
		}
		case 9:
		{
			if (InventoryNum < 8)  CurrentItemNum = InventoryNum;
			else CurrentItemNum = 8;
			break;
		}
	}
	// UE_LOG(LogTemp, Warning, TEXT("SelectItem(): Current Item Number: %d"), CurrentItemNum);
	if (GameUIWidget)
	{
		GameUIWidget->Init();
	}
}

// 마우스 스크롤을 통해 아이템을 변경하는 함수.
void AHorrorGameCharacter::ScrollUpItem()
{
	// 현재 아이템 인덱스를 1 증가시키고, 최대 인덱스를 넘어가면 0으로 설정함.
	CurrentItemNum++;
	if (CurrentItemNum > InventoryNum)
	{
		CurrentItemNum = 0;
	}
	
	// 현재 라이터를 켠 상태에서 라이터를 선택하지 않았다면, 끄도록 설정함.
	if (GetCurrentItemNumber() != 1 && bIsCigarLightOn)
	{
		UseCigarLight();
	}
	// 현재 플래시 라이트를 켠 상태에서 플래시 라이트를 선택하지 않았다면, 끄도록 설정함.
	if (GetCurrentItemNumber() != 2 && bIsFlashLightOn)
	{
		UseFlashLight();
	}

	CurrentItem();
}

// 마우스 스크롤을 통해 아이템을 변경하는 함수.
void AHorrorGameCharacter::ScrollDownItem()
{
	// 현재 아이템 인덱스를 1 감소시키고, 0 미만으로 내려가면 최대 인덱스로 설정함.
	CurrentItemNum--;
	if (CurrentItemNum < 0 && InventoryNum >= 0)
	{
		CurrentItemNum = InventoryNum;
	}
	
	// 현재 라이터를 켠 상태에서 라이터를 선택하지 않았다면, 끄도록 설정함.
	if (GetCurrentItemNumber() != 1 && bIsCigarLightOn)
	{
		UseCigarLight();
	}
	// 현재 플래시 라이트를 켠 상태에서 플래시 라이트를 선택하지 않았다면, 끄도록 설정함.
	if (GetCurrentItemNumber() != 2 && bIsFlashLightOn)
	{
		UseFlashLight();
	}

	CurrentItem();	
}

// 라이터를 인벤토리에 추가하는 함수.
void AHorrorGameCharacter::AddCigarLight()
{
	// 라이터 데이터를 가져옴.
	FHorrorGameItemData* CigarLightData = ItemTable->FindRow<FHorrorGameItemData>(*FString::FromInt(1), TEXT(""));
	//bool isFind = false;

	if (CigarLightData)
	{
		// 인벤토리에 있는지 체크해 내부에 존재한다면 나감.
		for (auto Item : Inventory) 
		{
			if (Item.ItemNumber == CigarLightData->ItemNumber)
			{
				//isFind = true;
		
				// 아이템 상태를 업데이트 함.
				CurrentItem();
				return;
			}
		}

		//if (!isFind) // 인벤토리에 해당 아이템이 없다고 판별났을 경우
		//{
		
		// 인벤토리의 최대 수를 넘겼으면 못 얻음
			if (InventoryNum >= 8) 
			{
				// 인벤토리가 가득 찼다고 알림.
				ErrorInteractText = NSLOCTEXT("AHorrorGameCharacter", "Full_Inventory", "Your Inventory is FULL!. You CANNOT get more items");
				SetErrorText(ErrorInteractText, 3);
				bCanItemGet = false;
				return;
			}

			// 인벤토리에 해당 아이템이 없다면, 라이터 데이터를 할당함.
			Inventory[++InventoryNum].ItemName = CigarLightData->ItemName;
			Inventory[InventoryNum].ItemNumber = CigarLightData->ItemNumber;
			Inventory[InventoryNum].ItemIcon = CigarLightData->ItemIcon;
			Inventory[InventoryNum].Type = EItemType::ITEM_Useable;
			Inventory[InventoryNum].ItemCount = 1;

			// 만약 아이템이 없었다면 0으로 인덱스를 설정함
			if (CurrentItemNum < 0)
			{
				CurrentItemNum = 0;
			}
		//}

		// 아이템을 얻을 수 있는 상황이면 획득 음을 재생함.
		if (bCanItemGet)
		{
			if (IsValid(ItemGetSoundCue))
			{
				UGameplayStatics::PlaySound2D(this, ItemGetSoundCue);
			}
		}

		// 아이템 상태를 업데이트 함.
		CurrentItem();
	}
}

// 플래시 라이트를 인벤토리에 추가하는 함수.
void AHorrorGameCharacter::AddFlashLight()
{
	// 플래시 라이트 데이터를 가져옴.
	FHorrorGameItemData* FlashLightData = ItemTable->FindRow<FHorrorGameItemData>(*FString::FromInt(2), TEXT(""));
	//bool isFind = false;
	if (FlashLightData)
	{
		// 인벤토리에 있는지 체크해 내부에 존재한다면 배터리 잔량을 초기화해주고 이 함수에서 나감.
		for (auto Item : Inventory)
		{
			if (Item.ItemNumber == FlashLightData->ItemNumber)
			{
				//isFind = true;
				Item.ItemCount = 1;
				FlashLightBattery = 200;

				// 아이템 상태를 업데이트 함.
				CurrentItem();
				return;
			}
		}

		//if (!isFind) // 인벤토리에 해당 아이템이 없다고 판별될 경우
		//{
		// 인벤토리의 최대 수를 넘겼으면 못 얻음
			if (InventoryNum >= 8) 
			{
				// 인벤토리가 가득 찼다고 알림.
				ErrorInteractText = NSLOCTEXT("AHorrorGameCharacter", "Full_Inventory", "Your Inventory is FULL!. You CANNOT get more items");
				SetErrorText(ErrorInteractText, 3);
				bCanItemGet = false;
				return;
			}

			// 인벤토리에 해당 아이템이 없다면, 배터리 잔량을 최대로 설정하고 플래시 라이트 데이터를 할당함.
			FlashLightBattery = 200;
			Inventory[++InventoryNum].ItemName = FlashLightData->ItemName;
			Inventory[InventoryNum].ItemNumber = FlashLightData->ItemNumber;
			Inventory[InventoryNum].ItemIcon = FlashLightData->ItemIcon;
			Inventory[InventoryNum].Type = EItemType::ITEM_Useable;
			Inventory[InventoryNum].ItemCount = 1;

			// 만약 아이템이 없었다면 0으로 인덱스를 설정함
			if (CurrentItemNum < 0)
			{
				CurrentItemNum = 0;
			}
		//}

		// 아이템을 얻을 수 있는 상황이면 획득 음을 재생함.
		if (bCanItemGet)
		{
			if (IsValid(ItemGetSoundCue))
			{
				UGameplayStatics::PlaySound2D(this, ItemGetSoundCue);
			}
		}

		// 아이템 상태를 업데이트 함.
		CurrentItem();

		// 배터리 위젯을 출력함.
		if (GameUIWidget)
		{
			GameUIWidget->SetBatteryHUD(FlashLightBattery);
		}
	}
	bFLIntenseDown = false;
	FlashLight->SetIntensity(15000.f);
}

// 열쇠를 인벤토리에 추가하는 함수.
void AHorrorGameCharacter::AddKey()
{
	// 열쇠 데이터를 가져옴.
	FHorrorGameItemData* KeyData = ItemTable->FindRow<FHorrorGameItemData>(*FString::FromInt(3), TEXT(""));
	// bool isFind = false;
	if (KeyData)
	{
		// 인벤토리에 있는지 체크해 내부에 존재한다면 개수를 1개 더함.
		for (auto Item = Inventory.CreateIterator(); Item; ++Item)
		{
			if (Item->ItemNumber == KeyData->ItemNumber)
			{
				//isFind = true;
				Item->ItemCount++;

				// 아이템 상태를 업데이트 함.
				CurrentItem();
				return;
			}
		}

		//if (!isFind) // 없다고 판별날 경우
		//{
		// // 인벤토리의 최대를 넘겼다면 못 얻음
			if (InventoryNum >= 8) 
			{
				// 인벤토리가 가득 찼다고 알림.
				ErrorInteractText = NSLOCTEXT("AHorrorGameCharacter", "Full_Inventory", "Your Inventory is FULL!. You CANNOT get more items");
				SetErrorText(ErrorInteractText, 3);

				bCanItemGet = false;
				return;
			}

			// 인벤토리에 해당 아이템이 없다면 열쇠 데이터를 할당함.
			Inventory[++InventoryNum].ItemName = KeyData->ItemName;
			Inventory[InventoryNum].ItemNumber = KeyData->ItemNumber;
			Inventory[InventoryNum].ItemIcon = KeyData->ItemIcon;
			Inventory[InventoryNum].Type = EItemType::ITEM_Useable;
			Inventory[InventoryNum].ItemCount += 1;

			// 만약 아이템이 없었다면 0으로 인덱스를 설정함
			if (CurrentItemNum < 0)
			{
				CurrentItemNum = 0;
			}
		//}

		// 아이템을 얻을 수 있는 상황이면 획득 음을 재생함.
		if (bCanItemGet)
		{
			if (IsValid(ItemGetSoundCue))
			{
				UGameplayStatics::PlaySound2D(this, ItemGetSoundCue);
			}
		}

		// 아이템 상태를 업데이트 함.
		CurrentItem();
	}
}

// 타이머를 인벤토리에 추가하는 함수.
void AHorrorGameCharacter::AddTimer()
{
	FHorrorGameItemData* TimerData = ItemTable->FindRow<FHorrorGameItemData>(*FString::FromInt(4), TEXT(""));
	//bool isFind = false;
	if (TimerData)
	{
		// 인벤토리에 있는지 체크해 내부에 존재한다면 개수를 1개 더함.
		for (auto Item = Inventory.CreateIterator(); Item; ++Item)
		{
			if (Item->ItemNumber == TimerData->ItemNumber)
			{
				//isFind = true;
				Item->ItemCount++;

				// 아이템 상태를 업데이트 함.
				CurrentItem();
				return;
			}
		}

		//if (!isFind) // 없다고 판별나면
		//{
		// // 가질 수 있는 양을 넘겼다면 못 얻음
			if (InventoryNum >= 8) 
			{
				// 인벤토리가 가득 찼다고 알림.
				ErrorInteractText = NSLOCTEXT("AHorrorGameCharacter", "Full_Inventory", "Your Inventory is FULL!. You CANNOT get more items");
				SetErrorText(ErrorInteractText, 3);

				bCanItemGet = false;
				return;
			}

			// 인벤토리에 해당 아이템이 없다면, 타이머 데이터를 할당함.
			Inventory[++InventoryNum].ItemName = TimerData->ItemName;
			Inventory[InventoryNum].ItemNumber = TimerData->ItemNumber;
			Inventory[InventoryNum].ItemIcon = TimerData->ItemIcon;
			Inventory[InventoryNum].Type = EItemType::ITEM_Useable;
			Inventory[InventoryNum].ItemCount += 1;

			// 만약 아이템이 없었다면 0으로 인덱스를 설정함
			if (CurrentItemNum < 0)
			{
				CurrentItemNum = 0;
			}
		//}

		// 아이템을 얻을 수 있는 상황이면 획득 음을 재생함.
		if (bCanItemGet)
		{
			if (IsValid(ItemGetSoundCue))
			{
				UGameplayStatics::PlaySound2D(this, ItemGetSoundCue);
			}
		}

		// 아이템 상태를 업데이트 함.
		CurrentItem();
	}
}

// 청동 검을 인벤토리에 추가하는 함수.
void AHorrorGameCharacter::AddSword()
{
	FHorrorGameItemData* SwordData = ItemTable->FindRow<FHorrorGameItemData>(*FString::FromInt(5), TEXT(""));
	//bool isFind = false;


	if (SwordData)
	{
		// 인벤토리에 있는지 체크해 내부에 존재한다면 개수를 1개 더함.
		for (auto Item = Inventory.CreateIterator(); Item; ++Item)
		{
			if (Item->ItemNumber == SwordData->ItemNumber) // Sword
			{
			//	isFind = true;
				SwordCount++;
				Item->ItemCount = SwordCount;

				// 아이템 상태를 업데이트 함.
				CurrentItem();
				return;
			}
		}

		//if (!isFind) // 처음 Sword를 얻는 경우
		//{
		// // 가질 수 있는 최대 수를 넘기면 못 얻음
			if (InventoryNum >= 8) 
			{
				// 인벤토리가 가득 찼다고 알림.
				ErrorInteractText = NSLOCTEXT("AHorrorGameCharacter", "Full_Inventory", "Your Inventory is FULL!. You CANNOT get more items");
				SetErrorText(ErrorInteractText, 3);
				bCanItemGet = false;
				return;
			}

			// 인벤토리에 해당 아이템이 없다면, 청동 검 데이터를 할당함.
			Inventory[++InventoryNum].ItemName = SwordData->ItemName;
			Inventory[InventoryNum].ItemNumber = SwordData->ItemNumber;
			Inventory[InventoryNum].ItemIcon = SwordData->ItemIcon;
			Inventory[InventoryNum].Type = EItemType::ITEM_Useable;
			SwordCount = 1;
			Inventory[InventoryNum].ItemCount = SwordCount;

			// 만약 아이템이 없었다면 0으로 인덱스를 설정함
			if (CurrentItemNum < 0)
			{
				CurrentItemNum = 0;
			}

		//}

		// 아이템 상태를 업데이트 함.
		CurrentItem();
		
		// 오브젝트 UI에 검 개수를 업데이트 함.
		if (GameUIWidget)
		{
			GameUIWidget->SetObjectCount(1, SwordCount);
		}
	}
}

// 청동 방울을 인벤토리에 추가하는 함수.
void AHorrorGameCharacter::AddBell()
{
	FHorrorGameItemData* BellData = ItemTable->FindRow<FHorrorGameItemData>(*FString::FromInt(6), TEXT(""));
	//bool isFind = false;

	if (BellData)
	{
		// 인벤토리에 있는지 체크해 내부에 존재한다면 개수를 1개 더함.
		for (auto Item = Inventory.CreateIterator(); Item; ++Item)
		{
			if (Item->ItemNumber == BellData->ItemNumber)
			{
				//isFind = true;
				BellCount++;
				Item->ItemCount = BellCount;

				// 아이템 상태를 업데이트 함.
				CurrentItem();
				return;
			}
		}

		//if (!isFind) // 처음 Bell을 얻는 경우
		//{
		//  // 가질 수 있는 최대 수를 넘기면 못 얻음
			if (InventoryNum >= 8)
			{
				// 인벤토리가 가득 찼다고 알림.
				ErrorInteractText = NSLOCTEXT("AHorrorGameCharacter", "Full_Inventory", "Your Inventory is FULL!. You CANNOT get more items");
				SetErrorText(ErrorInteractText, 3);
				
				bCanItemGet = false;
				return;
			}

			// 인벤토리에 해당 아이템이 없다면, 청동 방울 데이터를 할당함.
			Inventory[++InventoryNum].ItemName = BellData->ItemName;
			Inventory[InventoryNum].ItemNumber = BellData->ItemNumber;
			Inventory[InventoryNum].ItemIcon = BellData->ItemIcon;
			Inventory[InventoryNum].Type = EItemType::ITEM_Useable;
			BellCount = 1;
			Inventory[InventoryNum].ItemCount = BellCount;

			// 만약 아이템이 없었다면 0으로 인덱스를 설정함
			if (CurrentItemNum < 0)
			{
				CurrentItemNum = 0;
			}

		//}

		// 아이템 상태를 업데이트 함.
		CurrentItem();
		
		// 오브젝트 UI에 방울 개수를 업데이트 함.
		if (GameUIWidget)
		{
			GameUIWidget->SetObjectCount(2, BellCount);
		}
	}
}

// 청동 거울을 인벤토리에 추가하는 함수.
void AHorrorGameCharacter::AddMirror()
{
	FHorrorGameItemData* MirrorData = ItemTable->FindRow<FHorrorGameItemData>(*FString::FromInt(7), TEXT(""));
//	bool isFind = false;

	if (MirrorData)
	{
		// 인벤토리에 있는지 체크해 내부에 존재한다면 개수를 1개 더함.
		for (auto Item = Inventory.CreateIterator(); Item; ++Item)
		{
			if (Item->ItemNumber == MirrorData->ItemNumber)
			{
				//isFind = true;
				MirrorCount++;
				Item->ItemCount = MirrorCount;

				// 아이템 상태를 업데이트 함.
				CurrentItem();
				return;
			}
		}

		//if (!isFind)
		//{
		//  // 가질 수 있는 최대 수를 넘기면 못 얻음
			if (InventoryNum >= 8)
			{
				// 인벤토리가 가득 찼다고 알림.
				ErrorInteractText = NSLOCTEXT("AHorrorGameCharacter", "Full_Inventory", "Your Inventory is FULL!. You CANNOT get more items");
				SetErrorText(ErrorInteractText, 3);
				bCanItemGet = false;
				return;
			}

			// 인벤토리에 해당 아이템이 없다면, 청동 거울 데이터를 할당함.
			Inventory[++InventoryNum].ItemName = MirrorData->ItemName;
			Inventory[InventoryNum].ItemNumber = MirrorData->ItemNumber;
			Inventory[InventoryNum].ItemIcon = MirrorData->ItemIcon;
			Inventory[InventoryNum].Type = EItemType::ITEM_Useable;
			MirrorCount = 1;
			Inventory[InventoryNum].ItemCount = MirrorCount;

			// 만약 아이템이 없었다면 0으로 인덱스를 설정함
			if (CurrentItemNum < 0)
			{
				CurrentItemNum = 0;
			}

		//}

		// 아이템 상태를 업데이트 함.
		CurrentItem();
		
		// 오브젝트 UI에 거울 개수를 업데이트 함.
		if (GameUIWidget)
		{
			GameUIWidget->SetObjectCount(3, MirrorCount);
		}
	}
}

// 소화기를 인벤토리에 추가하는 함수.
void AHorrorGameCharacter::AddExtinguisher()
{
	FHorrorGameItemData* ExtinguisherData = ItemTable->FindRow<FHorrorGameItemData>(*FString::FromInt(8), TEXT(""));
	//bool isFind = false;
	if (ExtinguisherData)
	{
		// 인벤토리에 있는지 체크해 내부에 존재한다면 소화기 잔량을 최대로 설정함.
		for (auto Item : Inventory)
		{
			if (Item.ItemNumber == ExtinguisherData->ItemNumber)
			{
				//isFind = true;
				Item.ItemCount = 1;
				ExtinguisherLeft = 100;
				//GameUIWidget->Init();

				// 아이템 상태를 업데이트 함.
				CurrentItem();
				return;
			}
		}

		//if (!isFind) // 없다면 판별날 경우
		//{
		// // 가질 수 있는 최대 양을 넘기면 못 얻음
			if (InventoryNum >= 8) 
			{
				// 인벤토리가 가득 찼다고 알림.
				ErrorInteractText = NSLOCTEXT("AHorrorGameCharacter", "Full_Inventory", "Your Inventory is FULL!. You CANNOT get more items");
				SetErrorText(ErrorInteractText, 3);
				bCanItemGet = false;
				return;
			}

			// 인벤토리에 해당 아이템이 없다면, 소화기 잔량을 최대로 설정하고 소화기 데이터를 할당함.
			ExtinguisherLeft = 100;
			Inventory[++InventoryNum].ItemName = ExtinguisherData->ItemName;
			Inventory[InventoryNum].ItemNumber = ExtinguisherData->ItemNumber;
			Inventory[InventoryNum].ItemIcon = ExtinguisherData->ItemIcon;
			Inventory[InventoryNum].Type = EItemType::ITEM_Useable;
			Inventory[InventoryNum].ItemCount = 1;

			// 만약 아이템이 없었다면 0으로 인덱스를 설정함
			if (CurrentItemNum < 0)
			{
				CurrentItemNum = 0;
			}

		//}

		// 아이템을 얻을 수 있는 상황이면 획득 음을 재생함.
		if (bCanItemGet)
		{
			if (IsValid(ItemGetSoundCue))
			{
				UGameplayStatics::PlaySound2D(this, ItemGetSoundCue);
			}
		}

		// 아이템 상태를 업데이트 함.
		CurrentItem();

		// 소화기 위젯을 출력함.
		if (GameUIWidget)
		{
			GameUIWidget->SetExtHUD(ExtinguisherLeft);
		}
	}
}

// 절단기를 인벤토리에 추가하는 함수.
void AHorrorGameCharacter::AddCutter()
{
	FHorrorGameItemData* CutterData = ItemTable->FindRow<FHorrorGameItemData>(*FString::FromInt(9), TEXT(""));
	//bool isFind = false;
	if (CutterData)
	{
		// 인벤토리에 있는지 체크해 내부에 존재한다면 내구도를 최대치로 설정함나감.
		for (auto Item : Inventory)
		{
			if (Item.ItemNumber == CutterData->ItemNumber)
			{
				//isFind = true;
				Item.ItemCount = 1;

				// 아이템 상태를 업데이트 함.
				CurrentItem();
				return;
			}
		}

		//if (!isFind) // 없다면
		//{
		// // 최대 개수를 넘기면 못 얻음
			if (InventoryNum >= 8) 
			{
				// 인벤토리가 가득 찼다고 알림.
				ErrorInteractText = NSLOCTEXT("AHorrorGameCharacter", "Full_Inventory", "Your Inventory is FULL!. You CANNOT get more items");
				SetErrorText(ErrorInteractText, 3);
				bCanItemGet = false;
				return;
			}

			// 인벤토리에 해당 아이템이 없다면, 절단기 데이터를 할당함.
			Inventory[++InventoryNum].ItemName = CutterData->ItemName;
			Inventory[InventoryNum].ItemNumber = CutterData->ItemNumber;
			Inventory[InventoryNum].ItemIcon = CutterData->ItemIcon;
			Inventory[InventoryNum].Type = EItemType::ITEM_Useable;
			Inventory[InventoryNum].ItemCount = 1;

			// 만약 아이템이 없었다면 0으로 인덱스를 설정함
			if (CurrentItemNum < 0)
			{
				CurrentItemNum = 0;
			}
		//}

		// 아이템을 얻을 수 있는 상황이면 획득 음을 재생함.
		if (bCanItemGet)
		{
			if (IsValid(ItemGetSoundCue))
			{
				UGameplayStatics::PlaySound2D(this, ItemGetSoundCue);
			}
		}

		// 아이템 상태를 업데이트 함.
		CurrentItem(); 

		// 절단기 위젯을 출력함.
		if (GameUIWidget)
		{
			GameUIWidget->SetCutterHUD(CutterDurability);
		}
	}
}

// 영혼 랜턴을 인벤토리에 추가하는 함수.
void AHorrorGameCharacter::AddLantern()
{
	FHorrorGameItemData* LanternData = ItemTable->FindRow<FHorrorGameItemData>(*FString::FromInt(10), TEXT(""));
	//bool isFind = false;

	if (LanternData)
	{
		// 인벤토리에 있는지 체크해 내부에 존재한다면 이미 있다고 알리고 나감.
		for (auto Item : Inventory)
		{
			if (Item.ItemNumber == LanternData->ItemNumber)
			{
				//isFind = true;
				ErrorInteractText = NSLOCTEXT("AHorrorGameCharacter", "Already_Get_Lantern", "You can get ONLY 1 Lantern");
				SetErrorText(ErrorInteractText, 3);
				bCanItemGet = false;

				// 아이템 상태를 업데이트 함.
				CurrentItem();
				return;
			}
		}

		//if (!isFind) // 없다면
		//{
			// 최대 아이템 개수를 초과하면 못 얻음
			if (InventoryNum >= 8)
			{
				// 인벤토리가 가득 찼다고 알림.
				ErrorInteractText = NSLOCTEXT("AHorrorGameCharacter", "Full_Inventory", "Your Inventory is FULL!. You CANNOT get more items");
				SetErrorText(ErrorInteractText, 3);
				bCanItemGet = false;
				return;
			}

			// 아이템 획득 음을 재생함.
			if (IsValid(ItemGetSoundCue))
			{
				UGameplayStatics::PlaySound2D(this, ItemGetSoundCue);
			}
			
			// 인벤토리에 해당 아이템이 없다면, 영혼 랜턴 데이터를 할당함.
			Inventory[++InventoryNum].ItemName = LanternData->ItemName;
			Inventory[InventoryNum].ItemNumber = LanternData->ItemNumber;
			Inventory[InventoryNum].ItemIcon = LanternData->ItemIcon;
			Inventory[InventoryNum].Type = EItemType::ITEM_Useable;
			Inventory[InventoryNum].ItemCount = 1;

			// 만약 아이템이 없었다면 0으로 인덱스를 설정함
			if (CurrentItemNum < 0)
			{
				CurrentItemNum = 0;
			}

			// 아이템 상태를 업데이트 함.
			CurrentItem();
		//}
	}
}

// 야광봉을 인벤토리에 추가하는 함수.
void AHorrorGameCharacter::AddGlowStick()
{
	FHorrorGameItemData* GlowStickData = ItemTable->FindRow<FHorrorGameItemData>(*FString::FromInt(11), TEXT(""));
	//bool isFind = false;
	if (GlowStickData)
	{
		// 인벤토리에 있는지 체크해 내부에 존재한다면 개수를 1개 더함.
		for (auto Item = Inventory.CreateIterator(); Item; ++Item)
		{
			if (Item->ItemNumber == GlowStickData->ItemNumber)
			{
				//isFind = true;
				Item->ItemCount++;

				// 아이템 상태를 업데이트 함.
				CurrentItem();
				return;
			}
		}

		//if (!isFind) // 없다고 판별나면
		//{
		// // 가질 수 있는 양을 넘겼다면 못 얻음
			if (InventoryNum >= 8) 
			{
				// 인벤토리가 가득 찼다고 알림.
				ErrorInteractText = NSLOCTEXT("AHorrorGameCharacter", "Full_Inventory", "Your Inventory is FULL!. You CANNOT get more items");
				SetErrorText(ErrorInteractText, 3);
				bCanItemGet = false;
				return;
			}

			// 인벤토리에 해당 아이템이 없다면, 야광봉 데이터를 할당함.
			Inventory[++InventoryNum].ItemName = GlowStickData->ItemName;
			Inventory[InventoryNum].ItemNumber = GlowStickData->ItemNumber;
			Inventory[InventoryNum].ItemIcon = GlowStickData->ItemIcon;
			Inventory[InventoryNum].Type = EItemType::ITEM_Useable;
			Inventory[InventoryNum].ItemCount += 1;

			// 만약 아이템이 없었다면 0으로 인덱스를 설정함
			if (CurrentItemNum < 0)
			{
				CurrentItemNum = 0;
			}
		//}

		// 아이템을 얻을 수 있는 상황이면 획득 음을 재생함.
		if (bCanItemGet)
		{
			if (IsValid(ItemGetSoundCue))
			{
				UGameplayStatics::PlaySound2D(this, ItemGetSoundCue);
			}
		}

		// 아이템 상태를 업데이트 함.
		CurrentItem();
	}
}
// Use Item Functions
// 라이터를 사용하는 함수.
void AHorrorGameCharacter::UseCigarLight()
{
	// 라이터가 켜져있는 경우 빛을 끄고, 라이터도 끔으로 설정함.
	if (bIsCigarLightOn) 
	{
		CigarLight->SetVisibility(false);
		bIsCigarLightOn = false;
		
		// 라이터 끄는 소리를 재생함.
		CigarLightOffSound->Play();
	}
	// 라이터가 꺼져 있는 경우 라이터를 켜는 소리를 재생함.
	else
	{
		CigarLightOnSound->Play();
	}

	// 아이템 상태를 업데이트 함.
	CurrentItem();
}

// CigarLightOnSound의 재생이 완료되면 호출될 콜백 함수.
void AHorrorGameCharacter::CigarLightOn()
{
	// 라이터를 켬으로 설정하고, 빛도 킴.
	CigarLight->SetVisibility(true);
	bIsCigarLightOn = true;
}

// 플래시 라이트를 사용하는 함수.
void AHorrorGameCharacter::UseFlashLight()
{
	// 플래시 라이트가 켜져있는 경우 빛을 끄고, 플래시 라이트도 끔으로 설정함.
	if (bIsFlashLightOn)
	{
		FlashLight->SetVisibility(false);
		bIsFlashLightOn = false;
	}
	// 플래시 라이트가 꺼져 있는 경우 플래시 라이트를 켬으로 설정하고, 빛도 킴.
	else
	{
		FlashLight->SetVisibility(true);
		bIsFlashLightOn = true;
	}
	// 라이터를 켜는 소리를 재생함.
	Turnon->Play();
	
	// Deprecated
	//FlashLightBatteryChange();

	// 아이템 상태를 업데이트 함.
	CurrentItem();
}

// 열쇠를 사용하는 함수.
void AHorrorGameCharacter::UseKey()
{
	//AActor* HitActor = nullptr;
	//bool isHit = GetLineTraceSingle(HitActor);

	//if (isHit)
	//{
	// Ray Cast에 충돌한 액터가 있다면
		if (HitActor)
		{
			// 그 액터가 문 류일 경우
			if (HitActor->GetClass()->ImplementsInterface(UDoorInterface_cpp::StaticClass()))
			{
				auto InterfaceVariable = Cast<IDoorInterface_cpp>(HitActor);
				
				// 열쇠 사용 효과음을 재생하고, 해당 액터의 아이템 사용 메서드를 호출함.
				if (KeySoundCue)
				{
					UGameplayStatics::PlaySoundAtLocation(this, KeySoundCue, GetActorLocation());
				}
				InterfaceVariable->UseInteract(this);

				// 열쇠 사용으로 문을 열었을 경우, 열쇠의 개수를 줄이고 초기화함.
				if (bIsFinishUnlock)
				{
					Inventory[CurrentItemNum].ItemCount--;
					bIsFinishUnlock = false;
				}
				
				// 위 로직으로 열쇠 개수가 0개일 경우
				if (Inventory[CurrentItemNum].ItemCount == 0)
				{
					// 인벤토리에서 제거하고, 인벤토리의 최대 개수와 현재 인벤토리 커서를 1 줄임.
					Inventory.RemoveAt(CurrentItemNum);
					InventoryNum--;
					CurrentItemNum--;

					// 만약 인벤토리에 아이템이 남아있음에도 커서가 -1 이하일 경우, 커서를 0으로 설정함.
					if (CurrentItemNum < 0 && InventoryNum >= 0)
					{
						CurrentItemNum = 0;
					}

					// 인벤토리가 비어져있는 경우, 커서를 -1로 설정함.
					if (InventoryNum < 0)
					{
						CurrentItemNum = -1;
					}
					
					// 임시 데이터를 생성해 인벤토리 제일 마지막에 추가함.
					FHorrorGameItemData TempItem;
					TempItem.Clear();
					Inventory.Add(TempItem);
				}
			}
			// 그 액터가 사물함 문일 경우
			else if (ALockerDoorActor_cpp* LockerDoor = Cast<ALockerDoorActor_cpp>(HitActor))
			{
				// 문이 잠겨 있는 경우
				if (LockerDoor->bIsLockerLocked)
				{
					// 열쇠 사용 효과음을 재생하고, 해당 액터의 아이템 사용 메서드를 호출함.
					if (KeySoundCue)
					{
						UGameplayStatics::PlaySoundAtLocation(this, KeySoundCue, GetActorLocation());
					}
					LockerDoor->UseInteract(this);
					
					// 열쇠 사용으로 문을 열었을 경우, 열쇠의 개수를 줄이고 초기화함.
					if (bIsFinishUnlock)
					{
						Inventory[CurrentItemNum].ItemCount--;
						bIsFinishUnlock = false;
					}

					// 위 로직으로 열쇠 개수가 0개일 경우
					if (Inventory[CurrentItemNum].ItemCount == 0)
					{
						// 인벤토리에서 제거하고, 인벤토리의 최대 개수와 현재 인벤토리 커서를 1 줄임.
						Inventory.RemoveAt(CurrentItemNum);
						InventoryNum--;
						CurrentItemNum--;

						// 만약 인벤토리에 아이템이 남아있음에도 커서가 -1 이하일 경우, 커서를 0으로 설정함.
						if (CurrentItemNum < 0 && InventoryNum >= 0)
						{
							CurrentItemNum = 0;
						}

						// 인벤토리가 비어져있는 경우, 커서를 -1로 설정함.
						if (InventoryNum < 0)
						{
							CurrentItemNum = -1;
						}

						// 임시 데이터를 생성해 인벤토리 제일 마지막에 추가함.
						FHorrorGameItemData TempItem;
						TempItem.Clear();
						Inventory.Add(TempItem);
					}
				}
			}

			// 아이템 상태를 업데이트 함.
			CurrentItem();
		}
	//}
}

// 타이머를 사용하는 함수.
void AHorrorGameCharacter::UseTimer()
{
	// 투사체 클래스가 있는 경우
	if (ProjectileClass)
	{
		// 투사체를 던지는 효과를 위해 투사체의 시작 지점과 회전 값을 설정함.
		FVector CameraLocatoin;
		FRotator CameraRotation;
		GetActorEyesViewPoint(CameraLocatoin, CameraRotation);

		FVector MuzzleLocation = CameraLocatoin + FTransform(CameraRotation).TransformVector(MuzzleOffset);
		FRotator MuzzleRotation = CameraRotation;

		MuzzleRotation.Pitch += 10.f;
		UWorld* World = GetWorld();
		
		// 투사체를 던지는 효과를 줌.
		if (World)
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
			SpawnParams.Instigator = GetInstigator();
			ATimerProjectile_cpp* Projectile = World->SpawnActor<ATimerProjectile_cpp>(ProjectileClass, MuzzleLocation, MuzzleRotation, SpawnParams);
			if (Projectile)
			{
				FVector LaunchDirection = MuzzleRotation.Vector();
				Projectile->FireInDirection(LaunchDirection);
			}
		}
	}
	
	// 타이머의 개수를 하나 제거함.
	Inventory[CurrentItemNum].ItemCount--;

	// 위 로직으로 타이머 개수가 0개일 경우
	if (Inventory[CurrentItemNum].ItemCount == 0)
	{
		// 인벤토리에서 제거하고, 인벤토리의 최대 개수와 현재 인벤토리 커서를 1 줄임.
		Inventory.RemoveAt(CurrentItemNum);
		InventoryNum--;
		CurrentItemNum--;
		
		// 만약 인벤토리에 아이템이 남아있음에도 커서가 -1 이하일 경우, 커서를 0으로 설정함.
		if (CurrentItemNum < 0 && InventoryNum >= 0)
		{
			CurrentItemNum = 0;
		}

		// 인벤토리가 비어져있는 경우, 커서를 -1로 설정함.
		if (InventoryNum < 0)
		{
			CurrentItemNum = -1;
		}
	}

	// 임시 데이터를 생성해 인벤토리 제일 마지막에 추가함.
	FHorrorGameItemData TempItem;
	TempItem.Clear();
	Inventory.Add(TempItem);
	
	// 아이템의 상태를 업데이트 함.
	CurrentItem();
}

// 청동 검을 사용하는 함수.
void AHorrorGameCharacter::UseSword()
{
	// 현재 숨은 상태가 아닐 경우
	if (!bIsHiding)
	{
		// 애니메이션 클래스를 가져와 공격 애니메이션 재생을 호출함.
		auto AnimInstance = Cast<UPlayerAnim>(GetMesh()->GetAnimInstance());
		if (nullptr == AnimInstance) return;

		AnimInstance->PlayAttackMontage();
		
		// 그 후 쥐고 있는 청동 검의 아이템 사용 메서드를 호출함.
		AActor* ChildActor = Sword->GetChildActor();

		if (APlayerSword_cpp* BronzeSword = Cast<APlayerSword_cpp>(ChildActor))
		{
			BronzeSword->UseInteract(this);
		}
	}
}

// 청동 방울을 사용하는 함수.
void AHorrorGameCharacter::UseBell()
{
	// 현재 재생 중인 방울 소리를 끄고, 다시 재생함.
	BellSound->Stop();
	BellSound->Play();
	bIsBellSoundOn = true;

	// 착란 게이지 전량을 빼줌.
	//AddPatience(-100);

	// 바로 착란 게이지가 증가하지 않도록 쿨타임을 설정함.
	bIsCooldown = true;

	// 방울의 개수를 1개 줄이고 인벤토리에 적용함.
	BellCount--;
	Inventory[CurrentItemNum].ItemCount = BellCount;

	// 위 로직으로 인해 방울 개수가 0개일 경우
	if (Inventory[CurrentItemNum].ItemCount == 0)
	{
		// 인벤토리에서 제거하고, 인벤토리의 최대 개수와 현재 인벤토리 커서를 1 줄임.
		Inventory.RemoveAt(CurrentItemNum);
		InventoryNum--;
		CurrentItemNum--;
		
		// 만약 인벤토리에 아이템이 남아있음에도 커서가 -1 이하일 경우, 커서를 0으로 설정함.
		if (CurrentItemNum < 0 && InventoryNum >= 0)
		{
			CurrentItemNum = 0;
		}

		// 인벤토리가 비어져있는 경우, 커서를 -1로 설정함.
		if (InventoryNum < 0)
		{
			CurrentItemNum = -1;
		}

		// 임시 데이터를 생성해 인벤토리 제일 마지막에 추가함.
		FHorrorGameItemData TempItem;
		TempItem.Clear();
		Inventory.Add(TempItem);
	}
	
	// 오브젝트 UI의 방울 개수를 업데이트 함.
	GameUIWidget->SetObjectCount(2, BellCount);
	
	// 아이템의 상태를 업데이트 함.
	CurrentItem();
}

void AHorrorGameCharacter::BreatheSoundFinish()
{
	bisSoundOn = false;
}

// 방울 소리가 끝났을 때 작동할 함수.
void AHorrorGameCharacter::BellSoundFinish()
{
	// 감소량과 감소할 값을 설정해줌.
	bIsBellSoundOn = false;
	bIsPatienceReduce = true;
	PatienceToReduce = 0;
}

// 청동 거울을 사용하는 함수.
// 타임 스톱의 개념(사실은 거울 세계로 잠시 들어가 탈출)
void AHorrorGameCharacter::UseMirror()
{
	// 시간 정지가 되었다고 설정하고, 리퍼가 플레이어를 보고 있지 않다고 설정함.
	bIsTimeStop = true;
	bReaperWatchPlayer = false;
	ReaperWatchElapsedTime = 0.0f;

	// 거울의 아이템 사용 메서드를 호출함.
	AActor* ChildActor = Mirror->GetChildActor();
	if (AMirror_cpp* BronzeMirror = Cast<AMirror_cpp>(ChildActor))
	{
		BronzeMirror->UseInteract(this);
		Cast<AHorrorGameGameMode>(UGameplayStatics::GetGameMode(GetWorld()))->PlayMirrorUseBackGroundMusic();
	}
	
	// 거울의 개수를 하나 줄임.
	Inventory[CurrentItemNum].ItemCount = MirrorCount;

	// 위 로직으로 인해 거울 개수가 0개일 경우
	if (Inventory[CurrentItemNum].ItemCount == 0)
	{
		// 인벤토리에서 제거하고, 인벤토리의 최대 개수와 현재 인벤토리 커서를 1 줄임.
		Inventory.RemoveAt(CurrentItemNum);
		InventoryNum--;
		CurrentItemNum--;
		
		// 만약 인벤토리에 아이템이 남아있음에도 커서가 -1 이하일 경우, 커서를 0으로 설정함.
		if (CurrentItemNum < 0 && InventoryNum >= 0)
		{
			CurrentItemNum = 0;
		}

		// 인벤토리가 비어져있는 경우, 커서를 -1로 설정함.
		if (InventoryNum < 0)
		{
			CurrentItemNum = -1;
		}
	
		// 임시 데이터를 생성해 인벤토리 제일 마지막에 추가함.
		FHorrorGameItemData TempItem;
		TempItem.Clear();	
		Inventory.Add(TempItem);
	}

	// 카메라에 포스트 프로세싱 머티리얼을 추가함.
	FirstPersonCameraComponent->AddOrUpdateBlendable(PostProcessMaterial, 1.f);
	
	// 오브젝트 UI의 거울 개수를 업데이트 함.
	GameUIWidget->SetObjectCount(3, MirrorCount);

	// 아이템의 상태를 업데이트 함.
	CurrentItem();
}

// 소화기를 사용하는 함수.
void AHorrorGameCharacter::UseExtinguisher()
{
	if (FirstPersonCameraComponent == nullptr)
		return;

	// 숨어있는 경우에는 작동하지 않도록 함.
	if (bIsHiding)
	{
		return;
	}

	// 소화기를 사용가능하지 않는 상태일 경우 작동하지 않도록 함.
	if (!bCanExtinguisherUse)
	{
		return;
	}

	// 연기 나이아가라 이펙트를 활성화함.
	SmokeComponent->Activate(true);
	bCanExtinguisherUse = false;
	
	// 소화기 사용 음을 재생함.
	if (ExtinguisherCue)
	{
		UGameplayStatics::PlaySoundAtLocation(this, ExtinguisherCue, GetActorLocation());
	}

	FTimerHandle SmokeTimer;

	// 충돌체를 생성시킴.
	TArray<FOverlapResult>OverlapResults;
	bool bResult = GetWorld()->OverlapMultiByChannel(
		OverlapResults,
		GetActorLocation() + GetActorForwardVector() * 300.f,
		FRotationMatrix::MakeFromZ(GetActorForwardVector() * 600.f).ToQuat(),
		ECollisionChannel::ECC_EngineTraceChannel4,
		FCollisionShape::MakeCapsule(200.f, 600.f)
	);

	// 충돌이 감지되었을 경우
	if (bResult)
	{
		for (auto OverlapResult : OverlapResults)
		{
			FHitResult OutHit;
			FVector Start = GetActorLocation();
			FVector End = OverlapResult.GetActor()->GetActorLocation();

			// Ray Cast를 통해서 벽 뒤에서 충돌한 경우가 아닌 경우.
			if (!GetWorld()->LineTraceSingleByChannel(OutHit, Start, End, ECC_Visibility))
			{
				// 각 요괴 개체들에게 거리별로 행동 불능을 만들도록 설정함.
				if (auto Reaper = Cast<AReaper_cpp>(OverlapResult.GetActor()))
				{
					float Dist = FVector::Distance(GetActorLocation(), Reaper->GetActorLocation());
					Reaper->Stunning(Dist);
				}
				else if (auto Runner = Cast<ARunner_cpp>(OverlapResult.GetActor()))
				{
					float Dist = FVector::Distance(GetActorLocation(), Runner->GetActorLocation());
					Runner->Stunning(Dist);
				}
				else if (auto Brute = Cast<ABrute_cpp>(OverlapResult.GetActor()))
				{
					float Dist = FVector::Distance(GetActorLocation(), Brute->GetActorLocation());
					Brute->Stunning(Dist);
				}
			}
		}
	}
//#if ENABLE_DRAW_DEBUG
//	FVector TraceVec = GetActorForwardVector() * 600.f;
//	FVector Center = GetActorLocation() + TraceVec * 0.5f;
//	//반지름
//	float HalfHeight = 600.f;
//	//캡슐 회전방향
//	FQuat CapsuleRot = FRotationMatrix::MakeFromZ(TraceVec).ToQuat();
//	//타겟 발견시 녹색,미발견시 빨강
//	FColor DrawColor = bResult ? FColor::Green : FColor::Red;
//	//생성후 삭제되기까지의 시간
//	float DebugLifeTime = 1.0f;
//
//
//	//캡슐 디버그 메쉬그리기
//	DrawDebugCapsule(GetWorld(), Center, HalfHeight, 200.f, CapsuleRot, DrawColor, false, DebugLifeTime);
//#endif
	
	// 5초 후 작동하도록 함.
	GetWorld()->GetTimerManager().SetTimer(SmokeTimer, FTimerDelegate::CreateLambda([&]() {
		// 소화기를 사용 가능하게 설정하고 나이아가라 이펙트를 비활성화함.
		bCanExtinguisherUse = true;
		SmokeComponent->Deactivate();
		
		// 그 후 소화기 잔량을 20 감소시키고 소화기 위젯을 업데이트 함.
		ExtinguisherLeft -= 20;

		if (GameUIWidget)
		{
			GameUIWidget->SetExtHUD(ExtinguisherLeft);
		}

		// 잔량이 0 이하일 경우
		if (ExtinguisherLeft <= 0)
		{
			// 소화기 개수를 1 감소시킴.
			Inventory[CurrentItemNum].ItemCount--;
			
			// 위 로직으로 소화기 개수가 0일 경우
			if (Inventory[CurrentItemNum].ItemCount == 0)
			{
				// 인벤토리에서 제거하고, 인벤토리의 최대 개수와 현재 인벤토리 커서를 1 줄임.
				Inventory.RemoveAt(CurrentItemNum);
				InventoryNum--;
				CurrentItemNum--;

				// 만약 인벤토리에 아이템이 남아있음에도 커서가 -1 이하일 경우, 커서를 0으로 설정함.
				if (CurrentItemNum < 0 && InventoryNum >= 0)
				{
					CurrentItemNum = 0;
				}

				// 인벤토리가 비어져있는 경우, 커서를 -1로 설정함.
				if (InventoryNum < 0)
				{
					CurrentItemNum = -1;
				}
			}

			// 임시 데이터를 생성해 인벤토리 제일 마지막에 추가함.
			FHorrorGameItemData TempItem;
			TempItem.Clear();
			Inventory.Add(TempItem);

			// 소화기 위젯을 끔.
			if (GameUIWidget)
			{
				GameUIWidget->SetExtWidget(false);
			}
		}
		
		// 아이템의 상태를 업데이트 함.
		CurrentItem();

		GetWorld()->GetTimerManager().ClearTimer(SmokeTimer);
	}), 1.f, false);

	//// 잔량이 0 이하일 경우
	//if (ExtinguisherLeft <= 0)
	//{
	//	Inventory[CurrentItemNum].ItemCount--;
	//	if (Inventory[CurrentItemNum].ItemCount == 0)
	//	{
	//		Inventory.RemoveAt(CurrentItemNum);
	//		InventoryNum--;
	//		CurrentItemNum--;
	//		if (CurrentItemNum < 0 && InventoryNum >= 0)
	//		{
	//			CurrentItemNum = 0;
	//		}

	//		if (InventoryNum < 0)
	//		{
	//			CurrentItemNum = -1;
	//		}
	//	}
	//	FHorrorGameItemData TempItem;
	//	TempItem.Clear();
	//	Inventory.Add(TempItem); 
	//	if (GameUIWidget)
	//	{
	//		GameUIWidget->SetExtWidget(false);
	//	}
	//}
	//CurrentItem();
}

// 절단기를 사용하는 함수.
void AHorrorGameCharacter::UseCutter()
{
	//AActor* HitActor = nullptr;
	//bool isHit = GetLineTraceSingle(HitActor);

	//if (isHit)
	//{
	// Ray cast된 액터가 있을 경우
		if (HitActor)
		{
			// 그 액터가 사물함 문일 경우
			if (ALockerDoorActor_cpp* LockerDoor = Cast<ALockerDoorActor_cpp>(HitActor))
			{
				// 사물함 문이 잠겨있다면
				if (LockerDoor->bIsLockerLocked)
				{
					// 절단기 사용 음을 재생하고, 해당 액터의 아이템 사용 효과 메서드를 호출함.
					if (CutterSoundCue)
					{
						UGameplayStatics::PlaySoundAtLocation(this, CutterSoundCue, GetActorLocation());
					}
					LockerDoor->UseInteract(this);

					// 위 로직으로 잠긴 문을 열었다면, 절단기 내구도를 1 감소시키고, 초기화함.
					if (bIsFinishUnlock)
					{
						CutterDurability -= 1;
						bIsFinishUnlock = false;
					}
				}
			}
			// 그 액터가 교실(미닫이) 문일 경우
			else if (AClassroomDoorActor_cpp* ClassroomDoor = Cast<AClassroomDoorActor_cpp>(HitActor))
			{
				// 해당 문이 잠겨있다면
				if (ClassroomDoor->bIsDoorLocked)
				{
					// 절단기 사용 음을 재생하고, 해당 액터의 아이템 사용 효과 메서드를 호출함.
					ClassroomDoor->UseInteract(this);
					
					if (CutterSoundCue)
					{
						UGameplayStatics::PlaySoundAtLocation(this, CutterSoundCue, GetActorLocation());
					}

					// 위 로직으로 잠긴 문을 열었다면, 절단기 내구도를 1 감소시키고, 초기화함.
					if (bIsFinishUnlock)
					{
						CutterDurability -= 1;
						bIsFinishUnlock = false;
					}
				}
			}
			// 그 액터가 (여닫이)철문일 경우
			else if (AMetalDoor_cpp* MetalDoor = Cast<AMetalDoor_cpp>(HitActor))
			{
				// 해당 문이 잠겨있다면
				if (MetalDoor->bIsDoorLocked)
				{
					// 절단기 사용 음을 재생하고, 해당 액터의 아이템 사용 효과 메서드를 호출함.
					if (CutterSoundCue)
					{
						UGameplayStatics::PlaySoundAtLocation(this, CutterSoundCue, GetActorLocation());
					}
					MetalDoor->UseInteract(this);

					// 위 로직으로 잠긴 문을 열었다면, 절단기 내구도를 1 감소시키고, 초기화함.
					if (bIsFinishUnlock)
					{
						CutterDurability -= 1;
						bIsFinishUnlock = false;
					}
				}
			}
		}
	//}

	// 절단기 내구도가 0 이하일 경우
	if (CutterDurability <= 0)
	{
		// 절단기 개수를 1 감소시킴.
		Inventory[CurrentItemNum].ItemCount--;
		
		// 위 로직으로 절단기 개수가 0일 경우
		if (Inventory[CurrentItemNum].ItemCount == 0)
		{
			// 절단기 위젯을 안 보이게 설정함.
			if (GameUIWidget)
			{
				GameUIWidget->SetCutterWidget(false);
			}
			
			// 인벤토리에서 제거하고, 인벤토리의 최대 개수와 현재 인벤토리 커서를 1 줄임.
			Inventory.RemoveAt(CurrentItemNum);
			InventoryNum--;
			CurrentItemNum--;
			
			// 만약 인벤토리에 아이템이 남아있음에도 커서가 -1 이하일 경우, 커서를 0으로 설정함.
			if (CurrentItemNum < 0 && InventoryNum >= 0)
			{
				CurrentItemNum = 0;
			}

			// 인벤토리가 비어져있는 경우, 커서를 -1로 설정함.
			if (InventoryNum < 0)
			{
				CurrentItemNum = -1;
			}
		}
		
		// 임시 데이터를 생성해 인벤토리 제일 마지막에 추가함.
		FHorrorGameItemData TempItem;
		TempItem.Clear();
		Inventory.Add(TempItem);
	}
	
	// 임시 데이터를 생성해 인벤토리 제일 마지막에 추가함.
	CurrentItem();
	
	// 절단기 위젯을 업데이트 함.
	if (GameUIWidget)
	{
		GameUIWidget->SetCutterHUD(CutterDurability);
	}
}

// 랜턴을 사용하는 함수.
void AHorrorGameCharacter::UseLantern()
{
	// 랜턴의 아이템 사용 효과 메서드를 호출함.
	if (Lantern)
	{
		if (ASoul_Lantern_cpp* SoulLantern = Cast<ASoul_Lantern_cpp>(Lantern->GetChildActor()))
		{
			SoulLantern->UseInteract(this);
		}
	}
}

// 야광봉을 사용하는 함수.
void AHorrorGameCharacter::UseGlowStick()
{
	// 숨어있는 경우가 아닐 때 작동.
	if (bIsHiding)
	{
		return;
	}

	if (GlowStickClass)
	{
		// 플레이어가 밟고 있는 바닥에 야광봉이 생성될 수 있도록 값을 초기화함.
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.Instigator = GetInstigator();
		float Z = 110.f; // 122 - 110 = 12
		
		if (bIsCrouch)
		{
			Z = 45.f; // 56 - 45 = 11
		}

		FVector SpawnLocation = GetActorLocation() - FVector(0.f, 0.f, Z);
			
		// 야광봉 액터를 설정한 위치에 생성하고 아이템 사용 효과 메서드를 호출함.
		AGlowStick_cpp* GlowStick = GetWorld()->SpawnActor<AGlowStick_cpp>(GlowStickClass, SpawnLocation, FRotator(0.f, 0.f, 0.f), SpawnParams);
		if (GlowStick)
		{
			GlowStick->UseInteract(this);
		}
	}

	// 야광봉 개수를 1 감소시킴.
	Inventory[CurrentItemNum].ItemCount--;
	
	// 위 로직으로 야광봉 개수가 0일 경우
	if (Inventory[CurrentItemNum].ItemCount == 0)
	{
		// 인벤토리에서 제거하고, 인벤토리의 최대 개수와 현재 인벤토리 커서를 1 줄임.
		Inventory.RemoveAt(CurrentItemNum);
		InventoryNum--;
		CurrentItemNum--;
		
		// 만약 인벤토리에 아이템이 남아있음에도 커서가 -1 이하일 경우, 커서를 0으로 설정함.
		if (CurrentItemNum < 0 && InventoryNum >= 0)
		{
			CurrentItemNum = 0;
		}

		// 인벤토리가 비어져있는 경우, 커서를 -1로 설정함.
		if (InventoryNum < 0)
		{
			CurrentItemNum = -1;
		}
	}
	
	// 임시 데이터를 생성해 인벤토리 제일 마지막에 추가함.
	FHorrorGameItemData TempItem;
	TempItem.Clear();
	Inventory.Add(TempItem);

	// 임시 데이터를 생성해 인벤토리 제일 마지막에 추가함.
	CurrentItem();
}

// Battery Manage Function(Deprecated)
void AHorrorGameCharacter::FlashLightBatteryChange()
{
	if (bIsFlashLightOn)
	{
		if (FlashLightBattery > 0)
		{
			FlashLightBattery -= 1;
		}
		else // If FlashLightBattery <= 0
		{
			FlashLight->SetVisibility(false);
			bIsFlashLightOn = false;
			Inventory[CurrentItemNum].ItemCount--;
			if (Inventory[CurrentItemNum].ItemCount == 0)
			{
				GameUIWidget->SetBatteryWidget(false);
				Inventory.RemoveAt(CurrentItemNum);
				InventoryNum--;
				CurrentItemNum--;
				if (CurrentItemNum < 0 && InventoryNum >= 0)
				{
					CurrentItemNum = 0;
				}

				if (InventoryNum < 0)
				{
					CurrentItemNum = 0;
				}
			}
			FHorrorGameItemData TempItem;
			TempItem.Clear();
			Inventory.Add(TempItem);
			CurrentItem();
		}

		if (FlashLightBattery < 20 && !(bFLIntenseDown))
		{
			FlashLight->SetIntensity(8000.f);
			bFLIntenseDown = true;
		}
	}
	
	GameUIWidget->SetBatteryHUD(FlashLightBattery);
	
	GetWorld()->GetTimerManager().ClearTimer(_loopLightTimerHandle);
	GetWorld()->GetTimerManager().SetTimer(_loopLightTimerHandle, this, &AHorrorGameCharacter::FlashLightBatteryChange, 1.0f, false);
}

// 아이템 상태를 업데이트하는 함수.
void AHorrorGameCharacter::CurrentItem()
{
	// 현재 선택한 아이템 확인함.
	int32 currentItemNumber = GetCurrentItemNumber();
	
	// 현재 선택한 아이템이 플래시 라이트인 경우, 플래시 라이트 위젯을 보이게 설정함.
	if (currentItemNumber == 2) // FlashLight
	{
		if (GameUIWidget)
		{
			GameUIWidget->SetBatteryWidget(true);
		}
	}
	// 현재 선택한 아이템이 플래시 라이트가 아닌 경우, 플래시 라이트 위젯을 안 보이게 설정함.
	else
	{
		if (GameUIWidget)
		{
			GameUIWidget->SetBatteryWidget(false);
		}
	}

	// 현재 선택한 아이템이 소화기인 경우, 소화기 위젯을 보이게 설정함.
	if (currentItemNumber == 8) // Extinguisher
	{
		if (GameUIWidget)
		{
			GameUIWidget->SetExtWidget(true);
		}
	}
	// 현재 선택한 아이템이 소화기가 아닌 경우, 소화기 위젯을 보이게 설정함.
	else
	{
		if (GameUIWidget)
		{
			GameUIWidget->SetExtWidget(false);
		}
	}

	// 현재 선택한 아이템이 절단기인 경우, 절단기 위젯을 보이게 설정함.
	if (currentItemNumber == 9) // Cutter
	{
		if (GameUIWidget)
		{
			GameUIWidget->SetCutterWidget(true);
		}
	}
	// 현재 선택한 아이템이 절단기가 아닌 경우, 절단기 위젯을 보이게 설정함.
	else
	{
		if (GameUIWidget)
		{
			GameUIWidget->SetCutterWidget(false);
		}
	}

	// 현재 선택한 아이템이 영혼 랜턴인 경우, 손에 쥐고 있는 영혼 랜턴을 보이게 설정함.
	if (currentItemNumber == 10) // SoulLantern
	{
		Lantern->SetHiddenInGame(false);
	}
	// 현재 선택한 아이템이 영혼 랜턴이 아닌 경우
	else
	{
		// 영혼 랜턴 불이 켜져 있는 경우, 영혼 랜턴이 꺼지도록 설정함.
		AActor* ChildActor = Lantern->GetChildActor();
		if (ASoul_Lantern_cpp* SoulLantern = Cast<ASoul_Lantern_cpp>(ChildActor))
		{
			if (SoulLantern->bIsLightOn)
				SoulLantern->UseInteract(this);
		}
		
		// 손에 쥐고 있는 영혼 랜턴을 안 보이게 설정함.
		Lantern->SetHiddenInGame(true);
	}

	// 현재 선택한 아이템이 청동 거울인 경우, 손에 쥐고 있는 청동 거울을 보이게 설정함.
	if (currentItemNumber == 7) // Mirror
	{
		Mirror->SetHiddenInGame(false);
	}
	// 현재 선택한 아이템이 청동 거울이 아닌 경우, 손에 쥐고 있는 청동 거울이 안 보이게 설정함.
	else
	{
		Mirror->SetHiddenInGame(true);
	}

	// 현재 선택한 아이템이 청동 검인 경우, 손에 쥐고 있는 청동 검을 보이게 설정함.
	if (currentItemNumber == 5) // Sword
	{
		Sword->SetHiddenInGame(false);
	}
	// 현재 선택한 아이템이 청동 검이 아닌 경우, 손에 쥐고 있는 청동 검을 안 보이게 설정함.
	else
	{
		Sword->SetHiddenInGame(true);
	}

	// 인벤토리 위젯을 초기화함.
	if (GameUIWidget)
	{
		GameUIWidget->Init();
	}
}

// Get Functions: GetStamina(), GetFlashLightBattery(), GetCurrentItemNumber()
//				GetCurrentItemName(), GetExtinguisherLeft(), GetPatience(), GetPlayerStatus()
int32 AHorrorGameCharacter::GetStamina()
{
	return Stamina;
}

int32 AHorrorGameCharacter::GetFlashLightBattery()
{
	return FlashLightBattery;
}

int32 AHorrorGameCharacter::GetCurrentItemNumber()
{
	if (CurrentItemNum < 0)
	{
		return 0;
	}
	int32 CurrentItemNumber = Inventory[CurrentItemNum].ItemNumber;
	return CurrentItemNumber;
}

FText AHorrorGameCharacter::GetCurrentItemName()
{
	if(CurrentItemNum >= 0 && CurrentItemNum <= InventoryNum)
		return Inventory[CurrentItemNum].ItemName;
	return NSLOCTEXT("AHorrorGameCharacter", "NULL", "");
}

int32 AHorrorGameCharacter::GetExtinguisherLeft()
{
	return ExtinguisherLeft;
}

int32 AHorrorGameCharacter::GetPatience()
{
	return Patience;
}

FText AHorrorGameCharacter::GetInteractText()
{
	return InteractText;
}

FText AHorrorGameCharacter::GetErrorInteractText()
{
	return ErrorInteractText;
}

bool AHorrorGameCharacter::GetIsHiding()
{
	return bIsHiding;
}

bool AHorrorGameCharacter::GetIsSprinting()
{
	return bIsSprint;
}

Player_Status AHorrorGameCharacter::GetPlayerStatus()
{
	return PlayerStatus;
}

int32 AHorrorGameCharacter::GetObjectNumbers()
{
	return SwordCount + BellCount + MirrorCount;
}

int32 AHorrorGameCharacter::GetSwordNumbers()
{
	return SwordCount;
}

int32 AHorrorGameCharacter::GetMirrorNumbers()
{
	return MirrorCount;
}

int32 AHorrorGameCharacter::GetBellNumbers()
{
	return BellCount;
}

bool AHorrorGameCharacter::GetIsLightOn()
{
	return bIsCigarLightOn || bIsFlashLightOn;
}

// Set Functions: SetPlayerStatus()
// 플레이어의 상태를 업데이트하는 함수.
void AHorrorGameCharacter::SetPlayerStatus(Player_Status Value)
{
	if (PlayerStatus != Value)
	{
		PlayerStatus = Value;

		switch (PlayerStatus)
		{
			// 로딩 중인 상태일 경우, 컨트롤러를 통해 Input이 안 되게 설정함.
			case Player_Status::Loading: // 로딩 중
			{
				DisableInput(HorrorGamePlayerController);
			
				return;
			}
			// 게임이 시작된 상태일 경우, 컨트롤러를 통해 Input이 되도록 설정함.
			case Player_Status::Survive: // 시작
			{
				EnableInput(HorrorGamePlayerController);

				return;
			}
			// 추격당하는 상태일 경우
			case Player_Status::Chased: // 추격 시
			{
				// 추격 노이즈를 카메라에 주고, 착란 게이지가 증가할 수 있다면, 15 증가시킴.
				SetCameraComponentNoise(2);
				if (!bIsCooldown)
				{
					AddPatience(15);
				}
				return;
			}
			// 적 개체에게 잡혔을 경우
			case Player_Status::Catched: // 잡혔을 경우
			{
				// 충돌이 안 되게 설정하고, 컨트롤러를 통해 Input이 안 되게 설정함.
				SetActorEnableCollision(false);
				DisableInput(HorrorGamePlayerController);
				return;
			}
			// 리퍼에 의해 행동 불능 상태일 경우
			case Player_Status::Stunned: // 리퍼에 의해 기절 시
			{
				// 이동을 즉시 멈추고, 컨트롤러를 통해 Input이 안 되게 설정함.
				DisableInput(HorrorGamePlayerController);
				GetCharacterMovement()->StopMovementImmediately();
				return;
			}
			// 이 케이스는 숨은 상태를 나타내는 케이스가 아닌, Hide Object속에 배치된 캐릭터에 대한 케이스임.
			case Player_Status::Hiding: // 헷갈리면 안 됨. 숨은 상태는 bIsHiding이 나타냄.
			{
				
				return;
			}
			// 사망한 상태일 경우
			case Player_Status::Died: // 사망
			{
				// 충돌이 안 되게 설정하고 게임에서 안 보이게 설정하고 컨트롤러를 통해 Input이 안 되게 설정함.
				SetActorEnableCollision(false);
				GetMesh()->SetHiddenInGame(true);
				DisableInput(HorrorGamePlayerController);

				// 게임 플레이 타이머를 정지시키고, 사망 UI를 뷰포트에 출력하도록 함.
				if (GameUIWidget)
				{
					GameUIWidget->SetTimerStop(true);
				}
				HorrorGamePlayerController->ShowDeadUI();

				if (HorrorGamePlayerController != nullptr)
				{
					// 마우스를 보이게 설정함.
					HorrorGamePlayerController->SetShowMouseCursor(true);
					HorrorGamePlayerController->bEnableClickEvents = true;
					HorrorGamePlayerController->bEnableMouseOverEvents = true;
				}
				return;
			}
			// 게임을 클리어한 상태인 경우
			case Player_Status::Clear: // 클리어 및 엔딩
			{
				// 충돌이 안 되게 설정하고, 게임에서 안 보이게 설정함. 그리고 컨트롤러를 통해 Input이 안 되게 설정함.
				SetActorEnableCollision(false);
				GetMesh()->SetHiddenInGame(true);
				DisableInput(HorrorGamePlayerController);

				// 게임 플레이 타이머를 정지시킴.
				if (GameUIWidget)
				{
					GameUIWidget->SetTimerStop(true);
				}

				return;
			}
		}
	}
}

// 설명 텍스트를 설정하는 함수.
void AHorrorGameCharacter::SetExplainText(FText text, int32 time)
{
	if (GameUIWidget)
	{
		GameUIWidget->SetInteractDotExplainText(text);
	}
	TextTimer = time;
	cnt = 0.f;
}

// 상호 작용 실패 텍스트를 설정하는 함수.
void AHorrorGameCharacter::SetErrorText(FText text, int32 time)
{
	if (GameUIWidget)
	{
		GameUIWidget->SetInteractDotErrorText(text);
	}
	ErrorTextTimer = time;
	ErrorTextCount = 0.f;
}

// 착란 게이지를 증가시키는 함수.
void AHorrorGameCharacter::AddPatience(int32 value)
{
	// 게이지를 변동 시킴.
	Patience = FMath::Clamp(Patience + value, 0, 100);
	/*Patience += value;
	if (Patience <= 0)
	{
		Patience = 0;
	}

	if (Patience >= 100)
	{
		Patience = 100;
	}*/

	// 게이지가 몇 초간 증가되지 않게 설정함.
	bIsCooldown = true; 

	// 착란 UI를 업데이트함.
	if (GameUIWidget)
	{
		GameUIWidget->SetPatience(Patience);
	}
}

// 불빛을 깜빡이게 하는 함수.
void AHorrorGameCharacter::LightFlicker(float value)
{
	if (bIsCigarLightOn)
	{
		float LightIntense = FMath::Clamp(CigarIntensity * value, 0.0f, CigarIntensity);

		/*if (LightIntense < 0.f)
		{
			CigarLight->SetIntensity(0.0f);
		}
		else if (LightIntense > CigarIntensity)
		{
			CigarLight->SetIntensity(CigarIntensity);
		}
		else
		{
			CigarLight->SetIntensity(LightIntense);
		}*/
		CigarLight->SetIntensity(LightIntense);
	}

	if (bIsFlashLightOn)
	{
		float LightIntense = FMath::Clamp(FlashIntensity * value, 0.0f, FlashIntensity);

		/*if (LightIntense < 0.f)
		{
			FlashLight->SetIntensity(0.0f);
		}
		else if (LightIntense > FlashIntensity)
		{
			FlashLight->SetIntensity(FlashIntensity);
		}
		else
		{
			FlashLight->SetIntensity(LightIntense);
		}*/
		FlashLight->SetIntensity(LightIntense);
	}
}

void AHorrorGameCharacter::SetNotifyAttackStart(const bool value)
{
	bNotifyAttackStart = value;
	
}

bool AHorrorGameCharacter::GetNotifyAttackStart()
{
	return bNotifyAttackStart;
}

// 공격이 성공했는지 체크하는 함수.
void AHorrorGameCharacter::AttackCheck(bool value)
{
	bShouldAttack = value;
	auto BronzeSword = Cast<APlayerSword_cpp>(Sword->GetChildActor());
	if (!value) // 만약 Attack Check의 값이 false면
	{
		SetNotifyAttackStart(false); // 공격 시작 알림을 false로
		if (BronzeSword)
		{
			BronzeSword->SetShouldExorcism(bShouldAttack);
		}
	}
	if (BronzeSword)
	{
		BronzeSword->SetShouldExorcism(bShouldAttack);
		if (BronzeSword->bHadExorcism) // 요괴를 처치한 상황인 경우에, 검 개수 감소
		{
			BronzeSword->SetShouldExorcism(false);
			if (SwordCount > 0) // 검 개수가 0보다 큰 경우에만 감소하도록 설정
			{
				SwordCount--;
				if (CurrentItemNum >= 0 && CurrentItemNum <= InventoryNum) // 또한 현재 아이템 인덱스가 인벤토리 배열의 범위 내에 있는 경우에만 
				{
					Inventory[CurrentItemNum].ItemCount = SwordCount;
				}
			}

			// 위 로직으로 열쇠 개수가 0개일 경우
			if (Inventory[CurrentItemNum].ItemCount == 0)
			{
				// 인벤토리에서 제거하고, 인벤토리의 최대 개수와 현재 인벤토리 커서를 1 줄임.
				Inventory.RemoveAt(CurrentItemNum);
				InventoryNum--;
				CurrentItemNum--;
				
				// 만약 인벤토리에 아이템이 남아있음에도 커서가 -1 이하일 경우, 커서를 0으로 설정함.
				if (CurrentItemNum < 0 && InventoryNum >= 0)
				{
					CurrentItemNum = 0;
				}

				// 인벤토리가 비어져있는 경우, 커서를 -1로 설정함.
				if (InventoryNum < 0)
				{
					CurrentItemNum = -1;
				}
				
				// 임시 데이터를 생성해 인벤토리 제일 마지막에 추가함.
				FHorrorGameItemData TempItem;
				TempItem.Clear();
				Inventory.Add(TempItem);
			}

			// 오브젝트 UI의 칼 개수를 업데이트 함.
			if (GameUIWidget)
			{
				GameUIWidget->SetObjectCount(1, SwordCount);
			}
			
			// 아이템 상태를 업데이트함.
			CurrentItem();
		}
	}

}

bool AHorrorGameCharacter::GetAttackCheck()
{
	return bShouldAttack;
}

void AHorrorGameCharacter::SetIsInWater(bool value)
{
	bIsInWater = value;
	if (bIsInWater)
	{
		GetCharacterMovement()->MaxWalkSpeed = 180.0f * InWaterSpeedDown;
	}
	else
	{
		GetCharacterMovement()->MaxWalkSpeed = 180.0f;
	}
}

// DEPRECATED
void AHorrorGameCharacter::OnDoorBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor != this && OtherActor)
	{
		if (auto Door = Cast<ADoor_cpp>(OtherActor))
		{
			Door->SetDoorCollision(true);
		}

		else if (auto ClassroomDoor = Cast<AClassroomDoors_cpp>(OtherActor))
		{
			ClassroomDoor->SetDoorCollision(true);
		}
		
		else if (auto Reaper = Cast<AReaper_cpp>(OtherActor))
		{
			Reaper->SetCreatureCollision(true);
		}

		else if (auto Runner = Cast<ARunner_cpp>(OtherActor))
		{
			Runner->SetCreatureCollision(true);
		}

		else if (auto Brute = Cast<ABrute_cpp>(OtherActor))
		{
			Brute->SetCreatureCollision(true);
		}
	}
}

// DEPRECATED
void AHorrorGameCharacter::OnDoorBoxEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor != this && OtherActor)
	{
		if (auto Door = Cast<ADoor_cpp>(OtherActor))
		{
			Door->SetDoorCollision(false);
		}

		else if (auto ClassroomDoor = Cast<AClassroomDoors_cpp>(OtherActor))
		{
			ClassroomDoor->SetDoorCollision(false);
		}

		else if (auto Reaper = Cast<AReaper_cpp>(OtherActor))
		{
			Reaper->SetCreatureCollision(false);
		}

		else if (auto Runner = Cast<ARunner_cpp>(OtherActor))
		{
			Runner->SetCreatureCollision(false);
		}

		else if (auto Brute = Cast<ABrute_cpp>(OtherActor))
		{
			Brute->SetCreatureCollision(false);
		}
	}
}

// DEPRECATED
void AHorrorGameCharacter::OnSprayOverlap(const TArray<FOverlapResult>& OverlapResults)
{
	/*if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 1.5f, FColor::Yellow, FString::Printf(TEXT("Spary Overlapped")));*/
	for (auto OverlapResult : OverlapResults)
	{
		FHitResult OutHit;
		FVector Start = FirstPersonCameraComponent->GetComponentLocation();
		FVector ForwardVector = FirstPersonCameraComponent->GetForwardVector();
		FVector End = (ForwardVector * 1200.0f) + Start;

		if (!GetWorld()->LineTraceSingleByChannel(OutHit, Start, End, ECC_EngineTraceChannel4))
		{
			if (auto Reaper = Cast<AReaper_cpp>(OverlapResult.GetActor()))
			{
				/*if (GEngine)
					GEngine->AddOnScreenDebugMessage(-1, 1.5f, FColor::Red, FString::Printf(TEXT("That is Reaper")));*/
				float Dist = FVector::Distance(GetActorLocation(), Reaper->GetActorLocation());
				Reaper->Stunning(Dist);
			}

			else if (auto Runner = Cast<ARunner_cpp>(OverlapResult.GetActor()))
			{
				/*if (GEngine)
					GEngine->AddOnScreenDebugMessage(-1, 1.5f, FColor::Red, FString::Printf(TEXT("That is Runner")));*/
				float Dist = FVector::Distance(GetActorLocation(), Runner->GetActorLocation());
				Runner->Stunning(Dist);
			}

			else if (auto Brute = Cast<ABrute_cpp>(OverlapResult.GetActor()))
			{
				/*if (GEngine)
					GEngine->AddOnScreenDebugMessage(-1, 1.5f, FColor::Red, FString::Printf(TEXT("That is Brute")));*/
				float Dist = FVector::Distance(GetActorLocation(), Brute->GetActorLocation());
				Brute->Stunning(Dist);
			}
		}
	}
}

// 착란으로 인한 소리 지름이 끝날 때 호출할 콜백 함수.
void AHorrorGameCharacter::SetPanicScreamEnd()
{
	// 착란 잔량을 0으로 설정하고, 컨트롤러를 통해 Input이 되도록 설정함.
	bIsScreaming = false;
	PatienceToReduce = 0;
	bIsPatienceReduce = true;
	EnableInput(HorrorGamePlayerController);
}

// 레벨이 시작될 때 호출할 함수.
void AHorrorGameCharacter::LevelStart()
{
	// 로딩 중 상태일 경우
	if (PlayerStatus == Player_Status::Loading)
	{
		// 생존 상태로 업데이트하고
		PlayerStatus = Player_Status::Survive;
		
		// Game UI를 초기화함.
		if (GameUIWidget)
		{
			GameUIWidget->Player = this;
			GameUIWidget->AllWidgetInit();
		}

		// 그 후 설정값을 적용시킴.
		SetPlayerSetting();
	}
}

void AHorrorGameCharacter::SetIsTimeStop(bool value)
{
	bIsTimeStopChange = value;
}

bool AHorrorGameCharacter::GetIsTimeStop()
{
	return bIsTimeStop;
}

bool AHorrorGameCharacter::GetIsScreaming()
{
	return bIsScreaming;
}

bool AHorrorGameCharacter::GetIsTimeStopChange()
{
	return bIsTimeStopChange;
}

void AHorrorGameCharacter::SetBrightness(float Value)
{
	FirstPersonCameraComponent->PostProcessSettings.bOverride_ColorGamma = true;
	FirstPersonCameraComponent->PostProcessSettings.ColorGamma = FVector4(1.f, 1.f, 1.f, Value * 1.5f);
}

// deprecated
void AHorrorGameCharacter::SetMainWidget(UGameUI* inMainWidget)
{
	GameUIWidget = inMainWidget;
	LevelStart();
	//SetPlayerSetting();
}

void AHorrorGameCharacter::SetReaperLookPlayer(bool inReaperWatchPlayer)
{
	// 시간이 정지한 상태라면(즉 거울을 사용한 상태면), 리퍼 능력 무효화
	if (bIsTimeStop) 
	{
		bReaperWatchPlayer = false;
		ReaperWatchElapsedTime = 0.0f;
	}
	// 그게 아닐 경우
	else
	{
		// 머티리얼을 한 번만 넣기 위함으로 두 개를 비교해서 달라야 추가, 제거 가능
		if (bReaperWatchPlayer != inReaperWatchPlayer) 
		{
			// 리퍼가 플레이어를 보고 있다면
			if (inReaperWatchPlayer)
			{
				// 카메라에 포스트 프로세스 머티리얼을 적용하고 리퍼 능력 사용 효과음을 재생함.
				FirstPersonCameraComponent->AddOrUpdateBlendable(PostProcessDynamicInstance, 1.f);
				if (ReaperSoundCue)
				{
					UGameplayStatics::PlaySound2D(this, ReaperSoundCue);
				}
			}
			// 아닐 경우, 포스트 프로세스 머티리얼을 제거함.
			else
			{
				FirstPersonCameraComponent->RemoveBlendable(PostProcessDynamicInstance);
			}
		} 
		bReaperWatchPlayer = inReaperWatchPlayer;
	}

}

bool AHorrorGameCharacter::GetReaperLookPlayer()
{
	return bReaperWatchPlayer;
}

// 문서 보관함 문서를 획득할 때 텍스트를 출력할 함수.
void AHorrorGameCharacter::SetArchiveGetText(FText inText)
{
	if (PlayerStatus == Player_Status::Died)
	{
		HorrorGamePlayerController->SetDeadUIText(inText);
	}
	else
	{
		if (GameUIWidget)
		{
			GameUIWidget->SetArchiveGetText(inText);
		}
	}
	bIsArchiveTextOn = true;
	ArchiveTextTimer = 0.f;
}

// 사망 시 적 개체를 바라 보게 하기 위한 함수.
void AHorrorGameCharacter::OnFocus(FVector TargetLocation)
{
	FirstPersonCameraComponent->bUsePawnControlRotation = false;

	// 플레이어의 이동을 즉시 멈추고, 컨트롤러를 통한 Input도 불가능하게 설정함.
	GetCharacterMovement()->StopMovementImmediately();
	DisableInput(HorrorGamePlayerController);

	// 해당 액터를 바라보도록 회전 값을 설정하고 카메라 회전 타임라인을 재생함.
	FVector CameraLocation = FirstPersonCameraComponent->GetComponentLocation();
	LookAtRotation = UKismetMathLibrary::FindLookAtRotation(CameraLocation, TargetLocation);
	RotateCameraTimeline.PlayFromStart();
}

// 카메라 회전 타임라인이 재생될 때 호출할 콜백 함수.
void AHorrorGameCharacter::OnFocusRotation(float inLerpAlpha)
{
	// 해당 액터를 바라보도록 카메라를 회전시킴.
	FirstPersonCameraComponent->bUsePawnControlRotation = false;
	FRotator CameraRotation = FirstPersonCameraComponent->GetComponentRotation();
	FRotator NewRotation = FMath::Lerp(CameraRotation, LookAtRotation, inLerpAlpha);

	FirstPersonCameraComponent->SetWorldRotation(NewRotation);
}

// 카메라 회전 타임라인이 끝날 때 호출할 콜백 함수.
void AHorrorGameCharacter::OnFocusFinished()
{
	// 컨트롤러의 Input이 가능하도록 설정함.
	EnableInput(HorrorGamePlayerController);
	FirstPersonCameraComponent->bUsePawnControlRotation = true;
}

// 달리기를 할 때 카메라 FOV 타임라인을 재생할 때 호출할 콜백 함수.
void AHorrorGameCharacter::OnSprintCameraView(float inLerpAlpha)
{
	// 카메라의 FOV를 변경시킴.
	float FieldOfView = FMath::Lerp(90.f, 100.f, inLerpAlpha);

	FirstPersonCameraComponent->SetFieldOfView(FieldOfView);
}

void AHorrorGameCharacter::OnAnnounce()
{
	SetExplainText(NSLOCTEXT("AHorrorGameCharacter", "Reaper_Unseal", "Some ominous sound comes..."), 3);
	// 번역은 "뭔가 불길한 소리가 들려온다."로
}