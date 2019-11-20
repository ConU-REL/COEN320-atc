#include "Line.h"
#include <cmath>


Line::Line(int x_coord, int y_coord, int x_vector, int y_vector) {
	px = (float)x_coord;
	py = (float)y_coord;
	vx = (float)x_vector;
	vy = (float)y_vector;

	m = isnan(vy / vx) ? 0.0f : (vy / vx);
	b = py - m*px;
}

Line::~Line() {
	// TODO Auto-generated destructor stub
}

bool Line::parallel(Line lineToCompare) {
	if ((m - lineToCompare.m) != 0) {
		return false;
	}
	return true;
}

// Don't call this on lines that are strictly horizontal or strictly vertical
float Line::findXIntersectPoint(Line lineToCompare) {
	return (lineToCompare.b - b) / (m - lineToCompare.m);
}
