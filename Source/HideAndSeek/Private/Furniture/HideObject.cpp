// CopyrightNotice 2023 Sunggon Kim kimdave205@gmail.com. All Rights Reserved.

#include "Furniture/HideObject.h"
#include "Camera/CameraComponent.h"
#include "Components/PointLightComponent.h"
#include "Components/SpotLightComponent.h"
#include "Components/AudioComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Sound/SoundCue.h"
#include "Player/HorrorGameCharacter.h"
#include "Player/InventoryComponent.h"
#include "Player/PlayerStatComponent.h"
#include "Items/SoulLantern.h"

// Sets default values
AHideObject::AHideObject()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	// 불빛 깜빡임 등을 관리해야하므로 active시킴.
	PrimaryActorTick.bCanEverTick = true;

	// Root Component 설정용 컴포넌트임.
	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = RootComp;

	// 캐비닛 메시임. 뒤에서 못 열게 하기 위해 설정함.
	ObjectMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	ObjectMesh->SetupAttachment(RootComp);

	// 플레이어처럼 행동할 수 있게 하기 위한 카메라 컴포넌트임. 플레이어를 넣을 수 있으면 넣어보고 싶긴 함.
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(RootComp);

	// 플레이어처럼 라이터 켜기용 컴포넌트
	CigarlighterComp = CreateDefaultSubobject<UChildActorComponent>(TEXT("Cigarlighter"));
	CigarlighterComp->SetupAttachment(Camera);
	/*CigarLight = CreateDefaultSubobject<UPointLightComponent>(TEXT("CigarLight"));
	CigarLight->SetupAttachment(Camera);
	CigarLight->SetVisibility(false);
	CigarLight->SetIntensity(800.0f);
	CigarLight->SetLightFColor(FColor(255.0f, 188.0f, 124.0f));*/

	// 플레이어처럼 플래시 켜기 용 컴포넌트
	FlashlightComp = CreateDefaultSubobject<UChildActorComponent>(TEXT("Flashlight"));
	FlashlightComp->SetupAttachment(Camera);
	/*FlashLight = CreateDefaultSubobject<USpotLightComponent>(TEXT("FlashLight"));
	FlashLight->SetupAttachment(Camera);
	FlashLight->SetVisibility(false);
	FlashLight->SetIntensity(800.0f);
	FlashLight->SetInnerConeAngle(15.0f);
	FlashLight->SetOuterConeAngle(30.0f);*/

	// 플레이어의 영혼 랜턴
	LanternComp = CreateDefaultSubobject<UChildActorComponent>(TEXT("SoulLantern"));
	LanternComp->SetupAttachment(Camera);

	// 숨을 때 낼 소리
	Sound = CreateDefaultSubobject<UAudioComponent>(TEXT("HidingSound"));
	Sound->SetupAttachment(Camera);
	Sound->SetSound(SoundCue);
	Sound->SetAutoActivate(false);

	PlayerOutPoint = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PlayerOutPoint"));
	PlayerOutPoint->SetupAttachment(ObjectMesh);
}

// Called when the game starts or when spawned
void AHideObject::BeginPlay()
{
	Super::BeginPlay();

	Pitch = OriginRotation.Pitch;
	Yaw = OriginRotation.Yaw;

	// 근처 요괴가 있을 때, 불빛이 깜빡이는 것을 구현하기 위한 타임 라인
	if (CurveFloat)
	{
		FOnTimelineFloat TimelineProgress;
		TimelineProgress.BindDynamic(this, &AHideObject::LightFlicker);
		FlickeringLight.AddInterpFloat(CurveFloat, TimelineProgress);
		FlickeringLight.SetLooping(true);
	}
}

