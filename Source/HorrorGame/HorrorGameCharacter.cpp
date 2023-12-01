// CopyrightNotice=0 2023 Sunggon Kim kimdave205@gmail.com

#include "HorrorGameCharacter.h"
#include "HorrorGameProjectile.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/BoxComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "GameFramework/CharacterMovementComponent.h"
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
#include "Items/PlayerSword_cpp.h"
#include "Furniture/DistributionBox.h"
#include "EngineUtils.h"
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "MetalDoor_cpp.h"
#include "DeskDrawer_cpp.h"
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
	static ConstructorHelpers::FObjectFinder<USoundBase> TurnonSound(TEXT("/Game/Assets/Sounds/Turnon"));
	if (TurnonSound.Succeeded())
	{
		TurnOnSoundCue = TurnonSound.Object;
	}

	PanicSound = CreateDefaultSubobject<UAudioComponent>(TEXT("PanicSound"));
	PanicSound->SetupAttachment(FirstPersonCameraComponent);

	HeartBeat = CreateDefaultSubobject<UAudioComponent>(TEXT("HeartBeatSound"));
	HeartBeat->SetupAttachment(FirstPersonCameraComponent);
	HeartBeat->SetAutoActivate(false);

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
	static ConstructorHelpers::FObjectFinder<USoundBase> BellSoundWav(TEXT("/Game/Assets/Sounds/SoundCues/BellSoundCue"));
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

	/*if (GEngine)
	{
		FString checktext = TEXT("");
		if (PostProcessMaterialInstance != nullptr)
			checktext = TEXT("is not null");
		else
			checktext = TEXT("is null");
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString::Printf(TEXT("PostProcessMaterialInstance: %s"), *checktext));
	}*/
	
	//if (PostProcessMaterial)
	//{
	//	if (GEngine)
	//		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Green, FString::Printf(TEXT("PostProcessMaterial is here")));
	//	// PostProcessMaterialInstance = UMaterialInstanceDynamic::Create(PostProcessMaterial, this);
	//}

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


	PlayerStatus = Player_Status::Survive;
	GameUIWidget = HorrorGamePlayerController->GetMainWidget();
	
	
	LevelStart();

	if (CurveFloat)
	{
		FOnTimelineFloat TimelineProgress;
		TimelineProgress.BindDynamic(this, &AHorrorGameCharacter::LightFlicker);
		FlickeringLight.AddInterpFloat(CurveFloat, TimelineProgress);
		FlickeringLight.SetLooping(true);
	}

	if (PanicSoundCue)
	{
		PanicSound->SetAutoActivate(false);
		PanicSound->SetSound(PanicSoundCue);
		PanicSound->OnAudioFinished.AddDynamic(this, &AHorrorGameCharacter::SetPanicScreamEnd);
	}
}

void AHorrorGameCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AActor* HitActor = nullptr;
	FString LevelName = UGameplayStatics::GetCurrentLevelName(GetWorld());

	GetLineTraceSingle(HitActor);

	FlickeringLight.TickTimeline(DeltaTime);

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
			SetExplainText(TEXT(""), 0);
		}
	}

	if (ErrorTextTimer > 0)
	{
		ErrorTextCount += DeltaTime;

		if (ErrorTextCount >= ErrorTextTimer)
		{
			SetErrorText(TEXT(""), 0);
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

	if (Patience == 100 && !bIsScreaming) // 패닉 게이지가 100까지 도달했을 경우엔 어그로를 끌게 됨
	{
		bIsScreaming = true;
		PanicSound->Play();
	} // 패닉이 100일 때 계속 소리를 내는 것을 방지하기 위해, Screaming하는 지 체크하여 하지 않을 경우에만 발동하도록 함
	
	if (bIsCrouch)
	{
		if (Stamina <= 399)
		{
			Stamina += 1;
			GameUIWidget->SetStaminaHUD(Stamina);
		}
	}
	else
	{
		if (bIsSprint)
		{
			if (Stamina > 0)
			{
				Stamina -= 2;
				GameUIWidget->SetStaminaHUD(Stamina);
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
				GameUIWidget->SetStaminaHUD(Stamina);
			}
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
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

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
		AddMovementInput(GetActorForwardVector(), MovementVector.Y);
		AddMovementInput(GetActorRightVector(), MovementVector.X);
	}
}

void AHorrorGameCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();
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
						Wardrobe->Pitch += LookAxisVector.Y * -1.f;
						Wardrobe->Yaw += LookAxisVector.X;
						Wardrobe->MoveCamera(); // Call Cabinet_cpp Hide Function
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

	if (SprintSound->GetPlayState() == EAudioComponentPlayState::Playing)
	{
		SprintSound->Stop();
	}
	// if (!bisSoundOn && bIsSprint)
		// bisSoundOn = true;
}


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
			}

			else if (AWardrobeDrawer_cpp* WardDrawer = Cast<AWardrobeDrawer_cpp>(HitActor))
			{
				//AWardrobeDrawer_cpp* WardDrawer = Cast<AWardrobeDrawer_cpp>(HitActor);
				
				WardDrawer->OnInteract();
			}

			//else if (ActorName.Contains(TEXT("Drawer_cpp"), ESearchCase::CaseSensitive, ESearchDir::FromEnd))
			else if (ADrawer_cpp* Drawer = Cast<ADrawer_cpp>(HitActor))
			{
				/*ADrawer_cpp* Drawer = Cast<ADrawer_cpp>(HitActor);*/
				Drawer->OnInteract();
			}

			//else if (ActorName.Contains(TEXT("Door_cpp"), ESearchCase::CaseSensitive, ESearchDir::FromEnd))
			//else if(ADoor_cpp* Door = Cast<ADoor_cpp>(HitActor))
			//{
			//	/*ADoor_cpp* Door = Cast<ADoor_cpp>(HitActor);*/
			//	if (Door->bIsDoorLocked)
			//	{
			//		ErrorInteractText = TEXT("Locked");
			//		SetErrorText(ErrorInteractText, 3);
			//		/*GameUIWidget->SetInteractDotErrorText(ErrorInteractText);
			//		ErrorTextTimer = 1;
			//		ErrorTextCount = 0;
			//		GetWorld()->GetTimerManager().SetTimer(_TextTimerHandle, FTimerDelegate::CreateLambda([&]() {
			//			ErrorInteractText = TEXT("");
			//			GameUIWidget->SetInteractDotErrorText(ErrorInteractText);
			//			GetWorld()->GetTimerManager().ClearTimer(_TextTimerHandle);
			//		}), 1.0f, false);*/
			//	}
			//	Door->OnInteract();
			//}

			//else if (ActorName.Contains(TEXT("ClassroomDoorActor_cpp"), ESearchCase::CaseSensitive, ESearchDir::FromEnd))
			//else if(AClassroomDoorActor_cpp* ClassroomDoor = Cast<AClassroomDoorActor_cpp>(HitActor))
			//{
			//	/*AClassroomDoorActor_cpp* LockedDoor = Cast<AClassroomDoorActor_cpp>(HitActor);*/
			//	if (ClassroomDoor->bIsDoorLocked)
			//	{
			//		ErrorInteractText = TEXT("Locked");
			//		SetErrorText(ErrorInteractText, 3);
			//		/*GameUIWidget->SetInteractDotErrorText(ErrorInteractText);
			//		ErrorTextTimer = 1;
			//		ErrorTextCount = 0;
			//		GetWorld()->GetTimerManager().SetTimer(_TextTimerHandle, FTimerDelegate::CreateLambda([&]() {
			//			ErrorInteractText = TEXT("");
			//			GameUIWidget->SetInteractDotErrorText(ErrorInteractText);
			//			GetWorld()->GetTimerManager().ClearTimer(_TextTimerHandle);
			//		}), 1.0f, false);*/
			//	}
			//	ClassroomDoor->OnInteract();
			//}

			//else if (ActorName.Contains(TEXT("LockerDoorActor_cpp"), ESearchCase::CaseSensitive, ESearchDir::FromEnd))
			else if(ALockerDoorActor_cpp* Locker = Cast<ALockerDoorActor_cpp>(HitActor))
			{
				//ALockerDoorActor_cpp* Locker = Cast<ALockerDoorActor_cpp>(HitActor);
				//if (Locker->bIsLockerLocked)
				//{
				//	ErrorInteractText = TEXT("Locked");
				//	SetErrorText(ErrorInteractText, 3);
				//	/*GameUIWidget->SetInteractDotErrorText(ErrorInteractText);
				//	GetWorld()->GetTimerManager().SetTimer(_TextTimerHandle, FTimerDelegate::CreateLambda([&]() {
				//		ErrorInteractText = TEXT("");
				//		GameUIWidget->SetInteractDotErrorText(ErrorInteractText);
				//		GetWorld()->GetTimerManager().ClearTimer(_TextTimerHandle);
				//	}), 1.0f, false);*/
				//}
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
				Altar->OnInteract(this);
			}

			else if (ASwitchLever* Lever = Cast<ASwitchLever>(HitActor))
			{
				Lever->OnInteract(this);
			}

			else if (ADistributionBox* DBox = Cast<ADistributionBox>(HitActor))
			{
				DBox->OnInteract(this);
			}
		}
	}
	
}

