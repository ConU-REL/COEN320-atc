/*
 * TestCase.h
 *
 *	Description: Test case for the demo of ATC project.
 *			     Instantiate and use member array.
 *
 */
#pragma once

/*
 * Class name: TestCase
 *
 * Description: Provides int array containing data of incoming aircraft provided by the radar.
 *
 * */
class TestCase {
public:
	//each line is an airplane: ID, speed_x, speed_y, speed_z, x, y, z, entry time.
	int airplane_schedule[160] = {
//  ID, speed_x, 	speed_y, 	speed_z, 	x, 			y, 				z, 		entry time.
	0, 	0, 			0, 			0, 			0, 			0, 				0, 		0, 	// First 8 rows are the corners of our airspace
	1, 	0, 			0, 			0, 			100*5280, 	0, 				0, 		0,
	2, 	0, 			0, 			0, 			0, 			0, 				25000, 	0,
	3, 	0, 			0, 			0, 			100*5280, 	0, 				25000, 	0,
	4, 	0, 			0, 			0, 			0, 			100*5280, 		0, 		0,
	5, 	0, 			0, 			0, 			100*5280, 	100*5280, 		0, 		0,
	6, 	0, 			0, 			0, 			0, 			100*5280, 		25000, 	0,
	7, 	0, 			0, 			0, 			100*5280, 	100*5280, 		25000, 	0, 	// First 8 rows are the corners of our airspace
	8, 	200, 		0, 			0, 			0, 			100000, 		1000, 	5,	// First plane moving in only 1 dimension
	9, 	-200, 		0, 			0, 			3000+5280, 	100000, 		1000, 	10, // Second plane will crash head-on into first
	10, -200, 		0, 			0, 			5000+5280, 	100000+5280, 	1000, 	15, // Edge case, exactly at separation distance from first plane, should not trigger warning
	11, -200, 		0, 			0, 			7000+5280, 	100000+5279, 	1000, 	20, // Edge case, barely within separation distance, should trigger warning
	12, -512, 		-482, 		578, 		35000, 	400000, 4000, 	142,
	13, 619, 		-280, 		194, 		74000, 	0, 		10000, 	157,
	14, -141, 		427, 		-321, 		410000, 	102251, 11000, 	165,
	15, -199, 		242, 		-205, 		560000, 	100000, 4000, 	172,
	16, 315, 		-197, 		-365, 		77000, 	0, 		1000, 	187,
	17, -138, 		455, 		602, 		23000, 	10290, 14000, 	199,
	18, -150, 		557, 		-356, 		38000, 	100000, 1000, 	204,
	19, 194, 		184, 		598, 		35000, 	0, 		2000, 	221,
	};

	TestCase() {}
};
