#include "CubeClass.h"

bool Cube::findnrm(int id) // Определение видимости грани
{
	coord sn = { 0, 0, 0 }, pov = {0, 0, 600 };
	for (int i = 0; i < 4; i++) {
		sn.x += crd[getidside(id, i)].x;
		sn.y += crd[getidside(id, i)].y;
		sn.z += crd[getidside(id, i)].z;
	}
	float ab, as, bs, res;
	ab = sn.x*pov.x + sn.y*pov.y + sn.z*pov.z;
	as = sqrt(sn.x*sn.x + sn.y*sn.y + sn.z*sn.z);
	bs = 600;
	res = ab / (as*bs);
	if (res > 0) return true;
	else return false;
}

void Cube::recalc()
{
	if (akson == false) {
		for (int i = 1; i < 9; i++) {
			//Плоскость XOZ
			float kx = (pov.z - crd[i].z) / (pov.x - crd[i].x); //тангенс угла наклона прямой, соединяющей pov и crdxy[i], и осью OX
			if (pov.z <= 0 && crd[i].z <= 0) //Если обе точки находятся под осью OX...
			{
				if (pov.z >= crd[i].z) //... и если pov находится выше crdxy[i]
				{
					crdxy[i].x = pov.x - pov.z / kx;
				}
				else //...и если pov находится ниже crdxy[i]
				{
					crdxy[i].x = crd[i].x - crd[i].z / kx;
				}
			}
			else //Значит они или над ней, или по разные стороны от OX
			{
				if (pov.z >= crd[i].z) //... и если pov находится выше crdxy[i]
				{
					crdxy[i].x = crd[i].x - crd[i].z / kx;
				}
				else //...и если pov находится ниже crdxy[i]
				{
					crdxy[i].x = pov.x - pov.z / kx;
				}
			}

			//Плоскость YOZ
			float ky = (pov.z - crd[i].z) / (pov.y - crd[i].y); //тангенс угла наклона прямой, соединяющей pov и crdxy[i], и осью OY
			if (pov.z <= 0 && crd[i].z <= 0) //Если обе точки находятся под осью OY...
			{
				if (pov.z >= crd[i].z) //... и если pov находится выше crdxy[i]
				{
					crdxy[i].y = pov.y - pov.z / ky;
				}
				else //...и если pov находится ниже crdxy[i]
				{
					crdxy[i].y = crd[i].y - crd[i].z / ky;
				}
			}
			else //Значит они или над ней, или по разные стороны от OY
			{
				if (pov.z >= crd[i].z) //... и если pov находится выше crdxy[i]
				{
					crdxy[i].y = crd[i].y - crd[i].z / ky;
				}
				else //...и если pov находится ниже crdxy[i]
				{
					crdxy[i].y = pov.y - pov.z / ky;
				}

			}
			crdxy[i].z = 0;
		}
	}
	else {
		for (int i = 0; i < 9; i++) {
			crdxy[i] = { 0, 0, 0 };
		}
		for (int n = 1; n < 9; n++) {
			crdxy[n].x = crd[n].x - crd[n].z * cos(M_PI_4) / 2;
			crdxy[n].y = crd[n].y - crd[n].z * sin(M_PI_4) / 2;
		}
	}
}

void Cube::reset()
{
	crd[0] = { 0, 0, 0 },
	crd[1] = { side, side, side };
	crd[2] = { side, side, -side };
	crd[3] = { side, -side, side };
	crd[4] = { side, -side, -side };
	crd[5] = { -side, side, side };
	crd[6] = { -side, side, -side };
	crd[7] = { -side, -side, side };
	crd[8] = { -side, -side, -side };
}
int Cube::getidside(int i, int j)
{
	int id = sd[i][j];
	return id;
}
void Cube::switchpv() {
	akson == false ? akson = true : akson = false;
}
void Cube::rotatey(float k)
{
	double cm = cos(step*k);
	double sm = sin(step*k);
	double mtrR[3][3] = {
		{ 1, 0, 0 },
		{ 0, cm, -sm },
		{ 0, sm, cm }
	};
	for (int n = 1; n < 9; n++) {
		double my = 0, mz = 0;
		my = cm*crd[n].y - sm*crd[n].z;
		mz = cm*crd[n].z + sm*crd[n].y;
		crd[n].y = my;
		crd[n].z = mz;
	}
	return;
}
void Cube::rotatex(float k)
{
	double cm = cos(step*k);
	double sm = sin(step*k);
	double mtrR[3][3] = {
		{ cm, 0, sm },
		{ 0, 1, 0 },
		{ -sm, 0, cm }
	};
	for (int n = 1; n < 9; n++) {
		double mx = 0, mz = 0;
		mx = cm*crd[n].x - sm*crd[n].z;
		mz = cm*crd[n].z + sm*crd[n].x;
		crd[n].x = mx;
		crd[n].z = mz;
	}
	return;
}