void AHorrorGameCharacter::ItemUse()
{
	if (CurrentItemNum >= 0 && 0 <= InventoryNum) // Avoid Index Error
	{
		FHorrorGameItemData& CurrentItem = Inventory[CurrentItemNum]; // Get Selected Item

		if (CurrentItem.ItemName.Contains(TEXT("CigarLight")))
		{
			UseCigarLight();
		}

		else if (CurrentItem.ItemName.Contains(TEXT("FlashLight")))
		{
			UseFlashLight();
		}

		else if (CurrentItem.ItemName.Contains(TEXT("Key")))
		{
			UseKey();
		}

		else if (CurrentItem.ItemName.Contains(TEXT("Timer")))
		{
			UseTimer();
		}

		else if (CurrentItem.ItemName.Contains(TEXT("Sword")))
		{
			UseSword();
		}

		else if (CurrentItem.ItemName.Contains(TEXT("Bell")))
		{
			UseBell();
		}

		else if (CurrentItem.ItemName.Contains(TEXT("Mirror")))
		{
			UseMirror();
		}

		else if (CurrentItem.ItemName.Contains(TEXT("Extinguisher")))
		{
			if(bCanExtinguisherUse)
				UseExtinguisher();
		}

		else if (CurrentItem.ItemName.Contains(TEXT("Cutter")))
		{
			UseCutter();
		}

		else if (CurrentItem.ItemName.Contains(TEXT("Lantern")))
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
	FVector End = (ForwardVector * 200.0f) + Start;


	bool isHit = GetWorld()->LineTraceSingleByChannel(OutHit, Start, End, ECC_Visibility);
	HitActor = OutHit.GetActor();
	if (isHit)
	{
		if (OutHit.GetActor())
		{
			//FString ActorName = HitActor->GetName();
			//if (ActorName.Contains(TEXT("CigarLighter_cpp"), ESearchCase::CaseSensitive, ESearchDir::FromEnd))
			if (HitActor->IsA<AItems>())
			{
				GameUIWidget->SetInteractDotText(TEXT("Take"));
				GameUIWidget->SetInteractDot(true);
				return true;
			}
			// 이걸로도 됨. 경우 나눌 필요 없이 이걸로 하면 될 듯

			/*if(HitActor->IsA<ACigarLighter_cpp>())
			{
				GameUIWidget->SetInteractDotText(TEXT("Take"));
				GameUIWidget->SetInteractDot(true);
				return true;
			}

			//else if (ActorName.Contains(TEXT("FlashLight_cpp"), ESearchCase::CaseSensitive, ESearchDir::FromEnd))
			//else if(HitActor->IsA<AFlashLight_cpp>())
			//{
			//	GameUIWidget->SetInteractDotText(TEXT("Take"));
			//	GameUIWidget->SetInteractDot(true);
			//	return true;
			//}

			////else if (ActorName.Contains(TEXT("Key_cpp"), ESearchCase::CaseSensitive, ESearchDir::FromEnd))
			//else if(HitActor->IsA<AKey_cpp>())
			//{
			//	GameUIWidget->SetInteractDotText(TEXT("Take"));
			//	GameUIWidget->SetInteractDot(true);
			//	return true;
			//}

			////else if (ActorName.Contains(TEXT("Timer_cpp"), ESearchCase::CaseSensitive, ESearchDir::FromEnd))
			//else if(HitActor->IsA<ATimer_cpp>())
			//{
			//	GameUIWidget->SetInteractDotText(TEXT("Take"));
			//	GameUIWidget->SetInteractDot(true);
			//	return true;
			//}

			////else if (ActorName.Contains(TEXT("Extinguisher_cpp"), ESearchCase::CaseSensitive, ESearchDir::FromEnd))
			//else if(HitActor->IsA<AExtinguisher_cpp>())
			//{
			//	GameUIWidget->SetInteractDotText(TEXT("Take"));
			//	GameUIWidget->SetInteractDot(true);
			//	return true;
			//}

			////else if (ActorName.Contains(TEXT("Cutter"), ESearchCase::CaseSensitive, ESearchDir::FromEnd))
			//else if(HitActor->IsA<ACutter_cpp>())
			//{
			//	GameUIWidget->SetInteractDotText(TEXT("Take"));
			//	GameUIWidget->SetInteractDot(true);
			//	return true;
			//}

			////else if (ActorName.Contains(TEXT("Sword_cpp"), ESearchCase::CaseSensitive, ESearchDir::FromEnd))
			//else if (HitActor->IsA<ASword_cpp>())
			//{
			//	GameUIWidget->SetInteractDotText(TEXT("Take"));
			//	GameUIWidget->SetInteractDot(true);
			//	return true;
			//}

			else if (HitActor->IsA<AMirror_cpp>())
			{
				GameUIWidget->SetInteractDotText(TEXT("Take"));
				GameUIWidget->SetInteractDot(true);
				return true;
			}

			else if (HitActor->IsA<ABell_cpp>())
			{
				GameUIWidget->SetInteractDotText(TEXT("Take"));
				GameUIWidget->SetInteractDot(true);
				return true;
			}

			//else if (ActorName.Contains(TEXT("Drawer_cpp"), ESearchCase::CaseSensitive, ESearchDir::FromEnd))
			//else if(HitActor->IsA<ADrawer_cpp>())
			//else if (ActorName.Contains(TEXT("Door_cpp"), ESearchCase::CaseSensitive, ESearchDir::FromEnd))
			else if(HitActor->IsA<ADoor_cpp>())
			{
				GameUIWidget->SetInteractDotText(TEXT("Open/Close"));
				GameUIWidget->SetInteractDot(true);
				return true;
			}

			//else if (ActorName.Contains(TEXT("ClassroomDoorActor_cpp"), ESearchCase::CaseSensitive, ESearchDir::FromEnd))
			else if(HitActor->IsA<AClassroomDoorActor_cpp>())
			{
				GameUIWidget->SetInteractDotText(TEXT("Open/Close"));
				GameUIWidget->SetInteractDot(true);
				return true;
			}*/

			//else if (ActorName.Contains(TEXT("Cabinet_cpp"), ESearchCase::CaseSensitive, ESearchDir::FromEnd))
			else if(HitActor->IsA<ADrawerClass>())
			{
				//UE_LOG(LogTemp, Warning, TEXT("%s"), *ActorName);
				GameUIWidget->SetInteractDotText(TEXT("Open/Close"));
				GameUIWidget->SetInteractDot(true);
				return true;
			}

			else if (HitActor->GetClass()->ImplementsInterface(UDoorInterface_cpp::StaticClass()))
			{
				GameUIWidget->SetInteractDotText(TEXT("Open/Close"));
				GameUIWidget->SetInteractDot(true);
				return true;
			}

			else if (HitActor->GetClass()->ImplementsInterface(UHideInterface::StaticClass()))
			{
				if (bIsHiding)
				{
					GameUIWidget->SetInteractDotText(TEXT("Exit"));
				}
				else
				{
					GameUIWidget->SetInteractDotText(TEXT("Hide"));
				}
				GameUIWidget->SetInteractDot(true);
				return true;
			}

			/*else if(HitActor->IsA<ACabinet_cpp>())
			{
				if(bIsHiding)
				{
					GameUIWidget->SetInteractDotText(TEXT("Exit"));
				}
				else
				{
					GameUIWidget->SetInteractDotText(TEXT("Hide"));
				}
				GameUIWidget->SetInteractDot(true);
				return true;
			}*/

			//else if (ActorName.Contains(TEXT("LockerDoorActor_cpp"), ESearchCase::CaseSensitive, ESearchDir::FromEnd))
			else if(HitActor->IsA<ALockerDoorActor_cpp>())
			{
				GameUIWidget->SetInteractDotText(TEXT("Open/Close"));
				GameUIWidget->SetInteractDot(true);
				return true;
			}

			/*else if (HitActor->IsA<AWardrobeDrawer_cpp>())
			{
				GameUIWidget->SetInteractDotText(TEXT("Open/Close"));
				GameUIWidget->SetInteractDot(true);
				return true;
			}*/

			/*else if (HitActor->IsA<AWardrobe_cpp>())
			{
				if (bIsHiding)
				{
					GameUIWidget->SetInteractDotText(TEXT("Exit"));
				}
				else
				{
					GameUIWidget->SetInteractDotText(TEXT("Hide"));
				}
				GameUIWidget->SetInteractDot(true);
				return true;
			}*/

			/*else if (HitActor->IsA<AReaper_cpp>())
			{
				if (GetCurrentItemName() == TEXT("Sword"))
				{
					GameUIWidget->SetInteractDotText(TEXT("Exorcism"));
					GameUIWidget->SetInteractDot(true);
					return true;
				}
				else if (GetCurrentItemName() == TEXT("Extinguisher"))
				{
					GameUIWidget->SetInteractDotText(TEXT("Spray"));
					GameUIWidget->SetInteractDot(true);
					return true;
				}
			}*/

			else if (HitActor->IsA<AAlarm>())
			{
				GameUIWidget->SetInteractDotText(TEXT("Ring"));
				GameUIWidget->SetInteractDot(true);
				return true;
			}

			/*else if (HitActor->IsA<AMetalDoor_cpp>())
			{
				GameUIWidget->SetInteractDotText(TEXT("Open/Close"));
				GameUIWidget->SetInteractDot(true);
				return true;
			}*/

			/*else if (HitActor->IsA<ADeskDrawer_cpp>())
			{
				GameUIWidget->SetInteractDotText(TEXT("Open/Close"));
				GameUIWidget->SetInteractDot(true);
				return true;
			}*/

			else if (AHangingLight* HangLight = Cast<AHangingLight>(HitActor))
			{
				if (!HangLight->bIsLightOn)
				{
					GameUIWidget->SetInteractDotText(TEXT("Turn On"));
					GameUIWidget->SetInteractDot(true);
					return true;
				}
			}

			else if (ADistributionBox* DBox = Cast<ADistributionBox>(HitActor))
			{
			if (!DBox->bIsPowered)
			{
				GameUIWidget->SetInteractDotText(TEXT("Power On"));
				GameUIWidget->SetInteractDot(true);
				return true;
			}
			}

			else if (HitActor->IsA<ALightSwitch>())
			{
				GameUIWidget->SetInteractDotText(TEXT("Turn On"));
				GameUIWidget->SetInteractDot(true);
				return true;
			}

			else if (HitActor->IsA<AAltar_cpp>())
			{
				GameUIWidget->SetInteractDotText(TEXT("Place the Reaper's Items"));
				GameUIWidget->SetInteractDot(true);
				return true;
			}

			else if (HitActor->IsA<ASwitchLever>())
			{
				GameUIWidget->SetInteractDotText(TEXT("Turn On/Off"));
				GameUIWidget->SetInteractDot(true);
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
	
	GameUIWidget->SetInteractDotText(TEXT(""));;
	GameUIWidget->SetInteractDot(false); // 에러
	return false;
}

//FString AHorrorGameCharacter::GetLineTraceSingleForBPActor()
//{
//	FHitResult OutHit;
//	FVector Start = FirstPersonCameraComponent->GetComponentLocation();
//	FVector FowardVector = FirstPersonCameraComponent->GetForwardVector();
//	FVector End = (FowardVector * 160.0f) + Start;
//	FString ReturnName = "";
//
//	bool isHit = GetWorld()->LineTraceSingleByChannel(OutHit, Start, End, ECC_Visibility);
//	if (isHit)
//	{
//		AActor* HitActor = OutHit.GetActor();
//		if (HitActor)
//		{
//			FString ActorName = HitActor->GetName();
//			// Take
//			if (ActorName.Contains(TEXT("CigarLighter_cpp"), ESearchCase::CaseSensitive, ESearchDir::FromEnd))
//			{
//				ReturnName = "Take";
//			}
//
//			else if (ActorName.Contains(TEXT("FlashLight_cpp"), ESearchCase::CaseSensitive, ESearchDir::FromEnd))
//			{
//				ReturnName = "Take";
//			}
//
//			else if (ActorName.Contains(TEXT("Key_cpp"), ESearchCase::CaseSensitive, ESearchDir::FromEnd))
//			{
//				ReturnName = "Take";
//			}
//
//			else if (ActorName.Contains(TEXT("Timer_cpp"), ESearchCase::CaseSensitive, ESearchDir::FromEnd))
//			{
//				ReturnName = "Take";
//			}
//
//			else if (ActorName.Contains(TEXT("Sword_cpp"), ESearchCase::CaseSensitive, ESearchDir::FromEnd))
//			{
//				ReturnName = "Take";
//			}
//
//			else if (ActorName.Contains(TEXT("Extinguisher_cpp"), ESearchCase::CaseSensitive, ESearchDir::FromEnd))
//			{
//				ReturnName = "Take";
//			}
//
//			else if (ActorName.Contains(TEXT("Cutter_cpp"), ESearchCase::CaseSensitive, ESearchDir::FromEnd))
//			{
//				ReturnName = "Take";
//			}
//
//			// Open/Close
//			else if (ActorName.Contains(TEXT("Drawer_cpp"), ESearchCase::CaseSensitive, ESearchDir::FromEnd))
//			{
//				ReturnName = "Open/Close";
//			}
//
//			else if (ActorName.Contains(TEXT("Door_cpp"), ESearchCase::CaseSensitive, ESearchDir::FromEnd))
//			{
//				ReturnName = "Open/Close";
//			}
//
//			else if (ActorName.Contains(TEXT("ClassroomDoorActor_cpp"), ESearchCase::CaseSensitive, ESearchDir::FromEnd))
//			{
//				ReturnName = "Open/Close";
//			}
//
//			else if (ActorName.Contains(TEXT("LockerDoorActor_cpp"), ESearchCase::CaseSensitive, ESearchDir::FromEnd))
//			{
//				ReturnName = "Open/Close";
//			}
//
//			// Hide
//			else if (ActorName.Contains(TEXT("Cabinet_cpp"), ESearchCase::CaseSensitive, ESearchDir::FromEnd))
//			{
//				ReturnName = "Hide";
//			}
//		}
//	}
//	return ReturnName;
//}
// 
void AHorrorGameCharacter::SetCameraComponentNoise(int32 WhichStatus)
{
	FTimerHandle NoiseTimer;
	GetWorldTimerManager().ClearTimer(NoiseTimer);
	switch (WhichStatus)
	{
	case 1: // 근처에 Creature가 있는 상황이면, 카메라에 노이즈 걸 것임
		FirstPersonCameraComponent->PostProcessSettings.bOverride_VignetteIntensity = true;
		FirstPersonCameraComponent->PostProcessSettings.VignetteIntensity = 1.f;
		FirstPersonCameraComponent->PostProcessSettings.bOverride_FilmGrainIntensity = true;
		FirstPersonCameraComponent->PostProcessSettings.FilmGrainIntensity = 4.f;
		HeartBeat->Play(); // 동시에 심장박동 소리도 재생
		break;
	case 2: // 추격 판정이 뜰 경우 노이즈 크게 함
		FirstPersonCameraComponent->PostProcessSettings.bOverride_VignetteIntensity = true;
		FirstPersonCameraComponent->PostProcessSettings.VignetteIntensity = 1.f;
		FirstPersonCameraComponent->PostProcessSettings.bOverride_FilmGrainIntensity = true;
		FirstPersonCameraComponent->PostProcessSettings.FilmGrainIntensity = 15.f;
		HeartBeat->Stop(); // 추격 판정 시 박동 소리 없앰.
		GetWorldTimerManager().SetTimer(NoiseTimer, FTimerDelegate::CreateLambda([&]() {
			SetCameraComponentNoise(1); // 0.5초동안 노이즈 크게 하고 다시 1번 상태로 돌릴 것임
		}), 0.5f, false);
		break;
	default: // Creature가 근처에 없다면 카메라에 노이즈 해제함
		FirstPersonCameraComponent->PostProcessSettings.bOverride_VignetteIntensity = false;
		FirstPersonCameraComponent->PostProcessSettings.bOverride_FilmGrainIntensity = false;
		HeartBeat->Play();
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
	GameUIWidget->Init();
}

void AHorrorGameCharacter::ScrollUpItem()
{
	CurrentItemNum++;
	if (CurrentItemNum > InventoryNum) CurrentItemNum = 0;
	
	if (GetCurrentItemName() != TEXT("FlashLight") && bIsFlashLightOn) UseFlashLight();
	if (GetCurrentItemName() != TEXT("CigarLight") && bIsCigarLightOn) UseCigarLight();

	CurrentItem();
}

void AHorrorGameCharacter::ScrollDownItem()
{
	CurrentItemNum--;
	if (CurrentItemNum < 0 && InventoryNum >= 0) CurrentItemNum = InventoryNum;
	
	if (GetCurrentItemName() != TEXT("FlashLight") && bIsFlashLightOn) UseFlashLight();
	if (GetCurrentItemName() != TEXT("CigarLight") && bIsCigarLightOn) UseCigarLight();

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
			if (Item.ItemName == CigarLightData->ItemName)
			{
				isFind = true;
				break;
			}
		}

		if (!isFind) // 인벤토리에 해당 아이템이 없다고 판별났을 경우
		{
			if (InventoryNum >= 8) // 인벤토리의 최대 수를 넘겼으면 못 얻음
			{
				ErrorInteractText = TEXT("Your Inventory is FULL!. You CANNOT get more items");
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
			Inventory[InventoryNum].ItemIcon = CigarLightData->ItemIcon;
			Inventory[InventoryNum].Type = EItemType::ITEM_Useable;
			Inventory[InventoryNum].ItemCount = 1;
			if (CurrentItemNum < 0)
				CurrentItemNum = 0;
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
			if (Item.ItemName == FlashLightData->ItemName)
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
				ErrorInteractText = TEXT("Your Inventory is FULL!. You CANNOT get more items");
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
			Inventory[InventoryNum].ItemIcon = FlashLightData->ItemIcon;
			Inventory[InventoryNum].Type = EItemType::ITEM_Useable;
			Inventory[InventoryNum].ItemCount = 1;
			if (CurrentItemNum < 0)
				CurrentItemNum = 0;
		}
		CurrentItem();
		GameUIWidget->SetBatteryHUD(FlashLightBattery);
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
			if (Item->ItemName == KeyData->ItemName)
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
				ErrorInteractText = TEXT("Your Inventory is FULL!. You CANNOT get more items");
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
			Inventory[InventoryNum].ItemIcon = KeyData->ItemIcon;
			Inventory[InventoryNum].Type = EItemType::ITEM_Useable;
			Inventory[InventoryNum].ItemCount += 1;
			if (CurrentItemNum < 0)
				CurrentItemNum = 0;
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
			if (Item->ItemName == TimerData->ItemName)
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
				ErrorInteractText = TEXT("Your Inventory is FULL!. You CANNOT get more items");
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
			Inventory[InventoryNum].ItemIcon = TimerData->ItemIcon;
			Inventory[InventoryNum].Type = EItemType::ITEM_Useable;
			Inventory[InventoryNum].ItemCount += 1;
			if (CurrentItemNum < 0)
				CurrentItemNum = 0;
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
			if (Item->ItemName == SwordData->ItemName) // Sword
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
				ErrorInteractText = TEXT("Your Inventory is FULL!. You CANNOT get more items");
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
			Inventory[InventoryNum].ItemIcon = SwordData->ItemIcon;
			Inventory[InventoryNum].Type = EItemType::ITEM_Useable;
			SwordCount = 1;
			Inventory[InventoryNum].ItemCount = SwordCount;
			if (CurrentItemNum < 0)
				CurrentItemNum = 0;
		}
		//GameUIWidget->Init();
		CurrentItem();
		// ObjectNumbers++;
		GameUIWidget->SetObjectCount(1, SwordCount);
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
			if (Item->ItemName == BellData->ItemName)
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
				ErrorInteractText = TEXT("Your Inventory is FULL!. You CANNOT get more items");
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
			Inventory[InventoryNum].ItemIcon = BellData->ItemIcon;
			Inventory[InventoryNum].Type = EItemType::ITEM_Useable;
			BellCount = 1;
			Inventory[InventoryNum].ItemCount = BellCount;
			if (CurrentItemNum < 0)
				CurrentItemNum = 0;
		}

		CurrentItem();
		// ObjectNumbers++;
		GameUIWidget->SetObjectCount(2, BellCount);
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
			if (Item->ItemName == MirrorData->ItemName)
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
				ErrorInteractText = TEXT("Your Inventory is FULL!. You CANNOT get more items");
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
			Inventory[InventoryNum].ItemIcon = MirrorData->ItemIcon;
			Inventory[InventoryNum].Type = EItemType::ITEM_Useable;
			MirrorCount = 1;
			Inventory[InventoryNum].ItemCount = MirrorCount;
			if (CurrentItemNum < 0)
				CurrentItemNum = 0;
		}
		//GameUIWidget->Init();
		CurrentItem();
		// ObjectNumbers++;
		GameUIWidget->SetObjectCount(3, MirrorCount);
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
			if (Item.ItemName == ExtinguisherData->ItemName)
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
				ErrorInteractText = TEXT("Your Inventory is FULL!. You CANNOT get more items");
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
			Inventory[InventoryNum].ItemIcon = ExtinguisherData->ItemIcon;
			Inventory[InventoryNum].Type = EItemType::ITEM_Useable;
			Inventory[InventoryNum].ItemCount = 1;
			if (CurrentItemNum < 0)
				CurrentItemNum = 0;
			//GameUIWidget->Init();
		}
		/*if (Inventory[CurrentItemNum].ItemName == ExtinguisherData->ItemName)
		{
			GameUIWidget->SetBatteryWidget(false);
			GameUIWidget->SetCutterWidget(false);
			GameUIWidget->SetExtWidget(true);
		}*/
		CurrentItem();
		GameUIWidget->SetExtHUD(ExtinguisherLeft);
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
			if (Item.ItemName == CutterData->ItemName)
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
				ErrorInteractText = TEXT("Your Inventory is FULL!. You CANNOT get more items");
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
		CurrentItem();
		GameUIWidget->SetCutterHUD(CutterDurability);
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
			if (Item.ItemName == LanternData->ItemName) // 인벤토리에 해당 아이템 있는지 확인
			{
				isFind = true;
				//Item.ItemCount = 1;
				////CutterDurability = 5;
				ErrorInteractText = TEXT("You can get ONLY 1 Lantern");
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
				ErrorInteractText = TEXT("Your Inventory is FULL!. You CANNOT get more items");
				SetErrorText(ErrorInteractText, 3);
				bCanItemGet = false;
				return;
			}

			Inventory[++InventoryNum].ItemName = LanternData->ItemName;
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
				InterfaceVariable->UseInteract(this);
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
					LockerDoor->UseInteract(this);
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
			CurrentItemNum = 0;
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
	SetNotifyAttackStart(true);
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Green, FString::Printf(TEXT("Current Notify Attack Start: %s"), bNotifyAttackStart ? TEXT("true") : TEXT("false")));
	
	auto AnimInstance = Cast<UPlayerAnim>(GetMesh()->GetAnimInstance());
	if (nullptr == AnimInstance) return;

	AnimInstance->PlayAttackMontage();
	AActor* ChildActor = Sword->GetChildActor();
	if (APlayerSword_cpp* BronzeSword = Cast<APlayerSword_cpp>(ChildActor))
	{
		BronzeSword->UseInteract(this);
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
			CurrentItemNum = 0;
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
	//FVector StartLocation = GetActorLocation(); // Player Location
	//FVector TargetLocation = FVector(29620.f, 30.f, 30.f); // Target Location
	//UNiagaraSystem* LaserBeamSystem = LoadObject<UNiagaraSystem>(nullptr, TEXT("/Game/Assets/Material/BeamSystem"));

	//// Get Normal Vector
	//FVector Direction = (TargetLocation - StartLocation).GetSafeNormal();
	//UNiagaraComponent* NiagaraComp = UNiagaraFunctionLibrary::SpawnSystemAtLocation(
	//	GetWorld(), LaserBeamSystem, StartLocation, Direction.Rotation());
	//if (NiagaraComp)
	//{
	//	NiagaraComp->SetWorldScale3D(FVector(1.f, 1.f, 160.f));
	//}
	//NiagaraComp->DestroyComponent();
	//NiagaraComp = nullptr;
	AActor* ChildActor = Mirror->GetChildActor();
	if (AMirror_cpp* BronzeMirror = Cast<AMirror_cpp>(ChildActor))
	{
		BronzeMirror->UseInteract(this);
	}

	bIsTimeStop = true;
	// bIsTimeStopChange = true;
	Inventory[CurrentItemNum].ItemCount = MirrorCount;
	// RadiusTime.Play();

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

	/*FPostProcessSettings& Settings = FirstPersonCameraComponent->PostProcessSettings;
	Settings.AddBlendable(PostProcessMaterialInstance, 1.0f);*/
	// FirstPersonCameraComponent->PostProcessSettings.AddBlendable(PostProcessMaterial, 0.02f);
	FirstPersonCameraComponent->AddOrUpdateBlendable(PostProcessMaterial, 1.f);
	

	GameUIWidget->SetObjectCount(3, MirrorCount);
	CurrentItem();
}

void AHorrorGameCharacter::UseExtinguisher()
{
	/*AActor* HitActor = nullptr;
	bool isHit = GetLineTraceSingle(HitActor);*/
	if (FirstPersonCameraComponent == nullptr)
		return;

	// ExtinguisherLeft -= 20;
	/*SmokeComponent->SetHiddenInGame(false);
	SmokeComponent->SetComponentTickEnabled(true);*/
	SmokeComponent->Activate(true);
	//CapsuleComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	bCanExtinguisherUse = false;
	USoundCue* ExtinguisherSound = LoadObject<USoundCue>(nullptr, TEXT("/Game/Assets/Sounds/SoundCues/SpraySoundCue"));
	if (ExtinguisherSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, ExtinguisherSound, GetActorLocation());
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
			/*if (GEngine)
				GEngine->AddOnScreenDebugMessage(-1, 1.5f, FColor::Red, FString::Printf(TEXT("Overlapped!")));*/
			FHitResult OutHit;
			FVector Start = GetActorLocation();
			/*FVector ForwardVector = FirstPersonCameraComponent->GetForwardVector();
			FVector End = (ForwardVector * 1200.0f) + Start;*/
			FVector End = OverlapResult.GetActor()->GetActorLocation();

			if (!GetWorld()->LineTraceSingleByChannel(OutHit, Start, End, ECC_Visibility))
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
		GameUIWidget->SetExtHUD(ExtinguisherLeft);

		if (ExtinguisherLeft <= 0)
		{
			Inventory[CurrentItemNum].ItemCount--;
			if (Inventory[CurrentItemNum].ItemCount == 0)
			{
				Inventory.RemoveAt(CurrentItemNum);
				InventoryNum--;
				CurrentItemNum--;
				if (CurrentItemNum < 0 && InventoryNum >= 0)
					CurrentItemNum = 0;
			}
			FHorrorGameItemData TempItem;
			TempItem.Clear();
			Inventory.Add(TempItem);
			GameUIWidget->SetExtWidget(false);
		}
		CurrentItem();

		GetWorld()->GetTimerManager().ClearTimer(SmokeTimer);
	}), 1.f, false);
	//UNiagaraSystem* FX = LoadObject<UNiagaraSystem>(nullptr, TEXT("/Game/Assets/Material/ExtinguisherSmoke"));
	/*FVector PlayerLocation = GetActorLocation();
	FVector ActorLocation = GetActorLocation();*/
	/*FVector Direction = (GetActorForwardVector()).GetSafeNormal();
	FVector PlayerForward = HorrorGamePlayerController->GetControlRotation().Vector();*/

	//FRotator Rotation = FRotationMatrix::MakeFromX(Direction).Rotator();
	//SmokeComponent->SetWorldRotation(Rotation);
	// FX->SetVectorParameter("Acceleration Force", PlayerForward);
	// SmokeComponent->SetVectorParameter("Acceleration Force", PlayerForward);
	//UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), FX, GetActorLocation());
	// SmokeComponent->ActivateSystem();
	// UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), SmokeComponent, GetActorLocation());
	//SmokeComponent->DeactivateImmediate();
	if (ExtinguisherLeft <= 0)
	{
		Inventory[CurrentItemNum].ItemCount--;
		if (Inventory[CurrentItemNum].ItemCount == 0)
		{
			Inventory.RemoveAt(CurrentItemNum);
			InventoryNum--;
			CurrentItemNum--;
			if (CurrentItemNum < 0 && InventoryNum >= 0)
				CurrentItemNum = 0;
		}
		FHorrorGameItemData TempItem;
		TempItem.Clear();
		Inventory.Add(TempItem);
		GameUIWidget->SetExtWidget(false);
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
					CutterDurability -= 1;
					USoundCue* CutSound = LoadObject<USoundCue>(nullptr, TEXT("/Game/Assets/Sounds/SoundCues/BreakLock"));
					if (CutSound)
					{
						UGameplayStatics::PlaySoundAtLocation(this, CutSound, GetActorLocation());
					}
					LockerDoor->UseInteract(this);
				}
			}
			else if (AClassroomDoorActor_cpp* ClassroomDoor = Cast<AClassroomDoorActor_cpp>(HitActor))
			{
				// AClassroomDoorActor_cpp* ClassroomDoor = Cast<AClassroomDoorActor_cpp>(HitActor);
				if (ClassroomDoor->bIsDoorLocked)
				{
					CutterDurability -= 1;
					ClassroomDoor->UseInteract(this);
					USoundCue* CutSound = LoadObject<USoundCue>(nullptr, TEXT("/Game/Assets/Sounds/SoundCues/BreakLock"));
					if (CutSound)
					{
						UGameplayStatics::PlaySoundAtLocation(this, CutSound, GetActorLocation());
					}
				}
			}

			else if (AMetalDoor_cpp* MetalDoor = Cast<AMetalDoor_cpp>(HitActor))
			{
				if (MetalDoor->bIsDoorLocked)
				{
					CutterDurability -= 1;
					USoundCue* CutSound = LoadObject<USoundCue>(nullptr, TEXT("/Game/Assets/Sounds/SoundCues/BreakLock"));
					if (CutSound)
					{
						UGameplayStatics::PlaySoundAtLocation(this, CutSound, GetActorLocation());
					}
					MetalDoor->UseInteract(this);
				}
			}
		}
	}
	if (CutterDurability <= 0)
	{
		Inventory[CurrentItemNum].ItemCount--;
		if (Inventory[CurrentItemNum].ItemCount == 0)
		{
			GameUIWidget->SetCutterWidget(false);
			Inventory.RemoveAt(CurrentItemNum);
			InventoryNum--;
			CurrentItemNum--;
			if (CurrentItemNum < 0 && InventoryNum >= 0)
				CurrentItemNum = 0;
		}
		FHorrorGameItemData TempItem;
		TempItem.Clear();
		Inventory.Add(TempItem);
	}
	CurrentItem();
	GameUIWidget->SetCutterHUD(CutterDurability);
}

