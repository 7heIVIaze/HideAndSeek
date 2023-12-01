// CopyrightNotice=0 2023 Sunggon Kim kimdave205@gmail.com

#include "Altar_cpp.h"
#include "HorrorGame/HorrorGameCharacter.h"
#include "AI/Reaper_cpp.h"
#include "End_Mirror.h"

// Sets default values
AAltar_cpp::AAltar_cpp()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = RootComp;

	Altar = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Altar"));
	Altar->SetupAttachment(RootComp);
	static ConstructorHelpers::FObjectFinder<UStaticMesh>SM_Altar(TEXT("/Game/Assets/Furniture/Altar"));
	if (SM_Altar.Succeeded())
	{
		Altar->SetStaticMesh(SM_Altar.Object);
	}
	
	Sword = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Sword"));
	Sword->SetupAttachment(Altar);
	static ConstructorHelpers::FObjectFinder<UStaticMesh>SM_Sword(TEXT("/Game/Assets/Items/Sword"));
	if (SM_Sword.Succeeded())
	{
		Sword->SetStaticMesh(SM_Sword.Object);
	}
	Mirror = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mirror"));
	Mirror->SetupAttachment(Altar);
	static ConstructorHelpers::FObjectFinder<UStaticMesh>SM_Mirror(TEXT("/Game/Assets/Items/BronzeMirror"));
	if (SM_Mirror.Succeeded())
	{
		Mirror->SetStaticMesh(SM_Mirror.Object);
	}
	Bell = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Bell"));
	Bell->SetupAttachment(Altar);
	static ConstructorHelpers::FObjectFinder<UStaticMesh>SM_Bell(TEXT("/Game/Assets/Items/bell"));
	if (SM_Bell.Succeeded())
	{
		Bell->SetStaticMesh(SM_Bell.Object);
	}

	UnSealedItemNum = 0;
}

// Called when the game starts or when spawned
void AAltar_cpp::BeginPlay()
{
	Super::BeginPlay();
	
	// 당연히 오브젝트를 얻어 제단에 바친 상태가 아니므로 안 보이게 설정함.
	Sword->SetVisibility(false);
	Mirror->SetVisibility(false);
	Bell->SetVisibility(false);
	RespawnTimer = 0.f;
	bIsLevelStart = false;
}

void AAltar_cpp::OnInteract(class AHorrorGameCharacter* Player)
{
	if (Player->GetSwordNumbers() && Player->GetMirrorNumbers() && Player->GetBellNumbers())
	{
		//Destroy();
		Sword->SetVisibility(true);
		Mirror->SetVisibility(true);
		Bell->SetVisibility(true);
		if (IsValid(EndingMirror)) // Ending Mirror가 존재한 상황이면
		{
			EndingMirror->SetIsCleared(true);
		}
	}
	else
	{
		Player->SetErrorText(TEXT("You Need 1 Sword, 1 Mirror, 1 Bell!"), 3);
	}
}

void AAltar_cpp::ReaperSpawn()
{
	FActorSpawnParameters spawnParams;

	//if(Reaper != nullptr)
	CurrentReaper = GetWorld()->SpawnActor<AReaper_cpp>(Reaper, GetActorLocation()+GetActorForwardVector()*100.f, FRotator(0.f, 0.f, 0.f), spawnParams);
	bIsLevelStart = true;
}
// Called every frame
void AAltar_cpp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (CurrentReaper == nullptr && bIsLevelStart)
	{
		RespawnTimer += DeltaTime;

		if (RespawnTimer >= 20.f)
		{
			ReaperSpawn();
			RespawnTimer = 0.f;
		}
	}
}

void AAltar_cpp::UnSealedObjectNumber(int32 value)
{
	UnSealedItemNum += value;
	if (CurrentReaper)
	{
		CurrentReaper->UnSealedItemNumber = UnSealedItemNum;
	}
}