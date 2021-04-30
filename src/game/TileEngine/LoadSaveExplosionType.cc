#include "Debug.h"
#include "FileMan.h"
#include "LoadSaveExplosionType.h"
#include "LoadSaveData.h"
#include "Overhead.h"


void ExtractExplosionTypeFromFile(HWFILE const file, EXPLOSIONTYPE* const e)
{
	FileDataReader{36, file}
	  >> skip<4>
	  >> e->owner
	  >> e->ubTypeID
	  >> e->usItem
	  >> e->sX
	  >> e->sY
	  >> e->sZ
	  >> e->sGridNo
	  >> skip<1>
	  >> e->bLevel
	  >> skip<2>
	  >> e->fAllocated
	  >> skip<1>
	  >> e->sCurrentFrame
	  >> skip<12>;

	e->light = NULL;
}


void InjectExplosionTypeIntoFile(HWFILE const file, EXPLOSIONTYPE const* e)
{
	FileDataWriter{36, file}
	  << skip<4>
	  << e->owner
	  << e->ubTypeID
	  << e->usItem
	  << e->sX
	  << e->sY
	  << e->sZ
	  << e->sGridNo
	  << skip<1>
	  << e->bLevel
	  << skip<2>
	  << e->fAllocated
	  << skip<1>
	  << e->sCurrentFrame
	  << skip<12>;
}