void AHorrorGameCharacter::UseLantern()
{
	AActor* ChildActor = Lantern->GetChildActor();
	if (ASoul_Lantern_cpp* SoulLantern = Cast<ASoul_Lantern_cpp>(ChildActor))
	{
		SoulLantern->UseInteract(this);
	}
}

// Battery Manage Function
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
					CurrentItemNum = 0;
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
	FString currentItemName = GetCurrentItemName();

	if (currentItemName == TEXT("FlashLight"))
	{
		GameUIWidget->SetBatteryWidget(true);
	}
	else
	{
		GameUIWidget->SetBatteryWidget(false);
	}

	if (currentItemName == TEXT("Extinguisher"))
	{
		GameUIWidget->SetExtWidget(true);
	}
	else
	{
		GameUIWidget->SetExtWidget(false);
	}

	if (currentItemName == TEXT("Cutter"))
	{
		GameUIWidget->SetCutterWidget(true);
	}
	else
	{
		GameUIWidget->SetCutterWidget(false);
	}

	if (currentItemName == TEXT("SoulLantern"))
	{
		Lantern->SetHiddenInGame(false);
	}
	else
	{
		AActor* ChildActor = Lantern->GetChildActor();
		if (ASoul_Lantern_cpp* SoulLantern = Cast<ASoul_Lantern_cpp>(ChildActor))
		{
			if(SoulLantern->bIsLightOn)
				SoulLantern->UseInteract(this);
		}
		Lantern->SetHiddenInGame(true);
	}

	if (currentItemName == TEXT("Mirror"))
	{
		Mirror->SetHiddenInGame(false);
	}
	else
	{
		Mirror->SetHiddenInGame(true);
	}

	if (currentItemName == TEXT("Sword"))
	{
		Sword->SetHiddenInGame(false);
	}
	else
	{
		Sword->SetHiddenInGame(true);
	}

	/*if (currentItemName == TEXT("CigarLighter"))
	{
		CigarLighter->SetHiddenInGame(false);
	}
	else
	{
		AActor* ChildActor = CigarLighter->GetChildActor();
		if (ACigarLighter_cpp* Lighter = Cast<ACigarLighter_cpp>(ChildActor))
		{
			if (bIsCigarLightOn)
				Lighter->UseInteract(this);
		}
		CigarLighter->SetHiddenInGame(true);
	}*/

	// 구 버전(굳이 이럴 필요가 없다고 판단)
	/*if (currentItemName == TEXT("CigarLighter"))
	{
		Lantern->SetHiddenInGame(true);
		Compass->SetHiddenInGame(true);
		GameUIWidget->SetBatteryWidget(false);
		GameUIWidget->SetCutterWidget(false);
		GameUIWidget->SetExtWidget(false);
		GetWorld()->GetTimerManager().ClearTimer(_MirrorTimerHandle);
	}

	else if (currentItemName == TEXT("FlashLight"))
	{
		Lantern->SetHiddenInGame(true);
		Compass->SetHiddenInGame(true);
		GameUIWidget->SetBatteryWidget(true);
		GameUIWidget->SetCutterWidget(false);
		GameUIWidget->SetExtWidget(false);
		GetWorld()->GetTimerManager().ClearTimer(_MirrorTimerHandle);
	}

	else if (currentItemName == TEXT("Key"))
	{
		Lantern->SetHiddenInGame(true);
		Compass->SetHiddenInGame(true);
		GameUIWidget->SetBatteryWidget(false);
		GameUIWidget->SetCutterWidget(false);
		GameUIWidget->SetExtWidget(false);
		GetWorld()->GetTimerManager().ClearTimer(_MirrorTimerHandle);
	}

	else if (currentItemName == TEXT("Timer"))
	{
		Lantern->SetHiddenInGame(true);
		Compass->SetHiddenInGame(true);
		GameUIWidget->SetBatteryWidget(false);
		GameUIWidget->SetCutterWidget(false);
		GameUIWidget->SetExtWidget(false);
		GetWorld()->GetTimerManager().ClearTimer(_MirrorTimerHandle);
	}

	else if (currentItemName == TEXT("Sword"))
	{
		Lantern->SetHiddenInGame(true);
		Compass->SetHiddenInGame(true);
		GameUIWidget->SetBatteryWidget(false);
		GameUIWidget->SetCutterWidget(false);
		GameUIWidget->SetExtWidget(false);
		GetWorld()->GetTimerManager().ClearTimer(_MirrorTimerHandle);
	}

	else if (currentItemName == TEXT("Bell"))
	{
		Lantern->SetHiddenInGame(true);
		Compass->SetHiddenInGame(true);
		GameUIWidget->SetBatteryWidget(false);
		GameUIWidget->SetCutterWidget(false);
		GameUIWidget->SetExtWidget(false);
		GetWorld()->GetTimerManager().ClearTimer(_MirrorTimerHandle);
	}

	else if (currentItemName == TEXT("Mirror"))
	{
		Lantern->SetHiddenInGame(true);
		Compass->SetHiddenInGame(true);
		GameUIWidget->SetBatteryWidget(false);
		GameUIWidget->SetCutterWidget(false);
		GameUIWidget->SetExtWidget(false);
		GetWorld()->GetTimerManager().SetTimer(_MirrorTimerHandle, this, &AHorrorGameCharacter::UseMirror, 0.2f, false);
	}

	else if (currentItemName == TEXT("Extinguisher"))
	{
		Lantern->SetHiddenInGame(true);
		Compass->SetHiddenInGame(true);
		GameUIWidget->SetBatteryWidget(false);
		GameUIWidget->SetCutterWidget(false);
		GameUIWidget->SetExtWidget(true);
		GetWorld()->GetTimerManager().ClearTimer(_MirrorTimerHandle);
	}

	else if (currentItemName == TEXT("Cutter"))
	{
		Lantern->SetHiddenInGame(true);
		Compass->SetHiddenInGame(true);
		GameUIWidget->SetBatteryWidget(false);
		GameUIWidget->SetCutterWidget(true);
		GameUIWidget->SetExtWidget(false);
		GetWorld()->GetTimerManager().ClearTimer(_MirrorTimerHandle);
	}

	else if (currentItemName == TEXT("SoulLantern"))
	{
		AActor* ChildActor = Lantern->GetChildActor();
		if (ASoul_Lantern_cpp* SoulLantern = Cast<ASoul_Lantern_cpp>(ChildActor))
		{
			SoulLantern->UseInteract();
		}
		Lantern->SetHiddenInGame(false);
		Compass->SetHiddenInGame(true);
		GameUIWidget->SetBatteryWidget(false);
		GameUIWidget->SetCutterWidget(false);
		GameUIWidget->SetExtWidget(false);
		GetWorld()->GetTimerManager().ClearTimer(_MirrorTimerHandle);
	}*/

	GameUIWidget->Init();
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
	return CurrentItemNum;
}

