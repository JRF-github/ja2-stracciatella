#ifndef SECTOR_COORDS_H
#define SECTOR_COORDS_H

#include <cstdint>
#include "string_theory/format"

// Defined in StrategicMap.cc
ST::string GetShortSectorString(int16_t, int16_t);


// TBD: store the world coords as a sector_coords
extern int16_t gWorldSectorX;
extern int16_t gWorldSectorY;
extern int8_t  gbWorldSectorZ;


struct sector_coords
{
	int16_t x;
	int16_t y;
	int8_t  z;

	constexpr sector_coords() : x{0}, y{0}, z{0} {}

	constexpr sector_coords(int16_t const xx, int16_t const yy, int8_t const zz = 0)
		: x{xx}, y{yy}, z{zz} {}

	// Construct an object form a "SectorInfo index" (0-255),
	// see Campaign_Types.h
	constexpr sector_coords(uint8_t const sectorId)
		: x{static_cast<int16_t>(sectorId % 16 + 1)}, y{static_cast<int16_t>(sectorId / 16 + 1)}, z{0} {}


	constexpr void clear()
	{
		x = 0;
		y = 0;
		z = 0;
	}

	constexpr sector_coords& operator=(uint8_t const sectorId)
	{
		x = sectorId % 16 + 1;
		y = sectorId / 16 + 1;
		z = 0;
		return *this;
	}

	// Return the "SectorInfo index" (0-255) for these coordinates
	constexpr operator uint8_t() const { return (y - 1) * 16 + x - 1; }

	constexpr bool operator==(sector_coords const& other) const
	{
		return x == other.x && y == other.y && z == other.z;
	}

	constexpr bool operator!=(sector_coords const& other) const
	{
		return !operator==(other);
	}

	// Return the index into the StrategicMap array [0-323]
	constexpr int get_strategic_index() const { return x + y * 16; }

	ST::string get_short_string() const
	{
		return GetShortSectorString(x, y);
	}

	bool equals_world_coords() const
	{
		return x == gWorldSectorX && y == gWorldSectorY && z == gbWorldSectorZ;
	}

	// TBD: this can go when the world coords are stored as a sector_coords
	static sector_coords from_world_coords()
	{
		return sector_coords{gWorldSectorY, gWorldSectorY, gbWorldSectorZ};
	}
};

#endif
