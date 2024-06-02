// CopyrightNotice 2023 Sunggon Kim kimdave205@gmail.com. All Rights Reserved.

#include "Items/TimerProjectile_cpp.h"
#include "Components/BoxComponent.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundCue.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Items/ThrownTimer_cpp.h"

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
	Collision->OnComponentHit.AddDynamic(this, &ATimerProjectile_cpp::OnHit);

	TimerMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Timer"));
	TimerMesh->SetupAttachment(Collision);
	TimerMesh->SetRelativeLocation(DefaultLoc);
	TimerMesh->SetRelativeScale3D(DefaultScale);

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovement->SetUpdatedComponent(TimerMesh);
	ProjectileMovement->InitialSpeed = 1000.f;
	ProjectileMovement->MaxSpeed = 1000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = true;
	ProjectileMovement->Bounciness = 0.3f;
//	InitialLifeSpan = 4.0f;
	bIsHit = false;
}

// Called when the game starts or when spawned
void ATimerProjectile_cpp::BeginPlay()
{
	Super::BeginPlay();
	
	bIsHit = true;
}

// Called every frame
void ATimerProjectile_cpp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsHit)
	{
		/*if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Yellow, FString::Printf(TEXT("bIsHit = true")));*/
		if (count >= 4.f) // 벽이나 바닥과 충돌하고 난 뒤 4초 후에 알람이 울리도록
		{
			/*if (GEngine)
				GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Yellow, FString::Printf(TEXT("Now Ring")));*/
			// TimerSound->Play();
			// SetPlaySound(true);
			// bIsHit = false;
			count = 0;
			if (Timer)
			{
				FActorSpawnParameters SpawnParams;
				AThrownTimer_cpp* TimerActor = GetWorld()->SpawnActor<AThrownTimer_cpp>(Timer, TimerMesh->GetComponentLocation(), TimerMesh->GetComponentRotation(), SpawnParams);
				TimerActor->RingingStart();
				Destroy();
			}
		}
		count += DeltaTime;
	}
}

//void ATimerProjectile_cpp::RangEnd() // Player Use Event
//{
//	/*if (GEngine)
//		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Yellow, FString::Printf(TEXT("Ring End")));*/
//	bPlaySound = false;
//	Destroy();
//}

void ATimerProjectile_cpp::FireInDirection(const FVector& ShootDirection)
{
	ProjectileMovement->Velocity = ShootDirection * ProjectileMovement->InitialSpeed;
}

//void ATimerProjectile_cpp::SetPlaySound(bool bValue)
//{
//	bPlaySound = bValue;
//}
//
//bool ATimerProjectile_cpp::GetPlaySound()
//{
//	return bPlaySound;
//}

void ATimerProjectile_cpp::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr))
	{
		OtherComp->AddImpulseAtLocation(GetVelocity() * 100.0f, GetActorLocation());

		bIsHit = true;
	}
}