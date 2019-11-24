#pragma once

#include <algorithm>

struct TimeInterval {
	float start, end;

	// Given another time interval, determine whether there is overlap
	// If there is overlap return the first time at which the overlap occurs
	float firstFutureOverlap(TimeInterval interval2) {
		float t = -1.0f; // Flag value for "No future intersect"
		if (start < 0.0f && end < 00.f && interval2.start < 0.0f && interval2.end < 0.0f ) {
			// The intersection is in the past (or is presently occurring)
			// We do not care about the past
			// If it is presently occurring it would have been caught before this evaluation
		}
		else { // Intersection occurs in the future (or is presently occurring)
			if (start < 0.0f) {
				start = 0.0f;
			}
			if (interval2.start < 0.0f) {
				interval2.start = 0.0f;
			}

			// Check if the time intervals overlap
			if (std::max(start,interval2.start) - std::min(end,interval2.end) <= 0.0f) {
				t = std::max(start,interval2.start);
			}
		}
		return t;
	}

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
