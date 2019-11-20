#pragma once


	struct Line {
	public:
		// y = mx + b
		float px, py; // a point on the line (typically the start point)
		float vx, vy;
		float m; // slope
		float b; // y intercept

		bool parallel(Line);
		float findXIntersectPoint(Line);

		Line(int, int, int, int);
		virtual ~Line();
	};


