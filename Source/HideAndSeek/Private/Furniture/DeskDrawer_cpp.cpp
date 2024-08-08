// CopyrightNotice 2023 Sunggon Kim kimdave205@gmail.com. All Rights Reserved.

#include "Furniture/DeskDrawer_cpp.h"
#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ADeskDrawer_cpp::ADeskDrawer_cpp()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 메시들의 기본 설정을 해줌. (세세한 설정은 블루프린트 클래스에서 수행)
	static ConstructorHelpers::FObjectFinder<UStaticMesh> DrawerMesh(TEXT("/Game/Assets/Furniture/LongDeskDrawer"));
	if (DrawerMesh.Succeeded())
	{
		Drawer->SetStaticMesh(DrawerMesh.Object);
		Drawer->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
		//Drawer->SetRelativeScale3D(DefaultScale);
	}

	DrawerOpenMove = 50.0f;
}

// Called when the game starts or when spawned
void ADeskDrawer_cpp::BeginPlay()
{
	Super::BeginPlay();
	
	// 타임라인 커브 값이 있다면 타임라인에 할당하고, 재생될 때 실행할 콜백 함수도 바인딩함.
	if (CurveFloat)
	{
		FOnTimelineFloat TimelineProgress;
		TimelineProgress.BindDynamic(this, &ADeskDrawer_cpp::OpenDrawer);
		OpenAndClose.AddInterpFloat(CurveFloat, TimelineProgress);
	}

	// 서랍 메시들 중 하나를 선택해 메시를 변경함.
	if (DrawerMeshes.Num() > 0)
	{
		int RandIdx = FMath::RandRange(0, DrawerMeshes.Num() - 1);
		Drawer->SetStaticMesh(DrawerMeshes[RandIdx]);
	}
}

// Called every frame
void ADeskDrawer_cpp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	OpenAndClose.TickTimeline(DeltaTime);
}

// 타임라인이 재생될 때 호출될 콜백 함수.
void ADeskDrawer_cpp::OpenDrawer(float Value)
{
	Super::OpenDrawer(Value);
	
	// 서랍이 열리는 효과를 줌.
	FVector Location = FVector(0.0f, DrawerOpenMove * Value, 0.0f);

	Drawer->SetRelativeLocation(Location);
}

// 플레이어가 상호작용할 때 작동할 함수.
void ADeskDrawer_cpp::OnInteract()
{
	// 서랍이 닫혀있을 때
	if (bIsDrawerClosed)
	{
		// 소리를 재생함.
		if (DrawerOpenSound)
		{
			UGameplayStatics::PlaySoundAtLocation(this, DrawerOpenSound, GetActorLocation());
		}
	}
	// 서랍이 열려있을 때
	else
	{
		// 소리를 재생함.
		if (DrawerCloseSound)
		{
			UGameplayStatics::PlaySoundAtLocation(this, DrawerCloseSound, GetActorLocation());
		}
	}

	// 그 후 부모 클래스의 메서드를 호출함(타임라인 재생임).
	Super::OnInteract();
}