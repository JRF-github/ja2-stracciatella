#pragma once

#include "Observable.h"
#include "JA2Types.h"
#include "Sector_Coords.h"

#define NUMBER_OF_SAMS 4

// min condition for sam site to be functional
#define MIN_CONDITION_FOR_SAM_SITE_TO_WORK 80

extern Observable<> OnAirspaceControlUpdated;

void InitializeSAMSites();

void UpdateSAMDoneRepair(sector_coords const& coords);

void UpdateAirspaceControl();

bool IsThisSectorASAMSector(sector_coords const& coords);

bool DoesSAMExistHere(sector_coords const& coords, GridNo const gridno);

// number of SAM sites under player control
INT32 GetNumberOfSAMSitesUnderPlayerControl();

// is there a FUNCTIONAL SAM site in this sector?
bool IsThereAFunctionalSAMSiteInSector(sector_coords const& coords);

INT8 GetSAMIdFromSector(sector_coords const& coords);
