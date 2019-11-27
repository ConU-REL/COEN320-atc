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
	int airplane_schedule[32] = {
//  ID, speed_x, 	speed_y, 	speed_z, 	x, 			y, 				z, 		entry time.
//	0, 	0, 			0, 			0, 			0, 			0, 				0, 		0, 	// First 8 rows are the corners of our airspace
//	1, 	0, 			0, 			0, 			100*5280, 	0, 				0, 		0,
//	2, 	0, 			0, 			0, 			0, 			0, 				25000, 	0,
//	3, 	0, 			0, 			0, 			100*5280, 	0, 				25000, 	0,
	4, 	0, 			0, 			0, 			0, 			105*5280, 		3333, 		0,
//	5, 	0, 			0, 			0, 			100*5280, 	105*5280, 		0, 		0,
//	6, 	0, 			0, 			0, 			0, 			105*5280, 		25000, 	0,
	7, 	0, 			0, 			0, 			105*5280, 	105*5280, 		25000, 	0, 	// First 8 rows are the corners of our airspace
//	8, 	1000, 		-500, 			0, 			100000, 			300000, 		500, 	5,	// First plane moving in only 1 dimension
//	9, 	1500, 		-750, 			0, 			100000, 			300000, 				1400, 	40,  // Second plane will crash into first
	10, 2000, 		1, 		0, 			0, 		300000+5280, 	23900, 	10, // Edge case, exactly at separation distance from first plane, should not trigger warning
	11, 1, 		2000, 		0, 			300000+5280, 	0, 	23000, 	10, // Edge case, barely within separation distance, should trigger warning
//	12, 200, 		-1,			0, 			2500, 		100000, 		1000, 	5,
//	13, 619, 		-280, 		194, 		74000, 	0, 		10000, 	30,
//	14, -141, 		427, 		-321, 		410000, 	102251, 11000, 	40,
//	15, -199, 		242, 		-205, 		560000, 	100000, 4000, 	50,
//	16, 315, 		-197, 		-365, 		77000, 	0, 		1000, 	80,
//	17, -138, 		455, 		602, 		23000, 	10290, 14000, 	100,
//	18, -150, 		557, 		-356, 		38000, 	100000, 1000, 	120,
//	19, 194, 		184, 		598, 		35000, 	0, 		2000, 	130,
	};

	TestCase() {}
};
