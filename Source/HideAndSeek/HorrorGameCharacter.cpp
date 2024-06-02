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

	/*static ConstructorHelpers::FObjectFinder<USoundBase> RestSound(TEXT("/Game/Assets/Sounds/RunStop"));
	if (RestSound.Succeeded())
	{
		RunStop = RestSound.Object;
		Sound->OnAudioFinished.AddDynamic(this, &AHorrorGameCharacter::BreatheSoundFinish);
	}*/

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

	/*HeartBeat = CreateDefaultSubobject<UAudioComponent>(TEXT("HeartBeatSound"));
	HeartBeat->SetupAttachment(FirstPersonCameraComponent);
	HeartBeat->SetAutoActivate(false);*/

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

	//SelectedItem = CreateDefaultSubobject<AItems>(TEXT("CurrentItem"));
	//// SelectedItem->SetupAttachment(FirstPersonCameraComponent);
	//SelectedItem->AttachToComponent(FirstPersonCameraComponent, FAttachmentTransformRules::KeepRelativeTransform);

	Lantern = CreateDefaultSubobject<UChildActorComponent>(TEXT("Lantern"));
	Lantern->SetupAttachment(FirstPersonCameraComponent);
	Lantern->SetHiddenInGame(true);

	Sword = CreateDefaultSubobject<UChildActorComponent>(TEXT("Sword"));
	Sword->SetupAttachment(GetMesh(), TEXT("hand_r_socket"));
	Sword->SetHiddenInGame(true);
	// Sword->DoesSocketExist(TEXT("hand_r"));

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

	// ObjectNumbers = 0;
	SwordCount = 0;
	MirrorCount = 0;
	BellCount = 0;
	DoorBoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("DoorBox"));
	DoorBoxComp->SetupAttachment(FirstPersonCameraComponent);
	DoorBoxComp->OnComponentBeginOverlap.AddDynamic(this, &AHorrorGameCharacter::OnDoorBoxBeginOverlap);
	DoorBoxComp->OnComponentEndOverlap.AddDynamic(this, &AHorrorGameCharacter::OnDoorBoxEndOverlap);
	
	PlayerStatus = Player_Status::Loading;
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

	CurrentItemNum = -1;

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

	//GameUIWidget = HorrorGamePlayerController->GetMainWidget();
	
	//SetPlayerSetting();
	//LevelStart();

	if (CurveFloat)
	{
		FOnTimelineFloat TimelineProgress;
		TimelineProgress.BindDynamic(this, &AHorrorGameCharacter::LightFlicker);
		FlickeringLight.AddInterpFloat(CurveFloat, TimelineProgress);
		FlickeringLight.SetLooping(true);
	}

	if (RotateCurveFloat)
	{
		FOnTimelineFloat TimelineProgress;
		TimelineProgress.BindDynamic(this, &AHorrorGameCharacter::OnFocusRotation);
		RotateCameraTimeline.AddInterpFloat(RotateCurveFloat, TimelineProgress);

		FOnTimelineEvent TimelineFinishEvent;
		TimelineFinishEvent.BindDynamic(this, &AHorrorGameCharacter::OnFocusFinished);
		RotateCameraTimeline.SetTimelineFinishedFunc(TimelineFinishEvent);

	}

	if (SprintCurveFloat)
	{
		FOnTimelineFloat TimelineProgress;
		TimelineProgress.BindDynamic(this, &AHorrorGameCharacter::OnSprintCameraView);
		SprintCameraTimeline.AddInterpFloat(SprintCurveFloat, TimelineProgress);
	}

	if (PanicSoundCue)
	{
		PanicSound->SetAutoActivate(false);
		PanicSound->SetSound(PanicSoundCue);
		PanicSound->OnAudioFinished.AddDynamic(this, &AHorrorGameCharacter::SetPanicScreamEnd);
	}

	PostProcessDynamicInstance = UMaterialInstanceDynamic::Create(PostProcessMaterialInstance, this);
}

void AHorrorGameCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AActor* HitActor = nullptr;

	GetLineTraceSingle(HitActor);

	if (bIsHiding) // 숨은 상태라면 계속 유지하게 설정
	{
		SetActorLocation(BeforeHideLocation);
		SetActorRotation(BeforeHideRotation);
	}

	FlickeringLight.TickTimeline(DeltaTime);
	RotateCameraTimeline.TickTimeline(DeltaTime);
	SprintCameraTimeline.TickTimeline(DeltaTime);

	if (PlayerStatus == Player_Status::Died)
	{
		//GameUIWidget->SetBaseInterface(false);
		if (HorrorGamePlayerController != nullptr)
		{
			HorrorGamePlayerController->SetShowMouseCursor(true);
			HorrorGamePlayerController->bEnableClickEvents = true;
			HorrorGamePlayerController->bEnableMouseOverEvents = true;
		}
	}
	if (PlayerStatus == Player_Status::Stunned)
	{
		StunTimer += DeltaTime;
		if (StunTimer >= 1.5f)
		{
			StunTimer = 0.f;
			SetPlayerStatus(Player_Status::Survive);
		}
	}

	if (GetCharacterMovement()->Velocity.Length() > 0.f)
	{
		bIsMove = true;
	}
	else
	{
		bIsMove = false;
	}
	
	if (TextTimer > 0)
	{
		cnt += DeltaTime;

		if (cnt >= TextTimer)
		{
			SetExplainText(NSLOCTEXT("AHorrorGameCharacter", "None_Explain", ""), 0);
		}
	}

	if (ErrorTextTimer > 0)
	{
		ErrorTextCount += DeltaTime;

		if (ErrorTextCount >= ErrorTextTimer)
		{
			SetErrorText(NSLOCTEXT("AHorrorGameCharacter", "None_Error", ""), 0);
		}
	}

	if (bIsArchiveTextOn)
	{
		ArchiveTextTimer += DeltaTime;

		if (ArchiveTextTimer >= 10.f)
		{
			if (PlayerStatus == Player_Status::Died)
			{
				HorrorGamePlayerController->SetDeadUIText(FText::FromString(TEXT("")));
			}
			else
			{
				if (GameUIWidget)
				{
					GameUIWidget->SetArchiveGetText(FText::FromString(TEXT("")));
				}
			}
			bIsArchiveTextOn = false;
			ArchiveTextTimer = 0.f;
		}
	}

	if (bIsInWater)
	{
		if(bIsSprint)
			GetCharacterMovement()->MaxWalkSpeed = 350.f * InWaterSpeedDown;
		else
			GetCharacterMovement()->MaxWalkSpeed = 180.f * InWaterSpeedDown;

		GetCharacterMovement()->MaxWalkSpeedCrouched = 80.f * InWaterSpeedDown;
	}
	else
	{
		if (bIsSprint)
			GetCharacterMovement()->MaxWalkSpeed = 350.f;
		else
			GetCharacterMovement()->MaxWalkSpeed = 180.f;

		GetCharacterMovement()->MaxWalkSpeedCrouched = 80.f;
	}

	if (bIsTimeStop)
	{
		TimeStopTimer += DeltaTime;

		if (TimeStopTimer >= 25) // 거울 속 세계로 들어간 (== 시간 정지) 시간은 25초. 
		{
			TimeStopTimer = 0;
			bIsTimeStop = false;
			FirstPersonCameraComponent->RemoveBlendable(PostProcessMaterial);
			Cast<AHorrorGameGameMode>(UGameplayStatics::GetGameMode(GetWorld()))->StopMirrorUseBackGroundMusic();
		}
	}

	if (bIsPatienceReduce) // 캐비닛이나 옷장에 숨은 경우로 패닉 게이지가 감소
	{
		AddPatience(-1);
		if (Patience == PatienceToReduce) // 패닉 게이지가 줄어들 양만큼 줄어들었으면 줄어들 양을 초기화
		{
			PatienceToReduce = -1;
			bIsPatienceReduce = false; // 감소 중이 아니라고 알려줌
		}
	}

	if (bIsCooldown)
	{
		CooldownTimer += DeltaTime;

		if (CooldownTimer >= 5.f) // 패닉 게이지 상승 쿨타임은 5초
		{
			CooldownTimer = 0.f;
			bIsCooldown = false;
		}
	}

	if (Patience == 100 && !bIsScreaming) // 패닉 게이지가 100까지 도달했을 경우엔 어그로를 끌게 됨
	{
		GetCharacterMovement()->StopMovementImmediately();
		DisableInput(HorrorGamePlayerController);
		bIsScreaming = true;
		PanicSound->Play();
	} // 패닉이 100일 때 계속 소리를 내는 것을 방지하기 위해, Screaming하는 지 체크하여 하지 않을 경우에만 발동하도록 함
	
	if (bIsCrouch)
	{
		if (Stamina <= 399)
		{
			Stamina += 1;
			if (GameUIWidget)
			{
				GameUIWidget->SetStaminaHUD(Stamina);
			}
		}
	}
	else
	{
		if (bIsSprint)
		{
			if (Stamina > 0)
			{
				Stamina -= 2;
				if (GameUIWidget)
				{
					GameUIWidget->SetStaminaHUD(Stamina);
				}
			}
			else
			{
				EndSprint();
			}
		}
		else
		{
			if (Stamina <= 399)
			{
				Stamina += 1;
				if (GameUIWidget)
				{
					GameUIWidget->SetStaminaHUD(Stamina);
				}
			}
		}
	}

	if (bReaperWatchPlayer && PlayerStatus != Player_Status::Stunned) // 리퍼가 능력을 사용하기 시작했다면
	{
		ReaperWatchElapsedTime += DeltaTime;
		//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Yellow, FString::Printf(TEXT("ReaperWatchElapsedTime: %f"), ReaperWatchElapsedTime));

		if (ReaperWatchElapsedTime < 3.0f) // 스턴에 걸리기까지 3초의 시간이 걸림
		{
			if (PostProcessMaterialInstance) // 스턴 효과 포스트 프로세스 머티리얼이 존재하면
			{
				PixelateIntensity = FMath::Lerp(0.0f, 1.0f, ReaperWatchElapsedTime / 3.0f);
				PostProcessDynamicInstance->SetScalarParameterValue(TEXT("Switch"), PixelateIntensity);
			}
		}
		else if (ReaperWatchElapsedTime >= 3.0f)
		{
			ReaperWatchElapsedTime = 0.f;
			if (StunSoundCue)
			{
				UGameplayStatics::PlaySound2D(this, StunSoundCue);
			}
			SetPlayerStatus(Player_Status::Stunned);
			//FirstPersonCameraComponent->RemoveBlendable(PostProcessDynamicInstance);
		//	SetReaperLookPlayer(false);
		}
	}
}

//////////////////////////////////////////////////////////////////////////// Input

void AHorrorGameCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	/*if (PlayerStatus == Player_Status::Survive)
	{*/
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

void AHorrorGameCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>() * MouseSensitive;
	if (HorrorGamePlayerController != nullptr)
	{
		if (bIsHiding)
		{
			AActor* HitActor = nullptr;
			bool isHit = GetLineTraceSingle(HitActor);

			if (isHit)
			{
				if (HitActor)
				{
					//if (HitActor->IsA<ACabinet_cpp>())
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

					//else if (HitActor->IsA<AWardrobe_cpp>())
					else if (AHideObject* HideObject = Cast<AHideObject>(HitActor))
					{
						//AWardrobe_cpp* Wardrobe = Cast<AWardrobe_cpp>(HitActor);
						HideObject->Pitch += LookAxisVector.Y * -1.0f;
						HideObject->Yaw += LookAxisVector.X;
						HideObject->MoveCamera(); // Call Cabinet_cpp Hide Function
					}
				}
			}
		}
		else
		{
			// add yaw and pitch input to controller
			AddControllerYawInput(LookAxisVector.X);
			AddControllerPitchInput(LookAxisVector.Y);
		}
	}
}

