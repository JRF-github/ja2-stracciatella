#ifndef __AUTO_RESOLVE_H
#define __AUTO_RESOLVE_H

#include "Sector_Coords.h"
#include "ScreenIDs.h"
#include "Types.h"
struct OBJECTTYPE;
struct SOLDIERTYPE;


void EnterAutoResolveMode( UINT8 ubSectorX, UINT8 ubSectorY );

// is the autoresolve active?
BOOLEAN IsAutoResolveActive( void );

void EliminateAllEnemies( UINT8 ubSectorX, UINT8 ubSectorY );
static inline void EliminateAllEnemies(sector_coords const& coords)
{
    EliminateAllEnemies(coords.x, coords.y);
}

void ConvertTacticalBattleIntoStrategicAutoResolveBattle(void);

UINT8 GetAutoResolveSectorID(void);

extern BOOLEAN gfTransferTacticalOppositionToAutoResolve;

//Returns TRUE if autoresolve is active or a sector is loaded.
BOOLEAN GetCurrentBattleSectorXYZ( INT16 *psSectorX, INT16 *psSectorY, INT16 *psSectorZ );

UINT32 VirtualSoldierDressWound(SOLDIERTYPE* pSoldier, SOLDIERTYPE* pVictim, OBJECTTYPE* pKit, INT16 sKitPts, INT16 sStatus);

//Returns TRUE if a battle is happening ONLY
BOOLEAN GetCurrentBattleSectorXYZAndReturnTRUEIfThereIsABattle(INT16* psSectorX, INT16* psSectorY, INT16* psSectorZ);

ScreenID AutoResolveScreenHandle(void);

#endif
