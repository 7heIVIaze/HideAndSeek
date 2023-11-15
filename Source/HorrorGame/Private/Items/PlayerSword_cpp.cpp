// CopyrightNotice=0 2023 Sunggon Kim kimdave205@gmail.com
// 플레이어가 직접 휘두를 칼임

#include "Items/PlayerSword_cpp.h"
#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"
#include "Components/CapsuleComponent.h"
#include "AI/Reaper_cpp.h"
#include "AI/Brute_cpp.h"
#include "AI/Runner_cpp.h"
#include "HorrorGame/HorrorGameCharacter.h"

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
}

// Called when the game starts or when spawned
void APlayerSword_cpp::BeginPlay()
{
	Super::BeginPlay();
	
}

void APlayerSword_cpp::UseInteract(class AHorrorGameCharacter* Player)
{
	/* bool bIsAttack = Player->GetAttackCheck();
	 SetOnCollision(bIsAttack);*/
	//if (!GetShouldExorcism())
	//{
	//	SetOnCollision(false);
	//	// SetShouldExorcism(false);
	//	Player->SwordCount--;
	//}
	if (OwnerPlayer == nullptr)
	{
		OwnerPlayer = Player;
	}
}

void APlayerSword_cpp::OnSwordBeginOverlap(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherComp != nullptr && OtherActor != this && OtherActor != nullptr)
	{
		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString::Printf(TEXT("Something Overlapped on Sword.")));
		if (AReaper_cpp* Reaper = Cast<AReaper_cpp>(OtherActor))
		{
			if (GEngine)
				GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString::Printf(TEXT("That is a Reaper.")));
			Reaper->Exorcism();
			bHadExorcism = true;
			//SetShouldExorcism(false);
		}

		else if (ABrute_cpp* Brute = Cast<ABrute_cpp>(OtherActor))
		{
			if (GEngine)
				GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString::Printf(TEXT("That is a Brute.")));
			Brute->Exorcism();
			bHadExorcism = true;
			//SetShouldExorcism(false);
		}

		else if (ARunner_cpp* Runner = Cast<ARunner_cpp>(OtherActor))
		{
			if (GEngine)
				GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString::Printf(TEXT("That is a Runner.")));
			Runner->Exorcism();
			bHadExorcism = true;
			//SetShouldExorcism(false);
		}
	}
}

void APlayerSword_cpp::SetOnCollision(const bool value)
{
	if (value) // 만약 true(즉 공격 모션이 시작된 경우)
	{
		CapsuleComp->OnComponentBeginOverlap.AddDynamic(this, &APlayerSword_cpp::OnSwordBeginOverlap);
	}
	else // 만약 false(즉 공격 모션이 끝나거나 아닌 경우)
	{
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