FString AHorrorGameCharacter::GetCurrentItemName()
{
	if(CurrentItemNum >= 0 && CurrentItemNum <= InventoryNum)
		return Inventory[CurrentItemNum].ItemName;
	return "";
}

int32 AHorrorGameCharacter::GetExtinguisherLeft()
{
	return ExtinguisherLeft;
}

int32 AHorrorGameCharacter::GetPatience()
{
	return Patience;
}

FString AHorrorGameCharacter::GetInteractText()
{
	return InteractText;
}

FString AHorrorGameCharacter::GetErrorInteractText()
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

// Set Functions: SetPlayerStatus()
void AHorrorGameCharacter::SetPlayerStatus(Player_Status Value)
{
	if (PlayerStatus != Value)
	{
		PlayerStatus = Value;

		switch (PlayerStatus)
		{
			case Player_Status::Loading:
			{
				DisableInput(HorrorGamePlayerController);
			
				break;
			}
			case Player_Status::Survive:
			{
				EnableInput(HorrorGamePlayerController);

				break;
			}
			case Player_Status::Catched:
			{
				DisableInput(HorrorGamePlayerController);

				break;
			}
			case Player_Status::Stunned:
			{
				DisableInput(HorrorGamePlayerController);
				GetCharacterMovement()->StopMovementImmediately();
				break;
			}
			case Player_Status::Died:
			{
				SetActorEnableCollision(false);
				GetMesh()->SetHiddenInGame(true);
				DisableInput(HorrorGamePlayerController);

				HorrorGamePlayerController->ShowDeadUI();
				break;
			}
		}
	}
}