// Called every frame
void AHideObject::Tick(float DeltaTime)
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
			//// 플레이어가 라이터를 켰는데 라이터를 켠 상태가 아닐 경우 라이터를 켬
			//if (bIsCigarLightOn != Player->bIsCigarLightOn)
			// 플레이어가 불은 켠 상태인 경우
			if (Player->bIsLightOn)
			{
				EItemNumber CurrentSelectedLightingItem = Player->GetInventoryComponent()->Lighting_Inventory[Player->GetInventoryComponent()->CurrentLightingItemIndex].ItemNumber;

				// 현재 선택 중인 아이템이 라이터인 경우 라이터를 켜게 함.
				if(CurrentSelectedLightingItem == EItemNumber::ITEM_CigarLighter)
				{
					if (!bIsCigarLightOn)
					{
						SetCigarLightOn();
					}
				}
				// 현재 선택 중인 아이템이 손전등인 경우, 손전등을 켜게 함.
				else if (CurrentSelectedLightingItem == EItemNumber::ITEM_FlashLight)
				{
					if (!bIsFlashLightOn)
					{
						SetFlashLightOn();
					}
				}
			}

			//// 플레이어가 플래시를 켰는데 플래시를 켠 상태가 아닐 경우, 플래시를 켬
			//if (bIsFlashLightOn != Player->bIsFlashLightOn)
			//{
			//	SetFlashLightOn();
			//}

			// 플레이어가 랜턴을 켰는데 랜턴을 켠 상태가 아닐 경우, 랜턴을 켬
			if (bIsLanternOn != Player->bIsLanternOn)
			{
				SetLanternOn();
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

			/*if (Player->GetInventoryComponent()-> == 10)
			{
				Lantern->SetVisibility(true);
			}
			else
			{
				Lantern->SetVisibility(false);
			}*/
		}
	}
}

// 플레이어가 클릭을 해 상호작용 시
void AHideObject::OnInteract(class AHorrorGameCharacter* PlayerCharacter)
{
	// 숨거나 나오기를 하고 사운드를 재생함.
	ToggleHide(PlayerCharacter);
	if (Sound)
	{
		Sound->Play();
	}
}

