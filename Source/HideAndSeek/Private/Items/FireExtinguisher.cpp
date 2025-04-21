// CopyrightNotice 2023 Sunggon Kim kimdave205@gmail.com. All Rights Reserved.

#include "Items/FireExtinguisher.h"
#include "Player/HorrorGameCharacter.h"
#include "ComponentAction/HorrorGameSaveGame.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/CapsuleComponent.h"
#include "Player/InventoryComponent.h"
#include "AI/CreatureClass.h"

// Sets default values
AFireExtinguisher::AFireExtinguisher()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Set Meshes default settings.
	PowderParticle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("PowderParticle"));
	PowderParticle->SetActive(false);

	PowderCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("PowderCollider"));
	PowderCapsule->SetupAttachment(MeshComponent);
}

// Called when the game starts or when spawned
void AFireExtinguisher::BeginPlay()
{
	Super::BeginPlay();

	PowderParticle->SetActive(false);

	if (GetOwner())
	{
		UE_LOG(LogTemp, Warning, TEXT("Owner: %s"), *GetOwner()->GetName());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Owner is nullptr"));
	}
}

// Called every frame
void AFireExtinguisher::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// If the powder is being sprayed.
	if (bIsSpraying)
	{
		Timer += DeltaTime;
		UE_LOG(LogTemp, Warning, TEXT("Timer Count: %f"), Timer);

		// After 5 seconds 
		if (Timer >= 5.0f)
		{
			UE_LOG(LogTemp, Warning, TEXT("5 seconds over!"));
			StopSpraying();

			if (auto InventoryComp = GetOwner()->GetComponentByClass<UInventoryComponent>())
			{
				InventoryComp->UseExtinguisher();
			}
		}
	}

}

// bool AFireExtinguisher::UseInteract(class APlayerCharacter* Player)
bool AFireExtinguisher::UseItem()
{
	Super::UseItem();

	// If the extinguisher is spraying, return fail.
	if (bIsSpraying)
	{
		return false;
	}

	bIsSpraying = true;
	UE_LOG(LogTemp, Warning, TEXT("Spray Start"));
	if (GetOwner())
	{
		if (auto InventoryComp = GetOwner()->FindComponentByClass<UInventoryComponent>())
		{
			InventoryComp->bIsExtinguisherSpraying = bIsSpraying;
		}
	}

	PowderCapsule->OnComponentBeginOverlap.AddDynamic(this, &AFireExtinguisher::OnCapsuleBeginOverlap);
	PowderParticle->SetActive(bIsSpraying);
	Timer = 0.0f;

	return true;
}

// bool AFireExtinguisher::StopSpraying(class APlayerCharacter* Player)
bool AFireExtinguisher::StopSpraying()
{
	if (bIsSpraying)
	{
		bIsSpraying = false;

		if (GetOwner())
		{
			if (auto InventoryComp = GetOwner()->FindComponentByClass<UInventoryComponent>())
			{
				InventoryComp->bIsExtinguisherSpraying = bIsSpraying;
				InventoryComp->UseExtinguisher();
			}
		}

		Durability -= 1;
		PowderCapsule->OnComponentBeginOverlap.RemoveDynamic(this, &AFireExtinguisher::OnCapsuleBeginOverlap);
		PowderParticle->SetActive(bIsSpraying);
		Timer = 0.0f;

		return true;
	}

	return false;
}

void AFireExtinguisher::OnCapsuleBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherComp != PowderParticle && OtherActor != this)
	{
		// Stun
		FHitResult OutHit;
		FVector Start = GetActorLocation();
		FVector End = OtherActor->GetActorLocation();

		// Ray Cast를 통해서 벽 뒤에서 충돌한 경우가 아닌 경우.
		if (!GetWorld()->LineTraceSingleByChannel(OutHit, Start, End, ECC_Visibility))
		{
			// 각 요괴 개체들에게 거리별로 행동 불능을 만들도록 설정함.
			if (auto Yokai = Cast<ACreatureClass>(OtherActor))
			{
				float Dist = FVector::Distance(GetActorLocation(), Yokai->GetActorLocation());
				Yokai->Stunning(Dist);
			}
		}
	}
}

void AFireExtinguisher::SetItemVisibility(bool NewVisibility)
{
	Super::SetItemVisibility(NewVisibility);

	if (PowderParticle)
	{
		PowderParticle->SetHiddenInGame(false);
	}
}