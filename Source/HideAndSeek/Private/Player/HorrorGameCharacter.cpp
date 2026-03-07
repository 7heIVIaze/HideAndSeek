// CopyrightNotice 2023 Sunggon Kim kimdave205@gmail.com. All Rights Reserved.

#include "Player/HorrorGameCharacter.h"
#include "LevelManager/HorrorGameGameMode.h"
#include "LevelManager/PatrolPoint_cpp.h"
#include "LevelManager/HorrorGameGameInstance.h"
#include "Player/InventoryComponent.h"
#include "Player/PlayerStatComponent.h"
#include "Player/InteractionComponent.h"
#include "ComponentAction/PrologueGameMode.h"
#include "HideAndSeek/HorrorGameProjectile.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/BoxComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"
#include "Blueprint/UserWidget.h"
#include "Components/PointLightComponent.h"
#include "Components/SpotLightComponent.h"
#include "HUD/InteractDot_cpp.h"
#include "AI/CreatureClass.h"
#include "Player/HorrorGamePlayerController.h"
#include "HUD/GameUI.h"
#include "Furniture/HideObject.h"
#include "Furniture/DoorClass.h"
#include "Items/ItemClass.h"
#include "Items/ActiveItem.h"
#include "Items/LightItem.h"
#include "Items/TimerProjectile_cpp.h"
#include "Items/Sword.h"
#include "Items/GlowStick.h"
#include "EngineUtils.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Materials/MaterialInstance.h"
#include "Player/PlayerAnim.h"
#include "Kismet/KismetMathLibrary.h"
#include "Camera/CameraShakeBase.h"
#include "Camera/PlayerCameraManager.h"
#include "Engine/OverlapResult.h"
#include "Engine/HitResult.h"

//////////////////////////////////////////////////////////////////////////
// AHorrorGameCharacter

