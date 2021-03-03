#include "Handle_Items.h"
#include "Isometric_Utils.h"
#include "Structure.h"
#include "TileDat.h"
#include "WorldDef.h"
#include "Render_Fun.h"
#include "RenderWorld.h"
#include "Dialogue_Control.h"
#include "Structure_Wrap.h"
#include "PathAI.h"
#include "Overhead.h"
#include "Smell.h"
#include "FOV.h"
#include "Rotting_Corpses.h"
#include "Keys.h"
#include "Random.h"
#include "Input.h"
#include "Exit_Grids.h"
#include "Environment.h"
#include "WorldMan.h"
#include "Fog_Of_War.h"
#include "StrategicMap.h"
#include "Boxing.h"
#include "OppList.h"
#include "Lighting.h"
#include "Soldier_Macros.h"

#ifdef _DEBUG
#include "Font.h"
#include "Font_Control.h"
#include "Timer_Control.h"
#include "Video.h"
#endif

#include <string_theory/format>

#include <algorithm>
#include <iterator>
#include <vector>

/* view directions */
#define DLEFT           0
#define DRIGHT          1
#define UP              2
#define LEFT            3
#define RIGHT           4
#define NOVIEW          5
#define MAXVIEWPATHS    17
#define VIEWPATHLENGTH  13

UINT8 gubGridNoMarkers[ WORLD_MAX ];
UINT8 gubGridNoValue          = 254;

#ifdef _DEBUG
UINT8 gubFOVDebugInfoInfo[ WORLD_MAX ];
#endif


static UINT8 const ViewPath[MAXVIEWPATHS][VIEWPATHLENGTH] {
	{NOVIEW,  UP,     UP,     UP,     UP,     UP,     UP,     UP,     UP,     UP,     UP,     UP,     UP    },
	{UP,      UP,     UP,     UP,     DRIGHT, UP,     UP,     UP,     UP,     UP,     UP,     UP,     UP    },
	{UP,      UP,     UP,     UP,     DLEFT,  UP,     UP,     UP,     UP,     UP,     UP,     UP,     UP    },
	{UP,      UP,     DLEFT,  UP,     DLEFT,  UP,     UP,     UP,     UP,     UP,     UP,     UP,     UP    },
	{UP,      UP,     DRIGHT, UP,     DRIGHT, UP,     UP,     UP,     UP,     UP,     UP,     UP,     UP    },
	{UP,      UP,     DRIGHT, DRIGHT, DRIGHT, UP,     UP,     UP,     UP,     UP,     UP,     UP,     UP    },
	{UP,      UP,     DLEFT,  DLEFT,  DLEFT,  UP,     UP,     UP,     UP,     UP,     UP,     UP,     UP    },
	{UP,      RIGHT,  UP,     DRIGHT, DRIGHT, DRIGHT, UP,     UP,     UP,     UP,     UP,     UP,     UP    },
	{UP,      LEFT,   UP,     DLEFT,  DLEFT,  DLEFT,  UP,     UP,     UP,     UP,     UP,     UP,     UP    },
	{DLEFT,   DLEFT,  DLEFT,  DLEFT,  DLEFT,  UP,     UP,     UP,     UP,     UP,     UP,     UP,     UP    },
	{DRIGHT,  DRIGHT, DRIGHT, DRIGHT, DRIGHT, UP,     UP,     UP,     UP,     UP,     UP,     UP,     UP    },
	{RIGHT,   DRIGHT, DRIGHT, DRIGHT, DRIGHT, DRIGHT, UP,     UP,     UP,     UP,     UP,     UP,     UP    },
	{LEFT,    DLEFT,  DLEFT,  DLEFT,  DLEFT,  DLEFT,  UP,     UP,     UP,     UP,     UP,     UP,     UP    },
	{DLEFT,   LEFT,   LEFT,   UP,     NOVIEW, NOVIEW, NOVIEW, NOVIEW, NOVIEW, NOVIEW, NOVIEW, NOVIEW, NOVIEW},
	{LEFT,    LEFT,   LEFT,   UP,     LEFT,   NOVIEW, NOVIEW, NOVIEW, NOVIEW, NOVIEW, NOVIEW, NOVIEW, NOVIEW},
	{DRIGHT,  RIGHT,  RIGHT,  UP,     NOVIEW, NOVIEW, NOVIEW, NOVIEW, NOVIEW, NOVIEW, NOVIEW, NOVIEW, NOVIEW},
	{RIGHT,   RIGHT,  RIGHT,  UP,     RIGHT,  NOVIEW, NOVIEW, NOVIEW, NOVIEW, NOVIEW, NOVIEW, NOVIEW, NOVIEW}
};

