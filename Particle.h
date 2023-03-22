#pragma once
#include <stdlib.h>
class Particle
{
public:
	int xc; //x center of particle
	int yc; //y center of particle
	int radius; //radius of particle

	Particle(int mousex, int mousey) {
		xc = mousex;
		yc = mousey;

		int range = 20 - 5 + 1;
		int num = rand() % range + 5;
		radius = num;
	}

private:

protected:

};

