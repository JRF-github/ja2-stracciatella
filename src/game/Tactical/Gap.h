#ifndef GAP_H
#define GAP_H

#include "Faces.h"

BOOLEAN PollAudioGap(UINT32 uiSoundId, std::vector<AUDIO_GAP> const& gapList);
UINT32  PlayJA2GapSample(const char* zSoundFile, UINT32 ubVolume, UINT32 ubLoops, UINT32 uiPan, std::vector<AUDIO_GAP> * gapList);

#endif
