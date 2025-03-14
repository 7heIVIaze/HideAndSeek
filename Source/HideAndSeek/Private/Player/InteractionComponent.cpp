// CopyrightNotice 2023 Sunggon Kim kimdave205@gmail.com. All Rights Reserved.

#include "Player/InteractionComponent.h"
#include "Player/HorrorGameCharacter.h"
#include "Furniture/DoorClass.h"
#include "Furniture/LockerDoorActor_cpp.h"
#include "Furniture/DrawerClass.h"
#include "Furniture/HideObject.h"
#include "Furniture/Alarm.h"
#include "Furniture/LightingClass.h"
#include "Furniture/DistributionBox.h"
#include "Furniture/LightSwitch.h"
#include "Furniture/SwitchLever.h"
#include "Furniture/Altar_cpp.h"
#include "Furniture/Paper.h"
#include "Furniture/End_Mirror.h"
#include "Items/ItemClass.h"
#include "HUD/GameUI.h"

// Sets default values for this component's properties
UInteractionComponent::UInteractionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// 초기화
	HitActor = nullptr;

	TraceLength = 160.0f;
	// ...
}


// Called when the game starts
void UInteractionComponent::BeginPlay()
{
	Super::BeginPlay();
	OwnerPlayer = Cast<AHorrorGameCharacter>(GetOwner());
	// ...
	
}


