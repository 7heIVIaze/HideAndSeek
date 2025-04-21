// CopyrightNotice 2023 Sunggon Kim kimdave205@gmail.com. All Rights Reserved.

#include "Items/TimerProjectile_cpp.h"
#include "Components/BoxComponent.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundCue.h"
#include "GameFramework/ProjectileMovementComponent.h"

// Sets default values
ATimerProjectile_cpp::ATimerProjectile_cpp()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 메시들의 기본 설정을 해줌. (세세한 설정은 블루프린트 클래스에서 수행)
	TimerMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Timer"));
	RootComponent = TimerMesh;

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovement->SetUpdatedComponent(TimerMesh);
	ProjectileMovement->InitialSpeed = 800.f;
	ProjectileMovement->MaxSpeed = 800.f;
	ProjectileMovement->bShouldBounce = true;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->Bounciness = 0.3f;
	ProjectileMovement->Friction = 0.5f;
	ProjectileMovement->BounceVelocityStopSimulatingThreshold = 10.0f;

	TimerSound = CreateDefaultSubobject<UAudioComponent>(TEXT("Sound"));
	TimerSound->SetupAttachment(RootComponent);
	static ConstructorHelpers::FObjectFinder<USoundCue>TimerSoundObj(TEXT("/Game/Assets/Sounds/SoundCues/Timer_Alarm1_Cue"));
	if (TimerSoundObj.Succeeded())
	{
		TimerSound->SetSound(TimerSoundObj.Object);
	}
	TimerSound->SetAutoActivate(false);
	TimerSound->OnAudioFinished.AddDynamic(this, &ATimerProjectile_cpp::RingingEnd);
}

// Called when the game starts or when spawned
void ATimerProjectile_cpp::BeginPlay()
{
	Super::BeginPlay();

	if (ProjectileMovement)
	{
		ProjectileMovement->OnProjectileStop.AddDynamic(this, &ATimerProjectile_cpp::RingingStart);
	}
}

// Called every frame
void ATimerProjectile_cpp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATimerProjectile_cpp::FireInDirection(const FVector& ShootDirection)
{
	ProjectileMovement->Velocity = ShootDirection * ProjectileMovement->InitialSpeed;
}

// 타이머가 울리는 효과를 시작할 함수.
void ATimerProjectile_cpp::RingingStart(const FHitResult& ImpactResult)
{
	// 타이머 소리를 재생하고, 소리가 울리고 있음을 설정함.
	TimerSound->Play();
	bIsSoundPlaying = true;
}

// 오디오 컴포넌트가 종료되면 호출될 콜백 함수.
void ATimerProjectile_cpp::RingingEnd()
{
	// 소리고 멈췄음을 설정하고, 이 액터를 제거함.
	bIsSoundPlaying = false;
	Destroy();
}

bool ATimerProjectile_cpp::GetIsSoundPlaying()
{
	return bIsSoundPlaying;
}