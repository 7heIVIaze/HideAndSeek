// CopyrightNotice=0 2023 Sunggon Kim kimdave205@gmail.com

#include "PatrolPoint_cpp.h"
#include "Components/BoxComponent.h"
#include "AI/Reaper_cpp.h"
#include "AI/Runner_cpp.h"
#include "AI/Brute_cpp.h"

// Sets default values
APatrolPoint_cpp::APatrolPoint_cpp()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = RootComp;

	/*CheckBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CheckBox"));
	CheckBox->SetupAttachment(RootComp);
	CheckBox->OnComponentBeginOverlap.AddDynamic(this, &APatrolPoint_cpp::CheckBoxBeginOverlap);*/
}

// Called when the game starts or when spawned
void APatrolPoint_cpp::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APatrolPoint_cpp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

//void APatrolPoint_cpp::CheckBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
//{
//	if (OtherActor != nullptr && OtherActor != this && OtherComp != nullptr)
//	{
//		if (auto Reaper = Cast<AReaper_cpp>(OtherActor))
//		{
//			Reaper->SetPatrolSuccess(true);
//		}
//
//		if (auto Runner = Cast<ARunner_cpp>(OtherActor))
//		{
//			Runner->SetPatrolSuccess(true);
//		}
//
//		if (auto Brute = Cast<ABrute_cpp>(OtherActor))
//		{
//			Brute->SetPatrolSuccess(true);
//		}
//	}
//}