//void AHorrorGameCharacter::OnGamePause()
//{
//	HorrorGamePlayerController->OnGamePause();
//}

void AHorrorGameCharacter::SetHasRifle(bool bNewHasRifle)
{
	bHasRifle = bNewHasRifle;
}

bool AHorrorGameCharacter::GetHasRifle()
{
	return bHasRifle;
}

// Sprint Functions: BeginSprint(), EndSprint(), StaminaChange()
void AHorrorGameCharacter::BeginSprint()
{
	if (!bIsCrouch) 
	{
		if (Stamina > 0)
		{
			bIsSprint = true;

			SprintCameraTimeline.PlayFromStart();

			if (Sound->GetPlayState() == EAudioComponentPlayState::Playing)
			{
				Sound->Stop();
			}

			/*if (bisSoundOn)
			{
				Sound->Stop();
				bisSoundOn = false;
			}*/
			SprintSound->Play();
		}
		// StaminaChange();
	}
}

void AHorrorGameCharacter::EndSprint()
{
	if (bIsSprint)
	{
		Sound->Play();
	}
	bIsSprint = false;

	SprintCameraTimeline.Reverse();

	if (SprintSound->GetPlayState() == EAudioComponentPlayState::Playing)
	{
		SprintSound->Stop();
	}
	// if (!bisSoundOn && bIsSprint)
		// bisSoundOn = true;
}

// deprecated
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
	GetWorld()->GetTimerManager().ClearTimer(_loopStaminaTimerHandle);
	GetWorld()->GetTimerManager().SetTimer(_loopStaminaTimerHandle, this, &AHorrorGameCharacter::StaminaChange, 0.05f, false);
}

// Courching Fucntion: BeginCrouch(), EndCrouch()
void AHorrorGameCharacter::BeginCrouch()
{
	Crouch();
	
	bIsCrouch = true;
}

void AHorrorGameCharacter::EndCrouch()
{
	UnCrouch();
	
	bIsCrouch = false;
}

// Interaction with another actors: Interact(), ItemUse() 
void AHorrorGameCharacter::Interact()
{
	AActor* HitActor = nullptr;
	bool isHit = GetLineTraceSingle(HitActor);

	if (isHit)
	{
		if (HitActor)
		{
			if (HitActor->GetClass()->ImplementsInterface(UInteractInterface::StaticClass()))
			{
				auto InterfaceVariable = Cast<IInteractInterface>(HitActor);
				
				InterfaceVariable->OnInteract(this);
				if(!bCanItemGet)
				{
					bCanItemGet = true;
				}
			}

			else if (HitActor->GetClass()->ImplementsInterface(UDoorInterface_cpp::StaticClass()))
			{
				auto InterfaceVariable = Cast<IDoorInterface_cpp>(HitActor);

				InterfaceVariable->OnInteract(this);
			}

			else if (HitActor->GetClass()->ImplementsInterface(UHideInterface::StaticClass()))
			{
				auto InterfaceVariable = Cast<IHideInterface>(HitActor);

				InterfaceVariable->OnInteract(this);
				if (bIsHiding) // 숨은 상태가 되면
				{
					BeforeHideLocation = GetActorLocation();
					BeforeHideRotation = GetActorRotation();
				}
			}

			else if (AWardrobeDrawer_cpp* WardDrawer = Cast<AWardrobeDrawer_cpp>(HitActor))
			{
				//AWardrobeDrawer_cpp* WardDrawer = Cast<AWardrobeDrawer_cpp>(HitActor);
				
				WardDrawer->OnInteract();
			}
			else if (ADrawer_cpp* Drawer = Cast<ADrawer_cpp>(HitActor))
			{
				/*ADrawer_cpp* Drawer = Cast<ADrawer_cpp>(HitActor);*/
				Drawer->OnInteract();
			}
			
			else if(ALockerDoorActor_cpp* Locker = Cast<ALockerDoorActor_cpp>(HitActor))
			{
				Locker->OnInteract(this);
			}

			//else if (ActorName.Contains(TEXT("Cabinet_cpp"), ESearchCase::CaseSensitive, ESearchDir::FromEnd))
			//else if(ACabinet_cpp* Cabinet = Cast<ACabinet_cpp>(HitActor))
			//{
			//	//ACabinet_cpp* Cabinet = Cast<ACabinet_cpp>(HitActor);
			//	Cabinet->OnInteract(); // Call Cabinet_cpp Hide Function
			//	if (!bIsPatienceReduce)
			//	{
			//		PatienceToReduce = Patience / 2;
			//		bIsPatienceReduce = true;
			//	}
			//}

			//else if (AWardrobe_cpp* Wardrobe = Cast<AWardrobe_cpp>(HitActor))
			//{
			//	//AWardrobe_cpp* Wardrobe = Cast<AWardrobe_cpp>(HitActor);
			//	
			//	Wardrobe->OnInteract();
			//	if (!bIsPatienceReduce)
			//	{
			//		PatienceToReduce = Patience / 2;
			//		bIsPatienceReduce = true;
			//	}
			//}

			//else if (AMetalDoor_cpp* MetalDoor = Cast<AMetalDoor_cpp>(HitActor))
			//{
			//	if (MetalDoor->bIsDoorLocked)
			//	{
			//		ErrorInteractText = TEXT("Locked");
			//		SetErrorText(ErrorInteractText, 3);
			//		/*GameUIWidget->SetInteractDotErrorText(ErrorInteractText);
			//		GetWorld()->GetTimerManager().SetTimer(_TextTimerHandle, FTimerDelegate::CreateLambda([&]() {
			//			ErrorInteractText = TEXT("");
			//			GameUIWidget->SetInteractDotErrorText(ErrorInteractText);
			//			GetWorld()->GetTimerManager().ClearTimer(_TextTimerHandle);
			//		}), 1.0f, false);*/
			//	}
			//	MetalDoor->OnInteract();
			//}

			else if (ADeskDrawer_cpp* DeskDrawer = Cast<ADeskDrawer_cpp>(HitActor))
			{
				DeskDrawer->OnInteract();
			}

			else if (AHangingLight* HangingLight = Cast<AHangingLight>(HitActor))
			{
				HangingLight->OnInteract();
			}

			else if (AAlarm* Alarm = Cast<AAlarm>(HitActor))
			{
				Alarm->OnInteract();
			}	

			else if (ALightSwitch* Switch = Cast<ALightSwitch>(HitActor))
			{
				Switch->OnInteract();
			}

			else if (AAltar_cpp* Altar = Cast<AAltar_cpp>(HitActor))
			{
				if (!bIsCleared)
				{
					Altar->OnInteract(this);
				}
			}

			else if (ASwitchLever* Lever = Cast<ASwitchLever>(HitActor))
			{
				Lever->OnInteract(this);
			}

			else if (ADistributionBox* DBox = Cast<ADistributionBox>(HitActor))
			{
				DBox->OnInteract(this);
			}

			else if (APaper* Paper = Cast<APaper>(HitActor))
			{
				Paper->OnInteract(this);
			}
			else if (AEnd_Mirror* EndMirror = Cast<AEnd_Mirror>(HitActor))
			{
				EndMirror->OnInteract(this);
			}
		}
	}
}

void AHorrorGameCharacter::ItemUse()
{
	if (CurrentItemNum >= 0 && 0 <= InventoryNum) // Avoid Index Error
	{
		FHorrorGameItemData& CurrentItem = Inventory[CurrentItemNum]; // Get Selected Item

		//if (CurrentItem.ItemName.Contains(TEXT("CigarLight")))
		if (CurrentItem.ItemNumber == 1) // CigarLighter
		{
			UseCigarLight();
		}

		//else if (CurrentItem.ItemName.Contains(TEXT("FlashLight")))
		else if (CurrentItem.ItemNumber == 2) // FlashLight
		{
			UseFlashLight();
		}

		//else if (CurrentItem.ItemName.Contains(TEXT("Key")))
		else if (CurrentItem.ItemNumber == 3) // Key
		{
			UseKey();
		}

		//else if (CurrentItem.ItemName.Contains(TEXT("Timer")))
		else if (CurrentItem.ItemNumber == 4) // Timer
		{
			UseTimer();
		}

		//else if (CurrentItem.ItemName.Contains(TEXT("Sword")))
		else if (CurrentItem.ItemNumber == 5) // Sword
		{
			UseSword();
		}

		//else if (CurrentItem.ItemName.Contains(TEXT("Bell")))
		else if (CurrentItem.ItemNumber == 6) // Bell
		{
			UseBell();
		}

		//else if (CurrentItem.ItemName.Contains(TEXT("Mirror")))
		else if (CurrentItem.ItemNumber == 7) // Mirror
		{
			UseMirror();
		}

		//else if (CurrentItem.ItemName.Contains(TEXT("Extinguisher")))
		else if (CurrentItem.ItemNumber == 8) // Extinguisher
		{
			if(bCanExtinguisherUse)
				UseExtinguisher();
		}

		//else if (CurrentItem.ItemName.Contains(TEXT("Cutter")))
		else if (CurrentItem.ItemNumber == 9) // Cutter
		{
			UseCutter();
		}

		//else if (CurrentItem.ItemName.Contains(TEXT("Lantern")))
		else if (CurrentItem.ItemNumber == 10) // Lantern
		{
			AActor* ChildActor = Lantern->GetChildActor();
			if (ASoul_Lantern_cpp* SoulLantern = Cast<ASoul_Lantern_cpp>(ChildActor))
			{
				SoulLantern->UseInteract(this);
			}
			/*if (Lantern->GetClass()->ImplementsInterface(UInteractInterface::StaticClass()))
			{
				Cast<IInteractInterface>(Lantern)->UseInteract();
			}*/
			// UseLantern();
		}

		else if (CurrentItem.ItemNumber == 11) // GlowStick
		{
			UseGlowStick();
		}

	}
}


