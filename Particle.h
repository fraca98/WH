#include <stdlib.h>
#include "CImg.h"
#include <vector>
using namespace std;

using namespace cimg_library;

class Particle
{
public:
	double id;	//id unique of particle
	double xc;	//x center of particle
	double yc;	//y center of particle
	int radius; //radius of particle
	double vx;	//velocity x
	double vy;	//velocity y
	//x:→
	//y:↓
	double mass; //mass
	double density = 1; //density
	unsigned char color[3]; //color


	Particle(int xpos, int ypos) : id(counter++) {
		xc = xpos;
		yc = ypos;
		radius = round(cimg::rand(5, 10));
		vx = cimg::rand(-1, 1);
		vy = cimg::rand(-1, 1);
		mass = 4.0 / 3.0 * cimg::PI * pow(radius, 3) * density;
		color[0] = cimg::rand(0, 255);
		color[1] = cimg::rand(0, 255);
		color[2] = cimg::rand(0, 255);

	}

	Particle(CImg<unsigned char> img) : id(counter++) { //random particle generated
		xc = cimg::rand(0, img.width() - 1);
		yc = cimg::rand(0, img.height() - 1);
		radius = round(cimg::rand(5, 10));
		vx = cimg::rand(-1, 1);
		vy = cimg::rand(-1, 1);
		mass = 4.0 / 3.0 * cimg::PI * pow(radius, 3) * density;
		color[0] = cimg::rand(0, 255);
		color[1] = cimg::rand(0, 255);
		color[2] = cimg::rand(0, 255);
	}

	void move(double dt, double gravity) {
		//Update velocities
		vx = vx;
		vy = vy + gravity * dt;

		//Update positions
		xc = xc + vx * dt;
		yc = yc + vy * dt + 0.5 * gravity * pow(dt, 2);
	}

	void increaseRM(int changeR) {
		radius = radius + changeR;
		mass = 4.0 / 3.0 * cimg::PI * pow(radius, 3) * density;
	}

	bool reduceRM(int changeR) {
		radius = radius - changeR;
		if (radius < 0) return true; //error
		else {
			mass = 4.0 / 3.0 * cimg::PI * pow(radius, 3) * density;
			return false;
		}
	}

	CImg<unsigned char> draw(CImg<unsigned char> img) { //drawing circle
		return img.draw_circle(xc, yc, radius, color);
	}

	bool verifyPosition(CImg<unsigned char> img, vector<Particle> vPar) { //verify position of a particle if it's valid (false:not valid)
		// Check position respect to walls
		if (xc - radius < 0 || xc + radius > img.width() - 1 || yc - radius < 0 || yc + radius > img.height() - 1) {
			return false;
		}
		// Check if particle overlaps with other existing particles:
		// Two circles intersect if, and only if, the distance between their centers is between the sum and the difference of their radii.
		for (Particle const& i : vPar) {
			double d = sqrt(pow(xc - i.xc, 2) + pow(yc - i.yc, 2));
			if (d <= radius - i.radius) {
				//cout << "Circle B is inside A\n";
				return false;
			}
			else if (d <= i.radius - radius) {
				//cout << "Circle A is inside B\n";
				return false;
			}
			else if (d < radius + i.radius) {
				//cout << "Circle intersect to each other\n";
				return false;
			}
			else if (d == radius + i.radius) {
				//cout << "Circle touch to each other\n";
				return false;
			}
			else {
				//cout << "Circle not touch to each other\n";
			}
		}
		return true;

	}


private:
	static int counter; //counter for id (auto-incremental)

protected:

};
int Particle::counter = 0;