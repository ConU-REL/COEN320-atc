#pragma once

#include <cmath>
#include <algorithm>

#include "TimeInterval.h"
#include "Intersection.h"

struct Line { // Used for x(t) and y(t) individually
	// y = mx + b
	float px, py; // a point on the line (typically the start point)
	float m; // slope
	float b; // y intercept

	Line(int, int, int);
	float findIntersection(Line, int);
};

struct DoubleLine : public Line { // Used for lines which are x and y (collectively) as a function of time
	// This is a line
	float vx, vy;

	DoubleLine(int, int, int, int);
	bool parallel(DoubleLine);
	Intersection findPerpendicularViolation(DoubleLine, float, int, int);
	Intersection find1DimensionalViolation(DoubleLine, float, int, int);
	Intersection findViolationPoint(DoubleLine, float, int, int);
};
