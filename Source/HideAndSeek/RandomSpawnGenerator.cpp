// CopyrightNotice 2023 Sunggon Kim kimdave205@gmail.com. All Rights Reserved.

#include "RandomSpawnGenerator.h"
#include "Components/BillboardComponent.h"
#include "Engine.h"
#include "Engine/LevelStreaming.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"


// Sets default values
ARandomSpawnGenerator::ARandomSpawnGenerator()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	center = CreateDefaultSubobject<UBillboardComponent>(TEXT("Root Billboard Comp"));
//	center->SetEditorScale(0.25f);
	center->SetHiddenInGame(false, true);
	RootComponent = center;
}

// Called when the game starts or when spawned
void ARandomSpawnGenerator::BeginPlay()
{
	Super::BeginPlay();
	
	int randIdx = FMath::RandRange(0, 2);
	//FRawStaticIndexBuffer::GetCopy(LevelNames[randIdx]);

	ULevelStreaming* temp = UGameplayStatics::GetStreamingLevel(GetWorld(), LevelNames[randIdx]);
	ULevelStreaming* SpawnedLevel = temp->CreateInstance(FString::FromInt(numX) + "x" + FString::FromInt(numY) + "Spawn");

	SpawnedLevel->LevelTransform = UKismetMathLibrary::MakeTransform(UKismetMathLibrary::MakeVector(numX * LevelSize, numY * LevelSize, 0.0f), UKismetMathLibrary::MakeRotator(0.0f, 0.0f, 0.0f));
	//temp->LevelTransform.SetComponents(FQuat(FRotator(0.0f, 0.0f, 90.0f * RotateDeg)), FVector(numX * LevelSize, numY * LevelSize, 0.0f), FVector(1.0f, 1.0f, 1.0f));
	
	SpawnedLevel->SetShouldBeLoaded(true);
	SpawnedLevel->SetShouldBeVisible(true);
}


