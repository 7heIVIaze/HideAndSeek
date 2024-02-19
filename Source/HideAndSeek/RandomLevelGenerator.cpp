// Fill out your copyright notice in the Description page of Project Settings.


#include "RandomLevelGenerator.h"
#include "Components/BillboardComponent.h"
#include "Engine.h"
#include "Engine/LevelStreaming.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ARandomLevelGenerator::ARandomLevelGenerator()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Center = CreateDefaultSubobject<UBillboardComponent>(TEXT("Root Billboard Comp"));
	//Center->SetEditorScale(0.25f);
	Center->SetHiddenInGame(false, true);
	RootComponent = Center;
}

// Called when the game starts or when spawned
void ARandomLevelGenerator::BeginPlay()
{
	Super::BeginPlay();

	for (int i = 0; i < numX; ++i) // LoopX = i
	{
		for (int j = 0; j < numY; ++j) // LoopY = j
		{
			FName LevelName;
			int randIdx = 0;
			float RotateDeg = 0.0f;
			/*if (BeforeLevel != nullptr)
			{
				if (BeforeLevel->IsLevelVisible() && BeforeLevel->IsLevelLoaded())
				{
					j++;
				}
				continue;
			}*/

			if (map[i][j] == 2) // Double Level
			{
				randIdx = FMath::RandRange(0, 3);
				while (true) // if index is used
				{
					if (!DoubleUse[randIdx])
					{
						DoubleUse[randIdx] = true;
						break;
					}
					randIdx = FMath::RandRange(0, 3);
				}
				LevelName = DoubleNames[randIdx];
				RotateDeg = 90.0f * LevelRotate[i][j];
			}
			else if (map[i][j] == 3) // Triple Level
			{
				randIdx = FMath::RandRange(0, 9);
				while (true) // if index is used
				{
					if (!TripleUse[randIdx])
					{
						TripleUse[randIdx] = true;
						break;
					}
					randIdx = FMath::RandRange(0, 9);
				}
				LevelName = TripleNames[randIdx];
				RotateDeg = 90.0f * LevelRotate[i][j];
			}
			else // Quadra Level
			{
				randIdx = FMath::RandRange(0, 10);
				while (true) // if index is used
				{
					if (!QuadUse[randIdx])
					{
						QuadUse[randIdx] = true;
						break;
					}
					randIdx = FMath::RandRange(0, 10);
				}
				LevelName = QuadNames[randIdx];
				RotateDeg = 90.0f * FMath::RandRange(0, 3);
			}

			FVector LevelOffset = FVector(i * LevelSize + 5200.0f, j * LevelSize - 10400.0f, 0.0f);
			FQuat fq = FRotator(0.0f, RotateDeg, 0.f).Quaternion();

			ULevelStreamingDynamic* StreamingLevel = static_cast<ULevelStreamingDynamic*>(
				StaticConstructObject_Internal(ULevelStreamingDynamic::StaticClass(), GetWorld(), NAME_None, RF_NoFlags, EInternalObjectFlags::None));
			
			if (StreamingLevel)
			{
				StreamingLevel->SetShouldBeLoaded(true);
				StreamingLevel->SetShouldBeVisible(true);

				StreamingLevel->LevelTransform.SetTranslation(LevelOffset);
				StreamingLevel->LevelTransform.SetRotation(fq);

				StreamingLevel->SetWorldAssetByPackageName(LevelName);
				StreamingLevel->PackageNameToLoad = LevelName;

				GetWorld()->AddStreamingLevel(StreamingLevel);
			}

			
			// BeforeLevel = UGameplayStatics::GetStreamingLevel(GetWorld(), LevelName);
			//if (LevelRotate[i][j] == -1) {
			//	RotateDeg = 90.0f * FMath::RandRange(0, 3);
			//}
			//else {
			//	RotateDeg = 90.0f * LevelRotate[i][j];
			//}
			//ULevelStreaming* result = BeforeLevel->CreateInstance(FString::FromInt(i) + "x" + FString::FromInt(j) + "Level");
			//// result->LevelTransform = UKismetMathLibrary::MakeTransform(UKismetMathLibrary::MakeVector(i * LevelSize + 5200.0f, j * LevelSize - 10400.0f, 0.0f), UKismetMathLibrary::MakeRotator(0.0f, 0.0f, RotateDeg));

			//// result->LevelTransform = UKismetMathLibrary::MakeTransform(FVector(i * LevelSize + 5200.0f, j * LevelSize - 10400.0f, 0.0f), FRotator(0.0f, RotateDeg, 0.f));
			//result->LevelTransform.SetLocation(FVector(i * LevelSize + 5200.0f, j * LevelSize - 10400.0f, 0.0f));
			//result->LevelTransform.SetRotation(FRotator(0.0f, RotateDeg, 0.f).Quaternion());

			//result->SetShouldBeLoaded(true);
			//result->SetShouldBeVisible(true);
			/*temp->LevelTransform.SetLocation(FVector(i * LevelSize + 4800.0f, j * LevelSize - 9600.0f, 0.0f));
			temp->LevelTransform.SetRotation(FQuat(FRotator(0.0f, RotateDeg, 0.0f)));*/
			// BeforeLevel->LevelTransform = UKismetMathLibrary::MakeTransform(UKismetMathLibrary::MakeVector(i * LevelSize + 5200.0f, j * LevelSize - 10400.0f, 0.0f), UKismetMathLibrary::MakeRotator(0.0f, 0.0f, RotateDeg));
			//BeforeLevel->LevelTransform.SetLocation(FVector(i * LevelSize + 5200.0f, j * LevelSize - 10400.0f, 0.0f));
			//BeforeLevel->LevelTransform.SetRotation(FRotator(0.0f, RotateDeg, 0.f).Quaternion());
			//BeforeLevel->SetShouldBeLoaded(true);
			//// UE_LOG(LogTemp, Warning, TEXT("%s"), ((temp->IsLevelLoaded()) ? "true" : "false"));
			//
			//BeforeLevel->SetShouldBeVisible(true);
			
		}
	}
}

