#include "Timer_Control.h"

#include "ContentManager.h"
#include "Debug.h"
#include "GameInstance.h"
#include "GamePolicy.h"
#include "Handle_Items.h"
#include "MapScreen.h"
#include "Overhead.h"
#include "Soldier_Control.h"
#include "WorldDef.h"

#include <SDL.h>
#include <array>
#include <stdexcept>


INT32	giTimerDiag  =  0;

UINT32 guiBaseJA2Clock = 0;

static BOOLEAN gfPauseClock = FALSE;

const TIMERS giTimerIntervals
{
	5ms, // Tactical Overhead
	20ms, // NEXTSCROLL
	200ms, // Start Scroll
	200ms, // Animate tiles
	1000ms, // FPS Counter
	80ms, // PATH FIND COUNTER
	150ms, // CURSOR TIMER
	300ms, // RIGHT CLICK FOR MENU
	300ms, // LEFT
	300ms, // MIDDLE
	200ms, // TARGET REFINE TIMER
	150ms, // CURSOR/AP FLASH
	20ms, // PHYSICS UPDATE
	100ms, // FADE ENEMYS
	20ms, // STRATEGIC OVERHEAD
	40ms, // CYCLE TIME RENDER ITEM COLOR
	500ms, // NON GUN TARGET REFINE TIMER
	250ms, // IMPROVED CURSOR FLASH
	500ms, // 2nd CURSOR FLASH
	400ms, // RADARMAP BLINK AND OVERHEAD MAP BLINK SHOUDL BE THE SAME
	10ms  // Music Overhead
};

// TIMER COUNTERS
TIMERS giTimerCounters;

milliseconds giTimerCustomizable{0ms};
milliseconds giTimerTeamTurnUpdate{0ms};

CUSTOMIZABLE_TIMER_CALLBACK gpCustomizableTimerCallback = 0;

// Clock Callback event ID
static SDL_TimerID g_timer;


extern UINT32 guiCompressionStringBaseTime;
extern INT32  giFlashHighlightedItemBaseTime;
extern INT32  giCompatibleItemBaseTime;
extern INT32  giAnimateRouteBaseTime;
extern INT32  giPotHeliPathBaseTime;
extern UINT32 guiSectorLocatorBaseTime;
extern INT32  giCommonGlowBaseTime;
extern INT32  giFlashAssignBaseTime;
extern INT32  giFlashContractBaseTime;
extern UINT32 guiFlashCursorBaseTime;
extern INT32  giPotCharPathBaseTime;


static void UpdateTimer(milliseconds & counter)
{
	counter = std::max<milliseconds>(0ms, counter - BASETIMESLICE);
}


static UINT32 TimeProc(UINT32 const interval, void*)
{
	if (!gfPauseClock)
	{
		guiBaseJA2Clock += BASETIMESLICE.count();

		for (milliseconds & counter : giTimerCounters) UpdateTimer(counter);

		// Update some specialized countdown timers...
		UpdateTimer(giTimerTeamTurnUpdate);

		if (gpCustomizableTimerCallback)
		{
			UpdateTimer(giTimerCustomizable);
		}

#ifndef BOUNDS_CHECKER
		if (fInMapMode)
		{
			// IN Mapscreen, loop through player's team
			FOR_EACH_IN_TEAM(s, OUR_TEAM)
			{
				UpdateTimer(s->PortraitFlashCounter);
				UpdateTimer(s->PanelAnimateCounter);
			}
		}
		else
		{
			// Set update flags for soldiers
			FOR_EACH_MERC(s)
			{
				UpdateTimer(s->UpdateCounter);
				UpdateTimer(s->DamageCounter);
				UpdateTimer(s->BlinkSelCounter);
				UpdateTimer(s->PortraitFlashCounter);
				UpdateTimer(s->AICounter);
				UpdateTimer(s->FadeCounter);
				UpdateTimer(s->NextTileCounter);
				UpdateTimer(s->PanelAnimateCounter);
			}
		}
#endif
	}

	return interval;
}


void InitializeJA2Clock(void)
{
	SDL_InitSubSystem(SDL_INIT_TIMER);

	// Init timer delays
	giTimerCounters = giTimerIntervals;

	g_timer = SDL_AddTimer(BASETIMESLICE.count(), TimeProc, nullptr);
	if (!g_timer) throw std::runtime_error("Could not create timer callback");
}


void ShutdownJA2Clock(void)
{
	SDL_RemoveTimer(g_timer);
}


void PauseTime(BOOLEAN const fPaused)
{
	gfPauseClock = fPaused;
}


void SetCustomizableTimerCallbackAndDelay(milliseconds const delay, CUSTOMIZABLE_TIMER_CALLBACK const callback, BOOLEAN const replace)
{
	if (!replace && gpCustomizableTimerCallback)
	{ // Replace callback but call the current callback first
		gpCustomizableTimerCallback();
	}

	RESETTIMECOUNTER(giTimerCustomizable, delay);
	gpCustomizableTimerCallback = callback;
}


void CheckCustomizableTimer(void)
{
	if (!gpCustomizableTimerCallback)             return;
	if (!TIMECOUNTERDONE(giTimerCustomizable, 0)) return;

	/* Set the callback to a temp variable so we can reset the global variable
	 * before calling the callback, so that if the callback sets up another
	 * instance of the timer, we don't reset it afterwards. */
	CUSTOMIZABLE_TIMER_CALLBACK const callback = gpCustomizableTimerCallback;
	gpCustomizableTimerCallback = 0;
	callback();
}


void ResetJA2ClockGlobalTimers(void)
{
	UINT32 const now = GetJA2Clock();

	guiCompressionStringBaseTime   = now;
	giFlashHighlightedItemBaseTime = now;
	giCompatibleItemBaseTime       = now;
	giAnimateRouteBaseTime         = now;
	giPotHeliPathBaseTime          = now;
	guiSectorLocatorBaseTime       = now;

	giCommonGlowBaseTime           = now;
	giFlashAssignBaseTime          = now;
	giFlashContractBaseTime        = now;
	guiFlashCursorBaseTime         = now;
	giPotCharPathBaseTime          = now;
}
