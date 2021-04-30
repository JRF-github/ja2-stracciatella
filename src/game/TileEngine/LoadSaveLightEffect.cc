#include "Debug.h"
#include "FileMan.h"
#include "LoadSaveData.h"
#include "LoadSaveLightEffect.h"


void ExtractLightEffectFromFile(HWFILE const file, LIGHTEFFECT* const l)
{
	FileDataReader{16, file}
	  >> l->sGridNo
	  >> l->ubDuration
	  >> l->bRadius
	  >> l->bAge
	  >> l->fAllocated
	  >> l->bType
	  >> skip<5>
	  >> l->uiTimeOfLastUpdate;

	l->light = NULL;
}


void InjectLightEffectIntoFile(HWFILE const file, const LIGHTEFFECT* const l)
{
	FileDataWriter{16, file}
	  << l->sGridNo
	  << l->ubDuration
	  << l->bRadius
	  << l->bAge
	  << l->fAllocated
	  << l->bType
	  << skip<5>
	  << l->uiTimeOfLastUpdate;
}
