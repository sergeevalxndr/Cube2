#pragma once


#define side 80
#define _USE_MATH_DEFINES
#include <math.h>

struct coord {
	double x, y, z;
};

class Cube {
private:
	bool akson = false;
	double step = 0.05;
	coord pov = { 0, 0, 12000 };
	int sd[6][4] = {
		{ 1, 2, 4, 3 },
		{ 1, 2, 6, 5 },
		{ 1, 3, 7, 5 },
		{ 3, 4, 8, 7 },
		{ 2, 4, 8, 6 },
		{ 5, 6, 8, 7 }
	};
	coord crd[9] = {
		{ 0, 0, 0 },
		{ side, side, side },
		{ side, side, -side },
		{ side, -side, side },
		{ side, -side, -side },
		{ -side, side, side },
		{ -side, side, -side },
		{ -side, -side, side },
		{ -side, -side, -side }
	};
public:
	Cube()
	{
		for (int i = 0; i < 9; i++) {
			crdxy[i] = { 0, 0, 0 };
		}
		for (int n = 1; n < 9; n++) {
			crdxy[n].x = crd[n].x - crd[n].z * cos(M_PI_4) / 2;
			crdxy[n].y = crd[n].y - crd[n].z * sin(M_PI_4) / 2;
		}
	}
	coord crdxy[9];
	bool findnrm(int);
	void recalc();
	void reset();
	void switchpv();
	int getidside(int, int);
	void rotatex(float);
	void rotatey(float);
};