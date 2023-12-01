// CopyrightNotice=0 2023 Sunggon Kim kimdave205@gmail.com

#include "MonsterPrologue.h"
#include "Components/BoxComponent.h"
#include "HorrorGame/HorrorGameCharacter.h"
#include "AI/Reaper_cpp.h"

// Sets default values
AMonsterPrologue::AMonsterPrologue()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	SpawnPoint = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SpawnPoint"));
	RootComponent = SpawnPoint;

	PlayerEnterBox = CreateDefaultSubobject<UBoxComponent>(TEXT("PlayerEnterBox"));
	PlayerEnterBox->SetupAttachment(SpawnPoint);
	PlayerEnterBox->OnComponentBeginOverlap.AddDynamic(this, &AMonsterPrologue::PlayerEnterBeginOverlap);
}

// Called when the game starts or when spawned
void AMonsterPrologue::BeginPlay()
{
	Super::BeginPlay();
	
}

void AMonsterPrologue::PlayerEnterBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor != nullptr && OtherActor != this && OtherComp != nullptr)
	{
		if (AHorrorGameCharacter* player = Cast<AHorrorGameCharacter>(OtherActor))
		{
			if (!bIsPlayerIn)
			{
				FString text = TEXT("You can avoid creatures by hiding in lockers");
				player->SetExplainText(text, 3);
				if (GEngine)
					GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("%s"), *player->GetErrorInteractText()));
				FActorSpawnParameters spawnParams;
			
				//AReaper_cpp* reaper = world->SpawnActor<AReaper_cpp>(ObjectToSpawn->GeneratedClass, spawnLocation, rotator, spawnParams);
				AReaper_cpp* reaper = GetWorld()->SpawnActor<AReaper_cpp>(ObjectToSpawn, GetActorLocation(), FRotator(0.f, 0.f, 0.f), spawnParams);
				reaper->bIsCollectMode = false;
				/*for (auto PatrolPoint : PatrolPointLists)
				{
					reaper->PatrolPointLists.Add(PatrolPoint);
				}*/
				reaper->PatrolPointLists = PatrolPointLists;
				bIsPlayerIn = true;
			}
		}
	}
}