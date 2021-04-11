#ifndef __QUEEN_COMMAND_H
#define __QUEEN_COMMAND_H

#include "Campaign_Types.h"
#include "JA2Types.h"
#include "Strategic_Movement.h"


extern BOOLEAN gfPendingEnemies;


//Counts enemies and crepitus, but not bloodcats.
UINT8 NumHostilesInSector( INT16 sSectorX, INT16 sSectorY, INT16 sSectorZ );
static inline uint8_t NumHostilesInSector(sector_coords const& coords)
{
	return NumHostilesInSector(coords.x, coords.y, coords.z);
}
UINT8 NumEnemiesInAnySector( INT16 sSectorX, INT16 sSectorY, INT16 sSectorZ );
static inline uint8_t NumEnemiesInAnySector(sector_coords const& coords)
{
	return NumEnemiesInAnySector(coords.x, coords.y, coords.z);
}
UINT8 NumEnemiesInSector( INT16 sSectorX, INT16 sSectorY );
static inline uint8_t NumEnemiesInSector(sector_coords const& coords)
{
	return NumEnemiesInSector(coords.x, coords.y);
}
UINT8 NumStationaryEnemiesInSector( INT16 sSectorX, INT16 sSectorY );
UINT8 NumMobileEnemiesInSector( INT16 sSectorX, INT16 sSectorY );
void GetNumberOfEnemiesInSector( INT16 sSectorX, INT16 sSectorY, UINT8 *pubNumAdmins, UINT8 *pubNumTroops, UINT8 *pubNumElites );

/* Called when entering a sector so the campaign AI can automatically insert the
 * correct number of troops of each type based on the current number in the
 * sector in global focus (gWorldSectorX/Y) */
void PrepareEnemyForSectorBattle();

void AddPossiblePendingEnemiesToBattle(void);
void EndTacticalBattleForEnemy(void);

void ProcessQueenCmdImplicationsOfDeath(const SOLDIERTYPE*);

void SaveUnderGroundSectorInfoToSaveGame(HWFILE);
void LoadUnderGroundSectorInfoFromSavedGame(HWFILE);

//Finds and returns the specified underground structure ( DONT MODIFY IT ).  Else returns NULL
UNDERGROUND_SECTORINFO* FindUnderGroundSector( INT16 sMapX, INT16 sMapY, UINT8 bMapZ );

void EnemyCapturesPlayerSoldier( SOLDIERTYPE *pSoldier );
void BeginCaptureSquence(void);
void EndCaptureSequence(void);

BOOLEAN PlayerSectorDefended( UINT8 ubSectorID );

BOOLEAN OnlyHostileCivsInSector(void);

extern INT16 gsInterrogationGridNo[3];

#endif
