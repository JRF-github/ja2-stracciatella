#include "Debug.h"
#include "FileMan.h"
#include "Gap.h"
#include "LoadSaveData.h"
#include "SoundMan.h"
#include "Sound_Control.h"

#include "ContentManager.h"
#include "GameInstance.h"

#include "Logger.h"

static void AudioGapListInit(const char* zSoundFile, std::vector<AUDIO_GAP> & gapList)
{
	// This procedure will load in the appropriate .gap file, corresponding to
	// the .wav file in szSoundEffects indexed by uiSampleNum.  The procedure
	// will then allocate and load in the AUDIO_GAP information, while counting
	// the number of elements loaded

	// strip .wav and change to .gap
	ST::string sFileName(FileMan::replaceExtension(zSoundFile, "gap"));

	try
	{
		AutoSGPFile f(GCM->openGameResForReading(sFileName));

		// gap file exists
		// now read in the AUDIO_GAPs
		const UINT32 size = FileGetSize(f);

		const UINT32 count = size / 8;
		if (count > 0)
		{
			gapList.reserve(count);
			std::unique_ptr<BYTE []> data{new BYTE[size]};
			FileRead(f, data.get(), size);

			DataReader d{data.get()};
			for (UINT32 i = 0; i < count; ++i)
			{
				UINT32 start;
				UINT32 end;

				EXTR_U32(d, start);
				EXTR_U32(d, end);

				gapList.emplace_back(start, end);

				SLOGD("Gap Start %d and Ends %d", start, end);
			}
			Assert(d.getConsumed() == size);

			SLOGD("gap list started from file %s", sFileName.c_str());
			return;
		}
	}
	catch (...) { /* Handled below */ }

	gapList.clear();
}


BOOLEAN PollAudioGap(UINT32 const uiSoundId, std::vector<AUDIO_GAP> const& gapList)
{
	// This procedure will access the AudioGapList pertaining to the .wav about
	// to be played and returns whether there is a gap currently.  This is done
	// by searching the gap list for an AUDIO_GAP element where the current time
	// is between the start and end. If we find one, return TRUE. In all other
	// cases, return FALSE.

	if (gapList.empty()) return FALSE;

	const UINT32 time = SoundGetPosition(uiSoundId);

	// See if we can find an element for this sound position
	auto const i = std::find_if(gapList.begin(), gapList.end(), [time](AUDIO_GAP const& gap)
		{ return gap.first <= time && time <= gap.second; });

	if (i == gapList.end()) return FALSE;

	// we are within the time frame
	STLOGD("Gap {}-{}, current {}", i->first, i->second, time);
	return TRUE;
}


UINT32 PlayJA2GapSample(const char* zSoundFile, UINT32 ubVolume, UINT32 ubLoops, UINT32 uiPan, std::vector<AUDIO_GAP> * pData)
{
	// Setup Gap Detection, if it is not null
	if (pData != NULL) AudioGapListInit(zSoundFile, *pData);

	const UINT32 vol = CalculateSpeechVolume(ubVolume);
	return SoundPlay(zSoundFile, vol, uiPan, ubLoops, NULL, NULL);
}
