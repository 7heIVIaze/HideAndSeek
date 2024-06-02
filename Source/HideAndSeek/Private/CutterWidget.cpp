// CopyrightNotice 2023 Sunggon Kim kimdave205@gmail.com. All Rights Reserved.

#include "CutterWidget.h"
#include "Components/Image.h"
#include "Components/ProgressBar.h"

void UCutterWidget::NativeConstruct()
{
	Super::NativeConstruct();
	CutterImg = Cast<UImage>(GetWidgetFromName("CutterIcon"));
	CutterBar = Cast<UProgressBar>(GetWidgetFromName("CutterLeft"));
}

void UCutterWidget::SetCutterPercent(int32 value)
{
	float percent = (float)value / 5.0f;
	CutterBar->SetPercent(percent);
}

//void UCutterWidget::Refresh()
//{
//
//}

//APlayer_HUD_cpp::APlayer_HUD_cpp()
//{
//	static ConstructorHelpers::FClassFinder<UUserWidget> FlashLightHUDClassFinder(TEXT("/Game/Assets/BluePrints/BatteryWidget_BP"));
//	if (FlashLightHUDClassFinder.Succeeded())
//	{
//		FlashLightBattery_HUD_Class = FlashLightHUDClassFinder.Class;
//	}
//
//	static ConstructorHelpers::FClassFinder<UUserWidget> BasicInterfaceClassFinder(TEXT("/Game/FirstPerson/Blueprints/BasicInterface"));
//	if (BasicInterfaceClassFinder.Succeeded())
//	{
//		BasicHUD_Class = BasicInterfaceClassFinder.Class;
//	}
//
//	static ConstructorHelpers::FClassFinder<UUserWidget> StaminaHUDClassFinder(TEXT("/Game/Assets/BluePrints/Stamina_HUD_BP"));
//	if (StaminaHUDClassFinder.Succeeded())
//	{
//		StaminaHUD_Class = StaminaHUDClassFinder.Class;
//	}
//}
//
//void APlayer_HUD_cpp::BeginPlay()
//{
//	Super::BeginPlay();
//
//	if (IsValid(BasicHUD_Class))
//	{
//		BasicHUD = CreateWidget(GetWorld(), BasicHUD_Class);
//		if (IsValid(BasicHUD))
//		{
//			BasicHUD->AddToViewport();
//		}
//	}
//
//	if (IsValid(FlashLightBattery_HUD_Class))
//	{
//		FlashLightBattery_HUD = Cast<UUserWidget>(CreateWidget(GetWorld(), FlashLightBattery_HUD_Class));
//		if (IsValid(FlashLightBattery_HUD))
//		{
//			FlashLightBattery_HUD->AddToViewport();
//		}
//	}
//
//	if (IsValid(StaminaHUD_Class))
//	{
//		StaminaWidget = Cast<UUserWidget>(CreateWidget(GetWorld(), StaminaHUD_Class));
//		if (IsValid(StaminaWidget))
//		{
//			StaminaWidget->AddToViewport();
//		}
//	}
//}
//
//void APlayer_HUD_cpp::Tick(float DeltaTime)
//{
//	Super::Tick(DeltaTime);
//
//	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
//	//AHorrorGamePlayer* PlayerCharacter = nullptr;
//	if (PlayerController)
//	{
//	//	PlayerCharacter = Cast<AHorrorGameCharacter>(PlayerController->GetPawn());
//	}
//	/*if (PlayerCharacter)
//	{
//		if (PlayerCharacter->GetStamina() < 200)
//		{
//			StaminaWidget->SetVisibility(ESlateVisibility::Visible);
//		}
//		else
//		{
//			StaminaWidget->SetVisibility(ESlateVisibility::Collapsed);
//		}
//	}*/
//}