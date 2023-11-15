// CopyrightNotice=0 2023 Sunggon Kim kimdave205@gmail.com

#include "AI/CreatureClass.h"
//#include "HorrorGame/HorrorGameCharacter.h"
//#include "PatrolPoint_cpp.h"
//#include "EngineUtils.h"
//
//// Sets default values
//ACreatureClass::ACreatureClass()
//{
// 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
//	PrimaryActorTick.bCanEverTick = true;
//
//}
//
//// Called when the game starts or when spawned
//void ACreatureClass::BeginPlay()
//{
//	Super::BeginPlay();
//	
//}
//
//// Called every frame
//void ACreatureClass::Tick(float DeltaTime)
//{
//	Super::Tick(DeltaTime);
//
//	/*if (bIsStunned)
//	{
//		if (cnt++ >= StunTime)
//		{
//			bIsStunned = false;
//			cnt = 0;
//		}
//	}*/
//}
//
//// Called to bind functionality to input
////void ACreatureClass::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
////{
////	Super::SetupPlayerInputComponent(PlayerInputComponent);
////
////}
////
////void ACreatureClass::Move(const FInputActionValue& Value)
////{
////	FVector2D MovementVector = Value.Get<FVector2D>();
////
////	if (Controller != nullptr)
////	{
////		const FRotator Rotation = Controller->GetControlRotation();
////		const FRotator YawRotation(0, Rotation.Yaw, 0);
////
////		// get Foward Vector
////		const FVector FowardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
////
////		// get Right Vector
////		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
////
////		AddMovementInput(FowardDirection, MovementVector.Y);
////		AddMovementInput(RightDirection, MovementVector.X);
////	}
////}
////
////void ACreatureClass::Look(const FInputActionValue& Value)
////{
////	FVector2D LookAxisVector = Value.Get<FVector2D>();
////
////	if (Controller != nullptr)
////	{
////		AddControllerYawInput(LookAxisVector.X);
////		AddControllerPitchInput(LookAxisVector.Y);
////	}
////}
////
////void ACreatureClass::StartChase()
////{
////	if (!bIsChase)
////	{
////		UE_LOG(LogTemp, Warning, TEXT("SetTrace: Start Chase Function called"));
////		bIsChase = true;
////		AHorrorGameCharacter* player = Cast<AHorrorGameCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
////
////		if (player)
////		{
////			if (!player->bIsCooldown)
////				player->AddPatience(15);
////		}
////	}
////}
////
////void ACreatureClass::EndChase()
////{
////	if (bIsChase)
////	{
////		UE_LOG(LogTemp, Warning, TEXT("SetTrace: End Chase Function called"));
////		/*AAIController_Brute* AIController = Cast<AAIController_Brute>(GetController());
////		if (AIController != nullptr)
////		{
////			AIController->EndChase();
////		}*/
////		bIsChase = false;
////		// ReaperSound->SetSound(PatrolSound);
////	}
////}
////
//////void ACreatureClass::SoundBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
//////{
//////	if (OtherActor != nullptr && OtherActor != this && OtherComp != nullptr)
//////	{
//////		if (OtherActor->IsA<AHorrorGameCharacter>())
//////		{
//////			auto Player = Cast<AHorrorGameCharacter>(OtherActor);
//////			// ReaperSound->Play();
//////			if (!Player->bIsCooldown)
//////				Player->Patience += 5;
//////		}
//////	}
//////}
//////
//////void ACreatureClass::SoundEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
//////{
//////	if (OtherActor != nullptr && OtherActor != this && OtherComp != nullptr)
//////	{
//////		if (OtherActor->IsA<AHorrorGameCharacter>())
//////		{
//////			// ReaperSound->Stop();
//////		}
//////	}
//////}
//////
//////void ACreatureClass::DoorBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
//////{
//////	if (OtherActor != nullptr && OtherActor != this && OtherComp != nullptr)
//////	{
//////		if (auto Classroom = Cast<AClassroomDoorActor_cpp>(OtherActor))
//////		{
//////			if (Classroom->bIsDoorClosed)
//////			{
//////				Classroom->AIInteract();
//////			}
//////		}
//////		else if (auto Door = Cast<ADoor_cpp>(OtherActor))
//////		{
//////			if (Door->bIsDoorClosed)
//////			{
//////				Door->OnInteract();
//////			}
//////		}
//////	}
//////}
////
////FVector ACreatureClass::GetPatrolPoint()
////{
////	int randIdx = FMath::RandRange(0, 24);
////	UWorld* world = GetWorld();
////	FVector ResultLocation = GetActorLocation();
////	for (TActorIterator<APatrolPoint_cpp> entity(world); entity; ++entity)
////	{
////		// TArray<UObject*>components;
////
////		if (entity->GetActorLabel() == PatrolPointList[randIdx].ToString()) {
////			ResultLocation = entity->GetActorLocation();
////			PatrolPoint = *entity;
////			break;
////		}
////	}
////	return ResultLocation;
////}
////
////
////bool ACreatureClass::GetIsStunned()
////{
////	return bIsStunned;
////}
////
////void ACreatureClass::Exorcism()
////{
////	bIsDied = true;
////	// GetMesh()->PlayAnimation(Anim, false);
////	Destroy();
////}
////
////void ACreatureClass::Stunning(float dist)
////{
////	bIsStunned = true;
////	if (dist < 40.f)
////	{
////		StunTime = baseStunTime * 1.5f;
////	}
////	else if (dist < 80.f)
////	{
////		StunTime = baseStunTime;
////	}
////	else if (dist < 120.f)
////	{
////		StunTime = baseStunTime * 0.75f;
////	}
////	else
////	{
////		StunTime = baseStunTime * 0.5f;
////	}
////}
