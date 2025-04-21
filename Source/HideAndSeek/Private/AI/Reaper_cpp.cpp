// CopyrightNotice 2023 Sunggon Kim kimdave205@gmail.com. All Rights Reserved.

#include "AI/Reaper_cpp.h"
#include "Components/InputComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/PointLightComponent.h"
#include "Components/SpotLightComponent.h"
#include "Components/AudioComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "Components/BoxComponent.h"
#include "Sound/SoundCue.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubSystems.h"
#include "AI/AIController_Reaper.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "LevelManager/PatrolPoint_cpp.h"
#include "EngineUtils.h"
#include "AI/ReaperAnim.h"
#include "Furniture/HideObject.h"
#include "Player/HorrorGameCharacter.h"
#include "Player/HorrorGamePlayerController.h"
#include "Player/InventoryComponent.h"
#include "Furniture/Alarm.h"
#include "Animation/AnimSequence.h"
#include "Kismet/KismetMathLibrary.h"
#include "NiagaraComponent.h"
#include "LevelManager/HorrorGameGameInstance.h"
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

	YokaiSound = CreateDefaultSubobject<UAudioComponent>(TEXT("ChasedSound"));
	YokaiSound->SetupAttachment(GetMesh());
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
	YokaiSound->SetAutoActivate(false);

	GetMesh()->SetRelativeLocation(FVector(0.f, 0.f, 0.f));
	GetMesh()->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));

	AIControllerClass = AAIController_Reaper::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	GetCapsuleComponent()->SetWorldLocation(FVector(0.f, 0.f, 70.f));
	
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

	UWorld* world = GetWorld();

	// 순찰 지점을 배열에 추가
	if (bIsCollectMode)
	{
		for (TActorIterator<APatrolPoint_cpp> entity(world); entity; ++entity)
		{
			PatrolPointList.Add(*entity);
		}
		Player = Cast<AHorrorGameCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
		Player->GetInventoryComponent()->OnObjectAcquired.AddDynamic(this, &AReaper_cpp::SetCurrentStatus);
	}

	YokaiSound->SetSound(PatrolSound);

	MapName = UGameplayStatics::GetCurrentLevelName(GetWorld());
	ReaperController = Cast<AAIController_Reaper>(GetController());
	if (ReaperController)
	{
		// 수집형 모드가 아닌 경우, 바로 처치할 수 있도록 설정.
		if (!bIsCollectMode)
		{
			ReaperController->SetCurrentSealStatus(Sealed::OneUnsealed);
			if (bSealedButChase)
			{
				ReaperController->GetBlackboardComponent()->SetValueAsObject(AAIController_Reaper::TargetKey, Player);
			}
		}
	}
}

// Called every frame
void AReaper_cpp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 플레이어를 보는 상황에서 쿨타임 중도 아니고 행동 불능 상태도 아니고 시간 정지 상태도 아니라면
	if (bIsPlayerWatch && !bIsCooldown && !bIsStunned && !bIsTimeStop)
	{
		CastingTime += DeltaTime;
		// 4초간 캐스팅했다면
		if (CastingTime >= 4.f)
		{
			// 쿨타임 중으로 설정하고 플레이어가 기절되도록 설정
			bIsCooldown = true;
			CastingTime = 0.f;
			SetPlayerWatch(bIsPlayerWatch);
		}
	}

	// 스킬이 쿨타임 중이라면
	if (bIsCooldown)
	{
		SkillCooldown += DeltaTime;
		// 25초의 쿨타임이 지나면
		if (SkillCooldown >= 25.f)
		{
			// 쿨타임 종료시키고 다시 능력을 사용하게 설정
			SkillCooldown = 0.f;
			bIsCooldown = false;
			SetPlayerWatch(bIsPlayerWatch);
		}
	}

	ReaperController->SetControlRotation(GetActorRotation());
}

void AReaper_cpp::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (IsPlayerControlled())
	{
		GetCharacterMovement()->MaxWalkSpeed = 160.f;
	}
	else
	{
		if (bIsChase) GetCharacterMovement()->MaxWalkSpeed = 300.f;
		else GetCharacterMovement()->MaxWalkSpeed = 160.f;
	}
}

