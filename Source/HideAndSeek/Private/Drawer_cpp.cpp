// Fill out your copyright notice in the Description page of Project Settings.

#include "Drawer_cpp.h"
//#include "Items.h"
#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ADrawer_cpp::ADrawer_cpp()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	/*DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	DefaultSceneRoot->SetWorldLocation(FVector(0.0f, 0.0f, 0.0f));
	FVector DefaultScale = FVector(1.0f, 1.0f, 1.0f);
	DefaultSceneRoot->SetWorldScale3D(DefaultScale);

	RootComponent = DefaultSceneRoot;*/

	/*Drawer = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Drawer"));
	Drawer->SetupAttachment(DefaultSceneRoot);*/
	static ConstructorHelpers::FObjectFinder<UStaticMesh> DrawerAssetMesh(TEXT("/Game/Assets/Furniture/Drawer_Cube_002"));
	if (DrawerAssetMesh.Succeeded())
	{
		Drawer->SetStaticMesh(DrawerAssetMesh.Object);
		Drawer->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
		/*Drawer->SetRelativeScale3D(DefaultScale);*/
	}

	/*Item = CreateDefaultSubobject<UChildActorComponent>(TEXT("Item"));
	Item->SetupAttachment(Drawer);
	Item->SetRelativeLocation(FVector(0.0f, 0.0f, -15.0f));*/

	/*DrawerSound = CreateDefaultSubobject<UAudioComponent>(TEXT("DrawerSound"));*/
	/*static ConstructorHelpers::FObjectFinder<USoundBase>SoundMesh(TEXT("/Game/Assets/Sounds/Drawer_Opening"));
	if (SoundMesh.Succeeded())
	{
		DrawerSound->SetSound(SoundMesh.Object);
	}*/
	/*DrawerSound->SetupAttachment(Drawer);
	DrawerSound->SetAutoActivate(false);*/

	DrawerOpenMove = 80.0f;
}

// Called when the game starts or when spawned
void ADrawer_cpp::BeginPlay()
{
	Super::BeginPlay();

	if (CurveFloat)
	{
		FOnTimelineFloat TimelineProgress;
		TimelineProgress.BindDynamic(this, &ADrawer_cpp::OpenDrawer);
		OpenAndClose.AddInterpFloat(CurveFloat, TimelineProgress); // 오류 해결 TObjectPtr이 아닌 FTimeline이 좋은 듯
	}

	if (DrawerMeshes.Num() > 0)
	{
		int RandIdx = FMath::RandRange(0, DrawerMeshes.Num() - 1);
		Drawer->SetStaticMesh(DrawerMeshes[RandIdx]);
	}
}

// Called every frame
void ADrawer_cpp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	OpenAndClose.TickTimeline(DeltaTime);
}

void ADrawer_cpp::OpenDrawer(float Value)
{
	Super::OpenDrawer(Value);
	FVector Location = FVector(0.0f, DrawerOpenMove * Value, 0.0f);

	Drawer->SetRelativeLocation(Location);
}

void ADrawer_cpp::OnInteract()
{
	//UE_LOG(LogTemp, Warning, TEXT("Interact!"));

	//USoundCue* DrawerSound = LoadObject<USoundCue>(nullptr, TEXT("/Game/Assets/Sounds/SoundCues/DrawerOpenSoundCue"));
	if (DrawerSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, DrawerSound, GetActorLocation());
	}

	Super::OnInteract();
	//if (bIsDrawerClosed)
	//{
	//	//UE_LOG(LogTemp, Warning, TEXT("OPEN"));
	//	if (!bIsFirstOpen) // 서랍을 처음 여는 것이라면, 아이템을 생성하거나 미생성
	//	{
	//		float fNoItemProbability = 0.5f; // 아이템이 안 나올 확률
	//		float RandomValue = FMath::FRandRange(0.0f, 1.0f);
	//		if (GEngine)
	//			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("First Random Value: %f"), RandomValue));
	//		int idx = -1;

	//		if (RandomValue > fNoItemProbability)
	//		{
	//			float fTotalProbability = 1.0f - fNoItemProbability;
	//			RandomValue = FMath::FRandRange(0.0f, fTotalProbability);
	//			if (GEngine)
	//				GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Second Random Value: %f"), RandomValue));

	//			for (int i = 0; i < ItemProbability.Num(); ++i)
	//			{
	//				if (i > 0)
	//				{
	//					if (RandomValue < ItemProbability[i] && RandomValue >= ItemProbability[i-1])
	//					{
	//						idx = i;
	//						break;
	//					}
	//				}
	//				else
	//				{
	//					if (RandomValue < ItemProbability[i] && RandomValue >= 0)
	//					{
	//						idx = i;
	//						break;
	//					}
	//				}
	//			}

	//			if (idx >= 0)
	//			{
	//				if (GEngine)
	//					GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Class: %s"), *Items[idx]->GetName()));
	//				Item->SetChildActorClass(Items[idx]);
	//			}
	//		}
	//		bIsFirstOpen = true;
	//	}
	//	DrawerSound->Play();
	//	OpenAndClose.Play();
	//}
	//else
	//{
	//	//UE_LOG(LogTemp, Warning, TEXT("CLOSE"));
	//	DrawerSound->Play();
	//	OpenAndClose.Reverse();
	//}
	//bIsDrawerClosed = !bIsDrawerClosed; // flip flop
}