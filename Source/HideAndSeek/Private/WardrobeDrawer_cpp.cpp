// CopyrightNotice 2023 Sunggon Kim kimdave205@gmail.com. All Rights Reserved.

#include "WardrobeDrawer_cpp.h"
#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AWardrobeDrawer_cpp::AWardrobeDrawer_cpp()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	// 메시들의 기본 설정을 해줌. (세세한 설정은 블루프린트 클래스에서 수행)
	static ConstructorHelpers::FObjectFinder<UStaticMesh> DrawerMesh(TEXT("/Game/Assets/Furniture/wardrobeDrawer"));
	if (DrawerMesh.Succeeded())
	{
		Drawer->SetStaticMesh(DrawerMesh.Object);
		Drawer->SetRelativeLocation(FVector(0.f, 0.f, 0.f));
		Drawer->SetRelativeScale3D(FVector(2.f, 2.f, 1.7f));
	}

	DrawerOpenMove = -80.f;
}

// Called when the game starts or when spawned
void AWardrobeDrawer_cpp::BeginPlay()
{
	Super::BeginPlay();
	
	// 타임라인 커브 값이 있다면 타임라인에 할당하고, 재생될 때 실행할 콜백 함수도 바인딩함.
	if (CurveFloat)
	{
		FOnTimelineFloat TimelineProgress;
		TimelineProgress.BindDynamic(this, &AWardrobeDrawer_cpp::OpenDrawer);
		OpenAndClose.AddInterpFloat(CurveFloat, TimelineProgress);
	}
}

// Called every frame
void AWardrobeDrawer_cpp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	OpenAndClose.TickTimeline(DeltaTime);
}

// 타임라인이 재생될 때 호출될 콜백 함수.
void AWardrobeDrawer_cpp::OpenDrawer(float Value)
{
	Super::OpenDrawer(Value);

	// 서랍이 열리는 효과를 줌.
	FVector Location = FVector(0.f, DrawerOpenMove * Value, 0.f);

	Drawer->SetRelativeLocation(Location);
}

// 플레이어가 상호작용할 때 작동할 함수.
void AWardrobeDrawer_cpp::OnInteract()
{
	// 서랍이 닫혀있을 경우
	if (bIsDrawerClosed)
	{
		// 열리는 효과음을 재생함.
		if (DrawerOpenSound)
		{
			UGameplayStatics::PlaySoundAtLocation(this, DrawerOpenSound, GetActorLocation());
		}
	}
	// 서랍이 열려있을 경우
	else
	{
		// 닫히는 효과음을 재생함.
		if (DrawerCloseSound)
		{
			UGameplayStatics::PlaySoundAtLocation(this, DrawerCloseSound, GetActorLocation());
		}
		
	}

	// 그 후 부모 클래스의 메서드를 호출함(타임라인 재생임).
	Super::OnInteract();
}