static UINT8 const ViewPath2[MAXVIEWPATHS][VIEWPATHLENGTH] {
	{NOVIEW,  UP,     UP,     UP,     UP,     UP,     UP,     NOVIEW, NOVIEW, NOVIEW, NOVIEW, NOVIEW, NOVIEW},
	{UP,      UP,     DLEFT,  UP,     UP,     UP,     DLEFT,  DRIGHT, NOVIEW, NOVIEW, NOVIEW, NOVIEW, NOVIEW},
	{UP,      UP,     DLEFT,  UP,     UP,     UP,     DRIGHT, DLEFT,  NOVIEW, NOVIEW, NOVIEW, NOVIEW, NOVIEW},
	{UP,      UP,     DLEFT,  UP,     UP,     DLEFT,  DLEFT,  UP,     NOVIEW, NOVIEW, NOVIEW, NOVIEW, NOVIEW},
	{UP,      UP,     DRIGHT, UP,     UP,     DRIGHT, DRIGHT, UP,     NOVIEW, NOVIEW, NOVIEW, NOVIEW, NOVIEW},
	{UP,      DLEFT,  UP,     UP,     DLEFT,  DLEFT,  DLEFT,  UP,     UP,     UP,     UP,     UP,     UP    },
	{UP,      DRIGHT, UP,     UP,     DRIGHT, DRIGHT, DRIGHT, UP,     UP,     UP,     UP,     UP,     UP    },
	{DLEFT,   DLEFT,  UP,     UP,     DLEFT,  DLEFT,  DLEFT,  UP,     UP,     UP,     UP,     UP,     UP    },
	{DRIGHT,  DRIGHT, UP,     UP,     DRIGHT, DRIGHT, DRIGHT, UP,     UP,     UP,     UP,     UP,     UP    },
	{DLEFT,   DLEFT,  UP,     DLEFT,  DLEFT,  DLEFT,  DLEFT,  UP,     UP,     UP,     UP,     UP,     UP    },
	{DRIGHT,  DRIGHT, UP,     DRIGHT, DRIGHT, DRIGHT, DRIGHT, UP,     UP,     UP,     UP,     UP,     UP    },
	{DLEFT,   DLEFT,  DLEFT,  DLEFT,  DLEFT,  DLEFT,  DLEFT,  UP,     UP,     UP,     UP,     UP,     UP    },
	{DRIGHT,  DRIGHT, DRIGHT, DRIGHT, DRIGHT, DRIGHT, DRIGHT, UP,     UP,     UP,     UP,     UP,     UP    },
	{DLEFT,   LEFT,   DLEFT,  NOVIEW, NOVIEW, NOVIEW, NOVIEW, NOVIEW, NOVIEW, NOVIEW, NOVIEW, NOVIEW, NOVIEW},
	{DRIGHT,  RIGHT,  DRIGHT, NOVIEW, NOVIEW, NOVIEW, NOVIEW, NOVIEW, NOVIEW, NOVIEW, NOVIEW, NOVIEW, NOVIEW},
	{LEFT,    LEFT,   DLEFT,  NOVIEW, NOVIEW, NOVIEW, NOVIEW, NOVIEW, NOVIEW, NOVIEW, NOVIEW, NOVIEW, NOVIEW},
	{RIGHT,   RIGHT,  DRIGHT, NOVIEW, NOVIEW, NOVIEW, NOVIEW, NOVIEW, NOVIEW, NOVIEW, NOVIEW, NOVIEW, NOVIEW}
};