void AHorrorGameCharacter::SetExplainText(FString text, int32 time)
{
	GameUIWidget->SetInteractDotExplainText(text);
	TextTimer = time;
	cnt = 0.f;
}

void AHorrorGameCharacter::SetErrorText(FString text, int32 time)
{
	GameUIWidget->SetInteractDotErrorText(text);
	ErrorTextTimer = time;
	ErrorTextCount = 0.f;
}

//UCameraComponent* AHorrorGameCharacter::GetCameraComponent()
//{
//	return FirstPersonCameraComponent;
//}

void AHorrorGameCharacter::AddPatience(int32 value)
{
	
	Patience += value;
	if (Patience <= 0)
		Patience = 0;

	if (Patience >= 100)
		Patience = 100;
	GameUIWidget->SetPatience(Patience);
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
	if (!value) // 만약 Attack Check의 값이 false면
	{
		SetNotifyAttackStart(false); // 공격 시작 알림을 false로
	}
	auto BronzeSword = Cast<APlayerSword_cpp>(Sword->GetChildActor());
	if (BronzeSword)
	{
		BronzeSword->SetShouldExorcism(bShouldAttack);
		if (BronzeSword->bHadExorcism) // 크리쳐를 처치한 상황인 경우에, 칼 갯수 감소
		{
			SwordCount--;
			Inventory[CurrentItemNum].ItemCount = SwordCount;
			// RadiusTime.Play();

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

			GameUIWidget->SetObjectCount(1, SwordCount);
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
}

void AHorrorGameCharacter::LevelStart()
{
	GameUIWidget->Player = this;
	GameUIWidget->AllWidgetInit();
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