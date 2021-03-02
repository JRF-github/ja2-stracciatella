#include "gtest/gtest.h"

#include "Easings.h"

TEST(EasingsTest, easeInCubic)
{
	constexpr time_point start = time_point(15h) + 18min + 56s + 111ms + 23us + 7ns;
	constexpr time_point end   = start + 100ms;

	EXPECT_EQ(EaseInCubic(time_point(100ms), time_point(101ms), time_point(100ms)), 0.0);
	EXPECT_EQ(EaseInCubic(start, end, end), 1.0);

	EXPECT_EQ(EaseInCubic(start, end, start + 25ms), 0.0625);
	EXPECT_EQ(EaseInCubic(start, end, start + 75ms), 0.9375);
}