static void BuildSightDir(UINT32 dir, UINT32* One, UINT32* Two, UINT32* Three, UINT32* Four, UINT32* Five)
{
	switch(dir)
	{
		case NORTH:
			*One   = NORTHWEST;
			*Two   = NORTHEAST;
			*Three = NORTH;
			*Four  = WEST;
			*Five  = EAST;
			break;
		case NORTHEAST:
			*One = NORTH;
			*Two = EAST;
			*Three = NORTHEAST;
			*Four   = NORTHWEST;
			*Five  = SOUTHEAST;
			break;
		case EAST:
			*One  = NORTHEAST;
			*Two = SOUTHEAST;
			*Three = EAST;
			*Four = NORTH;
			*Five = SOUTH;
			break;
		case SOUTHEAST:
			*One = EAST;
			*Two = SOUTH;
			*Three = SOUTHEAST;
			*Four = NORTHEAST;
			*Five = SOUTHWEST;
			break;
		case SOUTH:
			*One = SOUTHEAST;
			*Two = SOUTHWEST;
			*Three = SOUTH;
			*Four = EAST;
			*Five = WEST;
			break;
		case SOUTHWEST:
			*One = SOUTH;
			*Two = WEST;
			*Three = SOUTHWEST;
			*Four = SOUTHEAST;
			*Five = NORTHWEST;
			break;
		case WEST:
			*One = SOUTHWEST;
			*Two = NORTHWEST;
			*Three = WEST;
			*Four = SOUTH;
			*Five = NORTH;
			break;
		case NORTHWEST:
			*One = WEST;
			*Two = NORTH;
			*Three = NORTHWEST;
			*Four = SOUTHWEST;
			*Five = NORTHEAST;
			break;
		default:
			break;
	}
}

static std::vector<GridNo> gSlantRoofData;


void ClearSlantRoofs( void )
{
	gSlantRoofData.clear();
}


void AddSlantRoofFOVSlot( INT16 sGridNo )
{
	// Check if this is a duplicate!
	if (std::find(gSlantRoofData.begin(), gSlantRoofData.end(), sGridNo) == gSlantRoofData.end())
	{
		gSlantRoofData.push_back(sGridNo);
	}
}


void ExamineSlantRoofFOVSlots( )
{
	std::for_each(gSlantRoofData.begin(), gSlantRoofData.end(), ExamineGridNoForSlantRoofExtraGraphic);
	gSlantRoofData.clear();
}


