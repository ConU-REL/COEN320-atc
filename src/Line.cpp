#include "Line.h"

using namespace std;

Line::Line(int x_coord, int y_coord, int slope) {
	px = (float)x_coord;
	py = (float)y_coord;
	m = (float)slope;
	b = py - m*px;
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

float Line::findIntersection(Line line2, int offset) {
	return (line2.b + offset - b) / (m - line2.m);
}

Intersection DoubleLine::findViolationPoint(DoubleLine line2, float max_time, int sep_x, int sep_y) {
	float ix, iy, it; // Intersection x coordinate, y coordinate and time

	Line xLine1(0.0f,px,vx);
	Line xLine2(0.0f,line2.px,line2.vx);
	float t_one = xLine1.findIntersection(xLine2,sep_x);
	float t_two = xLine1.findIntersection(xLine2,-sep_x);

	TimeInterval int1{ min(t_one,t_two), max(t_one,t_two) };

	Line yLine1(0.0f,py,vy);
	Line yLine2(0.0f,line2.py,line2.vy);
	t_one = yLine1.findIntersection(yLine2,sep_y);
	t_two = yLine1.findIntersection(yLine2,-sep_y);

	TimeInterval int2{ min(t_one,t_two), max(t_one,t_two) };

	TimeInterval int3 = int1.getOverlap(int2);
	TimeInterval int4 = int3.getOverlap({0.1f,max_time});

	if (int3.start <= max_time) {
		it = int4.start;
		ix = px + it * vx;
		iy = py + it * vy;
	}
	else {
		it = -1.0f;
		ix = -1.0f;
		iy = -1.0f;
	}
	return Intersection{ix, iy, it};
}


