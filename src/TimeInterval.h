#pragma once

#include <algorithm>

struct TimeInterval {
	float start, end;

	TimeInterval getOverlap(TimeInterval interval2) {
		using namespace std;
		TimeInterval overlap{-1.0f, -1.0f}; // Default return values indicate no overlap
		if (max(start, interval2.start) - min(end, interval2.end) <= 0.0f) {
			overlap.start = max(start, interval2.start);
			overlap.end = min(end, interval2.end);
		}
		return overlap;
	}
};