void RevealRoofsAndItems(SOLDIERTYPE* const pSoldier, const BOOLEAN fShowLocators)
{
	const UINT8 ubLevel = pSoldier->bLevel;
	STRUCTURE*  pStructure, *pDummy;
	UINT32  maincnt,markercnt,tilesLeftToSee,cnt;
	GridNo  marker, prevmarker;
	INT8    Blocking, markerDir;
	INT8    nextDir = 0;
	UINT8   dir,range;
	UINT8   ubRoomNo;
	UINT8   ubMovementCost;
	BOOLEAN fTravelCostObs;
	BOOLEAN fGoneThroughDoor = FALSE;
	BOOLEAN fThroughWindow = FALSE;
	BOOLEAN fItemsQuoteSaid = FALSE;
	BOOLEAN fRevealItems = TRUE;
	BOOLEAN fStopRevealingItemsAfterThisTile = FALSE;
	INT8    bTallestStructureHeight;
	INT32   iDoorGridNo;
	INT8    bStructHeight;
	INT8    bThroughWindowDirection = -1; // XXX HACK000E

	if ( pSoldier->uiStatusFlags & SOLDIER_ENEMY )
	{
		//pSoldier->needToLookForItems = FALSE;
		return;
	}

	if ( pSoldier->uiStatusFlags & SOLDIER_VEHICLE )
	{
		return;
	}

	// Return if this guy has no gridno, has bad life, etc
	if( pSoldier->sGridNo == NOWHERE || !pSoldier->bInSector || pSoldier->bLife < OKLIFE )
	{
		return;
	}

	if (pSoldier->bBlindedCounter > 0)
	{
		return;
	}
	gubGridNoValue++;

	if ( gubGridNoValue == 255 )
	{
		//Reset!
		std::fill(std::begin(gubGridNoMarkers), std::end(gubGridNoMarkers), 0);
		gubGridNoValue = 1;
	}

	// OK, look for doors
	MercLooksForDoors(*pSoldier);

	dir = pSoldier->bDirection;

	// a gassed merc can only see 1 tile away due to blurred vision
	if ( pSoldier->uiStatusFlags & SOLDIER_GASSED )
	{
		range = GASSED_VIEW_RANGE;
	}
	else
	{
		range = NORMAL_VIEW_RANGE;
		// balance item viewing range between normal and the limit set by opplist-type functions -- CJC
		range = (AdjustMaxSightRangeForEnvEffects(LightTrueLevel(pSoldier->sGridNo, pSoldier->bLevel), range) + range) / 2;
	}

	UINT32 Dir[6];
	BuildSightDir(dir,&Dir[0],&Dir[1],&Dir[2],&Dir[3],&Dir[4]);
	INT32 Inc[6];
	for (cnt = 0; cnt < 5; cnt++)
		Inc[cnt] = DirectionInc(Dir[cnt]);

	// create gridno increment for NOVIEW - in other words, no increment!
	Inc[5] = 0;
	Dir[5] = pSoldier->bDirection;

	// even numbers use ViewPath, odd numbers use ViewPath2
	auto const& viewPath = (dir % 2 == 0) ? ViewPath : ViewPath2;

	// ATE: if in this special cercumstance... our guys are moving on their own...
	// Stop sighting items
	// IN the future, we may want to do something else here...
	const BOOLEAN itemsToo = !(gTacticalStatus.uiFlags & OUR_MERCS_AUTO_MOVE);

	for (maincnt = 0; maincnt < MAXVIEWPATHS; maincnt++)
	{
		marker = pSoldier->sGridNo;
		Blocking = FALSE;
		tilesLeftToSee = 99;
		fRevealItems = TRUE;
		fStopRevealingItemsAfterThisTile = FALSE;

#ifdef _DEBUG
		if (_KeyDown(SDLK_NUMLOCKCLEAR))
		{
			std::fill(std::begin(gubFOVDebugInfoInfo), std::end(gubFOVDebugInfoInfo), 0);
			SetRenderFlags( RENDER_FLAG_FULL );
			RenderWorld( );
		}
#endif

		for (markercnt = 0; markercnt < range; markercnt++)
		{
			prevmarker = marker;

			nextDir = 99;
			fTravelCostObs = FALSE;
			if ( fStopRevealingItemsAfterThisTile )
			{
				fRevealItems = FALSE;
				fStopRevealingItemsAfterThisTile = FALSE;
			}
			markerDir = viewPath[maincnt][markercnt];
			if (markercnt < 12)
				nextDir = viewPath[maincnt][markercnt+1];

			// OK, check flags for going through door/window last tile
			if ( fThroughWindow == 1 )
			{
				// ATE: Make sure we are going through the same direction!
				// THis is to solve the drassen SAM problem with seeing through walls
				if ( static_cast<INT8>(Dir[markerDir]) == bThroughWindowDirection)
				{
					fThroughWindow = 2;
				}
				else
				{
					fThroughWindow = 0;
				}
			}
			else if ( fThroughWindow == 2 )
			{
				// We've overstayed our welcome - remove!
				fThroughWindow = 0;
			}

			if ( fGoneThroughDoor == 1 )
			{
				fGoneThroughDoor = 2;
			}
			else if ( fGoneThroughDoor == 2 )
			{
				// We've overstayed our welcome - remove!
				fGoneThroughDoor = 0;
			}

			//ATE CHECK FOR NOVIEW!
			if ( nextDir == NOVIEW )
			{
				nextDir = 99;
			}

			marker = NewGridNo(marker,(INT16)Inc[markerDir]);

			// End if this is a no view...
			if ( markerDir == NOVIEW && markercnt != 0 )
			{
				break;
			}

#ifdef _DEBUG
			if (_KeyDown(SDLK_NUMLOCKCLEAR))
			{
				int cnt = GetJA2Clock( );

				gubFOVDebugInfoInfo[ marker ] = (UINT8)markercnt;
				RenderFOVDebug( );
				SetFontAttributes(LARGEFONT1, FONT_MCOLOR_WHITE);
				MPrint( 10,  10 , ST::format("{}", maincnt) );
				//MPrint( 10,  20 , ST::format("{}", marker) );
				//MPrint( 50,  20 , ST::format("{}", pSoldier->sGridNo) );
				InvalidateScreen( );
				RefreshScreen();
				do
				{}
				while( ( GetJA2Clock( ) - cnt ) < 250 );
			}
#endif

			// Check if we can get to this gridno from our direction in
			ubMovementCost = gubWorldMovementCosts[ marker ][ Dir[ markerDir ] ][ ubLevel ];

			// ATE: Added: If our current sector is below ground, ignore any blocks!
			if ( gfCaves && ubMovementCost != TRAVELCOST_CAVEWALL )
			{
				ubMovementCost = TRAVELCOST_FLAT;
			}

			if ( IS_TRAVELCOST_DOOR( ubMovementCost ) )
			{
				ubMovementCost = DoorTravelCost(pSoldier, marker, ubMovementCost, pSoldier->bTeam == OUR_TEAM, &iDoorGridNo);
				pStructure = FindStructure( (INT16) iDoorGridNo, STRUCTURE_ANYDOOR );
				if ( pStructure != NULL && pStructure->fFlags & STRUCTURE_TRANSPARENT)
				{
					// cell door or somehow otherwise transparent; allow merc to see through
					ubMovementCost = TRAVELCOST_FLAT;
				}
			}

			// If we have hit an obstacle, STOP HERE
			if ( ubMovementCost >= TRAVELCOST_BLOCKED )
			{
				// We have an obstacle here...
				// If it is bigger than a breadbox... err... taller than a man...
				// Then stop path altogether
				// otherwise just stop revealing items

				// CJC:  only do this when the direction is horizontal; easier and faster to check
				// and the effect should still be good enough

				if ( ubMovementCost == TRAVELCOST_WALL || ubMovementCost == TRAVELCOST_DOOR || ubMovementCost == TRAVELCOST_EXITGRID )
				{
					fTravelCostObs = TRUE;
					fRevealItems = FALSE;
				}
				else
				{
					// walls are handled above, so the blocking object is guaranteed not to be a wall
					bTallestStructureHeight = GetTallestStructureHeight( marker, FALSE );
					if (bTallestStructureHeight >= 3)
					{
						fTravelCostObs = TRUE;
						fStopRevealingItemsAfterThisTile = TRUE;
					}
					else if ( bTallestStructureHeight != 0 )
					{
						// stop revealing items after this tile but keep going
						fStopRevealingItemsAfterThisTile = TRUE;
					}
				}

				if ( (Dir[markerDir] % 2) == 1 )
				{
					// diagonal
					fTravelCostObs = TRUE;
					// cheap hack... don't reveal items
					fRevealItems = FALSE;
				}
				else
				{
					bTallestStructureHeight = GetTallestStructureHeight( marker, FALSE );
					if (bTallestStructureHeight >= 3)
					{
						fTravelCostObs = TRUE;
						fStopRevealingItemsAfterThisTile = TRUE;
					}
					else if ( bTallestStructureHeight != 0 )
					{
						// stop revealing items after this tile but keep going
						fStopRevealingItemsAfterThisTile = TRUE;
					}
				}
			}

			// Check if it's been done already!
			if ( gubGridNoMarkers[ marker ] != gubGridNoValue )
			{
				// Mark gridno
				gubGridNoMarkers[ marker ] = gubGridNoValue;

				// check and see if the gridno changed
				// if the gridno is the same, avoid redundancy and break
				if (marker==prevmarker && markercnt != 0 )
				{

				}
				else   // it changed
				{
					// Skip others if we have gone through a door but are too far away....
					if ( fGoneThroughDoor )
					{
						if (markercnt > 5 )   // Are we near the door?
						{
							break;
						}
					}
					// DO MINE FINDING STUFF
					// GET INDEX FOR ITEM HERE
					// if there IS a direction after this one, nextdir WILL NOT be 99
					if (nextDir != 99)
					{
						Blocking = GetBlockingStructureInfo( marker, (INT8)Dir[ markerDir ], (INT8)Dir[ nextDir ], ubLevel, &bStructHeight, &pDummy, FALSE );
					}
					else
					{
						// no "next" direction, so pass in a NOWHERE so that
						// "SpecialViewObstruction" will know not to take it UINT32o consideration
						Blocking = GetBlockingStructureInfo( marker, (INT8)Dir[markerDir], (INT8)30, ubLevel, &bStructHeight, &pDummy, FALSE  );
					}

					if ( gfCaves )
					{
						Blocking = NOTHING_BLOCKING;
					}

					// CHECK FOR ROOMS
					if ( Blocking == BLOCKING_TOPLEFT_WINDOW || Blocking == BLOCKING_TOPLEFT_OPEN_WINDOW )
					{
						// CHECK FACING DIRECTION!
						if ( Dir[markerDir] == NORTH || Dir[markerDir] == SOUTH )
						{
							if (markercnt <= 1 )   // Are we right beside it?
							{
								fThroughWindow = TRUE;
								bThroughWindowDirection = ( INT8 ) Dir[ markerDir ];
							}
						}
					}
					if ( Blocking == BLOCKING_TOPRIGHT_WINDOW || Blocking == BLOCKING_TOPRIGHT_OPEN_WINDOW )
					{
						// CHECK FACING DIRECTION!
						if ( Dir[markerDir] == EAST || Dir[markerDir] == WEST )
						{
							if (markercnt <= 1 )   // Are we right beside it?
							{
								fThroughWindow = TRUE;
								bThroughWindowDirection = ( INT8 ) Dir[ markerDir ];
							}
						}
					}

					if ( Blocking == BLOCKING_TOPLEFT_DOOR )
					{
						fGoneThroughDoor = TRUE;
					}
					if ( Blocking == BLOCKING_TOPRIGHT_DOOR )
					{
						fGoneThroughDoor = TRUE;
					}

					// ATE: If we hit this tile, find item always!
					//if (Blocking < FULL_BLOCKING )
					{
						// Handle special things for our mercs, like uncovering roofs
						// and revealing objects...
						//gpSoldier->shad |= SEENBIT;

						// NOTE: don't allow object viewing if gassed XXX

						if (itemsToo && fRevealItems ) // && itemIndex < MAXOBJECTLIST)
						{
							// OK, look for corpses...
							LookForAndMayCommentOnSeeingCorpse( pSoldier, marker, ubLevel );

							if (SetItemsVisibilityOn(marker, ubLevel, INVISIBLE, fShowLocators))
							{
								SetRenderFlags(RENDER_FLAG_FULL);

								if ( fShowLocators )
								{
									// Set makred render flags
									//gpWorldLevelData[marker].uiFlags|=MAPELEMENT_REDRAW;
									//gpWorldLevelData[gusCurMousePos].pTopmostHead->uiFlags |= LEVELNODE_DYNAMIC;
									//SetRenderFlags(RENDER_FLAG_MARKED);
									SetRenderFlags(RENDER_FLAG_FULL);

									// Hault soldier
									// ATE: Only if in combat...
									if ( gTacticalStatus.uiFlags & INCOMBAT )
									{
										HaultSoldierFromSighting( pSoldier, FALSE );
									}
									else
									{
										// ATE: Make sure we show locators...
										gTacticalStatus.fLockItemLocators = FALSE;
									}
									if (!fItemsQuoteSaid && !gTacticalStatus.fLockItemLocators)
									{
										gTacticalStatus.fLockItemLocators = TRUE;

										if ( gTacticalStatus.ubAttackBusyCount > 0 && ( gTacticalStatus.uiFlags & INCOMBAT ) )
										{
											gTacticalStatus.fItemsSeenOnAttack           = TRUE;
											gTacticalStatus.items_seen_on_attack_soldier = pSoldier;
											gTacticalStatus.usItemsSeenOnAttackGridNo    = marker;
										}
										else
										{
											// Display quote!
											if ( !AM_AN_EPC( pSoldier ) )
											{
												MakeCharacterDialogueEventSignalItemLocatorStart(*pSoldier, marker);
											}
											else
											{
												// Turn off item lock for locators...
												gTacticalStatus.fLockItemLocators = FALSE;
												SlideToLocation(marker);
											}
										}
										fItemsQuoteSaid = TRUE;
									}
								}
							}
						}
						tilesLeftToSee--;
					}

					if (tilesLeftToSee <= 0)
						break;

					if ( Blocking == FULL_BLOCKING || ( fTravelCostObs && !fThroughWindow ) )
					{
						break;
					}

					// CHECK FOR SLANT ROOF!
					{
						STRUCTURE* pStructure;
						pStructure = FindStructure( marker, STRUCTURE_SLANTED_ROOF );

						if ( pStructure != NULL )
						{
							// ADD TO SLANTED ROOF LIST!
							AddSlantRoofFOVSlot( marker );
						}
					}

					// Set gridno as revealed
					if ( ubLevel == FIRST_LEVEL )
					{
						if ( gfBasement || gfCaves )
						{
							// OK, if we are underground, we don't want to reveal stuff if
							// 1 ) there is a roof over us and
							// 2 ) we are not in a room
							if (gubWorldRoomInfo[marker] != NO_ROOM || TypeRangeExistsInRoofLayer(marker, FIRSTROOF, FOURTHROOF) == NULL)
							{
								gpWorldLevelData[ marker ].uiFlags |= MAPELEMENT_REVEALED;
								if( gfCaves )
								{
									RemoveFogFromGridNo( marker );
								}
							}
						}
						else
						{
							gpWorldLevelData[ marker ].uiFlags |= MAPELEMENT_REVEALED;
						}

						// CHECK FOR ROOMS
						{
							if ( InAHiddenRoom( marker, &ubRoomNo ) )
							{
								RemoveRoomRoof( marker, ubRoomNo, pSoldier );
								if ( ubRoomNo == ROOM_SURROUNDING_BOXING_RING && gWorldSectorX == BOXING_SECTOR_X && gWorldSectorY == BOXING_SECTOR_Y && gbWorldSectorZ == BOXING_SECTOR_Z )
								{
									// reveal boxing ring at same time
									RemoveRoomRoof( marker, BOXING_RING, pSoldier );
								}
							}
						}
					}
					else
					{
						gpWorldLevelData[ marker ].uiFlags |= MAPELEMENT_REVEALED_ROOF;
					}

					// Check for blood....
					UpdateBloodGraphics( marker, ubLevel );

					if (Blocking != NOTHING_BLOCKING &&
						Blocking != BLOCKING_TOPLEFT_DOOR &&
						Blocking != BLOCKING_TOPRIGHT_DOOR &&
						Blocking != BLOCKING_TOPLEFT_WINDOW &&
						Blocking != BLOCKING_TOPRIGHT_WINDOW &&
						Blocking != BLOCKING_TOPRIGHT_OPEN_WINDOW &&
						Blocking != BLOCKING_TOPLEFT_OPEN_WINDOW)
					{
						break;
					}
					//gpWorldLevelData[ marker ].uiFlags |= MAPELEMENT_SHADELAND;

				}
			} // End of duplicate check
			else
			{
				if ( fTravelCostObs )
				{
					break;
				}
			}
		} // end of one path
	} // end of path loop
	// Loop through all availible slant roofs we collected and perform cool stuff on them
	ExamineSlantRoofFOVSlots( );
	//pSoldier->needToLookForItems = FALSE;
	//LookForDoors(pSoldier,UNAWARE);
}
