#include <stdlib.h>
#include "CImg.h"
#include <vector>
using namespace std;

using namespace cimg_library;

class Particle
{
public:
	int xc; //x center of particle
	int yc; //y center of particle
	int radius; //radius of particle
	double vx;
	double vy;

	double mass;



	Particle(int xpos, int ypos) {
		double density = 1;

		xc = xpos;
		yc = ypos;

		radius = (int)round(cimg::rand(5,10));
		vx = cimg::rand(-3, 3);
		vy = cimg::rand(-3, 3);

		mass = 4.0 / 3.0 * cimg::PI * pow(radius, 3);

		//x:→
		//y:↓
	}

	void move(int delta_time, double gravity) {
		vx = vx;
		vy = vy;

		xc = round(xc + vx * delta_time);
		yc = round(yc + vy * delta_time + 0.5 * gravity * pow(delta_time, 2));
	}

	void updateRadMass(int UD, int changeR) {
		if (UD == 0) { //Increase
			radius = radius + changeR;
			mass = 4.0 / 3.0 * cimg::PI * pow(radius, 3);

		}
		else { //Lower
			if (radius - changeR <= 0) {
				radius = 1;
				mass = 4.0 / 3.0 * cimg::PI * pow(radius, 3);
			}
			else {
				radius = radius - changeR;
				mass = 4.0 / 3.0 * cimg::PI * pow(radius, 3);
			}
		}
		
	}

private:

protected:

};

