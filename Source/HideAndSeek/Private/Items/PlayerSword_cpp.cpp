// CopyrightNotice 2023 Sunggon Kim kimdave205@gmail.com. All Rights Reserved.
// 플레이어가 직접 휘두를 칼임

#include "Items/PlayerSword_cpp.h"
#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"
#include "Components/CapsuleComponent.h"
#include "AI/Reaper_cpp.h"
#include "AI/Brute_cpp.h"
#include "AI/Runner_cpp.h"
#include "HideAndSeek/HorrorGameCharacter.h"
#include "HideAndSeek/HorrorGameGameMode.h"

// Sets default values
APlayerSword_cpp::APlayerSword_cpp()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	FVector fDefaultLoc(0.0f, 0.0f, 0.0f);
	FVector fDefaultScale(1.0f, 1.0f, 1.0f);

	// 메시들의 기본 설정을 해줌. (세세한 설정은 블루프린트 클래스에서 수행)
	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	DefaultSceneRoot->SetWorldLocation(fDefaultLoc);
	RootComponent = DefaultSceneRoot;

	Sword = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Sword"));
	Sword->SetupAttachment(DefaultSceneRoot);
	static ConstructorHelpers::FObjectFinder<UStaticMesh>SwordMesh(TEXT("/Game/Assets/Items/Sword"));
	if (SwordMesh.Succeeded())
	{
		Sword->SetStaticMesh(SwordMesh.Object);
	}
	Sword->SetRelativeLocation(fDefaultLoc);
	Sword->SetRelativeScale3D(fDefaultScale);
	Sword->SetCollisionProfileName(TEXT("ItemObjects"));

	CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("HitBox"));
	CapsuleComp->SetupAttachment(Sword);
}

// Called when the game starts or when spawned
void APlayerSword_cpp::BeginPlay()
{
	Super::BeginPlay();
	
	// 평소에는 콜리전을 제거함.
	SetOnCollision(false);
}

// 플레이어가 청동 검을 사용할 때 작동할 함수.
void APlayerSword_cpp::UseInteract(class AHorrorGameCharacter* Player)
{
	// 공격 음을 재생함.
	if (AttackSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, AttackSound, GetActorLocation());
	}

	// 소유자를 지정해줌.
	if (OwnerPlayer == nullptr)
	{
		OwnerPlayer = Player;
	}
}

// 충돌체에 액터가 충돌한 경우를 호출될 콜백 함수.
void APlayerSword_cpp::OnSwordBeginOverlap(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// 충돌체가 있을 경우
	if (OtherComp != nullptr && OtherActor != this && OtherActor != nullptr && OtherComp->IsA<USkeletalMeshComponent>())
	{
		// 그 액터가 리퍼인 경우
		if (AReaper_cpp* Reaper = Cast<AReaper_cpp>(OtherActor))
		{
			// 처리 효과음을 재생함.
			if (ExorcismSound)
			{
				UGameplayStatics::PlaySoundAtLocation(this, ExorcismSound, GetActorLocation());
			}

			// 리퍼를 처치하고 충돌체를 비활성화함.
			Reaper->Exorcism();
			SetOnCollision(false);

			// 추격 중이었다면 추격 BGM을 종료함.
			if(AHorrorGameGameMode* GameMode = Cast<AHorrorGameGameMode>(UGameplayStatics::GetGameMode(GetWorld())))
			{
				GameMode->StopChaseBackGroundMusic();
				
				// 플레이어 근처에 요괴가 없다면 긴장 BGM도 종료함.
				if (OwnerPlayer->CreatureNum <= 0)
				{
					GameMode->StopNervousBackGroundMusic();
				}
			}

			// 처치했다고 설정함.
			bHadExorcism = true;
		}
		// 그 액터가 브루트인 경우
		else if (ABrute_cpp* Brute = Cast<ABrute_cpp>(OtherActor))
		{
			// 처리 효과음을 재생함.
			if (ExorcismSound)
			{
				UGameplayStatics::PlaySoundAtLocation(this, ExorcismSound, GetActorLocation());
			}

			// 브루트를 처치하고 충돌체를 비활성화함.
			Brute->Exorcism();
			SetOnCollision(false);

			// 추격 중이었다면 추격 BGM을 종료함.
			if (AHorrorGameGameMode* GameMode = Cast<AHorrorGameGameMode>(UGameplayStatics::GetGameMode(GetWorld())))
			{
				GameMode->StopChaseBackGroundMusic();

				// 플레이어 근처에 요괴가 없다면 긴장 BGM도 종료함.
				if (OwnerPlayer->CreatureNum <= 0)
				{
					GameMode->StopNervousBackGroundMusic();
				}
			}

			// 처치했다고 설정함.
			bHadExorcism = true;
		}
		// 그 액터가 러너인 경우
		else if (ARunner_cpp* Runner = Cast<ARunner_cpp>(OtherActor))
		{
			// 처리 효과음을 재생함.
			if (ExorcismSound)
			{
				UGameplayStatics::PlaySoundAtLocation(this, ExorcismSound, GetActorLocation());
			}

			// 러너를 처치하고 충돌체를 비활성화함.
			Runner->Exorcism();
			SetOnCollision(false);

			// 추격 중이었다면 추격 BGM을 종료함.
			if (AHorrorGameGameMode* GameMode = Cast<AHorrorGameGameMode>(UGameplayStatics::GetGameMode(GetWorld())))
			{
				GameMode->StopChaseBackGroundMusic();

				// 플레이어 근처에 요괴가 없다면 긴장 BGM도 종료함.
				if (OwnerPlayer->CreatureNum <= 0)
				{
					GameMode->StopNervousBackGroundMusic();
				}
			}

			// 처치했다고 설정함.
			bHadExorcism = true;
		}
	}
}

// 검의 충돌체를 설정하는 함수.
void APlayerSword_cpp::SetOnCollision(const bool value)
{
	if (value) // 만약 true(즉 공격 모션이 시작된 경우)
	{
		CapsuleComp->OnComponentBeginOverlap.AddDynamic(this, &APlayerSword_cpp::OnSwordBeginOverlap);
	}
	else // 만약 false(즉 공격 모션이 끝나거나 크리쳐를 처치했거나 아닌 경우)
	{
		CapsuleComp->OnComponentBeginOverlap.RemoveDynamic(this, &APlayerSword_cpp::OnSwordBeginOverlap);
	}
}

// 플레이어가 칼을 휘두르는 모션을 할 때 호출되는 함수.
void APlayerSword_cpp::SetShouldExorcism(const bool value)
{
	// 모션의 시작과 끝에 따라 충돌 설정을 on/off 함.
	bShouldExorcism = value;
	SetOnCollision(bShouldExorcism);
}

bool APlayerSword_cpp::GetShouldExorcism()
{
	return bShouldExorcism;
}