// CopyrightNotice 2023 Sunggon Kim kimdave205@gmail.com. All Rights Reserved.


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

	// 메시 기본 값 설정. 세세한 설정은 블루프린트 클래스를 통해 변경함.
	Center = CreateDefaultSubobject<UBillboardComponent>(TEXT("Root Billboard Comp"));
	Center->SetHiddenInGame(false, true);
	RootComponent = Center;
}

// Called when the game starts or when spawned
void ARandomLevelGenerator::BeginPlay()
{
	Super::BeginPlay();

	// 5 * 5 규격으로 25개의 레벨이 조립되도록 하는 로직.
	for (int i = 0; i < numX; ++i) // LoopX = i
	{
		for (int j = 0; j < numY; ++j) // LoopY = j
		{
			FName LevelName;
			int randIdx = 0;
			float RotateDeg = 0.0f;
			
			// 현재 인덱스의 레벨이 2개 라인 레벨일 경우
			if (map[i][j] == 2) // Double Level
			{
				// 2개 라인 레벨은 4개이기에 랜덤 값을 찾음.
				randIdx = FMath::RandRange(0, 3);

				// 이미 배치되지 않는 레벨이 생성될 때까지 반복함.
				while (true) // if index is used
				{
					if (!DoubleUse[randIdx])
					{
						DoubleUse[randIdx] = true;
						break;
					}
					randIdx = FMath::RandRange(0, 3);
				}

				// 선택된 2개 라인 맵을 설정한 회전 값으로 회전되도록 설정함.
				LevelName = DoubleNames[randIdx];
				RotateDeg = 90.0f * LevelRotate[i][j];
			}
			// 현재 인덱스의 레벨이 3개 라인 레벨일 경우
			else if (map[i][j] == 3) // Triple Level
			{
				// 2개 라인 레벨은 10개이기에 랜덤 값을 찾음.
				randIdx = FMath::RandRange(0, 9);

				// 이미 배치되지 않는 레벨이 생성될 때까지 반복함.
				while (true) // if index is used
				{
					if (!TripleUse[randIdx])
					{
						TripleUse[randIdx] = true;
						break;
					}
					randIdx = FMath::RandRange(0, 9);
				}

				// 선택된 3개 라인 맵을 설정한 회전 값으로 회전되도록 설정함.
				LevelName = TripleNames[randIdx];
				RotateDeg = 90.0f * LevelRotate[i][j];
			}
			// 현재 인덱스의 레벨이 4개 라인 레벨일 경우
			else // Quadra Level
			{
				randIdx = FMath::RandRange(0, 10);
				
				// 이미 배치되지 않는 레벨이 생성될 때까지 반복함.
				while (true) // if index is used
				{
					if (!QuadUse[randIdx])
					{
						QuadUse[randIdx] = true;
						break;
					}
					randIdx = FMath::RandRange(0, 10);
				}

				// 선택된 4개 라인 맵을 설정한 회전 값으로 회전되도록 설정함.
				LevelName = QuadNames[randIdx];
				RotateDeg = 90.0f * FMath::RandRange(0, 3);
			}

			// 그 후 배치될 위치에 해당하는 회전값을 가지고 선택된 레벨이 조립되도록 설정함.
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
		}
	}
}

