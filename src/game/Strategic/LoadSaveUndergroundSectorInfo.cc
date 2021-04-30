#include "Debug.h"
#include "FileMan.h"
#include "LoadSaveUndergroundSectorInfo.h"
#include "LoadSaveData.h"


void ExtractUndergroundSectorInfoFromFile(HWFILE const file, UNDERGROUND_SECTORINFO* const u)
{
	FileDataReader{72, file}
	  >> u->uiFlags
	  >> u->ubSectorX
	  >> u->ubSectorY
	  >> u->ubSectorZ
	  >> u->ubNumElites
	  >> u->ubNumTroops
	  >> u->ubNumAdmins
	  >> u->ubNumCreatures
	  >> skip<5>
	  >> u->uiTimeCurrentSectorWasLastLoaded
	  >> u->next
	  >> u->ubAdjacentSectors
	  >> u->ubCreatureHabitat
	  >> u->ubElitesInBattle
	  >> u->ubTroopsInBattle
	  >> u->ubAdminsInBattle
	  >> u->ubCreaturesInBattle
	  >> skip<2>
	  >> u->uiNumberOfWorldItemsInTempFileThatCanBeSeenByPlayer
	  >> skip<36>;
}


void InjectUndergroundSectorInfoIntoFile(HWFILE const file, UNDERGROUND_SECTORINFO const* const u)
{
	FileDataWriter{72, file}
	  << u->uiFlags
	  << u->ubSectorX
	  << u->ubSectorY
	  << u->ubSectorZ
	  << u->ubNumElites
	  << u->ubNumTroops
	  << u->ubNumAdmins
	  << u->ubNumCreatures
	  << skip<5>
	  << u->uiTimeCurrentSectorWasLastLoaded
	  << u->next
	  << u->ubAdjacentSectors
	  << u->ubCreatureHabitat
	  << u->ubElitesInBattle
	  << u->ubTroopsInBattle
	  << u->ubAdminsInBattle
	  << u->ubCreaturesInBattle
	  << skip<2>
	  << u->uiNumberOfWorldItemsInTempFileThatCanBeSeenByPlayer
	  << skip<36>;
}
