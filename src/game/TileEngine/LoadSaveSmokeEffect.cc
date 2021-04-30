#include "Debug.h"
#include "FileMan.h"
#include "LoadSaveData.h"
#include "LoadSaveSmokeEffect.h"
#include "Overhead.h"
#include "SmokeEffects.h"


void ExtractSmokeEffectFromFile(HWFILE const file, SMOKEEFFECT* const s)
{
	FileDataReader d{16, file};
	d >> s->sGridNo
	  >> s->ubDuration
	  >> s->ubRadius
	  >> s->bFlags
	  >> s->bAge
	  >> s->fAllocated
	  >> s->bType
	  >> s->usItem
	  >> s->owner
	  >> skip<1>
	  >> s->uiTimeOfLastUpdate;
}


void InjectSmokeEffectIntoFile(HWFILE const file, SMOKEEFFECT const* const s)
{
	FileDataWriter{16, file}
	  << s->sGridNo
	  << s->ubDuration
	  << s->ubRadius
	  << s->bFlags
	  << s->bAge
	  << s->fAllocated
	  << s->bType
	  << s->usItem
	  << Soldier2ID(s->owner)
	  << skip<1>
	  << s->uiTimeOfLastUpdate;
}
