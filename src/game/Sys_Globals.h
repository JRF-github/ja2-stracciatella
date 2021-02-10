#ifndef __SYS_GLOBALS_H
#define __SYS_GLOBALS_H

#include <string_theory/string>


extern ST::string gubErrorText;
extern BOOLEAN gfEditMode;
extern BOOLEAN fFirstTimeInGameScreen;
extern BOOLEAN gfGlobalError;

extern UINT32  guiGameCycleCounter;

void SET_ERROR(const ST::string& msg);

extern char g_filename[200];

#endif
