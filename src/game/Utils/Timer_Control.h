#pragma once

#include "Types.h"
#include <array>
#include <chrono>
#include <functional>

using std::chrono::milliseconds;
using namespace std::literals::chrono_literals;
using CUSTOMIZABLE_TIMER_CALLBACK = std::function<void (void)>;


// TIMER DEFINES
enum class TIMERNAMES
{
	TOVERHEAD = 0,			// Overhead time slice
	NEXTSCROLL,			// Scroll Speed timer
	STARTSCROLL,			// Scroll Start timer
	ANIMATETILES,			// Animate tiles timer
	FPSCOUNTER,			// FPS value
	PATHFINDCOUNTER,		// PATH FIND COUNTER
	CURSORCOUNTER,			// ANIMATED CURSOR
	RMOUSECLICK_DELAY_COUNTER,	// RIGHT BUTTON CLICK DELAY
	LMOUSECLICK_DELAY_COUNTER,	// LEFT  BUTTON CLICK DELAY
	MMOUSECLICK_DELAY_COUNTER,	// LEFT  BUTTON CLICK DELAY
	TARGETREFINE,			// TARGET REFINE
	CURSORFLASH,			// Cursor/AP flash
	PHYSICSUPDATE,			// PHYSICS UPDATE.
	GLOW_ENEMYS,
	STRATEGIC_OVERHEAD,		// STRATEGIC OVERHEAD
	CYCLERENDERITEMCOLOR,		// CYCLE COLORS
	NONGUNTARGETREFINE,		// TARGET REFINE
	CURSORFLASHUPDATE,		//
	INVALID_AP_HOLD,		// TIME TO HOLD INVALID AP
	RADAR_MAP_BLINK,		// BLINK DELAY FOR RADAR MAP
	MUSICOVERHEAD,			// MUSIC TIMER
	TEAMTURNUPDATE,			// Team turn timer used by Interface.cc
	NUMTIMERS
};
using TIMERS = std::array<milliseconds, static_cast<int>(TIMERNAMES::NUMTIMERS)>;

// Base resultion of callback timer
constexpr milliseconds BASETIMESLICE = 5ms;

extern TIMERS const giTimerIntervals;
extern TIMERS giTimerCounters;


void InitializeJA2Clock(void);
void ShutdownJA2Clock(void);

// This value must only be modified by the Timer_Control code
extern UINT32 guiBaseJA2Clock;
static inline UINT32 GetJA2Clock() { return guiBaseJA2Clock; }

void PauseTime( bool fPaused );

void SetCustomizableTimerCallbackAndDelay( milliseconds iDelay, CUSTOMIZABLE_TIMER_CALLBACK pCallback, bool fReplace );
void CheckCustomizableTimer( void );

extern CUSTOMIZABLE_TIMER_CALLBACK gpCustomizableTimerCallback;


#define RESETCOUNTER( c ) ( giTimerCounters[ static_cast<int>(TIMERNAMES::c) ] = giTimerIntervals[ static_cast<int>(TIMERNAMES::c) ] )
#define COUNTERDONE( c )  ( giTimerCounters[ static_cast<int>(TIMERNAMES::c) ] == 0ms )

#define RESETTIMECOUNTER( c, d )	( c = d )

#ifdef BOUNDS_CHECKER
	#define TIMECOUNTERDONE(c, d) true
#else
	#define TIMECOUNTERDONE(c, d) (c == 0ms)
#endif

#define SYNCTIMECOUNTER()		(void)0
#define ZEROTIMECOUNTER( c )		( c = 0ms )

// Exclusive for LoadSavedGame: reset the JA2 base clock and
// all timers that use it as a reference.
void ResetJA2ClockGlobalTimers(decltype(GetJA2Clock()) uiJA2BaseClock);
