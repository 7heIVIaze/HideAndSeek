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
	// PowderParticle->SetupAttachment(MeshComponent);
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

			//if (Durability <= 0)
			//{
			//}

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
//
//// Sets default values
//AFireExtinguisher::AFireExtinguisher()
//{
// 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
//	PrimaryActorTick.bCanEverTick = false;
//	FVector DefaultPos = FVector(0.f, 0.f, 0.f);
//	FVector DefaultScale = FVector(1.f, 1.f, 1.f);
//
//	// 메시들의 기본 설정을 해줌. (세세한 설정은 블루프린트 클래스에서 수행)
//	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
//	DefaultSceneRoot->SetWorldLocation(DefaultPos);
//	DefaultSceneRoot->SetWorldScale3D(DefaultScale);
//	RootComponent = DefaultSceneRoot;
//
//	ExtinguisherMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Extinguisher"));
//	ExtinguisherMesh->SetupAttachment(DefaultSceneRoot);
//	static ConstructorHelpers::FObjectFinder<UStaticMesh>ExtinguisherObj(TEXT("/Game/Assets/Items/FireExtinguisher"));
//	if (ExtinguisherObj.Succeeded())
//	{
//		ExtinguisherMesh->SetStaticMesh(ExtinguisherObj.Object);
//	}
//	ExtinguisherMesh->SetRelativeLocation(DefaultPos);
//	ExtinguisherMesh->SetRelativeScale3D(FVector(2.4f, 2.4f, 2.0f));
//	ExtinguisherMesh->SetCollisionProfileName("ItemObjects");
//}
//
//// 플레이어가 소화기와 상호작용할 때 작동할 함수.
//void AFireExtinguisher::OnInteract(class AHorrorGameCharacter* Player)
//{
//	Super::OnInteract(Player);
//
//	// 플레이어의 소화기를 얻는 메서드를 호출함.
//	Player->AddExtinguisher();
//
//	// 위 메서드를 통해 플레이어가 아이템을 얻을 수 있는 상태이면
//	if (Player->bCanItemGet)
//	{
//		// 소화기를 처음 얻은 상태라면 소화기 문서를 세이브 데이터에 영구히 저장함.
//		if (UHorrorGameSaveGame* SaveData = UHorrorGameSaveGame::LoadObject(this, TEXT("Player"), 0))
//		{
//			if (!SaveData->CollectArchives.Item8_Extinguisher)
//			{
//				SaveData->CollectArchives.Item8_Extinguisher = true;
//				Player->SetArchiveGetText(NSLOCTEXT("AFireExtinguisher", "Get_Extinguisher", "Extinghuisher\nis added in archive"));
//				SaveData->SaveData();
//			}
//		}
//
//		// 그 후 배치된 이 액터를 제거함.
//		Destroy();
//	}
//}
//
//void AFireExtinguisher::UseInteract(class AHorrorGameCharacter* Player)
//{
//	Super::UseInteract(Player);
//}