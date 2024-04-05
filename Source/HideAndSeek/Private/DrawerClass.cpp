// CopyrightNotice=0 2023 Sunggon Kim kimdave205@gmail.com

#include "DrawerClass.h"

// Sets default values
ADrawerClass::ADrawerClass()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	DefaultSceneRoot->SetWorldLocation(FVector(0.0f, 0.0f, 0.0f));
	FVector DefaultScale = FVector(1.0f, 1.0f, 1.0f);
	DefaultSceneRoot->SetWorldScale3D(DefaultScale);

	RootComponent = DefaultSceneRoot;

	Drawer = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Drawer"));
	Drawer->SetupAttachment(DefaultSceneRoot);
	/*static ConstructorHelpers::FObjectFinder<UStaticMesh> DrawerAssetMesh(TEXT("/Game/Assets/Furniture/Drawer_Cube_002"));
	if (DrawerAssetMesh.Succeeded())
	{
		Drawer->SetStaticMesh(DrawerAssetMesh.Object);
		Drawer->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
		Drawer->SetRelativeScale3D(DefaultScale);
	}*/

	Item = CreateDefaultSubobject<UChildActorComponent>(TEXT("Item"));
	Item->SetupAttachment(Drawer);
	Item->SetRelativeLocation(FVector(0.0f, 0.0f, -15.0f));

	/*Deco_Left = CreateDefaultSubobject<UChildActorComponent>(TEXT("Deco_Left"));
	Deco_Left->SetupAttachment(Drawer);
	Deco_Left->SetRelativeLocation(FVector(0.0f, 0.0f, -15.0f));

	Deco_Right = CreateDefaultSubobject<UChildActorComponent>(TEXT("Deco_Right"));
	Deco_Right->SetupAttachment(Drawer);
	Deco_Right->SetRelativeLocation(FVector(0.0f, 0.0f, -15.0f));*/

	//DrawerSound = CreateDefaultSubobject<UAudioComponent>(TEXT("DrawerSound"));
	//static ConstructorHelpers::FObjectFinder<USoundBase>SoundMesh(TEXT("/Game/Assets/Sounds/Drawer_Opening"));
	//if (SoundMesh.Succeeded())
	//{
	//	DrawerSound->SetSound(SoundMesh.Object);
	//}
	/*DrawerSound->SetupAttachment(Drawer);
	DrawerSound->SetAutoActivate(false);*/

	bIsItemSpawned = false;
}

// Called when the game starts or when spawned
void ADrawerClass::BeginPlay()
{
	Super::BeginPlay();

	if (Item->GetChildActorClass() != nullptr)
	{
		bIsItemSpawned = true;
	}

	//if (Decorators.Num() > 0)
	//{
	//	float RandomProbability = FMath::RandRange(0.f, 1.f);
	//	if (RandomProbability > 0.3f) // 30퍼센트보다 더 큰 확률이면(즉 70퍼센트로 데코 생성)
	//	{
	//		int32 randIdx = FMath::RandRange(0, Decorators.Num() - 1);
	//		Deco_Left->SetChildActorClass(Decorators[randIdx]);
	//	}

	//	RandomProbability = FMath::RandRange(0.f, 1.f);
	//	if (RandomProbability > 0.3f) // 30퍼센트보다 더 큰 확률이면(즉 70퍼센트로 데코 생성)
	//	{
	//		int32 randIdx = FMath::RandRange(0, Decorators.Num() - 1);
	//		Deco_Right->SetChildActorClass(Decorators[randIdx]);
	//	}
	//}
	
	if (CurveFloat)
	{
		FOnTimelineFloat TimelineProgress;
		TimelineProgress.BindDynamic(this, &ADrawerClass::OpenDrawer);
		OpenAndClose.AddInterpFloat(CurveFloat, TimelineProgress); // 오류 해결 TObjectPtr이 아닌 FTimeline이 좋은 듯
	}
}

// Called every frame
void ADrawerClass::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	OpenAndClose.TickTimeline(DeltaTime);
}

void ADrawerClass::OpenDrawer(float Value)
{
	FVector Location = FVector(0.0f, DrawerOpenMove * Value, 0.0f);
}

void ADrawerClass::OnInteract()
{
	//UE_LOG(LogTemp, Warning, TEXT("Interact!"));

	if (bIsDrawerClosed)
	{
		//UE_LOG(LogTemp, Warning, TEXT("OPEN"));
		if (!bIsFirstOpen) // 서랍을 처음 여는 것이라면, 아이템을 생성하거나 미생성
		{
			// float fNoItemProbability = 0.5f; // 아이템이 안 나올 확률
			float RandomValue = FMath::FRandRange(0.0f, 1.0f);
			/*if (GEngine)
				GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("First Random Value: %f"), RandomValue));*/
			int idx = -1;

			if (RandomValue > fNoItemProbability)
			{
				float fTotalProbability = 1.0f - fNoItemProbability;
				RandomValue = FMath::FRandRange(0.0f, fTotalProbability);
				/*if (GEngine)
					GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Second Random Value: %f"), RandomValue));*/

				for (int i = 0; i < ItemProbability.Num(); ++i)
				{
					if (i > 0)
					{
						if (RandomValue < ItemProbability[i] && RandomValue >= ItemProbability[i - 1])
						{
							idx = i;
							break;
						}
					}
					else
					{
						if (RandomValue < ItemProbability[i] && RandomValue >= 0)
						{
							idx = i;
							break;
						}
					}
				}

				if (idx >= 0)
				{
					/*if (GEngine)
						GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Class: %s"), *Items[idx]->GetName()));*/
					Item->SetChildActorClass(Items[idx]);
				}
			}
			bIsFirstOpen = true;
		}
		/*if(DrawerSound)
			DrawerSound->Play();*/
		OpenAndClose.Play();
	}
	else
	{
		//UE_LOG(LogTemp, Warning, TEXT("CLOSE"));
		/*if (DrawerSound)
			DrawerSound->Play();*/
		OpenAndClose.Reverse();
	}
	bIsDrawerClosed = !bIsDrawerClosed; // flip flop
}

bool ADrawerClass::SetSpawnItem(TSubclassOf<AActor> inItem)
{
	if (!GetIsItemSpawned()) // 아이템이 없는 경우에만
	{
		Item->SetChildActorClass(inItem);
		bIsItemSpawned = true;

		return true;
	}

	return false;
}

bool ADrawerClass::GetIsItemSpawned()
{
	return bIsItemSpawned;
}