// 플레이어가 캐비닛에 숨거나 나오는 로직을 구현한 함수
void AHideObject::ToggleHide(class AHorrorGameCharacter* PlayerCharacter)
{
	APlayerController* PlayerController = Cast<APlayerController>(PlayerCharacter->GetController());

	// 숨어있다가 나올 때
	if (bIsHiding) // Hide on Cabinet
	{	// True
		// 숨었다는 정보를 없애고, 숨었을 때의 로직을 반대로 구현함.
		Player->GetStatComponent()->SetCurrentPlayerStates(EPlayerStatus::Survive);
		//Player->bIsHiding = false;
		bIsHiding = false;
		Player->SetActorLocation(PlayerOutPoint->GetComponentLocation());
		Player->GetInventoryComponent()->OnItemSwitch.RemoveDynamic(this, &AHideObject::SwitchItem);
		Player->SetActorHiddenInGame(false);

		// 메인 카메라를 캐비닛 카메라에서 플레이어의 카메라로 전환함.
		PlayerController->SetViewTargetWithBlend(Player, 0.2f);
		Pitch = OriginRotation.Pitch;
		Yaw = OriginRotation.Yaw;
		Camera->SetRelativeRotation(OriginRotation);

		//FRotator NewRotation = Camera->GetComponentRotation() - PlayerRotation;
		FRotator NewRotation = Camera->GetComponentRotation();
		NewRotation.Yaw += 90.0f;
		UE_LOG(LogTemp, Warning, TEXT("NewRotationt: %s"), *NewRotation.ToString());


		PlayerCharacter->SetActorRotation(Camera->GetComponentRotation());
		PlayerController->SetControlRotation(Camera->GetComponentRotation());

		// 모든 변수와 컴포넌트의 상태를 다 초기화함
		FlickeringLight.Stop();
		if (ALightItem* Flashlight = Cast<ALightItem>(FlashlightComp->GetChildActor()))
		{
			if (Flashlight->bIsLightOn)
			{
				UE_LOG(LogTemp, Warning, TEXT("HideObject: Casting Success"));
				Flashlight->TurnOff();
			}
		}
		FlashlightComp->SetVisibility(false);

		if (ALightItem* Cigarlighter = Cast<ALightItem>(CigarlighterComp->GetChildActor()))
		{
			if (Cigarlighter->bIsLightOn)
			{
				UE_LOG(LogTemp, Warning, TEXT("HideObject: Casting Success"));
				Cigarlighter->TurnOff();
			}
		}
		CigarlighterComp->SetVisibility(false);
		
		if (ALightItem* SoulLantern = Cast<ALightItem>(LanternComp->GetChildActor()))
		{
			if (SoulLantern->bIsLightOn)
			{
				UE_LOG(LogTemp, Warning, TEXT("HideObject: Casting Success"));
				SoulLantern->TurnOff();
			}
		}
		LanternComp->SetVisibility(false);

		bIsFlashLightOn = false;
		bIsCigarLightOn = false;
		bIsLanternOn = false;
		Player = nullptr;
	}
	// 숨는 경우
	else
	{	// False
		// 플레이어의 데이터를 얻기 위해 null 포인터일 경우 플레이어를 지정해줌.
		if (Player == nullptr)
		{
			Player = PlayerCharacter;
		}

		// 숨었다는 상태로 만듦.
		Player->GetStatComponent()->SetCurrentPlayerStates(EPlayerStatus::Hiding);

		Player->GetInventoryComponent()->OnItemSwitch.AddDynamic(this, &AHideObject::SwitchItem);

		//Player->bIsHiding = true;
		bIsHiding = true;
		UE_LOG(LogTemp, Warning, TEXT("Player Rotation Input: Roll-%f Pitch-%f Yaw-%f"), PlayerController->RotationInput.Roll, PlayerController->RotationInput.Pitch, PlayerController->RotationInput.Yaw);
		UE_LOG(LogTemp, Warning, TEXT("Player Rotation Input: %s"), *PlayerController->RotationInput.ToString());
		
		PlayerRotation = PlayerCharacter->GetActorRotation();

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

bool AHideObject::UseInteract(class AHorrorGameCharacter* PlayerCharacter)
{
	if (bIsHiding)
	{

	}
	return false;
}

// 플래시를 켜는 함수
void AHideObject::SetFlashLightOn()
{
	// 플레이어 변수가 null 포인터가 아닐 때 == 플레이어가 숨어있는 상태일 때
	if (Player)
	{
		// 플레이어가 플래시를 켠 상태면 똑같이 켬
		if (ALightItem* Flashlight = Cast<ALightItem>(FlashlightComp->GetChildActor()))
		{
			UE_LOG(LogTemp, Warning, TEXT("HideObject: Casting Success"));
			if (Player->bIsLightOn)
			{
				bIsFlashLightOn = true;
				Flashlight->TurnOn();
			}
			else
			{
				bIsFlashLightOn = false;
				Flashlight->TurnOff();
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("HideObject: Casting Error"));
		}
		//if (Player->bIsLightOn)
		//{
		//	bIsFlashLightOn = true;
		//	FlashLight->SetVisibility(true);
		//}
		//// 아니라면 똑같이 끔
		//else
		//{
		//	bIsFlashLightOn = false;
		//	FlashLight->SetVisibility(false);
		//}
	}
}

// 라이터를 켜는 함수
void AHideObject::SetCigarLightOn()
{
	// 플레이어 변수가 null 포인터가 아닐 때 == 플레이어가 숨어있는 상태일 때
	if (Player)
	{
		UE_LOG(LogTemp, Warning, TEXT("HideObject: SetCigarLightOn Called"));
		// 플레이어가 라이터를 켠 상태면 똑같이 켬
		if (ALightItem* Cigarlighter = Cast<ALightItem>(CigarlighterComp->GetChildActor()))
		{
			UE_LOG(LogTemp, Warning, TEXT("HideObject: Casting Success"));
			if (Player->bIsLightOn)
			{
				bIsCigarLightOn = true;
				Cigarlighter->TurnOn();
			}
			else
			{
				bIsCigarLightOn = false;
				Cigarlighter->TurnOff();
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("HideObject: Casting Error"));
		}
		//if (Player->bIsLightOn)
		//{
		//	bIsCigarLightOn = true;
		//	CigarLight->SetVisibility(true);
		//}
		//// 아니라면 똑같이 끔
		//else
		//{
		//	bIsCigarLightOn = false;
		//	CigarLight->SetVisibility(false);
		//}
	}
}

// 랜턴을 켜는 함수
void AHideObject::SetLanternOn()
{
	// 플레이어 변수가 null 포인터가 아닐 때 == 플레이어가 숨어있는 상태일 때
	if (Player)
	{
		UE_LOG(LogTemp, Warning, TEXT("HideObject: SetLanternOn Called"));
		// 플레이어가 라이터를 켠 상태면 똑같이 켬
		bIsLanternOn = Player->bIsLanternOn;

		if (ALightItem* SoulLantern = Cast<ALightItem>(LanternComp->GetChildActor()))
		{
			UE_LOG(LogTemp, Warning, TEXT("HideObject: Casting Success"));
			if (bIsLanternOn)
			{
				SoulLantern->TurnOn();
			}
			else
			{
				SoulLantern->TurnOff();
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("HideObject: Casting Error"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("HideObject: SetLanternOn Failed"));

	}
}

// 카메라를 움직이게 하는 함수. 플레이어를 child actor로 박아볼까 생각 중. 실험이 성공하면 deprecate할 것.
void AHideObject::MoveCamera()
{
	if (bIsHiding)
	{
		Camera->SetRelativeRotation(UKismetMathLibrary::MakeRotator(0, Pitch, Yaw));
		/*GetWorld()->GetTimerManager().SetTimer(_loopCameraTimerHandle, this, &AHideObject::MoveCamera, 0.1f, false);
		GetWorld()->GetTimerManager().ClearTimer(_loopCameraTimerHandle);*/
	}
}

// 요괴들로 인해 캐비닛이 부서지는 로직
void AHideObject::BreakHideObject()
{
	// 플레이어가 숨은 상태가 아니면 작동 안되게 함.
	if (Player != nullptr)
	{
		// 플레이어를 내보내고
		ToggleHide(Player);
		// 부서짐. -> 소리도 재생되게 해야 함.
		if (BreakSound)
		{
			UGameplayStatics::PlaySoundAtLocation(this, BreakSound, GetActorLocation());
		}
		Destroy();
	}
}

// 불빛을 깜빡이게 할 타임라인의 바인딩될 함수
void AHideObject::LightFlicker(float value)
{
	// 빛 세기는 원 밝기 세기와 타임 라인의 CurveFloat의 곱임. 범위는 0에서 원 밝기 세기 사이로 잡아줌.
	float LightIntense = FMath::Clamp(Intensity * value, 0.0f, Intensity);

	// 라이터가 켜진 경우엔 라이터 불을 깜빡이게 함.
	if (bIsCigarLightOn)
	{
		Cast<ALightItem>(CigarlighterComp->GetChildActor())->LightComponent->SetIntensity(LightIntense);
	}

	// 플래시가 켜진 경우엔 플레시 불을 깜빡이게 함.
	if (bIsFlashLightOn)
	{
		Cast<ALightItem>(FlashlightComp->GetChildActor())->LightComponent->SetIntensity(LightIntense);
	}
	//if (bIsCigarLightOn)
	//{
	//	CigarLight->SetIntensity(LightIntense);
	//}

	//if (bIsFlashLightOn)
	//{
	//	FlashLight->SetIntensity(LightIntense);
	//}
}

// 카메라 노이즈를 관리할 함수.
void AHideObject::SetCameraComponentNoise(int32 WhichStatus)
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

void AHideObject::SwitchItem(const FHorrorGameItemData& CurrentItemData, int32 CurrentItemIndex)
{
	switch (CurrentItemData.ItemType)
	{
		case EItemType::ITEM_Lighting:
		{
			auto Cigarlighter = Cast<ALightItem>(CigarlighterComp->GetChildActor());
			auto Flashlight = Cast<ALightItem>(FlashlightComp->GetChildActor());
			auto Lantern = Cast<ALightItem>(LanternComp->GetChildActor());
			//CigarlighterComp->SetHiddenInGame(true);
			if (Cigarlighter)
			{
				Cigarlighter->SetItemVisibility(true);
			}

			//FlashlightComp->SetHiddenInGame(true);
			if (Flashlight)
			{
				Flashlight->SetItemVisibility(true);
			}

			//LanternComp->SetHiddenInGame(true);
			if (Lantern)
			{
				Lantern->SetItemVisibility(true);
			}

			switch (CurrentItemData.ItemNumber)
			{
				case EItemNumber::ITEM_CigarLighter:
				{
					//CigarlighterComp->SetHiddenInGame(false);
					if (Cigarlighter)
					{
						Cigarlighter->SetItemVisibility(false);
					}
					return;
				}
				case EItemNumber::ITEM_FlashLight:
				{
					//FlashlightComp->SetHiddenInGame(false);

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
					//LanternComp->SetHiddenInGame(false);

					if (Lantern)
					{
						Lantern->SetItemVisibility(false);
					}
					return;
				}
			}

			return;
		}
	}

	return;
}