// Default Setting
AHorrorGameCharacter::AHorrorGameCharacter()
{
	bIsSprinting = false;
	bIsCrouch = false;
	bIsLightOn = false;
	bIsLanternOn = false;
	bFLIntenseDown = false;
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

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArmComp->SetupAttachment(GetCapsuleComponent());
	SpringArmComp->TargetArmLength = 0.0f;
	SpringArmComp->bUsePawnControlRotation = true;
	SpringArmComp->bEnableCameraRotationLag = true;

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

	CigarlighterComp = CreateDefaultSubobject<UChildActorComponent>(TEXT("Cigarlighter"));
	CigarlighterComp->SetupAttachment(FirstPersonCameraComponent);

	FlashlightComp = CreateDefaultSubobject<UChildActorComponent>(TEXT("Flashlight"));
	FlashlightComp->SetupAttachment(SpringArmComp);

	LanternComp = CreateDefaultSubobject<UChildActorComponent>(TEXT("Lantern"));
	LanternComp->SetupAttachment(FirstPersonCameraComponent);

	MirrorComp = CreateDefaultSubobject<UChildActorComponent>(TEXT("Mirror"));
	MirrorComp->SetupAttachment(FirstPersonCameraComponent);

	SwordComp = CreateDefaultSubobject<UChildActorComponent>(TEXT("Sword"));
	SwordComp->SetupAttachment(GetMesh(), TEXT("hand_r_socket"));
	
	FEComp = CreateDefaultSubobject<UChildActorComponent>(TEXT("FireExtinguisher"));
	FEComp->SetupAttachment(FirstPersonCameraComponent);

	// 스켈레탈 메시의 그림자 여부는 다 없앰.
	GetMesh()->SetOnlyOwnerSee(false);
	GetMesh()->bCastDynamicShadow = false;
	GetMesh()->CastShadow = false;

	InteractionComp = CreateDefaultSubobject<UInteractionComponent>(TEXT("InteractionComponent"));

	InventoryComp = CreateDefaultSubobject<UInventoryComponent>(TEXT("InventoryComponent"));

	PlayerStatComp = CreateDefaultSubobject<UPlayerStatComponent>(TEXT("PlayerStatComponent"));

	FlashlightTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("FlashlightTimeline"));

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

	if (InventoryComp)
	{
		InventoryComp->OnItemAcquired.AddDynamic(this, &AHorrorGameCharacter::SwitchItem);
		InventoryComp->OnItemSwitch.AddDynamic(this, &AHorrorGameCharacter::SwitchItem);
		InventoryComp->OnItemConsumed.AddDynamic(this, &AHorrorGameCharacter::SwitchItem);
	}

	if (PlayerStatComp)
	{
		PlayerStatComp->OnStaminaOver.AddDynamic(this, &AHorrorGameCharacter::EndCrouch);
		PlayerStatComp->OnDeath.AddDynamic(this, &AHorrorGameCharacter::Death);

		if (HorrorGamePlayerController)
		{
			PlayerStatComp->OnDeath.AddDynamic(HorrorGamePlayerController, &AHorrorGamePlayerController::OnDeath);
		}
	}

	if (FEComp)
	{
		if (auto FE = Cast<AActiveItem>(FEComp->GetChildActor()))
		{
			FE->SetOwner(this);
			FE->SetItemVisibility(true);
		}
	}

	if (FlashlightComp)
	{
		if (auto FL = Cast<ALightItem>(FlashlightComp->GetChildActor()))
		{
			FL->SetOwner(this);
			FL->SetItemVisibility(true);
		}
	}

	if (CigarlighterComp)
	{
		if (auto CL = Cast<ALightItem>(CigarlighterComp->GetChildActor()))
		{
			CL->SetOwner(this);
			CL->SetItemVisibility(true);
		}
	}

	if (LanternComp)
	{
		if (auto Lantern = Cast<ALightItem>(LanternComp->GetChildActor()))
		{
			Lantern->SetOwner(this);
			Lantern->SetItemVisibility(true);
		}
	}

	if (MirrorComp)
	{
		if (auto Mirror = Cast<AActiveItem>(MirrorComp->GetChildActor()))
		{
			Mirror->SetOwner(this);
			Mirror->SetItemVisibility(true);
		}
	}

	if (SwordComp)
	{
		if (auto Sword = Cast<AActiveItem>(SwordComp->GetChildActor()))
		{
			Sword->SetOwner(this);
			Sword->SetItemVisibility(true);
		}
	}

	if (WalkingSway)
	{
		FOnTimelineFloat ProgressFunction;
		ProgressFunction.BindDynamic(this, &AHorrorGameCharacter::HandleFlashlightSway);
		FlashlightTimeline->AddInterpFloat(WalkingSway, ProgressFunction, "Sway");
		FlashlightTimeline->SetLooping(false);
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

	// 카메라 쉐이크 타임라인 커브 값이 있다면 카메라 쉐이크 타임라인에 할당하고, 재생될 때 실행할 콜백 함수도 바인딩함.
	if (ShakeCurveFloat)
	{
		FOnTimelineFloat TimelineProgress;
		TimelineProgress.BindDynamic(this, &AHorrorGameCharacter::OnCameraShake);
		CameraShakeTimeline.AddInterpFloat(ShakeCurveFloat, TimelineProgress);
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

	// 플레이어가 숨은 상태라면 적 개체에 의해 밀려서 숨는 공간에서 나오지 못하는 상황을 막기 위한 로직임,
	if (PlayerStatComp->GetCurrentPlayerStates() == EPlayerStatus::Hiding) 
	{
		// 숨기 전의 트랜스폼을 계속 유지하게 설정
		SetActorLocation(BeforeHideLocation);
		SetActorRotation(BeforeHideRotation);
	}

	FlickeringLight.TickTimeline(DeltaTime);
	RotateCameraTimeline.TickTimeline(DeltaTime);
	CameraShakeTimeline.TickTimeline(DeltaTime);

	// 플레이어가 행동 불능 상태인 경우
	if (PlayerStatComp->GetCurrentPlayerStates() == EPlayerStatus::Stunned)
	{
		// 행동 불능 타이머를 프레임 전환 시간만큼 계속 더함.
		StunTimer += DeltaTime;

		// 행동 불능이 된 지 1.5초 이상된 경우
		if (StunTimer >= 1.5f)
		{
			// 타이머를 0으로 설정하고, 상태를 생존으로 변경함.
			StunTimer = 0.f;
			SetPlayerStatus(EPlayerStatus::Survive);
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
	
	// 문서 보관함에 문서가 추가된 경우
	if (bIsArchiveTextOn)
	{
		// 문서 보관함 텍스트 타이머를 프레임 전환 시간만큼 계속 더함.
		ArchiveTextTimer += DeltaTime;

		// 타이머가 지정한 10 초 이상으로 지속된 경우
		if (ArchiveTextTimer >= 10.f)
		{
			// 플레이어가 사망한 상태라면
			if (PlayerStatComp->GetCurrentPlayerStates() == EPlayerStatus::Died)
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
		if (bIsSprinting)
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
		if (bIsSprinting)
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
		GetStatComponent()->AddConfusionPoint(-1);
		
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
	
#pragma region Deprecated stamina
	//// 현재 웅크리기 중이라면
	//if (bIsCrouch)
	//{
	//	PlayerStatComp->IncreaseStamina();
	//}
	//// 웅크리기 중이 아니라면
	//else
	//{
	//	// 현재 달리는 중이라면
	//	/*if (bIsSprinting)
	//	{
	//		PlayerStatComp->DecreaseStamina();
	//	}*/
	//	// 달리는 중이 아니라면
	//	else
	//	{
	//		PlayerStatComp->IncreaseStamina();
	//	}
	//}
#pragma endregion

	// 리퍼가 능력을 사용하기 시작했다면
	if (bReaperWatchPlayer && PlayerStatComp->GetCurrentPlayerStates() != EPlayerStatus::Stunned) 
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
			SetPlayerStatus(EPlayerStatus::Stunned);
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
		//Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AHorrorGameCharacter::Move);

		//Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AHorrorGameCharacter::Look);

		// Crouch
		EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Started, this, &AHorrorGameCharacter::BeginCrouch);
		EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Completed, this, &AHorrorGameCharacter::EndCrouch);

		// Sprint
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Started, this, &AHorrorGameCharacter::BeginSprint);
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Completed, this, &AHorrorGameCharacter::EndSprint);

		// Interaction
		EnhancedInputComponent->BindAction(InteractionAction, ETriggerEvent::Canceled, this, &AHorrorGameCharacter::DoInteraction);
		EnhancedInputComponent->BindAction(InteractionAction, ETriggerEvent::Completed, this, &AHorrorGameCharacter::DoHoldInteraction);

		// Use Item 
		EnhancedInputComponent->BindAction(UseItemAction, ETriggerEvent::Started, this, &AHorrorGameCharacter::UseItem);

		// Use Lighting Item
		EnhancedInputComponent->BindAction(LightingAction, ETriggerEvent::Canceled, this, &AHorrorGameCharacter::UseLightingItem);
		EnhancedInputComponent->BindAction(LightingAction, ETriggerEvent::Completed, this, &AHorrorGameCharacter::ChangeLightingItem);

		// Change Item
		EnhancedInputComponent->BindAction(ChangeItemAction, ETriggerEvent::Triggered, this, &AHorrorGameCharacter::ChangeInventoryItem);

	}
}

