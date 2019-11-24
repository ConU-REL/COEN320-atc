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

Intersection DoubleLine::findPerpendicularViolation(DoubleLine line2, float max_time, int sep_x, int sep_y) {

	float ix, iy, it; // Intersection x coordinate, y coordinate and time

	ix = px; // line1 is a vertical line
	iy = line2.py; // line2 is a horizontal line

	float dist = ix - line2.px; // x distance between intersect and start of horizontal line
	float t1 = (dist - sep_x) / line2.vx; // times at which horizontal trajectory is at minimum separation threshold
	float t2 = (dist + sep_x) / line2.vx;
	TimeInterval interval1{ min(t1,t2), min(max(t1,t2),max_time) };

	dist = iy - py; // y distance between intersect and start of vertical line
	t1 = (dist - sep_y) / vy; // times at which vertical trajectory is at minimum separation threshold
	t2 = (dist + sep_y) / vy;
	TimeInterval interval2{ min(t1,t2), min(max(t1,t2),max_time) };

	if (interval1.start <= max_time && interval2.start <= max_time) {
		it = interval1.firstFutureOverlap(interval2);
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

float Line::findIntersection(Line line2, int offset) {
	return (line2.b + offset - b) / (m - line2.m);
}

#include <iostream>

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
	/*cout << int1.start << " - " << int1.end << endl;
	cout << int2.start << " - " << int2.end << endl;
	cout << int3.start << " - " << int3.end << endl;
	cout << int4.start << " - " << int4.end << endl;*/

	if (int4.start > 0.0f) {
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

	/*// first line points are given by (x1,y1),(x2,y2)
	// second line points are given by (x3,y3),(x4,y4)

	// intersection is then at x = (x2y1 - x1y2)(x4-x3) - (x4y3 - x3y4)(x2-x1)
	//                             (x2-x1)(y4-y3) - (x4-x3)(y2-y1)
	//
	//                         y = (x2y1-x1y2)(y4-y3)-(x4y3-x3y4)(y2-y1)
	//                             (x2-x1)(y4-y3)-(x4-x3)(y2-y1)

	float x1,x2,x3,x4,y1,y2,y3,y4;
	x1 = px;
	y1 = py;
	x2 = px+vx*max_time;
	y2 = py+vy*max_time;

	x3 = line2.px;
	y3 = line2.py;
	x4 = line2.px+line2.vx*max_time;
	y4 = line2.py+line2.vy*max_time;

	ix = ((x2*y1 - x1*y2)*(x4-x3) - (x4*y3 - x3*y4)*(x2-x1)) / ((x2-x1)*(y4-y3)-(x4-x3)*(y2-y1));
	iy = ((x2*y1-x1*y2)*(y4-y3)-(x4*y3-x3*y4)*(y2-y1)) / ((x2-x1)*(y4-y3)-(x4-x3)*(y2-y1));

	cout << ix << " , " << iy << endl;

	float t1 = ((ix-sep_x) - px) / vx;
	float t2 = ((ix+sep_x) - px) / vx;

	TimeInterval interval1{ min(t1,t2), min(max(t1,t2),max_time) };

	std::cout << "Int1: [" << interval1.start << "," << interval1.end << "]" << std::endl;

	if (interval1.start <= max_time) {
		it = interval1.start;
		ix = px + it * vx;
		iy = py + it * vy;
	}
	else {
		it = -1.0f; // Flag value for "No intersection before max_time"
		ix = -1.0f;
		iy = -1.0f;
	}

	return Intersection{ix, iy, it};*/
}

Intersection DoubleLine::find1DimensionalViolation(DoubleLine line2, float max_time, int sep_x, int sep_y) {
	// This function is called on a 1Dimensional line
	float ix, iy, it;

	if (vx == 0) { // I am fixed on the X axis
		float t1 = (px - (line2.px + sep_x)) / line2.vx;
		float t2 = (px - (line2.px - sep_x)) / line2.vx;

		TimeInterval interval1{ min(t1,t2), min(max(t1,t2),max_time) };

		iy = (-1.0f * line2.px) / line2.vx;

		t1 = (py - (iy + sep_y)) / vy;
		t2 = (py - (iy - sep_y)) / vy;

		TimeInterval interval2{ min(t1,t2), min(max(t1,t2),max_time) };

		if (interval1.start <= max_time && interval2.start <= max_time) {
			it = interval1.firstFutureOverlap(interval2);
			iy = py + it * vy;
			ix = px;
		}
		else {
			it = -1.0f; // Flag value for "No intersection before max_time"
			ix = -1.0f;
			iy = -1.0f;
		}
	}
	else { // I am fixed on the Y axis
		float t1 = (py - (line2.py + sep_y)) / line2.vy;
		float t2 = (py - (line2.py - sep_y)) / line2.vy;

		TimeInterval interval1{ min(t1,t2), min(max(t1,t2),max_time) };

		ix = (-1.0f * line2.py) / line2.vy;

		t1 = (px - (ix + sep_x)) / vx;
		t2 = (px - (ix - sep_x)) / vx;

		TimeInterval interval2{ min(t1,t2), min(max(t1,t2),max_time) };

		if (interval1.start <= max_time && interval2.start <= max_time) {
			it = interval1.firstFutureOverlap(interval2);
			ix = px + it * vx;
			iy = py;
		}
		else {
			it = -1.0f; // Flag value for "No intersection before max_time"
			ix = -1.0f;
			iy = -1.0f;
		}
	}

	return Intersection{ix, iy, it};
}


