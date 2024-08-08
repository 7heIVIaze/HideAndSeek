// CopyrightNotice 2023 Sunggon Kim kimdave205@gmail.com. All Rights Reserved.

#include "Furniture/Drawer_cpp.h"
//#include "Items.h"
#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ADrawer_cpp::ADrawer_cpp()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 메시들의 기본 설정을 해줌. (세세한 설정은 블루프린트 클래스에서 수행)
	static ConstructorHelpers::FObjectFinder<UStaticMesh> DrawerAssetMesh(TEXT("/Game/Assets/Furniture/drawerBox"));
	if (DrawerAssetMesh.Succeeded())
	{
		Drawer->SetStaticMesh(DrawerAssetMesh.Object);
		Drawer->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	}

	DrawerOpenMove = 80.0f;
}

// Called when the game starts or when spawned
void ADrawer_cpp::BeginPlay()
{
	Super::BeginPlay();

	// 타임라인 커브 값이 있다면 타임라인에 할당하고, 재생될 때 실행할 콜백 함수도 바인딩함.
	if (CurveFloat)
	{
		FOnTimelineFloat TimelineProgress;
		TimelineProgress.BindDynamic(this, &ADrawer_cpp::OpenDrawer);
		OpenAndClose.AddInterpFloat(CurveFloat, TimelineProgress); // 오류 해결 TObjectPtr이 아닌 FTimeline이 좋은 듯
	}

	// 서랍 메시들 중 하나를 선택해 메시를 변경함.
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

// 타임라인이 재생될 때 호출될 콜백 함수.
void ADrawer_cpp::OpenDrawer(float Value)
{
	Super::OpenDrawer(Value);

	// 서랍이 열리는 효과를 줌.
	FVector Location = FVector(0.0f, DrawerOpenMove * Value, 0.0f);

	Drawer->SetRelativeLocation(Location);
}

// 플레이어가 상호작용할 때 작동할 함수.
void ADrawer_cpp::OnInteract()
{
	// 효과음을 재생함.
	if (DrawerSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, DrawerSound, GetActorLocation());
	}

	// 그 후 부모 클래스의 메서드를 호출함(타임라인 재생임).
	Super::OnInteract();
}