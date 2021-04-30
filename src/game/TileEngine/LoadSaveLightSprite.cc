#include "Debug.h"
#include "Environment.h"
#include "FileMan.h"
#include "JAScreens.h"
#include "LoadSaveData.h"
#include "LoadSaveLightSprite.h"
#include "ScreenIDs.h"
#include "Sys_Globals.h"
#include "MemMan.h"

void ExtractLightSprite(HWFILE const f, UINT32 const light_time)
{
	INT16  x;
	INT16  y;
	UINT32 flags;
	UINT8  str_len;

	FileDataReader{25, f}
	  >> x
	  >> y
	  >> skip<12>
	  >> flags
	  >> skip<4>
	  >> str_len;

	char *template_name = new char[str_len]{};
	FileRead(f, template_name, str_len);
	template_name[str_len - 1] = '\0';

	LIGHT_SPRITE* const l = LightSpriteCreate(template_name);
	// if this fails, then we will ignore the light.
	if (l)
	{
		// power only valid lights
		if (gfEditMode ||
				(!gfCaves && (
					flags & light_time ||
					!(flags & (LIGHT_PRIMETIME | LIGHT_NIGHTTIME))
				)))
		{
			LightSpritePower(l, TRUE);
		}
		LightSpritePosition(l, x, y);
		if (flags & LIGHT_PRIMETIME)
		{
			l->uiFlags |= LIGHT_PRIMETIME;
		}
		else if (flags & LIGHT_NIGHTTIME)
		{
			l->uiFlags |= LIGHT_NIGHTTIME;
		}
	}
	delete[] template_name;
}


void InjectLightSpriteIntoFile(HWFILE const file, LIGHT_SPRITE const* const l)
{
	{ FileDataWriter{24, file}
	  << l->iX
	  << l->iY
	  << skip<12>
	  << l->uiFlags
	  << skip<4>;
	}

	const char* const light_name = LightSpriteGetTypeName(l);
	const UINT8       str_len    = static_cast<UINT8>(strlen(light_name) + 1);
	FileWrite(file, &str_len,   sizeof(str_len));
	FileWrite(file, light_name, str_len);
}
