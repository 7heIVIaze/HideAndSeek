// CopyrightNotice 2023 Sunggon Kim kimdave205@gmail.com. All Rights Reserved.

#include "FootStepSoundList.h"
#include "Sound/SoundCue.h"

USoundCue* UFootStepSoundList::GetSound(FString SoundName)
{
	for (TMap<FString, USoundCue*>::TIterator it = m_SoundList.CreateIterator(); it; ++it)
	{
		if (it->Key == SoundName)
		{
			return it->Value;
		}
	}
	return nullptr;
}