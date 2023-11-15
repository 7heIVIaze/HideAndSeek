// CopyrightNotice=0 2023 Sunggon Kim

#include "InteractDot_cpp.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"

void UInteractDot_cpp::NativeConstruct()
{
	Super::NativeConstruct();

	InteractDot = Cast<UImage>(GetWidgetFromName(TEXT("InteractCrosshair")));
	InteractText = Cast<UTextBlock>(GetWidgetFromName(TEXT("Interact_Text")));
	ErrorText = Cast<UTextBlock>(GetWidgetFromName(TEXT("Interact_Error")));
}

void UInteractDot_cpp::SetInteractText(FString strText)
{
	InteractText->SetText(FText::FromString(strText));
}

void UInteractDot_cpp::SetErrorText(FString strText)
{
	ErrorText->SetText(FText::FromString(strText));
}

void UInteractDot_cpp::SetInteractDot(bool Value)
{
	if (Value)
	{
		//InteractText->SetVisibility(ESlateVisibility::Visible);
		InteractDot->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		//InteractText->SetVisibility(ESlateVisibility::Collapsed);
		InteractDot->SetVisibility(ESlateVisibility::Collapsed);
	}
}

//void UInteractDot_cpp::InteractUIInit()
//{
//	Refresh();
//}
//
//void UInteractDot_cpp::Refresh()
//{
//	InteractText->Player = this->Player;
//	ErrorInteractText->Player = this->Player;
//	InteractText->InteractUIInit();
//	ErrorInteractText->InteractUIInit();
//}