// CopyrightNotice 2023 Sunggon Kim kimdave205@gmail.com. All Rights Reserved.
// 타이머를 던졌을 때 땅에 닫기 전까지 나타날 액터

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

	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	RootComponent = RootComp;

	// 메시들의 기본 설정을 해줌. (세세한 설정은 블루프린트 클래스에서 수행)
	TimerMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Timer"));
	TimerMesh->SetupAttachment(RootComp);
	TimerMesh->SetRelativeLocation(DefaultLoc);
	TimerMesh->SetRelativeScale3D(DefaultScale);

	//RootComponent = TimerMesh;

	/*Collision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	Collision->SetWorldScale3D(DefaultScale / 2.0);
	Collision->SetupAttachment(TimerMesh);
	Collision->OnComponentHit.AddDynamic(this, &ATimerProjectile_cpp::OnHit);*/

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
	
	//bIsHit = true;
}

// Called every frame
void ATimerProjectile_cpp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	// 벽이나 바닥과 충돌하고 난 뒤 5초 후에 알람이 울리도록
	if (count >= 5.f)
	{
		// 타이머를 0으로 초기화하고
		count = 0;

		// 던져진 타이머 클래스가 있다면, 해당 액터를 스폰한 뒤, 울리게 함. 그 후, 이 액터를 제거함.
		if (Timer)
		{
			FActorSpawnParameters SpawnParams;
			AThrownTimer_cpp* TimerActor = GetWorld()->SpawnActor<AThrownTimer_cpp>(Timer, TimerMesh->GetComponentLocation(), TimerMesh->GetComponentRotation(), SpawnParams);
			TimerActor->RingingStart();
			Destroy();
		}
	}

	// 타이머의 카운팅을 계속 더해 4초가 되도록 함.
	count += DeltaTime;
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

// 메시가 바닥/벽이나 무언가에 부딪혔을 때 작동할 함수.
//void ATimerProjectile_cpp::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
//{
//	if (OtherActor && (OtherActor != this) && OtherActor->GetName().Contains(TEXT("Floor")))
//	{
//		//OtherComp->AddImpulseAtLocation(GetVelocity() * 100.0f, GetActorLocation());
//
//		bIsHit = true;
//	}
//}