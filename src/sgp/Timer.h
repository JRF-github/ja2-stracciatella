#pragma once

#include "Types.h"
#include <chrono>

using time_point = std::chrono::time_point<std::chrono::steady_clock>;
using std::chrono::milliseconds;
using namespace std::literals::chrono_literals;


static inline time_point Now(void)
{
	return std::chrono::steady_clock::now();
}

// Convert a duration type to UINT32
// Intended to compute the number of milliseconds between two time_points
constexpr UINT32 CastDuration(std::chrono::nanoseconds const duration)
{
	return static_cast<UINT32>(std::chrono::duration_cast<milliseconds>(duration).count());
}
