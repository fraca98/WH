#include <iostream>
#include "CImg.h"
#include "Particle.h"
#include <vector>

using namespace std;
using namespace cimg_library;

//Functions declared before
vector<Particle> collision(vector<Particle> vPar, double dt, CImg<unsigned char> img, double gravity);



int main() {

	// Initial settings
	double dt = 1; //delta time to display (timestep)

	double gravity = 0; // default gravity
	double deltagrav = 0.1;

	int changeR = 1; //changing in radius (inc/dec size)

	unsigned int w = 500; // Image width
	unsigned int h = 500; // Image height

	int numPart = 100; //Number of particles in arena on start (if i select the correct option)

	/*-----------------------------------------------------------*/

	CImg<double> bg(w, h, 1, 3, 255); // Background white

	CImgDisplay dsp(w, h, "Brownian motion", 0); // To display a window
	dsp.display(bg);

	CImg<unsigned char> img; //create image (actual i draw on it)
	img = bg; //img as bg(background) to inizialize it

	vector<Particle> vPar; // Vector of Particles (vector --> undefined length)

	/*-----------------------------------------------------------*/
	// Staring: decide if empty space or with particles
	cout << "Select if you want an empty space or already with particles:\n0: Empty\n1: Particles\nEnter your choice: ";
	string start;
	while (true) {
		cin >> start;
		if (start == "0") {
			break;
		}
		else if (start == "1") {
			for (int i = 0; i < numPart; i++) {
				while (true) {
					Particle randPar = Particle(img);
					if (randPar.verifyPosition(img, vPar)) {
						vPar.push_back(randPar);
						img = randPar.draw(img);
						break;
					}
				}
			}
			break;
		}
		else cout << "Select something valid: ";
	}
	dsp.display(img); //display img


	while (!dsp.is_closed() && !dsp.is_keyESC()) {

		if (dsp.button() && dsp.mouse_x() >= 0 && dsp.mouse_y() >= 0) {
			//cout << dsp.mouse_x() << endl;
			//cout << dsp.mouse_y() << endl;
			Particle par(dsp.mouse_x(), dsp.mouse_y()); //create a particle from mouse click
			if (par.verifyPosition(img, vPar)) {
				vPar.push_back(par); // add to the vector
				img = par.draw(img);
			}
			dsp.set_button(); //avoid multiple clicks: set mouse click as released
		}

		// Higher gravity
		else if (dsp.is_keyP()) {
			cout << "P: Higher gravity\n";
			gravity = gravity + deltagrav;
			dsp.set_key(); //avoid multiple clicks: set keyboard key as released
		}
		// Lower gravity
		else if (dsp.is_keyO()) {
			cout << "O: Less gravity\n";
			gravity = gravity - deltagrav;
			dsp.set_key(); //avoid multiple clicks: set keyboard key as released
		}

		// Increase size of particles
		else if (dsp.is_keyU() && !vPar.empty()) {
			cout << "U: Increase size\n";
			img = bg;
			bool errorinc = false;
			vector<Particle> bigVpar; //vector with updated radius
			for (int i = 0; i < vPar.size(); i++) {
				Particle incPar = vPar[i];
				incPar.increaseRM(changeR);
				bigVpar.push_back(incPar);
			}
			for (int i = 0; i < vPar.size(); i++) {
				vector<Particle> bigWI;
				bigWI = bigVpar;
				bigWI.erase(bigWI.begin() + i);
				if (bigVpar[i].verifyPosition(img, bigWI) == false) {
					errorinc = true;
					break;
				}
			}
			if (errorinc) {
				cout << "Can't increase more the size\n";
			}
			else {
				vPar = bigVpar;
				for (int i = 0; i < vPar.size(); i++) {
					img = vPar[i].draw(img);
				}
			}
			dsp.set_key(); //avoid multiple clicks: set keyboard key as released
		}
		// Reduce dimension of particles
		else if (dsp.is_keyI() && !vPar.empty()) {
			cout << "I: Decrease size\n";
			img = bg;
			bool errordec = false;
			vector<Particle> copyVpar = vPar;
			for (int i = 0; i < copyVpar.size(); i++) {
				errordec = copyVpar[i].reduceRM(changeR);
				if (errordec) break;
			}
			if (errordec) {
				cout << "Can't decrease more the size\n";
			}
			else {
				vPar = copyVpar;
				for (int i = 0; i < vPar.size(); i++) {
					vPar[i].draw(img);
				}
			}
			dsp.set_key(); //avoid multiple clicks: set keyboard key as released
		}
		// Animation Controller
		else {
			img = bg;
			if (!vPar.empty())
			{
				vPar = collision(vPar, dt, img, gravity);
				for (int i = 0; i < vPar.size(); i++) {
					img = vPar[i].draw(img);
				}
			}

		}
		// Display
		dsp.display(img).wait(20);
	}
}