// 이동 관련
void AHorrorGameCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (HorrorGamePlayerController != nullptr)
	{
		// add movement 
		if (PlayerStatComp->GetCurrentPlayerStates() != EPlayerStatus::Hiding)
		{
			AddMovementInput(GetActorForwardVector(), MovementVector.Y);
			AddMovementInput(GetActorRightVector(), MovementVector.X);

			if (!FlashlightTimeline->IsPlaying() && bIsLightOn)
			{
				FlashlightTimeline->PlayFromStart();
			}
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
		if (PlayerStatComp->GetCurrentPlayerStates() == EPlayerStatus::Hiding)
		{
			if (AActor* HitActor = GetActorToInteract())
			{
				// 해당 액터가 숨을 수 있는 액터라면
				if (AHideObject* HideObject = Cast<AHideObject>(HitActor))
				{
					// 해당 액터 내부 카메라의 Pitch와 Yaw를 변경시켜 회전하는 것으로 함.
					HideObject->Pitch += LookAxisVector.Y * -1.0f;
					HideObject->Yaw += LookAxisVector.X;
					HideObject->MoveCamera(); // Call Cabinet_cpp Hide Function
				}
			}
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
			bIsSprinting = true;
			
			// 스태미너 회복 타이머 중지
			if (GetWorldTimerManager().IsTimerActive(PlayerStatComp->StaminaRechargeTimer))
			{
				GetWorldTimerManager().ClearTimer(PlayerStatComp->StaminaRechargeTimer);
			}

			// 스태미너 소모 타이머 시작
			GetWorld()->GetTimerManger().SetTimer(PlayerStatComp->StaminaReduceTimer, PlayerStatComp.Get(),
				&UPlayerStatComponent::DecreaseStamina, 0.5f, true);

			// 달리는 중엔 손전등이 흔들리는 속도가 더 빨라지게 설정.
			FlashlightTimeline->SetPlayRate(2.0f);

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
	if (bIsSprinting)
	{
		Sound->Play();
	}
	bIsSprinting = false;

	// 스태미너 소모 타이머 중지
	if (GetWorldTimerManager().IsTimerActive(PlayerStatComp->StaminaReduceTimer))
	{
		GetWorldTimerManager().ClearTimer(PlayerStatComp->StaminaReduceTimer);
	}

	// 스태미너 회복
	GetWorld()->GetTimerManger().SetTimer(PlayerStatComp->StaminaRechargeTimer, PlayerStatComp.Get(),
		&UPlayerStatComponent::IncreaseStamina, 0.5f, true);

	FlashlightTimeline->SetPlayRate(1.0f);

	// 달리는 소리가 재생 중이면, 멈춤.
	if (SprintSound->GetPlayState() == EAudioComponentPlayState::Playing)
	{
		SprintSound->Stop();
	}
}

void AHorrorGameCharacter::ChangeInventoryItem(const FInputActionValue& Value)
{
	// Input value is Vector1D.
	float ScrollValue = Value.Get<float>();

	if (ScrollValue > 0.0f)
	{
		InventoryComp->MoveItemIndex(1);
	}
	else if (ScrollValue < 0.0f)
	{
		InventoryComp->MoveItemIndex(-1);
	}
}

// Courching Fucntion: BeginCrouch(), EndCrouch()
// 웅크리는 함수.
void AHorrorGameCharacter::BeginCrouch()
{
	// 웅크리기를 활성화함.
	Crouch();

	// 스태미너 소모 타이머 중지
	if (GetWorldTimerManager().IsTimerActive(PlayerStatComp->StaminaReduceTimer))
	{
		GetWorldTimerManager().ClearTimer(PlayerStatComp->StaminaReduceTimer);
	}
	
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
void AHorrorGameCharacter::DoInteraction()
{
	AActor* HitActor = GetActorToInteract();
	if (HitActor)
	{
		FString HitActorName = HitActor->GetName();
		UE_LOG(LogTemp, Warning, TEXT("HitActor Name: %s"), *HitActorName);

		if (HitActor->GetClass()->ImplementsInterface(UInteractInterface::StaticClass()))
		{
			auto InterfaceActor = Cast<IInteractInterface>(HitActor);

			InterfaceActor->OnInteract(this);
		}
	}
}

void AHorrorGameCharacter::DoHoldInteraction()
{
	AActor* HitActor = GetActorToInteract();
	if (HitActor)
	{
		UE_LOG(LogTemp, Warning, TEXT("HitActor Name: %s"), *HitActor->GetName());
		if (auto Item = Cast<AItemClass>(HitActor))
		{
			Item->HoldInteract(this);
		}
	}
}

// 아이템을 사용하는 함수.
void AHorrorGameCharacter::UseItem()
{
	if (InventoryComp)
	{
		InventoryComp->UseConsumableItem(this);
	}
}

FVector AHorrorGameCharacter::GetCameraComponentLocation()
{
	if (FirstPersonCameraComponent)
	{
		return FirstPersonCameraComponent->GetComponentLocation();
	}

	return GetActorLocation();
}

FVector AHorrorGameCharacter::GetCameraComponentForwardVector()
{
	if (FirstPersonCameraComponent)
	{
		return FirstPersonCameraComponent->GetForwardVector();
	}

	return GetActorForwardVector();
}

// LineTrace Functions: GetLineTraceSingle(), GetLineTraceSingleForBP, GetLineTraceSingleForBPActor()

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
				if (PlayerStatComp->GetCurrentPlayerStates() == EPlayerStatus::Survive || PlayerStatComp->GetCurrentPlayerStates() == EPlayerStatus::Stunned) // 생존 또는 스턴 상태일 때만, 작동하도록
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
				else if (PlayerStatComp->GetCurrentPlayerStates() == EPlayerStatus::Died)
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
				else if (PlayerStatComp->GetCurrentPlayerStates() == EPlayerStatus::Clear)
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
				else if (PlayerStatComp->GetCurrentPlayerStates() == EPlayerStatus::Ending)
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

bool AHorrorGameCharacter::TakeItems(class AItemClass* TakenItem)
{
	bool Result = InventoryComp->AddToInventory(TakenItem, this);

	return Result;
}

bool AHorrorGameCharacter::ChangeItems(class AItemClass* TakenItem)
{
	bool Result = InventoryComp->ChangeCurrentItem(TakenItem, this);

	return Result;
}

void AHorrorGameCharacter::UseLightingItem()
{
	InventoryComp->UseLightingItem(this);
}

void AHorrorGameCharacter::ChangeLightingItem()
{
	InventoryComp->ChangeAnotherLightingItem(this);
}

bool AHorrorGameCharacter::ToggleCigarlight(bool inIsLightOn)
{
	bool result = false;
	if (CigarlighterComp)
	{
		if (ALightItem* Cigarlighter = Cast<ALightItem>(CigarlighterComp->GetChildActor()))
		{
			if (inIsLightOn)
			{
				result = Cigarlighter->TurnOn();
			}
			else
			{
				result = Cigarlighter->TurnOff();
			}
		}
	}

	bIsLightOn = inIsLightOn;

	return result;
}

bool AHorrorGameCharacter::SetFlashlight(int inDurability)
{
	if (FlashlightComp)
	{
		if (ALightItem* Flashlight = Cast<ALightItem>(FlashlightComp->GetChildActor()))
		{
			Flashlight->SetupDurability(inDurability);

			return true;
		}
	}

	return false;
}

bool AHorrorGameCharacter::ToggleFlashlight(bool inIsLightOn)
{
	bool result = false;
	if (FlashlightComp)
	{
		if (ALightItem* Flashlight = Cast<ALightItem>(FlashlightComp->GetChildActor()))
		{
			if (inIsLightOn)
			{
				result = Flashlight->TurnOn();
			}
			else
			{
				result = Flashlight->TurnOff();
			}
		}
	}

	bIsLightOn = inIsLightOn;

	return result;
}

bool AHorrorGameCharacter::UseNoiseItem()
{
	// If there is projectile class
	if (ProjectileClass)
	{
		// Set the starting point and rotation value of the projectile for the throwing effect.
		FVector CameraLocation;
		FRotator CameraRotation;
		GetActorEyesViewPoint(CameraLocation, CameraRotation);

		FVector MuzzleLocation = CameraLocation + FTransform(CameraRotation).TransformVector(MuzzleOffset);
		FRotator MuzzleRotation = CameraRotation;

		MuzzleRotation.Pitch += 10.0f;
		UWorld* World = GetWorld();

		// Gives the effect of throwing a projectile.
		if (World)
		{
			ATimerProjectile_cpp* Projectile = World->SpawnActor<ATimerProjectile_cpp>(ProjectileClass, MuzzleLocation, MuzzleRotation);

			if (Projectile)
			{
				FVector LaunchDirection = MuzzleRotation.Vector();
				Projectile->FireInDirection(LaunchDirection);

				return true;
			}
		}
	}

	return false;
}

// 청동 검을 사용하는 함수.
bool AHorrorGameCharacter::UseSword()
{
	// 현재 숨은 상태가 아닐 경우
	if (PlayerStatComp->GetCurrentPlayerStates() != EPlayerStatus::Hiding)
	{
		// 애니메이션 클래스를 가져와 공격 애니메이션 재생을 호출함.
		auto AnimInstance = Cast<UPlayerAnim>(GetMesh()->GetAnimInstance());
		if (nullptr == AnimInstance) return false;

		AnimInstance->PlayAttackMontage();
		
		// 그 후 쥐고 있는 청동 검의 아이템 사용 메서드를 호출함.
		AActor* ChildActor = SwordComp->GetChildActor();

		if (AActiveItem* BronzeSword = Cast<AActiveItem>(ChildActor))
		{
			bool result = BronzeSword->UseItem();
			return result;
		}
	}

	return false;
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
bool AHorrorGameCharacter::UseMirror()
{
	// 시간 정지가 되었다고 설정하고, 리퍼가 플레이어를 보고 있지 않다고 설정함.
	bIsTimeStop = true;
	bReaperWatchPlayer = false;
	ReaperWatchElapsedTime = 0.0f;

	// 거울의 아이템 사용 메서드를 호출함.
	AActor* ChildActor = MirrorComp->GetChildActor();
	if (AActiveItem* BronzeMirror = Cast<AActiveItem>(ChildActor))
	{
		bool result = BronzeMirror->UseItem();
		Cast<AHorrorGameGameMode>(UGameplayStatics::GetGameMode(GetWorld()))->PlayMirrorUseBackGroundMusic();

		if (result)
		{
			// 카메라에 포스트 프로세싱 머티리얼을 추가함.
			FirstPersonCameraComponent->AddOrUpdateBlendable(PostProcessMaterial, 1.f);
		}
		
		return result;
	}
	
	return false;
}

bool AHorrorGameCharacter::SetExtinguisher(int inDurability)
{
	if (FEComp)
	{
		if (AActiveItem* Extinguisher = Cast<AActiveItem>(FEComp->GetChildActor()))
		{
			Extinguisher->SetupDurability(inDurability);

			return true;
		}
	}

	return false;
}

// 소화기를 사용하는 함수.
bool AHorrorGameCharacter::UseExtinguisher()
{
	if (FirstPersonCameraComponent == nullptr)
	{
		return false;
	}

	if (PlayerStatComp->GetCurrentPlayerStates() == EPlayerStatus::Hiding)
	{
		return false;
	}

	// Spray
	if (FEComp)
	{
		if (AActiveItem* Item = Cast<AActiveItem>(FEComp->GetChildActor()))
		{
			bool result = Item->UseItem();

			return result;
		}
	}

	return false;
}

// 절단기를 사용하는 함수.
void AHorrorGameCharacter::UseCutter()
{
	if (CutterSoundCue)
	{
		UGameplayStatics::PlaySoundAtLocation(this, CutterSoundCue, GetActorLocation());
	}
}

bool AHorrorGameCharacter::SetLanternOn(bool inIsLightOn)
{
	if (LanternComp)
	{
		// If the lantern component can be casted to soul lantern class
		if (ALightItem* Lantern = Cast<ALightItem>(LanternComp->GetChildActor()))
		{
			bool result = false;
			// Call the lantern use interact function.
			if (inIsLightOn)
			{
				result = Lantern->TurnOn();
			}
			else
			{
				result = Lantern->TurnOff();
			}

			bIsLanternOn = inIsLightOn;

			return result;
		}
	}

	return false;
}

// 야광봉을 사용하는 함수.
bool AHorrorGameCharacter::UseGlowstick()
{
	bool result = false;
	// 숨어있는 경우가 아닐 때 작동.
	if (PlayerStatComp->GetCurrentPlayerStates() == EPlayerStatus::Hiding)
	{
		return false;
	}

	if (GlowStickClass)
	{
		// 플레이어가 밟고 있는 바닥에 야광봉이 생성될 수 있도록 값을 초기화함.
		float Z = 110.f; // 122 - 110 = 12
		
		if (bIsCrouch)
		{
			Z = 45.f; // 56 - 45 = 11
		}

		FVector SpawnLocation = GetActorLocation() - FVector(0.f, 0.f, Z);
			
		// 야광봉 액터를 설정한 위치에 생성하고 아이템 사용 효과 메서드를 호출함.
		AItemClass* GlowStick = GetWorld()->SpawnActor<AItemClass>(GlowStickClass, SpawnLocation, FRotator(0.f, 0.f, 0.f));
		if (GlowStick)
		{
			result = GlowStick->UseInteract(this);
		}
	}

	return result;
}

bool AHorrorGameCharacter::UseCompass()
{
	return false;
}

AActor* AHorrorGameCharacter::GetActorToInteract()
{
	return InteractionComp->GetTracedActor();
}

// Get Functions: GetStamina(), GetFlashLightBattery(), GetCurrentItemNumber()
//				GetCurrentItemName(), GetExtinguisherLeft(), GetPatience(), GetPlayerStatus()
int32 AHorrorGameCharacter::GetStamina()
{
	return GetStatComponent()->Stamina;
}

float AHorrorGameCharacter::GetConfusionPoint()
{
	return GetStatComponent()->ConfusionPoint;
}

bool AHorrorGameCharacter::GetIsHiding()
{
	if (GetPlayerStatus() == EPlayerStatus::Hiding)
	{
		return true;
	}

	return false;
}

bool AHorrorGameCharacter::GetIsSprinting()
{
	return bIsSprinting;
}

EPlayerStatus AHorrorGameCharacter::GetPlayerStatus()
{
	return PlayerStatComp->GetCurrentPlayerStates();
}

int32 AHorrorGameCharacter::GetObjectNumbers()
{
	return GetInventoryComponent()->ObjectCount;
}

bool AHorrorGameCharacter::GetIsLightOn()
{
	return bIsLightOn;
}

// Set Functions: SetPlayerStatus()
// 플레이어의 상태를 업데이트하는 함수.
void AHorrorGameCharacter::SetPlayerStatus(EPlayerStatus PlayerStatus)
{
	if (PlayerStatComp->GetCurrentPlayerStates() != PlayerStatus)
	{
		PlayerStatComp->SetCurrentPlayerStates(PlayerStatus);

		switch (PlayerStatus)
		{
			// 로딩 중인 상태일 경우, 컨트롤러를 통해 Input이 안 되게 설정함.
			case EPlayerStatus::Loading: // 로딩 중
			{
				DisableInput(HorrorGamePlayerController);
			
				return;
			}
			// 게임이 시작된 상태일 경우, 컨트롤러를 통해 Input이 되도록 설정함.
			case EPlayerStatus::Survive: // 시작
			{
				EnableInput(HorrorGamePlayerController);

				return;
			}
			// 추격당하는 상태일 경우
			case EPlayerStatus::Chased: // 추격 시
			{
				// 추격 노이즈를 카메라에 주고, 착란 게이지가 증가할 수 있다면, 15 증가시킴.
				SetCameraComponentNoise(2);
				if (!bIsCooldown)
				{
					GetStatComponent()->AddConfusionPoint(15);
				}
				return;
			}
			// 적 개체에게 잡혔을 경우
			case EPlayerStatus::Catched: // 잡혔을 경우
			{
				// 충돌이 안 되게 설정하고, 컨트롤러를 통해 Input이 안 되게 설정함.
				SetActorEnableCollision(false);
				DisableInput(HorrorGamePlayerController);
				return;
			}
			// 리퍼에 의해 행동 불능 상태일 경우
			case EPlayerStatus::Stunned: // 리퍼에 의해 기절 시
			{
				// 이동을 즉시 멈추고, 컨트롤러를 통해 Input이 안 되게 설정함.
				DisableInput(HorrorGamePlayerController);
				GetCharacterMovement()->StopMovementImmediately();
				return;
			}
			// 숨은 상태인 경우
			case EPlayerStatus::Hiding:
			{
				
				return;
			}
			// 사망한 상태일 경우
			case EPlayerStatus::Died: // 사망
			{
				// 충돌이 안 되게 설정하고 게임에서 안 보이게 설정하고 컨트롤러를 통해 Input이 안 되게 설정함.
				SetActorEnableCollision(false);
				GetMesh()->SetHiddenInGame(true);

				// 게임 플레이 타이머를 정지시키고, 사망 UI를 뷰포트에 출력하도록 함.
				if (GameUIWidget)
				{
					GameUIWidget->SetTimerStop(true);
				}

				GetStatComponent()->TakeDamage(GetStatComponent()->MaxHP);
				
				return;
			}
			// 게임을 클리어한 상태인 경우
			case EPlayerStatus::Clear: // 클리어 및 엔딩
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
		GameUIWidget->ShowMessage(text);
	}
}

// 착란 게이지를 증가시키는 함수.
void AHorrorGameCharacter::AddConfusionPoint(int32 value)
{
	GetStatComponent()->AddConfusionPoint(value);
}

// 불빛을 깜빡이게 하는 함수.
void AHorrorGameCharacter::LightFlicker(float value)
{
	if (bIsLightOn)
	{
		float LightIntense = FMath::Clamp(CigarIntensity * value, 0.0f, CigarIntensity);

		if (ALightItem* Item = Cast<ALightItem>(CigarlighterComp->GetChildActor()))
		{
			Item->LightComponent->SetIntensity(LightIntense);
		}

		if (ALightItem* Item = Cast<ALightItem>(FlashlightComp->GetChildActor()))
		{
			Item->LightComponent->SetIntensity(LightIntense);
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

// 공격이 성공했는지 체크하는 함수.
void AHorrorGameCharacter::AttackCheck(bool value)
{
	bShouldAttack = value;
	ASword* BronzeSword = Cast<ASword>(SwordComp->GetChildActor());

	// 만약 Attack Check의 값이 false면
	if (!value)
	{
		// 공격 시작 알림을 false로
		SetNotifyAttackStart(false);
		if (BronzeSword)
		{
			BronzeSword->SetShouldExorcism(bShouldAttack);
		}
	}
	if (BronzeSword)
	{
		BronzeSword->SetShouldExorcism(bShouldAttack);
		// 요괴를 처치한 상황인 경우에, 검 개수 감소
		if (BronzeSword->bHadExorcism)
		{
			BronzeSword->SetShouldExorcism(false);
		}
	}

}

void AHorrorGameCharacter::SwitchItem(const FHorrorGameItemData& CurrentItemData, int32 CurrentItemIndex)
{
	switch (CurrentItemData.ItemType)
	{
		case EItemType::ITEM_Lighting:
		{
			auto Cigarlighter = Cast<ALightItem>(CigarlighterComp->GetChildActor());
			auto Flashlight = Cast<ALightItem>(FlashlightComp->GetChildActor());
			auto Lantern = Cast<ALightItem>(LanternComp->GetChildActor());
			if (Cigarlighter)
			{
				Cigarlighter->SetItemVisibility(true);
			}

			if (Flashlight)
			{
				Flashlight->SetItemVisibility(true);
			}

			if (Lantern)
			{
				Lantern->SetItemVisibility(true);
			}

			switch (CurrentItemData.ItemNumber)
			{
				case EItemNumber::ITEM_CigarLighter:
				{
					if (Cigarlighter)
					{
						Cigarlighter->SetItemVisibility(false);
					}
					return;
				}
				case EItemNumber::ITEM_FlashLight:
				{
					// Durability synchronization
					if (Flashlight)
					{
						Flashlight->SetItemVisibility(false);
						Flashlight->SetupDurability(CurrentItemData.Durability);
					}

					return;
				}
				case EItemNumber::ITEM_Lantern:
				{
					if (Lantern)
					{
						Lantern->SetItemVisibility(false);
					}
					return;
				}
			}

			return;
		}
		case EItemType::ITEM_Consumable:
		{
			auto Sword = Cast<AActiveItem>(SwordComp->GetChildActor());
			auto Mirror = Cast<AActiveItem>(MirrorComp->GetChildActor());
			auto FireExtinguisher = Cast<AActiveItem>(FEComp->GetChildActor());

			if (Sword)
			{
				Sword->SetItemVisibility(true);
			}

			if (Mirror)
			{
				Mirror->SetItemVisibility(true);
			}

			if (FireExtinguisher)
			{
				FireExtinguisher->SetItemVisibility(true);
			}

			switch (CurrentItemData.ItemNumber)
			{
				case EItemNumber::ITEM_Sword:
				{
					// Durability synchronization
					if (Sword)
					{
						Sword->SetItemVisibility(false);
						Sword->SetupDurability(CurrentItemData.Durability);
					}

					return;
				}
				case EItemNumber::ITEM_Mirror:
				{
					if (Mirror)
					{
						Mirror->SetItemVisibility(false);
					}

					return;
				}
				case EItemNumber::ITEM_Extinguisher:
				{
					// Durability synchronization
					if (FireExtinguisher)
					{
						FireExtinguisher->SetItemVisibility(false);
						FireExtinguisher->SetupDurability(CurrentItemData.Durability);
					}

					return;
				}
			}

			return;
		}
	}

	return;
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
		if (auto Door = Cast<ADoorClass>(OtherActor))
		{
			Door->SetDoorCollision(true);
		}

		else if (ACreatureClass* Yokai = Cast<ACreatureClass>(OtherActor))
		{
			Yokai->SetCreatureCollision(true);
		}
	}
}

// DEPRECATED
void AHorrorGameCharacter::OnDoorBoxEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor != this && OtherActor)
	{
		if (auto Door = Cast<ADoorClass>(OtherActor))
		{
			Door->SetDoorCollision(false);
		}

		else if (auto Yokai = Cast<ACreatureClass>(OtherActor))
		{
			Yokai->SetCreatureCollision(false);
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
	if (GetPlayerStatus() == EPlayerStatus::Loading)
	{
		// 생존 상태로 업데이트하고
		SetPlayerStatus(EPlayerStatus::Survive);
		
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

void AHorrorGameCharacter::Death()
{
	SetPlayerStatus(EPlayerStatus::Died);
}

// 문서 보관함 문서를 획득할 때 텍스트를 출력할 함수.
void AHorrorGameCharacter::SetArchiveGetText(FText inText)
{
	if (GetPlayerStatus() == EPlayerStatus::Died)
	{
		HorrorGamePlayerController->SetDeadUIText(inText);
	}
	else
	{
		if (GameUIWidget)
		{
			GameUIWidget->ShowMessage(inText);
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
	//SetExplainText(NSLOCTEXT("AHorrorGameCharacter", "Reaper_Unseal", "Some ominous sound comes..."), 3);
	// 번역은 "뭔가 불길한 소리가 들려온다."로
}

// 카메라 쉐이크 타임라인을 재생할 때 호출할 콜백 함수.
void AHorrorGameCharacter::OnCameraShake(float inTimeCurve)
{
	if (inTimeCurve >= 1.0f)
	{
		AHorrorGamePlayerController* PC = Cast<AHorrorGamePlayerController>(Controller);
		APlayerCameraManager* PCM = UGameplayStatics::GetPlayerCameraManager(this, 0);

		if (CameraShake)
		{
			UGameplayStatics::PlayWorldCameraShake(this, CameraShake, PCM->GetCameraLocation(), 1000.0f, 10000.0f);
		}
	}
}

// 카메라 쉐이크 타임라인을 재생하는 함수.
void AHorrorGameCharacter::StartCameraShake()
{
	CameraShakeTimeline.PlayFromStart();
}

void AHorrorGameCharacter::HandleFlashlightSway(float Value)
{
	FRotator NewRotation = FRotator(Value * WalkSwaySpeed, Value * -WalkSwaySpeed, 0.0f);
	if (bIsSprinting)
	{
		NewRotation = FRotator(Value * (SprintSwaySpeed), Value * -SprintSwaySpeed, 0.0f);
	}
	FlashlightComp->SetRelativeRotation(NewRotation);
}