// 청동 검을 통해 처치될 때 호출할 함수
void AReaper_cpp::Exorcism()
{
	Super::Exorcism();

	// 나이아가라 시스템을 활성화하고, Dissolve 머티리얼 효과를 줘서 서서히 사라지는 듯한 연출을 함 
	DissolveParticleSystem->Activate(true);
	TArray<UMaterialInterface*> Materials = GetMesh()->GetMaterials();
	for (int i = 0; i < Materials.Num(); ++i)
	{
		MaterialInstances.Add(GetMesh()->CreateDynamicMaterialInstance(i, Materials[i]));
		MaterialInstances[i]->SetTextureParameterValue(TEXT("Texture"), Texture);
	}
	DissolveParticleSystem->SetVariableTexture(TEXT("Texture"), Texture);
	DissolveTimeline.PlayFromStart();
}

// Sphere 충돌체에 플레이어가 충돌한 경우 호출할 콜백 함수.
void AReaper_cpp::CatchBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherIndexBody, bool bFromSweep, const FHitResult& SweepResult)
{
	// 플레이어가 캐비닛이나 옷장에 숨어있는 걸 알고 있는지, 플레이어의 오브젝트 아이템 획득 수를 파악
	bool HideCatch = false;
	Sealed CurrentStatus = Sealed::Sealed;
	if (ReaperController != nullptr)
	{
		HideCatch = ReaperController->GetBlackboardComponent()->GetValueAsBool(AAIController_Reaper::LockerLighting);
		CurrentStatus = ReaperController->GetCurrentSealStatus();
	}

	// Runner가 행동 불능이거나 죽은 상태, 시간 정지 상태가 아니라면 실행함
	if (!bIsStunned && !bIsDied && !bIsTimeStop)
	{
		// 충돌한 액터가 존재할 경우
		if (OtherActor != this && OtherActor != nullptr && OtherComp != nullptr)
		{
			// 그 액터가 숨을 수 있는 액터이면
			if (auto HideObject = Cast<AHideObject>(OtherActor))
			{
				// 현재 추격 중이고 숨어있는 것을 알고 있는 상태에서 잡았다면
				if (bIsChase && HideCatch)
				{
					// 모든 능력이 봉인된 상태가 아니라면
					if (CurrentStatus != Sealed::Sealed)
					{
						// Catch 했음을 알리고 옷장을 잡았다고 함수를 호출함
						SetIsCatch(true);
						bIsHidingCatch = true;
						DetectPlayerHidingObject(HideObject);
					}
				}
			}
			// 그 액터가 플레이어면
			if (auto PlayerCharacter = Cast<AHorrorGameCharacter>(OtherActor))
			{
				// 플레이어의 상태가 Survive이거나, 추격 중(Chased)이면서 동시에 숨은 상태가 아닌 경우에만 수행
				if ((PlayerCharacter->GetPlayerStatus() == EPlayerStatus::Survive || PlayerCharacter->GetPlayerStatus() == EPlayerStatus::Chased) && !PlayerCharacter->GetIsHiding())
				{
					// 리퍼의 상태가 봉인된 상태면 패닉 게이지 증가시키고 소멸함
					if (CurrentStatus == Sealed::Sealed)
					{
						PlayerCharacter->CreatureNum--;
						PlayerCharacter->GetStatComponent()->AddConfusionPoint(20);
						PlayerCharacter->GetStatComponent()->TakeDamage(20);
						Destroy();
					}
					// 플레이어가 오브젝트 아이템을 하나라도 획득한 이후라면
					else
					{
						// 플레이어의 상태를 Catch로 설정함.
						PlayerCharacter->SetPlayerStatus(EPlayerStatus::Catched);
						
						if (UHorrorGameGameInstance* GameInstance = Cast<UHorrorGameGameInstance>(GetGameInstance()))
						{
							//GameInstance->Creature
							// 추가로 SaveGame 파일을 불러와서						
							if (UHorrorGameSaveGame* SaveData = GameInstance->GetSaveData())
							{
								// 플레이어가 처음 Reaper에게 잡힌 것이라면
								if (!SaveData->CollectArchives.CatchedByReaper)
								{
									// Reaper에게 잡혔다고 설정하고, SetArchiveGetText 메서드를 통해 Reaper의 문서가 추가되었다고 알림.
									SaveData->CollectArchives.CatchedByReaper = true;
									PlayerCharacter->SetArchiveGetText(NSLOCTEXT("AReaper_cpp", "Kill_By_Reaper", "Reaper\nis added in archive"));
									SaveData->SaveData();
								}
							}
						}

						// 그 후 플레이어의 카메라를 강제로 워치 포인트를 향하도록 옮기고
						PlayerCharacter->OnFocus(WatchPoint->GetComponentLocation());

						// 잡았다고 알림.
						SetIsCatch(true);
					}
				}
			}
			// 그 액터가 경보기일 경우
			if (auto Alarm = Cast<AAlarm>(OtherActor))
			{
				// 상호작용을 수행함.
				Alarm->AIInteract();
			}
		}
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

// 리퍼의 봉인 상태를 설정하는 함수
void AReaper_cpp::SetCurrentStatus(int32 Status)
{
	// 현재 상태는 봉인 상태로 설정
	Sealed CurrentStatus = Sealed::Sealed;

	switch (Status)
	{
		// 오브젝트 아이템을 0개 습득한 상태면 봉인
		case 0:// 봉인된 상태임. 착란 게이지 증가 및 HP 감소밖에 못하는 상태.
		{
			CurrentStatus = Sealed::Sealed;
			break;
		}
		// 1개나 2개 획득한 경우 1번 능력 봉인 해제(플레이어 처치 가능)
		case 1: // 하나 봉인이 해제된 상태임. 처치 가능.
		case 2: 
		{
			CurrentStatus = Sealed::OneUnsealed;
			break;
		}
		// 3개나 4개 획득한 경우 2번 능력 봉인 해제(플레이어 행동 불능 능력 사용 가능)
		case 3: // 이지 모드에선 여기가 마지막 봉인 해제가 될 것.
		case 4:
		{
			CurrentStatus = Sealed::TwoUnsealed;
			break;
		}
		// 5개 획득한 경우 3번 능력 봉인 해제(천리안을 사용해 플레이어 위치를 항상 파악)
		case 5: // 노말 이상의 모드에선 여기가 마지막 봉인 해제가 될 것.
		{
			CurrentStatus = Sealed::Unsealed;
			ReaperController->GetBlackboardComponent()->SetValueAsObject(AAIController_Reaper::TargetKey, Player);
			break;
		}
		// 프롤로그 챕터를 위한 상태.
		case 6: // 플레이어를 향해 계속 추격을 하며, 처치도 가능한 특수 상태.
		{
			CurrentStatus = Sealed::SealedButChase;
			bSealedButChase = true;
			StartChase();
			ReaperController->GetBlackboardComponent()->SetValueAsObject(AAIController_Reaper::TargetKey, Player);
			Player->SetCameraComponentNoise(2);
			GetCharacterMovement()->MaxWalkSpeed = 220.f;
			break;
		}
	}
	ReaperController->SetCurrentSealStatus(CurrentStatus);
}

// Exorcism될 때 Dissolve Material을 구현하기 위한 타임라인 콜백 함수.
void AReaper_cpp::ChangeMaterialInstance(float inValue)
{
	Super::ChangeMaterialInstance(inValue);

	// 1에서 0으로 서서히 Curve Float에 따라 감소하면서 머티리얼의 파라미터와 나이아가라 파티클의 파라미터를 변경함.
	float Amount = FMath::Lerp(1.0f, 0.0f, inValue);

	for (int i = 0; i < MaterialInstances.Num(); ++i)
	{
		MaterialInstances[i]->SetScalarParameterValue(TEXT("Amount"), Amount);
	}

	DissolveParticleSystem->SetVariableFloat(TEXT("Amount"), Amount);
}

// 타임라인 종료 후 호출할 콜백 함수.
void AReaper_cpp::DissolveFinish()
{
	Super::DissolveFinish();

	// 파티클을 비활성화시키고 해당 캐릭터를 제거함
	DissolveParticleSystem->SetVariableFloat(TEXT("Width"), 0.0f);

	DissolveParticleSystem->Deactivate();

	Destroy();
}