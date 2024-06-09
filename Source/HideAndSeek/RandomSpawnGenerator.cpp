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

	// 메시 기본 값 설정. 세세한 설정은 블루프린트 클래스를 통해 변경함.
	center = CreateDefaultSubobject<UBillboardComponent>(TEXT("Root Billboard Comp"));
	center->SetHiddenInGame(false, true);
	RootComponent = center;
}

// Called when the game starts or when spawned
void ARandomSpawnGenerator::BeginPlay()
{
	Super::BeginPlay();
	
	// 스폰 지점은 3개로 랜덤한 값을 선택함.
	int randIdx = FMath::RandRange(0, 2);

	// 그 후 배치될 위치에 해당하는 회전값을 가지고 선택된 레벨이 조립되도록 설정함.
	ULevelStreaming* temp = UGameplayStatics::GetStreamingLevel(GetWorld(), LevelNames[randIdx]);
	ULevelStreaming* SpawnedLevel = temp->CreateInstance(FString::FromInt(numX) + "x" + FString::FromInt(numY) + "Spawn");

	SpawnedLevel->LevelTransform = UKismetMathLibrary::MakeTransform(UKismetMathLibrary::MakeVector(numX * LevelSize, numY * LevelSize, 0.0f), UKismetMathLibrary::MakeRotator(0.0f, 0.0f, 0.0f));
	//temp->LevelTransform.SetComponents(FQuat(FRotator(0.0f, 0.0f, 90.0f * RotateDeg)), FVector(numX * LevelSize, numY * LevelSize, 0.0f), FVector(1.0f, 1.0f, 1.0f));
	
	SpawnedLevel->SetShouldBeLoaded(true);
	SpawnedLevel->SetShouldBeVisible(true);
}


