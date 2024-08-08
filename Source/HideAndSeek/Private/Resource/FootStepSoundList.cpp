// CopyrightNotice 2023 Sunggon Kim kimdave205@gmail.com. All Rights Reserved.

#include "Resource/FootStepSoundList.h"
#include "Sound/SoundCue.h"

// 발소리를 얻는 함수.
USoundCue* UFootStepSoundList::GetSound(FString SoundName)
{
	// 소리 클래스의 배열을 하나씩 확인하며, 파라미터랑 일치하는 키값을 가진 SoundCue를 가져옴. 
	for (TMap<FString, USoundCue*>::TIterator it = m_SoundList.CreateIterator(); it; ++it)
	{
		if (it->Key == SoundName)
		{
			return it->Value;
		}
	}
	return nullptr;
}