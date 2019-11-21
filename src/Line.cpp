#include "Line.h"

using namespace std;

Line::Line(int x_coord, int y_coord, int slope) {
	px = (float)x_coord;
	py = (float)y_coord;
	m = (float)slope;
	b = py - m*px;
}

float Line::findIntersection(Line line2, int offset) {
	return (line2.b + offset - b) / (1 - (line2.m / m));
}

DoubleLine::DoubleLine(int x_coord, int y_coord, int x_vector, int y_vector)
	: Line(x_coord, y_coord, x_vector)
{
	vx = (float)x_vector;
	vy = (float)y_vector;

	m = isnan(vy / vx) ? 0.0f : (vy / vx);
	b = py - m*px;
}

bool DoubleLine::parallel(DoubleLine lineToCompare) {
	if ((m - lineToCompare.m) != 0) {
		return false;
	}
	return true;
}

Intersection DoubleLine::findPerpendicularViolation(DoubleLine line2, float max_time, int sep_x, int sep_y) {

	float ix, iy, it; // Intersection x coordinate, y coordinate and time

	ix = px; // line1 is a vertical line
	iy = line2.py; // line2 is a horizontal line

	float dist = ix - line2.px; // x distance between intersect and start of horizontal line
	float t1 = (dist - sep_x) / line2.vx; // times at which horizontal trajectory is at minimum separation threshold
	float t2 = (dist + sep_x) / line2.vx;
	TimeInterval interval1{ min(t1,t2), min(max(t1,t2),max_time)};

	dist = iy - py; // y distance between intersect and start of vertical line
	t1 = (dist - sep_y) / vy; // times at which vertical trajectory is at minimum separation threshold
	t2 = (dist + sep_y) / vy;
	TimeInterval interval2{ min(t1,t2), min(max(t1,t2),max_time)};

	if (interval1.start <= max_time && interval2.start <= max_time) {
		it = interval1.firstFutureIntersect(interval2);
		ix = line2.px + it * line2.vx;
		iy = py + it * vy;
	}
	else {
		it = -1.0f; // Flag value for "No violation before max_time"
		ix = -1.0f;
		iy = -1.0f;
	}

	return Intersection{ix, iy, it};
}

Intersection DoubleLine::getProximityInterval(DoubleLine line2, float max_time, int sep_x, int sep_y) {
	float ix, iy, it; // Intersection x coordinate, y coordinate and time

	Line xLine1(px, py, vx);
	Line xLine2(line2.px, line2.py, line2.vx);
	float t1 = xLine1.findIntersection(xLine2, sep_x);
	float t2 = xLine1.findIntersection(xLine2, -sep_x);
	TimeInterval interval1{ min(t1,t2), min(max(t1,t2),max_time)};

	Line yLine1(px, py, vy);
	Line yLine2(line2.px, line2.py, line2.vy);
	t1 = yLine1.findIntersection(yLine2, sep_y);
	t2 = yLine1.findIntersection(yLine2, -sep_y);
	TimeInterval interval2{ min(t1,t2), min(max(t1,t2),max_time)};

	if (interval1.start <= max_time && interval2.start <= max_time) {
		it = interval1.firstFutureIntersect(interval2);
		ix = line2.px + it * line2.vx;
		iy = py + it * vy;
	}
	else {
		it = -1.0f; // Flag value for "No intersection before max_time"
		ix = -1.0f;
		iy = -1.0f;
	}

	return Intersection{ix, iy, it};
}

