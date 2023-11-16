// CopyrightNotice=0 2023 Sunggon Kim kimdave205@gmail.com

#include "Items/Soul_Lantern_cpp.h"
#include "Sound/SoundCue.h"
#include "Components/PointLightComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "EngineUtils.h"
#include "Altar_cpp.h"
#include "HorrorGame/HorrorGameCharacter.h"

ASoul_Lantern_cpp::ASoul_Lantern_cpp()
{
	PrimaryActorTick.bCanEverTick = true;
	
	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = RootComp;

	LanternMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Lantern"));
	LanternMesh->SetupAttachment(RootComp);
	static ConstructorHelpers::FObjectFinder<UStaticMesh>LanternObj(TEXT("/Game/Assets/Items/Lantern"));
	if (LanternObj.Succeeded())
	{
		LanternMesh->SetStaticMesh(LanternObj.Object);
	}
	LanternMesh->SetCollisionProfileName("ItemObjects");

	Light = CreateDefaultSubobject<UPointLightComponent>(TEXT("SoulLight"));
	Light->SetupAttachment(LanternMesh);

	LightNiagara = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Soul"));
	LightNiagara->SetupAttachment(LanternMesh);
	static ConstructorHelpers::FObjectFinder<UNiagaraSystem>Soul_NS(TEXT("/Game/Assets/Material/NS_Soul"));
	if (Soul_NS.Succeeded())
	{
		LightNiagara->SetAsset(Soul_NS.Object);
	}

	bIsLightOn = false;
	AltarLocation = FVector(0, 0, 0);
}

void ASoul_Lantern_cpp::BeginPlay()
{
	Super::BeginPlay();

	Light->SetVisibility(bIsLightOn);
	LightNiagara->Deactivate();

	for (TActorIterator<AAltar_cpp>entity(GetWorld()); entity; ++entity)
	{
		AAltar_cpp* Altar = *entity;
		if (Altar != nullptr) {
			AltarLocation = Altar->GetActorLocation();
			AltarLocation.Z = 0.f; // 정확한 Z축을 구하기 위한 Cross Product 계산을 위해 0으로 설정
		}
	}
}

void ASoul_Lantern_cpp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsLightOn)
	{
		FVector CurrentLocation = GetActorLocation();
		CurrentLocation.Z = 0.f; // 정확한 Z축을 구하기 위한 Cross Product 계산을 위해 0으로 설정

		FVector Direction = (AltarLocation - CurrentLocation).GetSafeNormal(); // 현재 랜턴(플레이어)의 위치와 목적지 위치 사이의 노말 벡터
		FVector Forward = GetActorForwardVector(); // 현재 랜턴의 forward 벡터

		float Dot = FVector::DotProduct(Forward, Direction); // 두 벡터 사이의 내적을 구함
		float Theta = FMath::Acos(Dot); // 내적값을 arc cos을 통해서 theta(각도)를 구함

		Theta = FMath::RadiansToDegrees(Theta); // 기존의 Theta의 값이 Radian 값이기 때문에 Rotator를 위해 Degree값으로 변경
		FVector Cross = FVector::CrossProduct(Forward, Direction); // 회전축 구하기
		FRotator NewRotation(0.f);

		if (Cross.Z > 0)
		{
			NewRotation = FRotator(0.f, Theta, 0.f); // 양수면 오른쪽으로
		}
		else if(Cross.Z < 0)
		{
			NewRotation = FRotator(0.f, -Theta, 0.f); // 음수면 왼쪽으로
		}

		// FRotator NewRotation = Direction.Rotation();
		LightNiagara->SetRelativeRotation(NewRotation);
	}
}

void ASoul_Lantern_cpp::OnInteract(class AHorrorGameCharacter* Player)
{
	Super::OnInteract(Player);

	Player->AddLantern();
	if (Player->bCanItemGet)
	{
		Destroy();
	}
}

void ASoul_Lantern_cpp::UseInteract(class AHorrorGameCharacter* Player)
{
	Super::UseInteract(Player);
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Use Interact Succeed!")));
	bIsLightOn = !bIsLightOn;
	if (bIsLightOn)
	{
		USoundCue* LanternCue = LoadObject<USoundCue>(nullptr, TEXT("/Game/Assets/Sounds/SoundCues/LanternSoundCue"));
		if (LanternCue)
		{
			UGameplayStatics::PlaySoundAtLocation(this, LanternCue, GetActorLocation());
		}
		LightNiagara->Activate(true);
	}
	else
	{
		LightNiagara->Activate(false);
		LightNiagara->Deactivate();
	}
	Light->SetVisibility(bIsLightOn);

}