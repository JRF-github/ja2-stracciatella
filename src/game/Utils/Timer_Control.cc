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
#include <stdexcept>


UINT32 guiBaseJA2Clock = 0;
static bool gfPauseClock = FALSE;

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
	10ms,  // Music Overhead
	100ms  // definition of PLAYER_TEAM_TIMER_SEC_PER_TICKS in Interface.cc, keep in sync
};

// TIMER COUNTERS
TIMERS giTimerCounters;

milliseconds giTimerCustomizable{0ms};
milliseconds giTimerTeamTurnUpdate{0ms};

CUSTOMIZABLE_TIMER_CALLBACK gpCustomizableTimerCallback = 0;

// Clock Callback event ID
static SDL_TimerID g_timer;


extern INT32  giCompatibleItemBaseTime;
extern INT32  giAnimateRouteBaseTime;
extern INT32  giPotHeliPathBaseTime;
extern INT32  giPotCharPathBaseTime;


static void UpdateTimer(milliseconds & counter)
{
	counter = std::max<milliseconds>(0ms, counter - BASETIMESLICE);
}


static Uint32 TimeProc(Uint32 const interval, void*)
{
	if (!gfPauseClock)
	{
		guiBaseJA2Clock += BASETIMESLICE.count();

		for (milliseconds & counter : giTimerCounters) UpdateTimer(counter);

		// Update some specialized countdown timers...
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

	int32_t msPerTimeSlice = gamepolicy(ms_per_time_slice);
	if (msPerTimeSlice <= 0)
	{
		throw std::runtime_error("ms_per_time_slice must be a positive integer");
	}
	g_timer = SDL_AddTimer(msPerTimeSlice, TimeProc, nullptr);
	if (!g_timer) throw std::runtime_error("Could not create timer callback");
}


void ShutdownJA2Clock(void)
{
	SDL_RemoveTimer(g_timer);
}


void PauseTime(bool const fPaused)
{
	gfPauseClock = fPaused;
}


void SetCustomizableTimerCallbackAndDelay(milliseconds const delay, CUSTOMIZABLE_TIMER_CALLBACK const callback, bool const replace)
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
	if (!gpCustomizableTimerCallback) return;
	if (giTimerCustomizable != 0ms) return;

	/* Set the callback to a temp variable so we can reset the global variable
	 * before calling the callback, so that if the callback sets up another
	 * instance of the timer, we don't reset it afterwards. */
	CUSTOMIZABLE_TIMER_CALLBACK const callback = gpCustomizableTimerCallback;
	gpCustomizableTimerCallback = nullptr;
	callback();
}


void ResetJA2ClockGlobalTimers(decltype(GetJA2Clock()) const now)
{
	guiBaseJA2Clock = now;

	giCompatibleItemBaseTime       = now;
	giAnimateRouteBaseTime         = now;
	giPotHeliPathBaseTime          = now;
	giPotCharPathBaseTime          = now;
}

#ifdef WITH_UNITTESTS
#include "gtest/gtest.h"
#include "Timer.h"

static time_point callbackCalled;
static void TimerTestCallback(void)
{
	callbackCalled = Now();
}

TEST(TimerControl, BasicTimerFunctionality)
{
	if (!g_timer) InitializeJA2Clock();

	{
		time_point const before = Now();
		callbackCalled = time_point::min();
		milliseconds const expectedDuration = 30ms;

		SetCustomizableTimerCallbackAndDelay(expectedDuration, TimerTestCallback, true);
		do {
			SDL_Delay(1);
			CheckCustomizableTimer();
		} while (callbackCalled == time_point::min());

		milliseconds const actualDuration = std::chrono::duration_cast<milliseconds>(callbackCalled - before);
		EXPECT_TRUE(std::chrono::abs(actualDuration - expectedDuration) <= (BASETIMESLICE + 2ms));
	}
	{
		#define USETIMER NEXTSCROLL
		milliseconds const expectedDuration = giTimerIntervals[static_cast<int>(TIMERNAMES::USETIMER)];

		RESETCOUNTER(USETIMER);
		EXPECT_FALSE(COUNTERDONE(USETIMER));
		SDL_Delay(expectedDuration.count() + 1);
		EXPECT_TRUE(COUNTERDONE(USETIMER));
	}
}

#endif
