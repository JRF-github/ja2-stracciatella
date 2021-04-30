#include "Debug.h"
#include "FileMan.h"
#include "LoadSaveStrategicMapElement.h"
#include "LoadSaveData.h"


void ExtractStrategicMapElementFromFile(HWFILE const f, StrategicMapElement& e)
{
	FileDataReader{41, f}
	  >> skip<16>
	  >> e.bNameId
	  >> e.fEnemyControlled
	  >> e.fEnemyAirControlled
	  >> skip<1>
	  >>  e.bSAMCondition
	  >> skip<20>;
}


void InjectStrategicMapElementIntoFile(HWFILE const f, StrategicMapElement const& e)
{
	FileDataWriter{41, f}
	  << skip<16>
	  << e.bNameId
	  << e.fEnemyControlled
	  << e.fEnemyAirControlled
	  << skip<1>
	  << e.bSAMCondition
	  << skip<20>;
}