// Called every frame
void UInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// 초기화
	TraceStartVector = OwnerPlayer->GetCameraComponentLocation();
	TraceForwardVector = OwnerPlayer->GetCameraComponentForwardVector();
	TraceEndVector = (TraceForwardVector * TraceLength) + TraceStartVector;

	FHitResult HitResult;

	// RayCast를 매 Tick마다 실시함.
	bool bIsHit = GetWorld()->LineTraceSingleByChannel(HitResult, TraceStartVector, TraceEndVector, ECC_Visibility);
	HitActor = HitResult.GetActor();
	//DrawDebugLine(GetWorld(), TraceStartVector, TraceEndVector, FColor::Red);
	// 부딪힌 대상이 있을 때
	if (bIsHit)
	{
		if (HitActor)
		{
			// If the actor's class is door class.
			if (HitActor->IsA<ADoorClass>())
			{
				// Player Game UI Widget
				if (OwnerPlayer)
				{
					if (OwnerPlayer->GameUIWidget)
					{
						OwnerPlayer->GameUIWidget->SetInteractDot(true);
						OwnerPlayer->GameUIWidget->SetInteractDotText(NSLOCTEXT("UInteractionComponent", "OpenClose", "Open/Close"));
						return;
					}
				}
			}
			// If the actor's class is item class.
			else if (HitActor->IsA<AItemClass>())
			{
				// Player Game UI Widget
				if (OwnerPlayer)
				{
					if (OwnerPlayer->GameUIWidget)
					{
						OwnerPlayer->GameUIWidget->SetInteractDot(true);
						OwnerPlayer->GameUIWidget->SetInteractDotText(NSLOCTEXT("UInteractionComponent", "Take", "Take"));
						return;
					}
				}
			}
			// 부딪힌 액터가 서랍 클래스인 경우
			else if (HitActor->IsA<ADrawerClass>()) // 서랍류(책상 서랍, 옷장 서랍, 일반 서랍)일 경우
			{
				// 상호 작용 조준점으로 조준점을 변경하고, "Open/Close" 텍스트를 출력함.
				if (OwnerPlayer->GameUIWidget)
				{
					OwnerPlayer->GameUIWidget->SetInteractDotText(NSLOCTEXT("AHorrorGameCharacter", "Look_Drawer", "Open/Close"));
					OwnerPlayer->GameUIWidget->SetInteractDot(true);
				}
				return;
			}
			// 부딪힌 액터가 숨는 액터 클래스인 경우
			else if (HitActor->IsA<AHideObject>()) // 숨는 포인트일 경우(옷장, 캐비닛)
			{
				// 상호 작용 조준점으로 조준점을 변경함
				if (OwnerPlayer->GameUIWidget)
				{
					// 숨은 상태일 경우, "Exit" 텍스트를 출력함.
					if (OwnerPlayer->GetIsHiding())
					{
						OwnerPlayer->GameUIWidget->SetInteractDotText(NSLOCTEXT("AHorrorGameCharacter", "Look_Cabinet_While_Hiding", "Exit"));
					}
					// 숨지 않은 상태일 경우, "Hide" 텍스트를 출력함.
					else
					{
						OwnerPlayer->GameUIWidget->SetInteractDotText(NSLOCTEXT("AHorrorGameCharacter", "Look_Cabinet", "Hide"));
					}
					OwnerPlayer->GameUIWidget->SetInteractDot(true);
				}
				return;
			}
			// 부딪힌 액터가 사물함 문 액터 클래스인 경우
			else if (HitActor->IsA<ALockerDoorActor_cpp>()) // 사물함 문일경우
			{
				// 상호 작용 조준점으로 조준점을 변경하고, "Open/Close" 텍스트를 출력함.
				if (OwnerPlayer->GameUIWidget)
				{
					OwnerPlayer->GameUIWidget->SetInteractDotText(NSLOCTEXT("AHorrorGameCharacter", "Look_Locker", "Open/Close"));
					OwnerPlayer->GameUIWidget->SetInteractDot(true);
				}
				return;
			}
			// 부딪힌 액터가 경보기 액터 클래스인 경우
			else if (HitActor->IsA<AAlarm>()) // 경보기일 경우
			{
				// 상호 작용 조준점으로 조준점을 변경하고, "Ring" 텍스트를 출력함.
				if (OwnerPlayer->GameUIWidget)
				{
					OwnerPlayer->GameUIWidget->SetInteractDotText(NSLOCTEXT("AHorrorGameCharacter", "Look_Alarm", "Ring"));
					OwnerPlayer->GameUIWidget->SetInteractDot(true);
				}
				return;
			}
			// 부딪힌 액터가 전등 액터 클래스인 경우
			else if (ALightingClass* Light = Cast<ALightingClass>(HitActor)) // 전등일 경우
			{
				// 불이 안 켜진 상태일 때만 상호 작용 조준점으로 조준점을 변경하고, "Turn on" 텍스트를 출력함.
				if (!Light->bIsLightOn)
				{
					if (OwnerPlayer->GameUIWidget)
					{
						OwnerPlayer->GameUIWidget->SetInteractDotText(NSLOCTEXT("AHorrorGameCharacter", "Look_Hanging_Light", "Turn On"));
						OwnerPlayer->GameUIWidget->SetInteractDot(true);
					}
					return;
				}
			}
			// 부딪힌 액터가 분전함 액터 클래스인 경우
			else if (ADistributionBox* DBox = Cast<ADistributionBox>(HitActor)) // 전압기일 경우
			{
				// 분전함의 전원이 안 켜진 상태일 때만 상호 작용 조준점으로 조준점을 변경하고, "Power on" 텍스트를 출력함.
				if (!DBox->bIsPowered)
				{
					if (OwnerPlayer->GameUIWidget)
					{
						OwnerPlayer->GameUIWidget->SetInteractDotText(NSLOCTEXT("AHorrorGameCharacter", "Look_Distribution_Box", "Power On"));
						OwnerPlayer->GameUIWidget->SetInteractDot(true);
					}
					return;
				}
			}
			// 부딪힌 액터가 전등 스위치 액터 클래스인 경우
			else if (HitActor->IsA<ALightSwitch>()) // 전등 스위치일 경우
			{
				// 상호 작용 조준점으로 조준점을 변경하고, "Turn on" 텍스트를 출력함.
				if (OwnerPlayer->GameUIWidget)
				{
					OwnerPlayer->GameUIWidget->SetInteractDotText(NSLOCTEXT("AHorrorGameCharacter", "Look_Light_Switch", "Turn On"));
					OwnerPlayer->GameUIWidget->SetInteractDot(true);
				}
				return;
			}
			// 부딪힌 액터가 제단 액터 클래스인 경우
			else if (HitActor->IsA<AAltar_cpp>()) // 제단일 경우
			{
				// 아직 클리어한 상태가 아닌 경우에만 상호 작용 조준점으로 조준점을 변경하고, "Place the Reaper's Items" 텍스트를 출력함.
				//if (!bIsCleared)
				//{
				if (OwnerPlayer->GameUIWidget)
				{
					OwnerPlayer->GameUIWidget->SetInteractDotText(NSLOCTEXT("AHorrorGameCharacter", "Look_Altar", "Place the Reaper's Items"));
					OwnerPlayer->GameUIWidget->SetInteractDot(true);
				}
				return;
				//}
			}
			// 부딪힌 액터가 스위치 레버 액터 클래스인 경우
			else if (HitActor->IsA<ASwitchLever>()) // 레버일 경우
			{
				// 상호 작용 조준점으로 조준점을 변경하고, "Lift Up/Down" 텍스트를 출력함.
				if (OwnerPlayer->GameUIWidget)
				{
					OwnerPlayer->GameUIWidget->SetInteractDotText(NSLOCTEXT("AHorrorGameCharacter", "Look_Switch_Lever", "Lift Up/Down"));
					OwnerPlayer->GameUIWidget->SetInteractDot(true);
				}
				return;
			}
			// 부딪힌 액터가 종이 액터 클래스인 경우
			else if (HitActor->IsA<APaper>()) // 종이일 경우
			{
				// 상호 작용 조준점으로 조준점을 변경하고, "Look" 텍스트를 출력함.
				if (OwnerPlayer->GameUIWidget)
				{
					OwnerPlayer->GameUIWidget->SetInteractDotText(NSLOCTEXT("AHorrorGameCharacter", "Look_Paper", "Look"));
					OwnerPlayer->GameUIWidget->SetInteractDot(true);
				}
				return;
			}
			// 부딪힌 액터가 엔딩 거울 액터 클래스인 경우
			else if (HitActor->IsA<AEnd_Mirror>()) // 엔딩용 거울일 경우
			{
				// 상호 작용 조준점으로 조준점을 변경하고, "Go To Natty" 텍스트를 출력함.
				if (OwnerPlayer->GameUIWidget)
				{
					OwnerPlayer->GameUIWidget->SetInteractDotText(NSLOCTEXT("AHorrorGameCharacter", "Look_Mirror", "Go To Natty"));
					OwnerPlayer->GameUIWidget->SetInteractDot(true);
				}
				return;
			}

			else
			{
				if (OwnerPlayer)
				{
					if (OwnerPlayer->GameUIWidget)
					{
						OwnerPlayer->GameUIWidget->SetInteractDot(false);
						OwnerPlayer->GameUIWidget->SetInteractDotText(NSLOCTEXT("UInteractionComponent", "None", ""));
						return;
					}
				}
			}
		}
	}

	if (OwnerPlayer)
	{
		if (OwnerPlayer->GameUIWidget)
		{
			OwnerPlayer->GameUIWidget->SetInteractDot(false);
			OwnerPlayer->GameUIWidget->SetInteractDotText(NSLOCTEXT("UInteractionComponent", "None", ""));
		}
	}
	// ...
}

AActor* UInteractionComponent::GetTracedActor()
{
	return HitActor;
}