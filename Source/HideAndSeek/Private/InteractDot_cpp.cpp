// CopyrightNotice 2023 Sunggon Kim kimdave205@gmail.com. All Rights Reserved.

#include "InteractDot_cpp.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"

void UInteractDot_cpp::NativeConstruct()
{
	Super::NativeConstruct();

	/*InteractDot = Cast<UImage>(GetWidgetFromName(TEXT("InteractCrosshair")));
	InteractText = Cast<UTextBlock>(GetWidgetFromName(TEXT("Interact_Text")));
	ErrorText = Cast<UTextBlock>(GetWidgetFromName(TEXT("Interact_Error")));*/
}

// 상호작용 텍스트 설정 함수.
void UInteractDot_cpp::SetInteractText(FText strText)
{
	InteractText->SetText(strText);
}

// 상호작용 실패 텍스트 설정 함수.
void UInteractDot_cpp::SetErrorText(FText strText)
{
	ErrorText->SetText(strText);
}

// 상호작용 조준점 보이게 할 지 설정하는 함수, (true면 보이고, false면 안 보임)
void UInteractDot_cpp::SetInteractDot(bool Value)
{
	if (Value)
	{
		InteractDot->SetVisibility(ESlateVisibility::HitTestInvisible);
	}
	else
	{
		InteractDot->SetVisibility(ESlateVisibility::Collapsed);
	}
}