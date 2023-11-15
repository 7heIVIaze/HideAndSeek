// CopyrightNotice=0 2023 Sunggon Kim kimdave205@gmail.com

#include "Items/TimerProjectile_cpp.h"
#include "Components/BoxComponent.h"
#include "Components/AudioComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

// Sets default values
ATimerProjectile_cpp::ATimerProjectile_cpp()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	FVector DefaultLoc = FVector(0.f, 0.f, 0.f);
	FVector DefaultScale = FVector(1.f, 1.f, 1.f);

	Collision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	Collision->SetWorldScale3D(DefaultScale / 2.0);
	RootComponent = Collision;

	TimerMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Timer"));
	TimerMesh->SetupAttachment(Collision);
	TimerMesh->SetRelativeLocation(DefaultLoc);
	TimerMesh->SetRelativeScale3D(DefaultScale);

	TimerSound = CreateDefaultSubobject<UAudioComponent>(TEXT("TimerSound"));
	TimerSound->SetupAttachment(TimerMesh);
	static ConstructorHelpers::FObjectFinder<USoundBase>TimerSoundObj(TEXT("/Game/Assets/Sounds/SoundCues/Timer_Alarm1_Cue"));
	if (TimerSoundObj.Succeeded())
	{
		TimerSound->SetSound(TimerSoundObj.Object);
	}
	TimerSound->SetAutoActivate(false);
	// TimerSound->IsPlaying
	// TimerSound->GetPlayState();

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovement->SetUpdatedComponent(TimerMesh);
	ProjectileMovement->InitialSpeed = 1000.f;
	ProjectileMovement->MaxSpeed = 1000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = true;
	ProjectileMovement->Bounciness = 0.3f;
//	InitialLifeSpan = 4.0f;
}

// Called when the game starts or when spawned
void ATimerProjectile_cpp::BeginPlay()
{
	Super::BeginPlay();
	
	GetWorldTimerManager().SetTimer(_TimerSoundHandle, FTimerDelegate::CreateLambda([&]() {
		TimerSound->Play();
		// SetPlaySound(true);
		GetWorldTimerManager().SetTimer(_TimerLifeHandle, this, &ATimerProjectile_cpp::RangEnd, 14.f, false);
	}), 5.f, false);

}

// Called every frame
void ATimerProjectile_cpp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (TimerSound->GetPlayState() == EAudioComponentPlayState::Playing)
		bPlaySound = true;
}

void ATimerProjectile_cpp::RangEnd() // Player Use Event
{
	Destroy();
}

void ATimerProjectile_cpp::FireInDirection(const FVector& ShootDirection)
{
	ProjectileMovement->Velocity = ShootDirection * ProjectileMovement->InitialSpeed;
}

void ATimerProjectile_cpp::SetPlaySound(bool bValue)
{
	bPlaySound = bValue;
}

bool ATimerProjectile_cpp::GetPlaySound()
{
	return bPlaySound;
}