// LineTrace Functions: GetLineTraceSingle(), GetLineTraceSingleForBP, GetLineTraceSingleForBPActor()
bool AHorrorGameCharacter::GetLineTraceSingle(AActor* &HitActor)
{
	if (FirstPersonCameraComponent == nullptr)
		return false;

	FHitResult OutHit;
	FVector Start = FirstPersonCameraComponent->GetComponentLocation();
	FVector ForwardVector = FirstPersonCameraComponent->GetForwardVector();
	FVector End = (ForwardVector * TraceLength) + Start;


	bool isHit = GetWorld()->LineTraceSingleByChannel(OutHit, Start, End, ECC_Visibility);
	HitActor = OutHit.GetActor();
	if (isHit)
	{
		if (OutHit.GetActor())
		{
			if (HitActor->IsA<AItems>()) // Item 클래스 일 경우
			{
				if (GameUIWidget)
				{
					GameUIWidget->SetInteractDotText(NSLOCTEXT("AHorrorGameCharacter","Look_Item","Take"));
					GameUIWidget->SetInteractDot(true);
				}
				return true;
			}
			
			else if(HitActor->IsA<ADrawerClass>()) // 서랍류(책상 서랍, 옷장 서랍, 일반 서랍)일 경우
			{
				if (GameUIWidget)
				{
					GameUIWidget->SetInteractDotText(NSLOCTEXT("AHorrorGameCharacter", "Look_Drawer", "Open/Close"));
					GameUIWidget->SetInteractDot(true);
				}
				return true;
			}

			else if (HitActor->GetClass()->ImplementsInterface(UDoorInterface_cpp::StaticClass())) // 문일 경우(교실문, 창살문, 방문)
			{
				if (GameUIWidget)
				{
					GameUIWidget->SetInteractDotText(NSLOCTEXT("AHorrorGameCharacter", "Look_Door", "Open/Close"));
					GameUIWidget->SetInteractDot(true);
				}
				return true;
			}

			else if (HitActor->GetClass()->ImplementsInterface(UHideInterface::StaticClass())) // 숨는 포인트일 경우(옷장, 캐비닛)
			{
				if (GameUIWidget)
				{
					if (bIsHiding)
					{
						GameUIWidget->SetInteractDotText(NSLOCTEXT("AHorrorGameCharacter", "Look_Cabinet_While_Hiding", "Exit"));
					}
					else
					{
						GameUIWidget->SetInteractDotText(NSLOCTEXT("AHorrorGameCharacter", "Look_Cabinet", "Hide"));
					}
					GameUIWidget->SetInteractDot(true);
				}
				return true;
			}

			else if(HitActor->IsA<ALockerDoorActor_cpp>()) // 사물함 문일경우
			{
				if (GameUIWidget)
				{
					GameUIWidget->SetInteractDotText(NSLOCTEXT("AHorrorGameCharacter", "Look_Locker", "Open/Close"));
					GameUIWidget->SetInteractDot(true);
				}
				return true;
			}

			else if (HitActor->IsA<AAlarm>()) // 경보기일 경우
			{
				if (GameUIWidget)
				{
					GameUIWidget->SetInteractDotText(NSLOCTEXT("AHorrorGameCharacter", "Look_Alarm", "Ring"));
					GameUIWidget->SetInteractDot(true);
				}
				return true;
			}

			else if (AHangingLight* HangLight = Cast<AHangingLight>(HitActor)) // 전등일 경우
			{
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

			else if (ADistributionBox* DBox = Cast<ADistributionBox>(HitActor)) // 전압기일 경우
			{
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

			else if (HitActor->IsA<ALightSwitch>()) // 전등 스위치일 경우
			{
				if (GameUIWidget)
				{
					GameUIWidget->SetInteractDotText(NSLOCTEXT("AHorrorGameCharacter", "Look_Light_Switch", "Turn On"));
					GameUIWidget->SetInteractDot(true);
				}
				return true;
			}

			else if (HitActor->IsA<AAltar_cpp>()) // 제단일 경우
			{
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

			else if (HitActor->IsA<ASwitchLever>()) // 레버일 경우
			{
				if (GameUIWidget)
				{
					GameUIWidget->SetInteractDotText(NSLOCTEXT("AHorrorGameCharacter", "Look_Switch_Lever", "Lift Up/Down"));
					GameUIWidget->SetInteractDot(true);
				}
				return true;
			}

			else if (HitActor->IsA<APaper>()) // 종이일 경우
			{
				if (GameUIWidget)
				{
					GameUIWidget->SetInteractDotText(NSLOCTEXT("AHorrorGameCharacter", "Look_Paper", "Look"));
					GameUIWidget->SetInteractDot(true);
				}
				return true;
			}

			else if (HitActor->IsA<AEnd_Mirror>()) // 엔딩용 거울일 경우
			{
				if (GameUIWidget)
				{
					GameUIWidget->SetInteractDotText(NSLOCTEXT("AHorrorGameCharacter", "Look_Mirror", "Go To Berith"));
					GameUIWidget->SetInteractDot(true);
				}
				return true;
			}
		}
		/*else
		{
			GameUIWidget->SetInteractDotText(TEXT(""));;
			GameUIWidget->SetInteractDot(false);
			return false;
		}*/
	}

	if (GameUIWidget)
	{
		GameUIWidget->SetInteractDotText(NSLOCTEXT("AHorrorGameCharacter", "Nothing_Look", ""));
		GameUIWidget->SetInteractDot(false); // 에러
	}
	return false;
}

void AHorrorGameCharacter::SetCameraComponentNoise(int32 WhichStatus)
{
	FTimerHandle NoiseTimer;
	GetWorldTimerManager().ClearTimer(NoiseTimer);
	AGameModeBase* GameMode = UGameplayStatics::GetGameMode(this);
	
	switch (WhichStatus)
	{
	case 1: // 근처에 Creature가 있는 상황이면, 카메라에 노이즈 걸 것임
		FirstPersonCameraComponent->PostProcessSettings.bOverride_VignetteIntensity = true;
		FirstPersonCameraComponent->PostProcessSettings.VignetteIntensity = 1.f;
		FirstPersonCameraComponent->PostProcessSettings.bOverride_FilmGrainIntensity = true;
		FirstPersonCameraComponent->PostProcessSettings.FilmGrainIntensity = 1.f;
		if (!bIsTimeStop) // 거울을 사용하지 않은 상태여야 재생안함
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
		//HeartBeat->Play(); // 동시에 심장박동 소리도 재생
		break;
	case 2: // 추격 판정이 뜰 경우 노이즈 크게 함
		FirstPersonCameraComponent->PostProcessSettings.bOverride_VignetteIntensity = true;
		FirstPersonCameraComponent->PostProcessSettings.VignetteIntensity = 3.f;
		FirstPersonCameraComponent->PostProcessSettings.bOverride_FilmGrainIntensity = true;
		FirstPersonCameraComponent->PostProcessSettings.FilmGrainIntensity = 15.f;
		//HeartBeat->Stop(); // 추격 판정 시 박동 소리 없앰.
		GetWorldTimerManager().SetTimer(NoiseTimer, FTimerDelegate::CreateLambda([&]() {
			SetCameraComponentNoise(1); // 0.5초동안 노이즈 크게 하고 다시 1번 상태로 돌릴 것임
		}), 0.5f, false);
		break;
	case 3: // 시네마틱용
		FirstPersonCameraComponent->PostProcessSettings.bOverride_VignetteIntensity = true;
		FirstPersonCameraComponent->PostProcessSettings.VignetteIntensity = 1.f;
		FirstPersonCameraComponent->PostProcessSettings.bOverride_FilmGrainIntensity = true;
		FirstPersonCameraComponent->PostProcessSettings.FilmGrainIntensity = 1.f;
		break;
	case 4: // 시네마틱용
		FirstPersonCameraComponent->PostProcessSettings.bOverride_VignetteIntensity = false;
		FirstPersonCameraComponent->PostProcessSettings.bOverride_FilmGrainIntensity = false;
		break;
	default: // Creature가 근처에 없다면 카메라에 노이즈 해제함
		FirstPersonCameraComponent->PostProcessSettings.bOverride_VignetteIntensity = false;
		FirstPersonCameraComponent->PostProcessSettings.bOverride_FilmGrainIntensity = false;
		if (!bIsTimeStop)
		{
			if (PlayerStatus == Player_Status::Survive || PlayerStatus == Player_Status::Stunned) // 생존 또는 스턴 상태일 때만, 작동하도록
			{
				if (AHorrorGameGameMode* HorrorGameGameMode = Cast<AHorrorGameGameMode>(GameMode))
				{
					HorrorGameGameMode->StopNervousBackGroundMusic();
				}
				else if (APrologueGameMode* PrologueGameMode = Cast<APrologueGameMode>(GameMode))
				{
					PrologueGameMode->StopNervousBackGroundMusic();
				}
			}
			else if (PlayerStatus == Player_Status::Died)
			{
				if (AHorrorGameGameMode* HorrorGameGameMode = Cast<AHorrorGameGameMode>(GameMode))
				{
					HorrorGameGameMode->PlayDiedBackGroundMusic();
				}
				else if (APrologueGameMode* PrologueGameMode = Cast<APrologueGameMode>(GameMode))
				{
					PrologueGameMode->PlayDiedBackGroundMusic();
				}
			}
			else if (PlayerStatus == Player_Status::Clear)
			{
				if (AHorrorGameGameMode* HorrorGameGameMode = Cast<AHorrorGameGameMode>(GameMode))
				{
					HorrorGameGameMode->PlayClearBackGroundMusic();
				}
				else if (APrologueGameMode* PrologueGameMode = Cast<APrologueGameMode>(GameMode))
				{
					PrologueGameMode->PlayClearBackGroundMusic();
				}
			}
			else if (PlayerStatus == Player_Status::Ending)
			{
				if (AHorrorGameGameMode* HorrorGameGameMode = Cast<AHorrorGameGameMode>(GameMode))
				{
					HorrorGameGameMode->PlayEndingBackGroundMusic();
				}
			}
			//HeartBeat->Stop();
		}
	}
}

void AHorrorGameCharacter::SetPlayerSetting()
{
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

void AHorrorGameCharacter::ScrollUpItem()
{
	CurrentItemNum++;
	if (CurrentItemNum > InventoryNum) CurrentItemNum = 0;
	
	//if (GetCurrentItemName() != TEXT("FlashLight") && bIsFlashLightOn) UseFlashLight();
	if (GetCurrentItemNumber() != 2 && bIsFlashLightOn) UseFlashLight();
	//if (GetCurrentItemName() != TEXT("CigarLight") && bIsCigarLightOn) UseCigarLight();
	if (GetCurrentItemNumber() != 1 && bIsCigarLightOn) UseCigarLight();

	CurrentItem();
}

void AHorrorGameCharacter::ScrollDownItem()
{
	CurrentItemNum--;
	if (CurrentItemNum < 0 && InventoryNum >= 0) CurrentItemNum = InventoryNum;
	
	//if (GetCurrentItemName() != TEXT("FlashLight") && bIsFlashLightOn) UseFlashLight();
	if (GetCurrentItemNumber() != 2 && bIsFlashLightOn) UseFlashLight();
	//if (GetCurrentItemName() != TEXT("CigarLight") && bIsCigarLightOn) UseCigarLight();
	if (GetCurrentItemNumber() != 1 && bIsCigarLightOn) UseCigarLight();

	CurrentItem();
	
}

void AHorrorGameCharacter::AddCigarLight()
{
	FHorrorGameItemData* CigarLightData = ItemTable->FindRow<FHorrorGameItemData>(*FString::FromInt(1), TEXT(""));
	bool isFind = false;
	if (CigarLightData)
	{
		for (auto Item : Inventory) // 인벤토리에 있는지 선 체크
		{
			if (Item.ItemNumber == CigarLightData->ItemNumber)
			{
				isFind = true;
				break;
			}
		}

		if (!isFind) // 인벤토리에 해당 아이템이 없다고 판별났을 경우
		{
			if (InventoryNum >= 8) // 인벤토리의 최대 수를 넘겼으면 못 얻음
			{
				ErrorInteractText = NSLOCTEXT("AHorrorGameCharacter", "Full_Inventory", "Your Inventory is FULL!. You CANNOT get more items");
				SetErrorText(ErrorInteractText, 3);
				/*GameUIWidget->SetInteractDotErrorText(ErrorInteractText);
				GetWorld()->GetTimerManager().SetTimer(_TextTimerHandle, FTimerDelegate::CreateLambda([&]() {
					ErrorInteractText = TEXT("");
					GameUIWidget->SetInteractDotErrorText(ErrorInteractText);
					GetWorld()->GetTimerManager().ClearTimer(_TextTimerHandle);
				}), 1.0f, false);*/
				bCanItemGet = false;
				return;
			}
			Inventory[++InventoryNum].ItemName = CigarLightData->ItemName;
			Inventory[InventoryNum].ItemNumber = CigarLightData->ItemNumber;
			Inventory[InventoryNum].ItemIcon = CigarLightData->ItemIcon;
			Inventory[InventoryNum].Type = EItemType::ITEM_Useable;
			Inventory[InventoryNum].ItemCount = 1;
			if (CurrentItemNum < 0)
				CurrentItemNum = 0;
		}
		if (bCanItemGet)
		{
			if (IsValid(ItemGetSoundCue))
			{
				UGameplayStatics::PlaySound2D(this, ItemGetSoundCue);
			}
		}
		CurrentItem();
	}
}

void AHorrorGameCharacter::AddFlashLight()
{
	FHorrorGameItemData* FlashLightData = ItemTable->FindRow<FHorrorGameItemData>(*FString::FromInt(2), TEXT(""));
	bool isFind = false;
	if (FlashLightData)
	{
		for (auto Item : Inventory) // 인벤토리에 해당 아이템이 존재하는 지 체크함
		{
			if (Item.ItemNumber == FlashLightData->ItemNumber)
			{
				isFind = true;
				Item.ItemCount = 1;
				FlashLightBattery = 200;
				break;
			}
		}

		if (!isFind) // 인벤토리에 해당 아이템이 없다고 판별될 경우
		{
			if (InventoryNum >= 8) // 인벤토리의 최대 수를 넘겼으면 못 얻음
			{
				ErrorInteractText = NSLOCTEXT("AHorrorGameCharacter", "Full_Inventory", "Your Inventory is FULL!. You CANNOT get more items");
				SetErrorText(ErrorInteractText, 3);
				/*GameUIWidget->SetInteractDotErrorText(ErrorInteractText);
				GetWorld()->GetTimerManager().SetTimer(_TextTimerHandle, FTimerDelegate::CreateLambda([&]() {
					ErrorInteractText = TEXT("");
					GameUIWidget->SetInteractDotErrorText(ErrorInteractText);
					GetWorld()->GetTimerManager().ClearTimer(_TextTimerHandle);
				}), 1.0f, false);*/
				bCanItemGet = false;
				return;
			}
			
			FlashLightBattery = 200;
			Inventory[++InventoryNum].ItemName = FlashLightData->ItemName;
			Inventory[InventoryNum].ItemNumber = FlashLightData->ItemNumber;
			Inventory[InventoryNum].ItemIcon = FlashLightData->ItemIcon;
			Inventory[InventoryNum].Type = EItemType::ITEM_Useable;
			Inventory[InventoryNum].ItemCount = 1;
			if (CurrentItemNum < 0)
				CurrentItemNum = 0;
		}
		if (bCanItemGet)
		{
			if (IsValid(ItemGetSoundCue))
			{
				UGameplayStatics::PlaySound2D(this, ItemGetSoundCue);
			}
		}
		CurrentItem();
		if (GameUIWidget)
		{
			GameUIWidget->SetBatteryHUD(FlashLightBattery);
		}
	}
	bFLIntenseDown = false;
	FlashLight->SetIntensity(15000.f);
}

void AHorrorGameCharacter::AddKey()
{
	FHorrorGameItemData* KeyData = ItemTable->FindRow<FHorrorGameItemData>(*FString::FromInt(3), TEXT(""));
	bool isFind = false;
	if (KeyData)
	{
		for (auto Item = Inventory.CreateIterator(); Item; ++Item) // 인벤토리에 해당 아이템 존재 여부 확인
		{
			if (Item->ItemNumber == KeyData->ItemNumber)
			{
				isFind = true;
				Item->ItemCount++;
				break;
			}
		}

		if (!isFind) // 없다고 판별날 경우
		{
			if (InventoryNum >= 8) // 인벤토리의 최대를 넘겼다면 못 얻음
			{
				ErrorInteractText = NSLOCTEXT("AHorrorGameCharacter", "Full_Inventory", "Your Inventory is FULL!. You CANNOT get more items");
				SetErrorText(ErrorInteractText, 3);
				/*GameUIWidget->SetInteractDotErrorText(ErrorInteractText);
				GetWorld()->GetTimerManager().SetTimer(_TextTimerHandle, FTimerDelegate::CreateLambda([&]() {
					ErrorInteractText = TEXT("");
					GameUIWidget->SetInteractDotErrorText(ErrorInteractText);
					GetWorld()->GetTimerManager().ClearTimer(_TextTimerHandle);
				}), 1.0f, false);*/
				bCanItemGet = false;
				return;
			}

			Inventory[++InventoryNum].ItemName = KeyData->ItemName;
			Inventory[InventoryNum].ItemNumber = KeyData->ItemNumber;
			Inventory[InventoryNum].ItemIcon = KeyData->ItemIcon;
			Inventory[InventoryNum].Type = EItemType::ITEM_Useable;
			Inventory[InventoryNum].ItemCount += 1;
			if (CurrentItemNum < 0)
				CurrentItemNum = 0;
		}
		if (bCanItemGet)
		{
			if (IsValid(ItemGetSoundCue))
			{
				UGameplayStatics::PlaySound2D(this, ItemGetSoundCue);
			}
		}
		CurrentItem();
	}
}

void AHorrorGameCharacter::AddTimer()
{
	FHorrorGameItemData* TimerData = ItemTable->FindRow<FHorrorGameItemData>(*FString::FromInt(4), TEXT(""));
	bool isFind = false;
	if (TimerData)
	{
		for (auto Item = Inventory.CreateIterator(); Item; ++Item) // 인벤토리에 해당 아이템 존재 여부 확인
		{
			if (Item->ItemNumber == TimerData->ItemNumber)
			{
				isFind = true;
				Item->ItemCount++;
				break;
			}
		}

		if (!isFind) // 없다고 판별나면
		{
			if (InventoryNum >= 8) // 가질 수 있는 양을 넘겼다면 못 얻음
			{
				ErrorInteractText = NSLOCTEXT("AHorrorGameCharacter", "Full_Inventory", "Your Inventory is FULL!. You CANNOT get more items");
				SetErrorText(ErrorInteractText, 3);
				/*GameUIWidget->SetInteractDotErrorText(ErrorInteractText);
				GetWorld()->GetTimerManager().SetTimer(_TextTimerHandle, FTimerDelegate::CreateLambda([&]() {
					ErrorInteractText = TEXT("");
					GameUIWidget->SetInteractDotErrorText(ErrorInteractText);
					GetWorld()->GetTimerManager().ClearTimer(_TextTimerHandle);
				}), 1.0f, false);*/
				bCanItemGet = false;
				return;
			}

			Inventory[++InventoryNum].ItemName = TimerData->ItemName;
			Inventory[InventoryNum].ItemNumber = TimerData->ItemNumber;
			Inventory[InventoryNum].ItemIcon = TimerData->ItemIcon;
			Inventory[InventoryNum].Type = EItemType::ITEM_Useable;
			Inventory[InventoryNum].ItemCount += 1;
			if (CurrentItemNum < 0)
				CurrentItemNum = 0;
		}
		if (bCanItemGet)
		{
			if (IsValid(ItemGetSoundCue))
			{
				UGameplayStatics::PlaySound2D(this, ItemGetSoundCue);
			}
		}
		CurrentItem();
	}
}

void AHorrorGameCharacter::AddSword()
{
	FHorrorGameItemData* SwordData = ItemTable->FindRow<FHorrorGameItemData>(*FString::FromInt(5), TEXT(""));
	bool isFind = false;


	if (SwordData)
	{
		for (auto Item = Inventory.CreateIterator(); Item; ++Item) // 인벤토리 내부에 Sword가 있는 경우
		{
			if (Item->ItemNumber == SwordData->ItemNumber) // Sword
			{
				isFind = true;
				SwordCount++;
				Item->ItemCount = SwordCount;
				break;
			}
		}

		if (!isFind) // 처음 Sword를 얻는 경우
		{
			if (InventoryNum >= 8) // 가질 수 있는 최대 수를 넘기면 못 얻음
			{
				ErrorInteractText = NSLOCTEXT("AHorrorGameCharacter", "Full_Inventory", "Your Inventory is FULL!. You CANNOT get more items");
				SetErrorText(ErrorInteractText, 3);
				/*GameUIWidget->SetInteractDotErrorText(ErrorInteractText);
				GetWorld()->GetTimerManager().SetTimer(_TextTimerHandle, FTimerDelegate::CreateLambda([&]() {
					ErrorInteractText = TEXT("");
					GameUIWidget->SetInteractDotErrorText(ErrorInteractText);
					GetWorld()->GetTimerManager().ClearTimer(_TextTimerHandle);
				}), 1.0f, false);*/
				bCanItemGet = false;
				return;
			}

			Inventory[++InventoryNum].ItemName = SwordData->ItemName;
			Inventory[InventoryNum].ItemNumber = SwordData->ItemNumber;
			Inventory[InventoryNum].ItemIcon = SwordData->ItemIcon;
			Inventory[InventoryNum].Type = EItemType::ITEM_Useable;
			SwordCount = 1;
			Inventory[InventoryNum].ItemCount = SwordCount;
			if (CurrentItemNum < 0)
				CurrentItemNum = 0;

			//if (!bIsPlayerSwordGet) // 검을 처음 얻은 상태가 아니라면
			//{
			//	bIsPlayerSwordGet = true; // 갱신
			//}
		}

		//GameUIWidget->Init();
		CurrentItem();
		// ObjectNumbers++;
		if (GameUIWidget)
		{
			GameUIWidget->SetObjectCount(1, SwordCount);
		}
	}
}

void AHorrorGameCharacter::AddBell()
{
	FHorrorGameItemData* BellData = ItemTable->FindRow<FHorrorGameItemData>(*FString::FromInt(6), TEXT(""));
	bool isFind = false;

	if (BellData)
	{
		for (auto Item = Inventory.CreateIterator(); Item; ++Item) // 인벤토리 내부에 Bell이 있는 경우
		{
			if (Item->ItemNumber == BellData->ItemNumber)
			{
				isFind = true;
				BellCount++;
				Item->ItemCount = BellCount;
				break;
			}
		}

		if (!isFind) // 처음 Bell을 얻는 경우
		{
			if (InventoryNum >= 8) // 가질 수 있는 최대 수를 넘기면 못 얻음
			{
				ErrorInteractText = NSLOCTEXT("AHorrorGameCharacter", "Full_Inventory", "Your Inventory is FULL!. You CANNOT get more items");
				SetErrorText(ErrorInteractText, 3);
				/*GameUIWidget->SetInteractDotErrorText(ErrorInteractText);
				GetWorld()->GetTimerManager().SetTimer(_TextTimerHandle, FTimerDelegate::CreateLambda([&]() {
					ErrorInteractText = TEXT("");
					GameUIWidget->SetInteractDotErrorText(ErrorInteractText);
					GetWorld()->GetTimerManager().ClearTimer(_TextTimerHandle);
				}), 1.0f, false);*/
				bCanItemGet = false;
				return;
			}

			Inventory[++InventoryNum].ItemName = BellData->ItemName;
			Inventory[InventoryNum].ItemNumber = BellData->ItemNumber;
			Inventory[InventoryNum].ItemIcon = BellData->ItemIcon;
			Inventory[InventoryNum].Type = EItemType::ITEM_Useable;
			BellCount = 1;
			Inventory[InventoryNum].ItemCount = BellCount;
			if (CurrentItemNum < 0)
				CurrentItemNum = 0;

			//if (!bIsPlayerBellGet) // 방울을 처음 얻은 상태가 아니라면
			//{
			//	bIsPlayerBellGet = true; // 갱신
			//}

			//if (bIsPlayerSwordGet && bIsPlayerMirrorGet && bIsPlayerBellGet) // 한 번씩 검, 거울, 방울을 얻은 상태라면
			//{
			//	SetExplainText(NSLOCTEXT("AHorrorGameCharacter", "Reaper_Unseal", "Some ominous sound comes..."), 3);
			//	// 번역은 "뭔가 불길한 소리가 들려온다."로
			//}
		}

		CurrentItem();
		// ObjectNumbers++;
		if (GameUIWidget)
		{
			GameUIWidget->SetObjectCount(2, BellCount);
		}
	}
}

void AHorrorGameCharacter::AddMirror()
{
	FHorrorGameItemData* MirrorData = ItemTable->FindRow<FHorrorGameItemData>(*FString::FromInt(7), TEXT(""));
	bool isFind = false;

	if (MirrorData)
	{
		for (auto Item = Inventory.CreateIterator(); Item; ++Item) // 인벤토리에 Mirror가 있는 경우
		{
			if (Item->ItemNumber == MirrorData->ItemNumber)
			{
				isFind = true;
				MirrorCount++;
				Item->ItemCount = MirrorCount;
				break;
			}
		}

		if (!isFind)
		{
			if (InventoryNum >= 8) // 가질 수 있는 최대 수를 넘기면 못 얻음
			{
				ErrorInteractText = NSLOCTEXT("AHorrorGameCharacter", "Full_Inventory", "Your Inventory is FULL!. You CANNOT get more items");
				SetErrorText(ErrorInteractText, 3);
				/*GameUIWidget->SetInteractDotErrorText(ErrorInteractText);
				GetWorld()->GetTimerManager().SetTimer(_TextTimerHandle, FTimerDelegate::CreateLambda([&]() {
					ErrorInteractText = TEXT("");
					GameUIWidget->SetInteractDotErrorText(ErrorInteractText);
					GetWorld()->GetTimerManager().ClearTimer(_TextTimerHandle);
				}), 1.0f, false);*/
				bCanItemGet = false;
				return;
			}

			Inventory[++InventoryNum].ItemName = MirrorData->ItemName;
			Inventory[InventoryNum].ItemNumber = MirrorData->ItemNumber;
			Inventory[InventoryNum].ItemIcon = MirrorData->ItemIcon;
			Inventory[InventoryNum].Type = EItemType::ITEM_Useable;
			MirrorCount = 1;
			Inventory[InventoryNum].ItemCount = MirrorCount;
			if (CurrentItemNum < 0)
				CurrentItemNum = 0;

			//if (!bIsPlayerMirrorGet) // 거울을 처음 얻은 상태가 아니라면
			//{
			//	bIsPlayerMirrorGet = true; // 갱신
			//}

			//if (bIsPlayerSwordGet && bIsPlayerMirrorGet && bIsPlayerBellGet) // 한 번씩 검, 거울, 방울을 얻은 상태라면
			//{
			//	SetExplainText(NSLOCTEXT("AHorrorGameCharacter", "Reaper_Unseal", "Some ominous sound comes..."), 3);
			//	// 번역은 "뭔가 불길한 소리가 들려온다."로
			//}
		}
		//GameUIWidget->Init();
		CurrentItem();
		// ObjectNumbers++;
		if (GameUIWidget)
		{
			GameUIWidget->SetObjectCount(3, MirrorCount);
		}
	}
}

void AHorrorGameCharacter::AddExtinguisher()
{
	FHorrorGameItemData* ExtinguisherData = ItemTable->FindRow<FHorrorGameItemData>(*FString::FromInt(8), TEXT(""));
	bool isFind = false;
	if (ExtinguisherData)
	{
		for (auto Item : Inventory) // 인벤토리에 해당 아이템이 있는지 확인
		{
			if (Item.ItemNumber == ExtinguisherData->ItemNumber)
			{
				isFind = true;
				Item.ItemCount = 1;
				ExtinguisherLeft = 100;
				//GameUIWidget->Init();
				break;
			}
		}

		if (!isFind) // 없다면 판별날 경우
		{
			if (InventoryNum >= 8) // 가질 수 있는 최대 양을 넘기면 못 얻음
			{
				ErrorInteractText = NSLOCTEXT("AHorrorGameCharacter", "Full_Inventory", "Your Inventory is FULL!. You CANNOT get more items");
				SetErrorText(ErrorInteractText, 3);
				/*GameUIWidget->SetInteractDotErrorText(ErrorInteractText);
				GetWorld()->GetTimerManager().SetTimer(_TextTimerHandle, FTimerDelegate::CreateLambda([&]() {
					ErrorInteractText = TEXT("");
					GameUIWidget->SetInteractDotErrorText(ErrorInteractText);
					GetWorld()->GetTimerManager().ClearTimer(_TextTimerHandle);
				}), 1.0f, false);*/
				bCanItemGet = false;
				return;
			}

			/*GameUIWidget->SetBatteryWidget(false);
			GameUIWidget->SetCutterWidget(false);*/
			ExtinguisherLeft = 100;
			Inventory[++InventoryNum].ItemName = ExtinguisherData->ItemName;
			Inventory[InventoryNum].ItemNumber = ExtinguisherData->ItemNumber;
			Inventory[InventoryNum].ItemIcon = ExtinguisherData->ItemIcon;
			Inventory[InventoryNum].Type = EItemType::ITEM_Useable;
			Inventory[InventoryNum].ItemCount = 1;
			if (CurrentItemNum < 0)
				CurrentItemNum = 0;
			//GameUIWidget->Init();
		}
		if (bCanItemGet)
		{
			if (IsValid(ItemGetSoundCue))
			{
				UGameplayStatics::PlaySound2D(this, ItemGetSoundCue);
			}
		}
		/*if (Inventory[CurrentItemNum].ItemName == ExtinguisherData->ItemName)
		{
			GameUIWidget->SetBatteryWidget(false);
			GameUIWidget->SetCutterWidget(false);
			GameUIWidget->SetExtWidget(true);
		}*/
		CurrentItem();
		if (GameUIWidget)
		{
			GameUIWidget->SetExtHUD(ExtinguisherLeft);
		}
	}
}

void AHorrorGameCharacter::AddCutter()
{
	FHorrorGameItemData* CutterData = ItemTable->FindRow<FHorrorGameItemData>(*FString::FromInt(9), TEXT(""));
	bool isFind = false;
	if (CutterData)
	{
		for (auto Item : Inventory) // 인벤토리에 해당 아이템 있는지 확인
		{
			if (Item.ItemNumber == CutterData->ItemNumber)
			{
				isFind = true;
				Item.ItemCount = 1;
				//CutterDurability = 5;
				//GameUIWidget->Init();
				break;
			}
		}

		if (!isFind) // 없다면
		{
			if (InventoryNum >= 8) // 최대 개수를 넘기면 못 얻음
			{
				ErrorInteractText = NSLOCTEXT("AHorrorGameCharacter", "Full_Inventory", "Your Inventory is FULL!. You CANNOT get more items");
				SetErrorText(ErrorInteractText, 3);
				/*GameUIWidget->SetInteractDotErrorText(ErrorInteractText);
				GetWorld()->GetTimerManager().SetTimer(_TextTimerHandle, FTimerDelegate::CreateLambda([&]() {
					ErrorInteractText = TEXT("");
					GameUIWidget->SetInteractDotErrorText(ErrorInteractText);
					GetWorld()->GetTimerManager().ClearTimer(_TextTimerHandle);
				}), 1.0f, false);*/
				bCanItemGet = false;
				return;
			}

			/*GameUIWidget->SetBatteryWidget(false);
			GameUIWidget->SetExtWidget(false);*/
			// CutterDurability = 5;
			Inventory[++InventoryNum].ItemName = CutterData->ItemName;
			Inventory[InventoryNum].ItemNumber = CutterData->ItemNumber;
			Inventory[InventoryNum].ItemIcon = CutterData->ItemIcon;
			Inventory[InventoryNum].Type = EItemType::ITEM_Useable;
			Inventory[InventoryNum].ItemCount = 1;
			if (CurrentItemNum < 0)
				CurrentItemNum = 0;
			//GameUIWidget->Init();
		}
		/*if (Inventory[CurrentItemNum].ItemName == CutterData->ItemName)
		{
			GameUIWidget->SetBatteryWidget(false);
			GameUIWidget->SetExtWidget(false);
			GameUIWidget->SetCutterWidget(true);
		}*/
		if (bCanItemGet)
		{
			if (IsValid(ItemGetSoundCue))
			{
				UGameplayStatics::PlaySound2D(this, ItemGetSoundCue);
			}
		}
		CurrentItem(); 
		if (GameUIWidget)
		{
			GameUIWidget->SetCutterHUD(CutterDurability);
		}
	}
}

void AHorrorGameCharacter::AddLantern()
{
	FHorrorGameItemData* LanternData = ItemTable->FindRow<FHorrorGameItemData>(*FString::FromInt(10), TEXT(""));
	bool isFind = false;

	if (LanternData)
	{
		for (auto Item : Inventory)
		{
			if (Item.ItemNumber == LanternData->ItemNumber) // 인벤토리에 해당 아이템 있는지 확인
			{
				isFind = true;
				//Item.ItemCount = 1;
				////CutterDurability = 5;
				ErrorInteractText = NSLOCTEXT("AHorrorGameCharacter", "Already_Get_Lantern", "You can get ONLY 1 Lantern");
				SetErrorText(ErrorInteractText, 3);
				/*GameUIWidget->SetInteractDotErrorText(ErrorInteractText);
				GetWorld()->GetTimerManager().SetTimer(_TextTimerHandle, FTimerDelegate::CreateLambda([&]() {
					ErrorInteractText = TEXT("");
					GameUIWidget->SetInteractDotErrorText(ErrorInteractText);
					GetWorld()->GetTimerManager().ClearTimer(_TextTimerHandle);
				}), 1.0f, false);*/
				bCanItemGet = false;
				break;
			}
		}

		if (!isFind) // 없다면
		{
			if (InventoryNum >= 8) // 최대 아이템 개수를 초과하면 못 얻음
			{
				ErrorInteractText = NSLOCTEXT("AHorrorGameCharacter", "Full_Inventory", "Your Inventory is FULL!. You CANNOT get more items");
				SetErrorText(ErrorInteractText, 3);
				bCanItemGet = false;
				return;
			}

			if (IsValid(ItemGetSoundCue))
			{
				UGameplayStatics::PlaySound2D(this, ItemGetSoundCue);
			}
			Inventory[++InventoryNum].ItemName = LanternData->ItemName;
			Inventory[InventoryNum].ItemNumber = LanternData->ItemNumber;
			Inventory[InventoryNum].ItemIcon = LanternData->ItemIcon;
			Inventory[InventoryNum].Type = EItemType::ITEM_Useable;
			Inventory[InventoryNum].ItemCount = 1;
			if (CurrentItemNum < 0)
				CurrentItemNum = 0;
			//GameUIWidget->Init();
			CurrentItem();
		}
	}
}

void AHorrorGameCharacter::AddGlowStick()
{
	FHorrorGameItemData* GlowStickData = ItemTable->FindRow<FHorrorGameItemData>(*FString::FromInt(11), TEXT(""));
	bool isFind = false;
	if (GlowStickData)
	{
		for (auto Item = Inventory.CreateIterator(); Item; ++Item) // 인벤토리에 해당 아이템 존재 여부 확인
		{
			if (Item->ItemNumber == GlowStickData->ItemNumber)
			{
				isFind = true;
				Item->ItemCount++;
				break;
			}
		}

		if (!isFind) // 없다고 판별나면
		{
			if (InventoryNum >= 8) // 가질 수 있는 양을 넘겼다면 못 얻음
			{
				ErrorInteractText = NSLOCTEXT("AHorrorGameCharacter", "Full_Inventory", "Your Inventory is FULL!. You CANNOT get more items");
				SetErrorText(ErrorInteractText, 3);
				/*GameUIWidget->SetInteractDotErrorText(ErrorInteractText);
				GetWorld()->GetTimerManager().SetTimer(_TextTimerHandle, FTimerDelegate::CreateLambda([&]() {
					ErrorInteractText = TEXT("");
					GameUIWidget->SetInteractDotErrorText(ErrorInteractText);
					GetWorld()->GetTimerManager().ClearTimer(_TextTimerHandle);
				}), 1.0f, false);*/
				bCanItemGet = false;
				return;
			}

			Inventory[++InventoryNum].ItemName = GlowStickData->ItemName;
			Inventory[InventoryNum].ItemNumber = GlowStickData->ItemNumber;
			Inventory[InventoryNum].ItemIcon = GlowStickData->ItemIcon;
			Inventory[InventoryNum].Type = EItemType::ITEM_Useable;
			Inventory[InventoryNum].ItemCount += 1;
			if (CurrentItemNum < 0)
				CurrentItemNum = 0;
		}
		if (bCanItemGet)
		{
			if (IsValid(ItemGetSoundCue))
			{
				UGameplayStatics::PlaySound2D(this, ItemGetSoundCue);
			}
		}
		CurrentItem();
	}
}
// Use Item Functions
void AHorrorGameCharacter::UseCigarLight()
{
	if (bIsCigarLightOn) {
		CigarLight->SetVisibility(false);
		bIsCigarLightOn = false;
		
		CigarLightOffSound->Play();
	}
	else if (!bIsCigarLightOn)
	{
		CigarLightOnSound->Play();
	}
	CurrentItem();
}

void AHorrorGameCharacter::CigarLightOn()
{
	CigarLight->SetVisibility(true);
	bIsCigarLightOn = true;
}

void AHorrorGameCharacter::UseFlashLight()
{
	if (bIsFlashLightOn) {
		FlashLight->SetVisibility(false);
		bIsFlashLightOn = false;
	}
	else if (!bIsFlashLightOn)
	{
		FlashLight->SetVisibility(true);
		bIsFlashLightOn = true;
	}
	Turnon->Play();
	FlashLightBatteryChange();
	CurrentItem();
}

void AHorrorGameCharacter::UseKey()
{
	AActor* HitActor = nullptr;
	bool isHit = GetLineTraceSingle(HitActor);

	if (isHit)
	{
		if (HitActor)
		{
			// FString ActorName = HitActor->GetName();
			if (HitActor->GetClass()->ImplementsInterface(UDoorInterface_cpp::StaticClass()))
			{
				auto InterfaceVariable = Cast<IDoorInterface_cpp>(HitActor);
				//USoundCue* ObjectSound = LoadObject<USoundCue>(nullptr, TEXT("/Game/Assets/Sounds/SoundCues/UnLock"));
				if (KeySoundCue)
				{
					UGameplayStatics::PlaySoundAtLocation(this, KeySoundCue, GetActorLocation());
				}
				InterfaceVariable->UseInteract(this);

				if (bIsFinishUnlock)
				{
					Inventory[CurrentItemNum].ItemCount--;
					bIsFinishUnlock = false;
				}

				if (Inventory[CurrentItemNum].ItemCount == 0)
				{
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
					FHorrorGameItemData TempItem;
					TempItem.Clear();
					Inventory.Add(TempItem);
				}
			}
			//if (ADoor_cpp* Door = Cast<ADoor_cpp>(HitActor))
			//{
			//	// ADoor_cpp* Door = Cast<ADoor_cpp>(HitActor);
			//	if (Door->bIsDoorLocked)
			//	{
			//		Inventory[CurrentItemNum].ItemCount--;
			//		if (Inventory[CurrentItemNum].ItemCount == 0)
			//		{
			//			Inventory.RemoveAt(CurrentItemNum);
			//			InventoryNum--;
			//			CurrentItemNum--;
			//			if (CurrentItemNum < 0 && InventoryNum >= 0)
			//				CurrentItemNum = 0;
			//			FHorrorGameItemData TempItem;
			//			TempItem.Clear();
			//			Inventory.Add(TempItem);
			//		}
			//		USoundCue* ObjectSound = LoadObject<USoundCue>(nullptr, TEXT("/Game/Assets/Sounds/SoundCues/UnLock"));
			//		if (ObjectSound)
			//		{
			//			UGameplayStatics::PlaySoundAtLocation(this, ObjectSound, GetActorLocation());
			//		}
			//		Door->UseInteract();
			//	}
			//}

			//else if (AClassroomDoorActor_cpp* ClassroomDoor = Cast<AClassroomDoorActor_cpp>(HitActor))
			//{
			//	if (ClassroomDoor->bIsDoorLocked)
			//	{
			//		Inventory[CurrentItemNum].ItemCount--;
			//		if (Inventory[CurrentItemNum].ItemCount == 0)
			//		{
			//			Inventory.RemoveAt(CurrentItemNum);
			//			InventoryNum--;
			//			CurrentItemNum--;
			//			if (CurrentItemNum < 0 && InventoryNum >= 0)
			//				CurrentItemNum = 0;
			//			FHorrorGameItemData TempItem;
			//			TempItem.Clear();
			//			Inventory.Add(TempItem);
			//		}
			//		USoundCue* ObjectSound = LoadObject<USoundCue>(nullptr, TEXT("/Game/Assets/Sounds/SoundCues/UnLock"));
			//		if (ObjectSound)
			//		{
			//			UGameplayStatics::PlaySoundAtLocation(this, ObjectSound, GetActorLocation());
			//		}
			//		ClassroomDoor->UseInteract();
			//	}
			//}

			else if (ALockerDoorActor_cpp* LockerDoor = Cast<ALockerDoorActor_cpp>(HitActor))
			{
				if (LockerDoor->bIsLockerLocked)
				{
					//USoundCue* ObjectSound = LoadObject<USoundCue>(nullptr, TEXT("/Game/Assets/Sounds/SoundCues/UnLock"));
					if (KeySoundCue)
					{
						UGameplayStatics::PlaySoundAtLocation(this, KeySoundCue, GetActorLocation());
					}
					LockerDoor->UseInteract(this);
					
					if (bIsFinishUnlock)
					{
						Inventory[CurrentItemNum].ItemCount--;
						bIsFinishUnlock = false;
					}

					if (Inventory[CurrentItemNum].ItemCount == 0)
					{
						Inventory.RemoveAt(CurrentItemNum);
						InventoryNum--;
						CurrentItemNum--;
						if (CurrentItemNum < 0 && InventoryNum >= 0)
							CurrentItemNum = 0;
						FHorrorGameItemData TempItem;
						TempItem.Clear();
						Inventory.Add(TempItem);
					}
				}
			}

			/*else if (AMetalDoor_cpp* MetalDoor = Cast<AMetalDoor_cpp>(HitActor))
			{
				if (MetalDoor->bIsDoorLocked)
				{
					Inventory[CurrentItemNum].ItemCount--;
					if (Inventory[CurrentItemNum].ItemCount == 0)
					{
						Inventory.RemoveAt(CurrentItemNum);
						InventoryNum--;
						CurrentItemNum--;
						if (CurrentItemNum < 0 && InventoryNum >= 0)
							CurrentItemNum = 0;
						FHorrorGameItemData TempItem;
						TempItem.Clear();
						Inventory.Add(TempItem);
					}
					USoundCue* ObjectSound = LoadObject<USoundCue>(nullptr, TEXT("/Game/Assets/Sounds/SoundCues/UnLock"));
					if (ObjectSound)
					{
						UGameplayStatics::PlaySoundAtLocation(this, ObjectSound, GetActorLocation());
					}
					MetalDoor->UseInteract(this);
				}
			}*/

			CurrentItem();
		}
	}
}

void AHorrorGameCharacter::UseTimer()
{
	if (ProjectileClass)
	{
		FVector CameraLocatoin;
		FRotator CameraRotation;
		GetActorEyesViewPoint(CameraLocatoin, CameraRotation);

		FVector MuzzleLocation = CameraLocatoin + FTransform(CameraRotation).TransformVector(MuzzleOffset);
		FRotator MuzzleRotation = CameraRotation;

		MuzzleRotation.Pitch += 10.f;
		UWorld* World = GetWorld();
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
	
	Inventory[CurrentItemNum].ItemCount--;
	if (Inventory[CurrentItemNum].ItemCount == 0)
	{
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

void AHorrorGameCharacter::UseSword()
{
	/*AActor* HitActor = nullptr;
	bool isHit = GetLineTraceSingle(HitActor);
	int32 count = 0;*/

	// UE_LOG(LogTemp, Warning, TEXT("Use Sword: %s"), (isHit) ? "true" : "false");

	//if (isHit)
	//{
	//	if (HitActor)
	//	{
	//		FString ActorName = HitActor->GetName();
	//		UE_LOG(LogTemp, Warning, TEXT("ACtorName = %s"), *ActorName);

	//		//if (ActorName.Contains(TEXT("Reaper_cpp"), ESearchCase::CaseSensitive, ESearchDir::FromEnd))
	//		if(AReaper_cpp* Reaper = Cast<AReaper_cpp>(HitActor))
	//		{
	//			// auto Reaper = Cast<AReaper_cpp>(HitActor);
	//			if (nullptr == Reaper) return;

	//			if (!Reaper->GetIsDied())
	//			{
	//				GameUIWidget->SetObjectCount(1, Inventory[CurrentItemNum].ItemCount < 0 ? 0 : Inventory[CurrentItemNum].ItemCount);
	//				Inventory[CurrentItemNum].ItemCount--;
	//				if (Inventory[CurrentItemNum].ItemCount == 0)
	//				{
	//					Inventory.RemoveAt(CurrentItemNum);
	//					InventoryNum--;
	//					CurrentItemNum--;
	//					if (CurrentItemNum < 0 && InventoryNum >= 0)
	//						CurrentItemNum = 0;
	//					FHorrorGameItemData TempItem;
	//					TempItem.Clear();
	//					Inventory.Add(TempItem);
	//				}
	//				CurrentItem();
	//				Reaper->Exorcism();
	//			}
	//		}
	//	}
	//}
	/*SetNotifyAttackStart(true);
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Green, FString::Printf(TEXT("Current Notify Attack Start: %s"), bNotifyAttackStart ? TEXT("true") : TEXT("false")));
	*/
	
	if (!bIsHiding)
	{
		auto AnimInstance = Cast<UPlayerAnim>(GetMesh()->GetAnimInstance());
		if (nullptr == AnimInstance) return;

		AnimInstance->PlayAttackMontage();
		AActor* ChildActor = Sword->GetChildActor();
		if (APlayerSword_cpp* BronzeSword = Cast<APlayerSword_cpp>(ChildActor))
		{
			BronzeSword->UseInteract(this);
		}
	}
}

void AHorrorGameCharacter::UseBell()
{
	/*if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, FString::Printf(TEXT("Bell Use")));
	}*/
	BellSound->Stop();
	BellSound->Play();
	bIsBellSoundOn = true;
	/*Patience -= 40;
	if (Patience < 0)
		Patience = 0;*/
	AddPatience(-100);

	bIsCooldown = true;

	BellCount--;
	Inventory[CurrentItemNum].ItemCount = BellCount;
	if (Inventory[CurrentItemNum].ItemCount == 0)
	{
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
		FHorrorGameItemData TempItem;
		TempItem.Clear();
		Inventory.Add(TempItem);
	}
	// GameUIWidget->SetPatience(Patience);
	GameUIWidget->SetObjectCount(2, BellCount);
	CurrentItem();
}

void AHorrorGameCharacter::BreatheSoundFinish()
{
	bisSoundOn = false;
}

void AHorrorGameCharacter::BellSoundFinish()
{
	bIsBellSoundOn = false;
	bIsPatienceReduce = true;
	PatienceToReduce = 0;
}

void AHorrorGameCharacter::UseMirror() // 타임 스톱의 개념(사실은 거울 세계로 잠시 들어가 탈출)
{
	bIsTimeStop = true;
	bReaperWatchPlayer = false;

	AActor* ChildActor = Mirror->GetChildActor();
	if (AMirror_cpp* BronzeMirror = Cast<AMirror_cpp>(ChildActor))
	{
		BronzeMirror->UseInteract(this);
		Cast<AHorrorGameGameMode>(UGameplayStatics::GetGameMode(GetWorld()))->PlayMirrorUseBackGroundMusic();
	}
	
	// bIsTimeStopChange = true;
	Inventory[CurrentItemNum].ItemCount = MirrorCount;
	// RadiusTime.Play();

	if (Inventory[CurrentItemNum].ItemCount == 0)
	{
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
	
		FHorrorGameItemData TempItem;
		TempItem.Clear();	
		Inventory.Add(TempItem);
	}

	FirstPersonCameraComponent->AddOrUpdateBlendable(PostProcessMaterial, 1.f);
	

	GameUIWidget->SetObjectCount(3, MirrorCount);
	CurrentItem();
}

void AHorrorGameCharacter::UseExtinguisher()
{
	if (FirstPersonCameraComponent == nullptr)
		return;

	if (bIsHiding)
	{
		return;
	}

	SmokeComponent->Activate(true);
	bCanExtinguisherUse = false;
	//USoundCue* ExtinguisherSound = LoadObject<USoundCue>(nullptr, TEXT("/Game/Assets/Sounds/SoundCues/SpraySoundCue"));
	if (ExtinguisherCue)
	{
		UGameplayStatics::PlaySoundAtLocation(this, ExtinguisherCue, GetActorLocation());
	}
	FTimerHandle SmokeTimer;

	TArray<FOverlapResult>OverlapResults;
	bool bResult = GetWorld()->OverlapMultiByChannel(
		OverlapResults,
		GetActorLocation() + GetActorForwardVector() * 300.f,
		FRotationMatrix::MakeFromZ(GetActorForwardVector() * 600.f).ToQuat(),
		ECollisionChannel::ECC_EngineTraceChannel4,
		FCollisionShape::MakeCapsule(200.f, 600.f)
	);

	if (bResult)
	{
		for (auto OverlapResult : OverlapResults)
		{
			FHitResult OutHit;
			FVector Start = GetActorLocation();
			FVector End = OverlapResult.GetActor()->GetActorLocation();

			if (!GetWorld()->LineTraceSingleByChannel(OutHit, Start, End, ECC_Visibility))
			{
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
	

	GetWorld()->GetTimerManager().SetTimer(SmokeTimer, FTimerDelegate::CreateLambda([&]() {
		bCanExtinguisherUse = true;
		SmokeComponent->Deactivate();
		//CapsuleComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		ExtinguisherLeft -= 20;
		if (GameUIWidget)
		{
			GameUIWidget->SetExtHUD(ExtinguisherLeft);
		}

		if (ExtinguisherLeft <= 0)
		{
			Inventory[CurrentItemNum].ItemCount--;
			if (Inventory[CurrentItemNum].ItemCount == 0)
			{
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
			if (GameUIWidget)
			{
				GameUIWidget->SetExtWidget(false);
			}
		}
		CurrentItem();

		GetWorld()->GetTimerManager().ClearTimer(SmokeTimer);
	}), 1.f, false);

	if (ExtinguisherLeft <= 0)
	{
		Inventory[CurrentItemNum].ItemCount--;
		if (Inventory[CurrentItemNum].ItemCount == 0)
		{
			Inventory.RemoveAt(CurrentItemNum);
			InventoryNum--;
			CurrentItemNum--;
			if (CurrentItemNum < 0 && InventoryNum >= 0)
			{
				CurrentItemNum = 0;
			}

			if (InventoryNum < 0)
			{
				CurrentItemNum = -1;
			}
		}
		FHorrorGameItemData TempItem;
		TempItem.Clear();
		Inventory.Add(TempItem); 
		if (GameUIWidget)
		{
			GameUIWidget->SetExtWidget(false);
		}
	}
	CurrentItem();
}

void AHorrorGameCharacter::UseCutter()
{
	AActor* HitActor = nullptr;
	bool isHit = GetLineTraceSingle(HitActor);

	if (isHit)
	{
		if (HitActor)
		{
			//	FString ActorName = HitActor->GetName();

			if (ALockerDoorActor_cpp* LockerDoor = Cast<ALockerDoorActor_cpp>(HitActor))
			{
				// ALockerDoorActor_cpp* LockerDoor = Cast<ALockerDoorActor_cpp>(HitActor);
				if (LockerDoor->bIsLockerLocked)
				{
					//	USoundCue* CutSound = LoadObject<USoundCue>(nullptr, TEXT("/Game/Assets/Sounds/SoundCues/BreakLock"));
					if (CutterSoundCue)
					{
						UGameplayStatics::PlaySoundAtLocation(this, CutterSoundCue, GetActorLocation());
					}
					LockerDoor->UseInteract(this);

					if (bIsFinishUnlock)
					{
						CutterDurability -= 1;
						bIsFinishUnlock = false;
					}
				}
			}
			else if (AClassroomDoorActor_cpp* ClassroomDoor = Cast<AClassroomDoorActor_cpp>(HitActor))
			{
				// AClassroomDoorActor_cpp* ClassroomDoor = Cast<AClassroomDoorActor_cpp>(HitActor);
				if (ClassroomDoor->bIsDoorLocked)
				{
					ClassroomDoor->UseInteract(this);
					//	USoundCue* CutSound = LoadObject<USoundCue>(nullptr, TEXT("/Game/Assets/Sounds/SoundCues/BreakLock"));
					if (CutterSoundCue)
					{
						UGameplayStatics::PlaySoundAtLocation(this, CutterSoundCue, GetActorLocation());
					}

					if (bIsFinishUnlock)
					{
						CutterDurability -= 1;
						bIsFinishUnlock = false;
					}
				}
			}

			else if (AMetalDoor_cpp* MetalDoor = Cast<AMetalDoor_cpp>(HitActor))
			{
				if (MetalDoor->bIsDoorLocked)
				{
					//	USoundCue* CutSound = LoadObject<USoundCue>(nullptr, TEXT("/Game/Assets/Sounds/SoundCues/BreakLock"));
					if (CutterSoundCue)
					{
						UGameplayStatics::PlaySoundAtLocation(this, CutterSoundCue, GetActorLocation());
					}
					MetalDoor->UseInteract(this);

					if (bIsFinishUnlock)
					{
						CutterDurability -= 1;
						bIsFinishUnlock = false;
					}
				}
			}
		}
	}
	if (CutterDurability <= 0)
	{
		Inventory[CurrentItemNum].ItemCount--;
		if (Inventory[CurrentItemNum].ItemCount == 0)
		{
			if (GameUIWidget)
			{
				GameUIWidget->SetCutterWidget(false);
			}
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
	}
	CurrentItem();
	if (GameUIWidget)
	{
		GameUIWidget->SetCutterHUD(CutterDurability);
	}
}

void AHorrorGameCharacter::UseLantern()
{
//	bLanternOn = !bLanternOn;
	if (Lantern)
	{
		if (ASoul_Lantern_cpp* SoulLantern = Cast<ASoul_Lantern_cpp>(Lantern->GetChildActor()))
		{
			SoulLantern->UseInteract(this);
		}
	}
}

void AHorrorGameCharacter::UseGlowStick()
{
	if (bIsHiding)
	{
		return;
	}

	if (GlowStickClass)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.Instigator = GetInstigator();
		float Z = 110.f; // 122 - 110 = 12
		
		if (bIsCrouch)
		{
			Z = 45.f; // 56 - 45 = 11
		}

		FVector SpawnLocation = GetActorLocation() - FVector(0.f, 0.f, Z);
			
		AGlowStick_cpp* GlowStick = GetWorld()->SpawnActor<AGlowStick_cpp>(GlowStickClass, SpawnLocation, FRotator(0.f, 0.f, 0.f), SpawnParams);
		if (GlowStick)
		{
			GlowStick->UseInteract(this);
		}
		//FVector LineTraceStart = GetActorLocation();
		//FVector LineTraceEnd = GetActorLocation() - FVector::UpVector * 150.0f;

		//FHitResult HitResult;
		//TArray<AActor*> Ignore;
		//Ignore.Add(GetMesh()->GetOwner());

		//bool bDebug = true;

		//EDrawDebugTrace::Type eDebug = EDrawDebugTrace::None;
		//if (bDebug) eDebug = EDrawDebugTrace::ForDuration;
		//TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes; // 히트 가능한 오브젝트 유형들.
		//TEnumAsByte<EObjectTypeQuery> WorldStatic = UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldStatic);
		//TEnumAsByte<EObjectTypeQuery> WorldDynamic = UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldDynamic);
		//ObjectTypes.Add(WorldStatic);
		//ObjectTypes.Add(WorldDynamic);

		//bool bResult = UKismetSystemLibrary::LineTraceSingleForObjects(GetMesh()->GetWorld(), LineTraceStart, LineTraceEnd, ObjectTypes, true, Ignore, eDebug, HitResult, true);

		//if (bResult)
		//{
		//	if (HitResult.GetActor() == nullptr)
		//	{
		//		return;
		//	}

		//	UE_LOG(LogTemp, Warning, TEXT("%s"), *HitResult.GetActor()->GetName());
		//	FActorSpawnParameters SpawnParams;
		//	SpawnParams.Owner = this;
		//	SpawnParams.Instigator = GetInstigator();
		//	FVector SpawnLocation = HitResult.GetActor()->GetActorLocation() + FVector(0.f, 0.f, 10.f);
		//	
		//	AGlowStick_cpp* GlowStick = GetWorld()->SpawnActor<AGlowStick_cpp>(GlowStickClass, SpawnLocation, FRotator(0.f, 0.f, 0.f), SpawnParams);
		//	if (GlowStick)
		//	{
		//		GlowStick->UseInteract(this);
		//	}
		//}
	}

	Inventory[CurrentItemNum].ItemCount--;
	if (Inventory[CurrentItemNum].ItemCount == 0)
	{
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


void AHorrorGameCharacter::CurrentItem()
{
	int32 currentItemNumber = GetCurrentItemNumber();
	//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Yellow, FString::Printf(TEXT("Current Item Number: %d"), currentItemNumber));

	//if (currentItemName == TEXT("FlashLight"))
	if (currentItemNumber == 2) // FlashLight
	{
		if (GameUIWidget)
		{
			GameUIWidget->SetBatteryWidget(true);
		}
	}
	else
	{
		if (GameUIWidget)
		{
			GameUIWidget->SetBatteryWidget(false);
		}
	}

	//if (currentItemName == TEXT("Extinguisher"))
	if (currentItemNumber == 8) // Extinguisher
	{
		if (GameUIWidget)
		{
			GameUIWidget->SetExtWidget(true);
		}
	}
	else
	{
		if (GameUIWidget)
		{
			GameUIWidget->SetExtWidget(false);
		}
	}

	//if (currentItemName == TEXT("Cutter"))
	if (currentItemNumber == 9) // Cutter
	{
		if (GameUIWidget)
		{
			GameUIWidget->SetCutterWidget(true);
		}
	}
	else
	{
		if (GameUIWidget)
		{
			GameUIWidget->SetCutterWidget(false);
		}
	}

	//if (currentItemName == TEXT("SoulLantern"))
	if (currentItemNumber == 10) // SoulLantern
	{
		Lantern->SetHiddenInGame(false);
	}
	else
	{
		AActor* ChildActor = Lantern->GetChildActor();
		if (ASoul_Lantern_cpp* SoulLantern = Cast<ASoul_Lantern_cpp>(ChildActor))
		{
			if (SoulLantern->bIsLightOn)
				SoulLantern->UseInteract(this);
		}
		Lantern->SetHiddenInGame(true);
	}

	//if (currentItemName == TEXT("Mirror"))
	if (currentItemNumber == 7) // Mirror
	{
		Mirror->SetHiddenInGame(false);
	}
	else
	{
		Mirror->SetHiddenInGame(true);
	}

	//if (currentItemName == TEXT("Sword"))
	if (currentItemNumber == 5) // Sword
	{
		Sword->SetHiddenInGame(false);
	}
	else
	{
		Sword->SetHiddenInGame(true);
	}

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
void AHorrorGameCharacter::SetPlayerStatus(Player_Status Value)
{
	if (PlayerStatus != Value)
	{
		PlayerStatus = Value;

		switch (PlayerStatus)
		{
			case Player_Status::Loading: // 로딩 중
			{
				DisableInput(HorrorGamePlayerController);
			
				break;
			}
			case Player_Status::Survive: // 시작
			{
				EnableInput(HorrorGamePlayerController);

				break;
			}
			case Player_Status::Chased: // 추격 시
			{
				SetCameraComponentNoise(2);
				if (!bIsCooldown)
				{
					AddPatience(15);
				}
				break;
			}
			case Player_Status::Catched: // 잡혔을 경우
			{
				SetActorEnableCollision(false);
				DisableInput(HorrorGamePlayerController);
				break;
			}
			case Player_Status::Stunned: // 리퍼에 의해 기절 시
			{
				DisableInput(HorrorGamePlayerController);
				GetCharacterMovement()->StopMovementImmediately();
				break;
			}
			// 이 케이스는 숨은 상태를 나타내는 케이스가 아닌, Hide Object속에 배치된 캐릭터에 대한 케이스임.
			case Player_Status::Hiding: // 헷갈리면 안 됨. 숨은 상태는 bIsHiding이 나타냄.
			{
				
				break;
			}
			case Player_Status::Died: // 사망
			{
				SetActorEnableCollision(false);
				GetMesh()->SetHiddenInGame(true);
				DisableInput(HorrorGamePlayerController);
				if (GameUIWidget)
				{
					GameUIWidget->SetTimerStop(true);
				}
				HorrorGamePlayerController->ShowDeadUI();
				break;
			}
			case Player_Status::Clear: // 클리어 및 엔딩
			{
				SetActorEnableCollision(false);
				GetMesh()->SetHiddenInGame(true);
				DisableInput(HorrorGamePlayerController);
				if (GameUIWidget)
				{
					GameUIWidget->SetTimerStop(true);
				}
				//HorrorGamePlayerController->ShowDeadUI();
				break;
			}
		}
	}
}

void AHorrorGameCharacter::SetExplainText(FText text, int32 time)
{
	if (GameUIWidget)
	{
		GameUIWidget->SetInteractDotExplainText(text);
	}
	TextTimer = time;
	cnt = 0.f;
}

void AHorrorGameCharacter::SetErrorText(FText text, int32 time)
{
	if (GameUIWidget)
	{
		GameUIWidget->SetInteractDotErrorText(text);
	}
	ErrorTextTimer = time;
	ErrorTextCount = 0.f;
}

void AHorrorGameCharacter::AddPatience(int32 value)
{
	Patience += value;
	if (Patience <= 0)
		Patience = 0;

	if (Patience >= 100)
		Patience = 100;
	bIsCooldown = true; 
	if (GameUIWidget)
	{
		GameUIWidget->SetPatience(Patience);
	}
}

void AHorrorGameCharacter::LightFlicker(float value)
{
	if (bIsCigarLightOn)
	{
		float LightIntense = CigarIntensity * value;

		if (LightIntense < 0.f)
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
		}
	}

	if (bIsFlashLightOn)
	{
		float LightIntense = FlashIntensity * value;

		if (LightIntense < 0.f)
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
		}
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
		if (BronzeSword->bHadExorcism) // 크리쳐를 처치한 상황인 경우에, 칼 갯수 감소
		{
			BronzeSword->SetShouldExorcism(false);
			if (SwordCount > 0) // 칼 개수가 0보다 큰 경우에만 감소하도록 설정
			{
				SwordCount--;
				if (CurrentItemNum >= 0 && CurrentItemNum <= InventoryNum) // 또한 현재 아이템 인덱스가 인벤토리 배열의 범위 내에 있는 경우에만 
				{
					Inventory[CurrentItemNum].ItemCount = SwordCount;
				}
			}
			// RadiusTime.Play();

			if (Inventory[CurrentItemNum].ItemCount == 0)
			{
				Inventory.RemoveAt(CurrentItemNum);
				InventoryNum--;
				CurrentItemNum--;
				if (CurrentItemNum < 0 && InventoryNum >= 0) // 인벤토리에 아이템이 있는데, 현재 인덱스가 마이너스일 경우엔 0으로
				{
					CurrentItemNum = 0;
				}

				if (InventoryNum < 0)
				{
					CurrentItemNum = 0;
				}
				FHorrorGameItemData TempItem;
				TempItem.Clear();
				//GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Yellow, FString::Printf(TEXT("Inventory Array Num: %d"), Inventory.Num()));
				Inventory.Add(TempItem);
				//GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Yellow, FString::Printf(TEXT("Inventory Array Num: %d"), Inventory.Num()));
			}

			if (GameUIWidget)
			{
				GameUIWidget->SetObjectCount(1, SwordCount);
			}
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
		GetCharacterMovement()->MaxWalkSpeed = 180.0f * InWaterSpeedDown;
	else
		GetCharacterMovement()->MaxWalkSpeed = 180.0f;
}

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

void AHorrorGameCharacter::SetPanicScreamEnd()
{
	bIsScreaming = false;
	PatienceToReduce = 0;
	bIsPatienceReduce = true;
	EnableInput(HorrorGamePlayerController);
}

void AHorrorGameCharacter::LevelStart()
{
	if (PlayerStatus == Player_Status::Loading)
	{
		PlayerStatus = Player_Status::Survive;
		//HorrorGamePlayerController->ShowMainUI();
		if (GameUIWidget)
		{
			GameUIWidget->Player = this;
			GameUIWidget->AllWidgetInit();
		}
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
	/*FString text;
	if (inReaperWatchPlayer)
	{
		text = TEXT("true");
	}
	else
	{
		text = TEXT("false");
	}
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, FString::Printf(TEXT("ReaperWatchPlayer: %s"), *text));*/
	if (bIsTimeStop) // 시간이 정지한 상태라면(즉 거울을 사용한 상태면), 리퍼 능력 무효화
	{
		bReaperWatchPlayer = false;
	}
	else
	{
		if (bReaperWatchPlayer != inReaperWatchPlayer) // 머티리얼을 한 번만 넣기 위함으로 두 개를 비교해서 달라야 추가, 제거 가능
		{
			if (inReaperWatchPlayer)
			{
				FirstPersonCameraComponent->AddOrUpdateBlendable(PostProcessDynamicInstance, 1.f);
				if (ReaperSoundCue)
				{
					UGameplayStatics::PlaySound2D(this, ReaperSoundCue);
				}
			}
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

void AHorrorGameCharacter::SetArchiveGetText(FText inText)
{
	//GetWorld()->GetTimerManager().ClearTimer(ArchiveTextTimer);
	
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

		/*GetWorld()->GetTimerManager().SetTimer(ArchiveTextTimer, FTimerDelegate::CreateLambda([&]() {
			GameUIWidget->SetArchiveGetText(FText::FromString(TEXT("")));
			GetWorld()->GetTimerManager().ClearTimer(ArchiveTextTimer);
		}), 10.f, false);*/
	}
	bIsArchiveTextOn = true;
	ArchiveTextTimer = 0.f;
}

void AHorrorGameCharacter::OnFocus(FVector TargetLocation)
{
	FirstPersonCameraComponent->bUsePawnControlRotation = false;
	
	GetCharacterMovement()->StopMovementImmediately();
	DisableInput(HorrorGamePlayerController);

	FVector CameraLocation = FirstPersonCameraComponent->GetComponentLocation();
	LookAtRotation = UKismetMathLibrary::FindLookAtRotation(CameraLocation, TargetLocation);
	RotateCameraTimeline.PlayFromStart();
}

void AHorrorGameCharacter::OnFocusRotation(float inLerpAlpha)
{
	FirstPersonCameraComponent->bUsePawnControlRotation = false;
	FRotator CameraRotation = FirstPersonCameraComponent->GetComponentRotation();
	FRotator NewRotation = FMath::Lerp(CameraRotation, LookAtRotation, inLerpAlpha);

	FirstPersonCameraComponent->SetWorldRotation(NewRotation);
}

void AHorrorGameCharacter::OnFocusFinished()
{
	EnableInput(HorrorGamePlayerController);
	FirstPersonCameraComponent->bUsePawnControlRotation = true;
}

void AHorrorGameCharacter::OnSprintCameraView(float inLerpAlpha)
{
	float FieldOfView = FMath::Lerp(90.f, 100.f, inLerpAlpha);

	FirstPersonCameraComponent->SetFieldOfView(FieldOfView);
}

void AHorrorGameCharacter::OnAnnounce()
{
	SetExplainText(NSLOCTEXT("AHorrorGameCharacter", "Reaper_Unseal", "Some ominous sound comes..."), 3);
	// 번역은 "뭔가 불길한 소리가 들려온다."로
}