vector<Particle> collision(vector<Particle> vPar, double dt, CImg<unsigned char> img, double gravity) {
	//Initialise collision related info
	double collpartner_1;
	double collpartner_2;
	int wall;
	double colltime = numeric_limits<double>::max();
	double colltime_particle = colltime;
	bool coll_with_wall = false;
	bool coll_with_particle = false;

	for (int i = 0; i < vPar.size(); i++) {
		Particle incPar = vPar[i];
		vector<Particle> copyVpar = vPar; //copy to check and remove the particle that i analyzie every cycle
		copyVpar.erase(copyVpar.begin() + i);

		if (incPar.verifyPosition(img, copyVpar) == false) {
			cout << "ERR\n";
			system("PAUSE");
		};
	}

	// Checking collision time with particles in main domain
	for (int i = 0; i < vPar.size(); i++) {
		for (int j = i + 1; j < vPar.size(); j++) {
			double rab[2] = { vPar[j].xc - vPar[i].xc, vPar[j].yc - vPar[i].yc };
			double vab[2] = { vPar[j].vx - vPar[i].vx, vPar[j].vy - vPar[i].vy };
			double sigma = vPar[i].radius + vPar[j].radius;
			double dvdr = rab[0] * vab[0] + rab[1] * vab[1];
			if (dvdr >= 0) colltime_particle = numeric_limits<int>::max();
			else {
				double dvdv = vab[0] * vab[0] + vab[1] * vab[1];
				double drdr = rab[0] * rab[0] + rab[1] * rab[1];
				double d = dvdr * dvdr - dvdv * (drdr - sigma * sigma);
				if (d < 0) colltime_particle = numeric_limits<int>::max();
				else colltime_particle = -(dvdr + sqrt(d)) / dvdv;
			}
			if (colltime_particle < colltime && colltime_particle >= 0) { //Collision with particle
				colltime = colltime_particle;
				collpartner_1 = i;
				collpartner_2 = j;
				coll_with_wall = false;
				coll_with_particle = true;
			}
		}
		double colltime_R_wallX = (img.width() - 1 - vPar[i].radius - vPar[i].xc) / (vPar[i].vx + 1e-20);
		if (colltime_R_wallX >= 0 && colltime_R_wallX <= colltime && vPar[i].vx > 0) {
			colltime = colltime_R_wallX;
			collpartner_1 = i;
			wall = 1;
			coll_with_wall = true;
			coll_with_particle = false;
		}
		double colltime_L_wallX = -(vPar[i].xc - vPar[i].radius) / (vPar[i].vx + 1e-20);
		if (colltime_L_wallX >= 0 && colltime_L_wallX <= colltime && vPar[i].vx < 0) {
			colltime = colltime_L_wallX;
			collpartner_1 = i;
			wall = 3;
			coll_with_wall = true;
			coll_with_particle = false;
		}

		double colltime_U_wallY = -1;
		if (gravity == 0) {
			colltime_U_wallY = (img.height() - 1 - vPar[i].yc - vPar[i].radius) / (vPar[i].vy + 1e-20);
		}
		else {
			colltime_U_wallY = (-vPar[i].vy + sqrt(pow(vPar[i].vy, 2) - 4 * 0.5 * gravity * (vPar[i].yc - (img.width() - 1) + vPar[i].radius))) / gravity;
		}

		if (colltime_U_wallY >= 0 && colltime_U_wallY <= colltime && vPar[i].yc && vPar[i].vy > 0) {
			colltime = colltime_U_wallY;
			collpartner_1 = i;
			wall = 2;
			coll_with_wall = true;
			coll_with_particle = false;
		}

		double colltime_H_wallY = -1;
		if (gravity == 0) {

		}
		else {

		}
		colltime_H_wallY = -(vPar[i].yc - vPar[i].radius) / (vPar[i].vy + 1e-20);
		if (colltime_H_wallY >= 0 && colltime_H_wallY <= colltime && vPar[i].vy < 0) {
			colltime = colltime_H_wallY;
			collpartner_1 = i;
			wall = 0;
			coll_with_wall = true;
			coll_with_particle = false;
		}
	}

	// Update positions and velocities (elastic collision)
	if (colltime < dt) {
		for (int i = 0; i < vPar.size(); i++) {
			vPar[i].move(colltime - 1e-4, gravity); //introduce this little error to prevent particle to collide exactly (else possible case of intersection due to double to int)
		}
		if (coll_with_particle) {

			double dx = vPar[collpartner_2].xc - vPar[collpartner_1].xc;
			double dy = vPar[collpartner_2].yc - vPar[collpartner_1].yc;
			double dvx = vPar[collpartner_2].vx - vPar[collpartner_1].vx;
			double dvy = vPar[collpartner_2].vy - vPar[collpartner_1].vy;
			double dvdr = dx * dvx + dy * dvy;
			double dist = vPar[collpartner_1].radius + vPar[collpartner_2].radius;

			// magnitude of normal force
			double magnitude = 2 * vPar[collpartner_1].mass * vPar[collpartner_2].mass * dvdr / ((vPar[collpartner_1].mass + vPar[collpartner_2].mass) * dist);

			// normal force, and in x and y directions
			double fx = magnitude * dx / dist;
			double fy = magnitude * dy / dist;

			// update velocities according to normal force
			vPar[collpartner_1].vx += fx / vPar[collpartner_1].mass;
			vPar[collpartner_1].vy += fy / vPar[collpartner_1].mass;
			vPar[collpartner_2].vx -= fx / vPar[collpartner_2].mass;
			vPar[collpartner_2].vy -= fy / vPar[collpartner_2].mass;

		}
		else if (coll_with_particle == false && coll_with_wall == true) {
			switch (wall) {
			case 0:
				vPar[collpartner_1].vy *= -1;
				break;
			case 1:
				vPar[collpartner_1].vx *= -1;
				break;
			case 2:
				vPar[collpartner_1].vy *= -1;
				//vPar[collpartner_1].yc = img.height()-1- vPar[collpartner_1].radius;
				break;
			case 3:
				vPar[collpartner_1].vx *= -1;
				break;
			}
		}
	}
	else if (colltime >= dt) { //Update positions and velocities using dt
		for (int i = 0; i < vPar.size(); i++) {
			vPar[i].move(dt, gravity);
		}
	}
	return vPar;
}
