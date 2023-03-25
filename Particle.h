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
	//double direction; //direction
	//double velocity; //velocity module
	double vx;
	double vy;

	double mass;



	Particle(int xpos, int ypos) {

		xc = xpos;
		yc = ypos;

		radius = (int)cimg::rand(5, 20);
		vx = cimg::rand(-5, 5);
		vy = cimg::rand(-5, 5);

		mass = 1;// pow(radius, 2)* cimg::PI;

		//x:→
		//y:↓*/
	}

	void move(int delta_time) {
		vx = vx;
		vy = vy;

		xc = xc + vx * delta_time;
		yc = yc + vy * delta_time;
	}

private:

protected:

};

