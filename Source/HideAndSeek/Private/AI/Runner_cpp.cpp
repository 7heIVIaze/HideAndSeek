// CopyrightNotice 2023 Sunggon Kim kimdave205@gmail.com. All Rights Reserved.

#include "AI/Runner_cpp.h"
#include "Components/InputComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/PointLightComponent.h"
#include "Components/SpotLightComponent.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "Sound/SoundCue.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AI/AIController_Runner.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "LevelManager/PatrolPoint_cpp.h"
#include "EngineUtils.h"
#include "Furniture/HideObject.h"
#include "Player/HorrorGameCharacter.h"
#include "Player/HorrorGamePlayerController.h"
#include "Furniture/DoorClass.h"
#include "Furniture/Alarm.h"
#include "Animation/AnimSequence.h"
#include "Kismet/KismetMathLibrary.h"
#include "NiagaraComponent.h"
#include "ComponentAction/HorrorGameSaveGame.h"

// Sets default values
ARunner_cpp::ARunner_cpp()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AIControllerClass = AAIController_Runner::StaticClass();

	GetMesh()->SetCollisionProfileName("AICharacters");
	bIsCollectMode = true;
}

// Called when the game starts or when spawned
void ARunner_cpp::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ARunner_cpp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (AAIController_Runner* RunnerController = Cast<AAIController_Runner>(GetController()))
	{
		RunnerController->SetControlRotation(GetActorRotation());
	}
}

// Called to bind functionality to input
void ARunner_cpp::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ARunner_cpp::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get Foward Vector
		const FVector FowardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		// get Right Vector
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		AddMovementInput(FowardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void ARunner_cpp::Look(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void ARunner_cpp::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (IsPlayerControlled())
	{
		//	SetControlMode();
		GetCharacterMovement()->MaxWalkSpeed = 180.f;
	}
	else
	{
		//	SetControlMode();
		if (bIsChase) GetCharacterMovement()->MaxWalkSpeed = 330.f;
		else GetCharacterMovement()->MaxWalkSpeed = 100.f;
	}
}

// 청동 검을 통해 처치될 때 호출할 함수
void ARunner_cpp::Exorcism()
{
	Super::Exorcism();

	// 나이아가라 시스템을 활성화하고, Dissolve 머티리얼 효과를 줘서 서서히 사라지는 듯한 연출을 함 
	DissolveParticleSystem->Activate(true);
	UMaterialInterface* Material = GetMesh()->GetMaterial(0);
	MaterialInstances[0] = GetMesh()->CreateDynamicMaterialInstance(0, Material);
	MaterialInstances[0]->SetTextureParameterValue(TEXT("Texture"), Texture);
	DissolveParticleSystem->SetVariableTexture(TEXT("Texture"), Texture);
	DissolveTimeline.PlayFromStart();
}

// Exorcism될 때 Dissolve Material을 구현하기 위한 타임라인 콜백 함수.
void ARunner_cpp::ChangeMaterialInstance(float inValue)
{
	Super::ChangeMaterialInstance(inValue);

	// 1에서 0으로 서서히 Curve Float에 따라 감소하면서 머티리얼의 파라미터와 나이아가라 파티클의 파라미터를 변경함.
	float Amount = FMath::Lerp(1.0f, 0.0f, inValue);

	MaterialInstances[0]->SetScalarParameterValue(TEXT("Amount"), Amount);

	DissolveParticleSystem->SetVariableFloat(TEXT("Amount"), Amount);
}

// 타임라인 종료 후 호출할 콜백 함수.
void ARunner_cpp::DissolveFinish()
{
	Super::DissolveFinish();

	// 파티클을 비활성화시키고 해당 캐릭터를 제거함
	DissolveParticleSystem->SetVariableFloat(TEXT("Width"), 0.0f);

	DissolveParticleSystem->Deactivate();

	Destroy();
}