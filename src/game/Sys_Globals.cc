#include <stdarg.h>
#include "Types.h"
#include "ScreenIDs.h"
#include "Sys_Globals.h"
#include "GameLoop.h"


char g_filename[200];

ST::string gubErrorText;
BOOLEAN gfEditMode             = FALSE;
BOOLEAN fFirstTimeInGameScreen = TRUE;
BOOLEAN gfGlobalError          = FALSE;

UINT32	guiGameCycleCounter = 0;


void SET_ERROR(const ST::string& msg)
{
	gubErrorText = msg;

	SetPendingNewScreen( ERROR_SCREEN );

	gfGlobalError = TRUE;
}
