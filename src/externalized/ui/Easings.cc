#include "sgp/Debug.h"
#include "sgp/Timer.h"

float EaseInCubic(time_point const tpStartTime, time_point const tpEndTime, time_point const tpCurrentTime)
{
	Assert(tpEndTime >= tpStartTime);
	Assert(tpCurrentTime <= tpEndTime);

	float fProgress = (float)(double(CastDuration(tpCurrentTime - tpStartTime)) / double(CastDuration(tpEndTime - tpStartTime)));

	if (fProgress < 0.5) {
		return 4.0f * fProgress * fProgress * fProgress;
	} else {
		return (fProgress - 1.0f) * (2.0f * fProgress - 2.0f) * (2.0f * fProgress - 2.0f) + 1.0f;
	}
}
