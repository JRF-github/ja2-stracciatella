#include "SAM_Sites.h"
#include "Campaign_Types.h"
#include "ContentManager.h"
#include "Explosion_Control.h"
#include "GameInstance.h"
#include "Logger.h"
#include "SamSiteModel.h"
#include "SaveLoadMap.h"
#include "StrategicMap.h"
#include "TileDat.h"
#include "TileDef.h"
#include "WorldMan.h"
#include <string_theory/format>


Observable<> OnAirspaceControlUpdated = {};

static void UpdateAndDamageSAMIfFound(INT16 sSectorX, INT16 sSectorY, INT8 sSectorZ, INT16 sGridNo, void*, UINT8 ubDamage, BOOLEAN fIsDestroyed);

void InitializeSAMSites()
{
	// handle SAM site damages
	OnStructureDamaged.addListener("default:sam", UpdateAndDamageSAMIfFound);

	// All SAM sites start game in perfect working condition.
	for (auto samSite : GCM->getSamSites())
	{
		UINT8 ubSectorID = samSite->sectorId;
		StrategicMap[SECTOR_INFO_TO_STRATEGIC_INDEX(ubSectorID)].bSAMCondition = 100;
	}

	UpdateAirspaceControl();
}

void UpdateSAMDoneRepair(sector_coords const& coords)
{
	// ATE: If we are below, return right away
	if (coords.z != 0) return;

	auto samSite = GCM->findSamSiteBySector(coords);
	if (samSite == NULL)
	{
		STLOGW("There is no SAM site at sector {}", coords.get_short_string());
		return;
	}

	UINT16 const good_graphic = GetTileIndexFromTypeSubIndex(EIGHTISTRUCT, samSite->graphicIndex);
	UINT16 const damaged_graphic = good_graphic - 2; // Damaged one (current) is 2 less
	GridNo const gridno = samSite->gridNos[0];
	if (coords.equals_world_coords())
	{ // Sector loaded, update graphic
		ApplyMapChangesToMapTempFile app;
		RemoveStruct(gridno, damaged_graphic);
		AddStructToHead(gridno, good_graphic);
	}
	else
	{ // We add temp changes to map not loaded
		RemoveStructFromUnLoadedMapTempFile(gridno, damaged_graphic, coords.x, coords.y, coords.z);
		AddStructToUnLoadedMapTempFile(gridno, good_graphic, coords.x, coords.y, coords.z);
	}

	// SAM site may have been put back into working order
	UpdateAirspaceControl();
}

void UpdateAirspaceControl()
{
	auto samList = GCM->getSamSites();
	for (int x = 1; x < (MAP_WORLD_X - 1); x++)
	{
		for (int y = 1; y < (MAP_WORLD_Y - 1); y++)
		{
			BOOLEAN fEnemyControlsAir = FALSE;
			INT8 bControllingSAM = GCM->getControllingSamSite(SECTOR(x, y));
			if (bControllingSAM >= 0 && (UINT8)bControllingSAM < samList.size())
			{
				UINT8 ubSector = samList[bControllingSAM]->sectorId;
				StrategicMapElement* pSAMStrategicMap = &(StrategicMap[SECTOR_INFO_TO_STRATEGIC_INDEX(ubSector)]);

				// if the enemies own the controlling SAM site, and it's in working condition
				if ((pSAMStrategicMap->fEnemyControlled) && (pSAMStrategicMap->bSAMCondition >= MIN_CONDITION_FOR_SAM_SITE_TO_WORK))
				{
					fEnemyControlsAir = TRUE;
				}
			}

			StrategicMap[CALCULATE_STRATEGIC_INDEX(x, y)].fEnemyAirControlled = fEnemyControlsAir;
		}
	}

	OnAirspaceControlUpdated();
}

INT32 GetNumberOfSAMSitesUnderPlayerControl()
{
	INT32 n = 0;
	for (auto samSite : GCM->getSamSites())
	{
		UINT8 ubSectorID = samSite->sectorId;
		if (!StrategicMap[SECTOR_INFO_TO_STRATEGIC_INDEX(ubSectorID)].fEnemyControlled) ++n;
	}
	return n;
}

bool IsThereAFunctionalSAMSiteInSector(sector_coords const& coords)
{
	return
		IsThisSectorASAMSector(coords) &&
		StrategicMap[coords.get_strategic_index()].bSAMCondition >= MIN_CONDITION_FOR_SAM_SITE_TO_WORK;
}


bool IsThisSectorASAMSector(sector_coords const& coords)
{
	if (coords.z != 0) return false;

	return (GCM->findSamIDBySector(coords) > -1);
}


// a -1 will be returned upon failure
INT8 GetSAMIdFromSector(sector_coords const& coords)
{
	// check if valid sector
	if (coords.z != 0)
	{
		return(-1);
	}

	// get the sector value
	return GCM->findSamIDBySector(coords);
}

bool DoesSAMExistHere(sector_coords const& coords, GridNo const gridno)
{
	// ATE: If we are below, return right away
	if (coords.z != 0) return false;

	for (auto s : GCM->getSamSites())
	{
		if (s->doesSamExistHere(coords.x, coords.y, gridno))
		{
			return true;
		}
	}
	return false;
}

// Look for a SAM site, update
static void UpdateAndDamageSAMIfFound(INT16 sSectorX, INT16 sSectorY, INT8 sSectorZ, INT16 sGridNo, void*, UINT8 ubDamage, BOOLEAN fIsDestroyed)
{
	sector_coords const coords{sSectorX, sSectorY, sSectorZ};

	// OK, First check if SAM exists, and if not, return
	if (!DoesSAMExistHere(coords, sGridNo))
	{
		return;
	}

	// Damage.....
	INT16 sSectorNo = coords.get_strategic_index();
	STLOGD("SAM site at sector {} is damaged by {} points", coords.get_short_string(), ubDamage);
	if (StrategicMap[sSectorNo].bSAMCondition >= ubDamage)
	{
		StrategicMap[sSectorNo].bSAMCondition -= ubDamage;
	}
	else
	{
		StrategicMap[sSectorNo].bSAMCondition = 0;
	}

	// SAM site may have been put out of commission...
	UpdateAirspaceControl();

	// ATE: GRAPHICS UPDATE WILL GET DONE VIA NORMAL EXPLOSION CODE.....
}
