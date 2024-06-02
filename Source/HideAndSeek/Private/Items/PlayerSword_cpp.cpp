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
	//CapsuleComp->OnComponentBeginOverlap.AddDynamic(this, &APlayerSword_cpp::OnSwordBeginOverlap);
	//CapsuleComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

// Called when the game starts or when spawned
void APlayerSword_cpp::BeginPlay()
{
	Super::BeginPlay();
	
//	SetActorEnableCollision(false);
	SetOnCollision(false);
}

void APlayerSword_cpp::UseInteract(class AHorrorGameCharacter* Player)
{
	if (AttackSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, AttackSound, GetActorLocation());
	}

	if (OwnerPlayer == nullptr)
	{
		OwnerPlayer = Player;
	}
}

void APlayerSword_cpp::OnSwordBeginOverlap(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherComp != nullptr && OtherActor != this && OtherActor != nullptr && OtherComp->IsA<USkeletalMeshComponent>())
	{
		if (AReaper_cpp* Reaper = Cast<AReaper_cpp>(OtherActor))
		{
			if (ExorcismSound)
			{
				UGameplayStatics::PlaySoundAtLocation(this, ExorcismSound, GetActorLocation());
			}
			Reaper->Exorcism();
			SetOnCollision(false);

			if(AHorrorGameGameMode* GameMode = Cast<AHorrorGameGameMode>(UGameplayStatics::GetGameMode(GetWorld())))
			{
				GameMode->StopChaseBackGroundMusic();
				if (OwnerPlayer->CreatureNum <= 0)
				{
					GameMode->StopNervousBackGroundMusic();
				}
			}
			bHadExorcism = true;
		}

		else if (ABrute_cpp* Brute = Cast<ABrute_cpp>(OtherActor))
		{
			if (ExorcismSound)
			{
				UGameplayStatics::PlaySoundAtLocation(this, ExorcismSound, GetActorLocation());
			}
			Brute->Exorcism();
			SetOnCollision(false);

			if (AHorrorGameGameMode* GameMode = Cast<AHorrorGameGameMode>(UGameplayStatics::GetGameMode(GetWorld())))
			{
				GameMode->StopChaseBackGroundMusic();
				if (OwnerPlayer->CreatureNum <= 0)
				{
					GameMode->StopNervousBackGroundMusic();
				}
			}
			bHadExorcism = true;
		}

		else if (ARunner_cpp* Runner = Cast<ARunner_cpp>(OtherActor))
		{
			if (ExorcismSound)
			{
				UGameplayStatics::PlaySoundAtLocation(this, ExorcismSound, GetActorLocation());
			}
			Runner->Exorcism();
			SetOnCollision(false);

			if (AHorrorGameGameMode* GameMode = Cast<AHorrorGameGameMode>(UGameplayStatics::GetGameMode(GetWorld())))
			{
				GameMode->StopChaseBackGroundMusic();
				if (OwnerPlayer->CreatureNum <= 0)
				{
					GameMode->StopNervousBackGroundMusic();
				}
			}
			bHadExorcism = true;
		}
	}
}

void APlayerSword_cpp::SetOnCollision(const bool value)
{
	if (value) // 만약 true(즉 공격 모션이 시작된 경우)
	{
		CapsuleComp->OnComponentBeginOverlap.AddDynamic(this, &APlayerSword_cpp::OnSwordBeginOverlap);
		//CapsuleComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		//SetActorEnableCollision(true);
	}
	else // 만약 false(즉 공격 모션이 끝나거나 크리쳐를 처치했거나 아닌 경우)
	{
		//SetActorEnableCollision(false);
	//	CapsuleComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		CapsuleComp->OnComponentBeginOverlap.RemoveDynamic(this, &APlayerSword_cpp::OnSwordBeginOverlap);
	}
}

void APlayerSword_cpp::SetShouldExorcism(const bool value)
{
	bShouldExorcism = value;
	SetOnCollision(bShouldExorcism); // Collision도 false로 설정
}

bool APlayerSword_cpp::GetShouldExorcism()
{
	return bShouldExorcism;
}