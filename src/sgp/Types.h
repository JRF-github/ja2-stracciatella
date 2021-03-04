#ifndef __TYPES_
#define __TYPES_

#include <stdlib.h>
#include <stdint.h>
#include <SDL_rect.h>
#include <SDL_video.h>

#include "Platform.h"
#include "SGPStrings.h"
#include "Logger.h"

#define UNIMPLEMENTED \
	SLOGA("===> %s:%d: %s() is not implemented", __FILE__, __LINE__, __func__);

#ifdef WITH_FIXMES
	#define FIXME \
		SLOGE("===> %s:%d: %s() FIXME", __FILE__, __LINE__, __func__);
#else
	#define FIXME (void)0;
#endif


#define lengthof(a) (sizeof(a) / sizeof(a[0]))
#define endof(a) ((a) + lengthof(a))


#define ABS(a) (UINT16)abs(int(a))
#define MAX(a, b) __max(a, b)
#define MIN(a, b) __min(a, b)

#define FOR_EACHX(type, iter, array, x) for (type* iter = (array); iter != endof((array)); (x), ++iter)
#define FOR_EACH(type, iter, array)     FOR_EACHX(type, iter, (array), (void)0)


typedef int32_t     INT;
typedef int32_t     INT32;
typedef uint32_t    UINT;
typedef uint32_t    UINT32;

// integers
typedef uint8_t         UINT8;
typedef int8_t          INT8;
typedef uint16_t        UINT16;
typedef int16_t         INT16;
// floats
typedef float           FLOAT;
typedef double          DOUBLE;
// strings
typedef char            CHAR8;

// other
typedef unsigned char		BOOLEAN;
typedef void *					PTR;
typedef UINT8						BYTE;
typedef CHAR8						STRING512[512];

#define SGPFILENAME_LEN 100
typedef CHAR8 SGPFILENAME[SGPFILENAME_LEN];


#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

#define PI 3.1415926

#ifndef NULL
#define NULL 0
#endif

// Unlike SGPRect this data structure uses 32-bit signed integers
// and width and height instead of right and bottom.
using SGPBox = SDL_Rect;

struct SGPRect
{
	UINT16 iLeft;
	UINT16 iTop;
	UINT16 iRight;
	UINT16 iBottom;

	void set(UINT16 left, UINT16 top, UINT16 right, UINT16 bottom)
	{
		iLeft       = left;
		iTop        = top;
		iRight      = right;
		iBottom     = bottom;
	}
};

struct SGPPoint
{
	UINT16 iX;
	UINT16 iY;

	void set(UINT16 x, UINT16 y)
	{
		iX = x;
		iY = y;
	}
};


struct SDL_Color;
typedef SDL_Color SGPPaletteEntry;


typedef UINT32 COLORVAL;

struct AuxObjectData;
struct ETRLEObject;
struct RelTileLoc;
struct SGPImage;

class SGPVObject;
typedef SGPVObject* HVOBJECT;
typedef SGPVObject* SGPFont;

class SGPVSurface;

struct BUTTON_PICS;

struct SGPFile;
typedef SGPFile* HWFILE;


#define SGP_TRANSPARENT ((UINT16)0)


#ifdef __cplusplus
#	define ENUM_BITSET(type)                                                                   \
		static inline type operator ~  (type  a)         { return     (type)~(int)a;           } \
		static inline type operator &  (type  a, type b) { return     (type)((int)a & (int)b); } \
		static inline type operator &= (type& a, type b) { return a = (type)((int)a & (int)b); } \
		static inline type operator |  (type  a, type b) { return     (type)((int)a | (int)b); } \
		static inline type operator |= (type& a, type b) { return a = (type)((int)a | (int)b); }
#else
#	define ENUM_BITSET(type)
#endif

namespace _Types
{
	// Object wrapper of a single primitive value.
	// This is used to implement "pass by reference" of Lua function arguments.
	// Usage:
	// - In C++ it can be used as the underlying type
	// - In Lua, the value is to be read or written with the `.val` member.
	template<typename T>
	struct BoxedValue
	{
		BoxedValue<T>(T v) : val(v) {};

		operator T() const { return val; }

		T val;
	};
}

typedef _Types::BoxedValue<BOOLEAN> BOOLEAN_S;
typedef _Types::BoxedValue<UINT8>   UINT8_S;

#endif
