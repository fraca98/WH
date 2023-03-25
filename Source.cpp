#include <iostream>
#include "CImg.h"
#include "Particle.h"
#include <vector>

using namespace std;
using namespace cimg_library;




//Functions declared before
Particle random_Particle(CImg<unsigned char> img, vector<Particle> vPar);
bool verify_position(Particle newPar, CImg<unsigned char> img, vector<Particle> vPar);
vector<Particle> collision(vector<Particle> vPar, double dt, CImg<unsigned char> img);



int main() {

	// Initial settings
	int dt = 2;  //delta_time

	int redsize = 5; // Reducing particle size
	int incsize = 5; // Increasing particle size

	int gravity = 0; // default gravity
	int deltagrav = 1;

	unsigned int w = 600; // Image width
	unsigned int h = 600; // Image height

	unsigned char red[] = { 255, 0, 0 }; // Color Particle

	int numbRandParticle = 10;

	/*-----------------------------------------------------------*/

	CImg<unsigned char> bg(w, h, 1, 3, 255); // Background white

	CImgDisplay dsp(w, h, "Brownian motion", 0); // To display a window
	dsp.display(bg);

	CImg<unsigned char> img; //create image (actual i draw on it)
	img = bg; //img as bg(background) to inizialize it

	vector<Particle> vPar; // Vector of Particles (vector --> undefined length)

	/*-----------------------------------------------------------*/
	// Staring: decide if empty space or with particles
	cout << "Select if you want an empty space or already with particles:\n0: Empty\n1: Particles\nEnter your choice: ";
	int start;
	while (true) {
		bool exitloop = false;
		cin >> start;
		switch (start) {
		case 0:
			exitloop = true;
			break;
		case 1:
			for (int i = 0; i < cimg::rand(1, numbRandParticle); i++) {
				Particle newPar = random_Particle(img, vPar);
				vPar.push_back(newPar);
				img.draw_circle(newPar.xc, newPar.yc, newPar.radius, red);
			}
			exitloop = true;
			break;
		default:
			cout << "Select something valid: ";
			break;
		}

		if (exitloop) break;
	}

	img.draw_circle(0, 0, 1, red);

	dsp.display(img); //display img


	while (!dsp.is_closed() && !dsp.is_keyESC()) {

		if (dsp.button() && dsp.mouse_x() >= 0 && dsp.mouse_y() >= 0) {
			//cout << "CLICK\n";
			//cout << dsp.mouse_x() << endl;
			//cout << dsp.mouse_y() << endl;

			Particle par(dsp.mouse_x(), dsp.mouse_y()); //create a particle from mouse click
			bool valid = verify_position(par, img, vPar);

			if (valid) {
				vPar.push_back(par); // add to the vector
				img.draw_circle(par.xc, par.yc, par.radius, red);
			}

			dsp.set_button(); //avoid multiple clicks: set mouse click as released
		}


		// Higher gravity
		else if (dsp.is_keyP()) {
			std::cout << "P: Higher gravity\n";
			gravity = gravity + deltagrav;
			dsp.set_key(); //avoid multiple clicks: set keyboard key as released
		}
		// Lower gravity
		else if (dsp.is_keyO()) {
			std::cout << "O: Less gravity\n";
			gravity = gravity - deltagrav;
			dsp.set_key(); //avoid multiple clicks: set keyboard key as released
		}

		// Increase size of particles
		else if (dsp.is_keyU() && !vPar.empty()) {
			std::cout << "U: Increase size\n";
			img = bg;
			for (int i = 0; i < vPar.size(); i++) {
				vector<Particle> copyVpar = vPar; //copy to check
				copyVpar.erase(copyVpar.begin() + i);
				Particle incPar = vPar[i];
				incPar.radius = incPar.radius + incsize;

				vPar[i] = (verify_position(incPar, img, copyVpar)) ? incPar : vPar[i];
				img.draw_circle(vPar[i].xc, vPar[i].yc, vPar[i].radius, red);
			}
			dsp.set_key(); //avoid multiple clicks: set keyboard key as released
		}
		// Reduce dimension of particles
		else if (dsp.is_keyI() && !vPar.empty()) {
			std::cout << "I: Decrease size\n";
			img = bg;
			for (int i = 0; i < vPar.size(); i++) {
				vPar[i].radius = (vPar[i].radius - redsize > 0) ? vPar[i].radius - redsize : 1; //if below 1: set 1
				img.draw_circle(vPar[i].xc, vPar[i].yc, vPar[i].radius, red);
			}
			dsp.set_key(); //avoid multiple clicks: set keyboard key as released
		}


		// Animation Controller
		else {
			img = bg;
			if (!vPar.empty())
			{
				vPar = collision(vPar, dt, img);
				for (int i = 0; i < vPar.size(); i++) {
					img.draw_circle(vPar[i].xc, vPar[i].yc, vPar[i].radius, red);
				}
			}

		}
		// Display
		dsp.display(img).wait(1000/60); //wait to slow down velocity of rendering
	};

}



Particle random_Particle(CImg <unsigned char> img, vector<Particle> vPar) { //Function to generate random particle at the beginning
	while (true) {
		Particle randPar(cimg::rand(0, img.width()), cimg::rand(0, img.height()));
		if (verify_position(randPar, img, vPar)) return randPar;
	}
}

bool verify_position(Particle newPar, CImg<unsigned char> img, vector<Particle> vPar) { //Function to check if particle generated (both random, inserted) is valid or not
	//Check if particle is in the box
	if (newPar.xc - newPar.radius < 0 || newPar.xc + newPar.radius > img.width() || newPar.yc - newPar.radius < 0 || newPar.yc + newPar.radius > img.height()) {
		return false;
	}

	// Check if overlaps with other existing particles:
	// Two circles intersect if, and only if, the distance between their centers is between the sum and the difference of their radii.
	for (Particle i : vPar) {
		double d = sqrt(pow(newPar.xc - i.xc, 2) + pow(newPar.yc - i.yc, 2));
		if (d <= newPar.radius - i.radius) {
			//cout << "Circle B is inside A\n";
			return false;
		}
		else if (d <= i.radius - newPar.radius) {
			//cout << "Circle A is inside B\n";
			return false;
		}
		else if (d < newPar.radius + i.radius) {
			//cout << "Circle intersect to each other\n";
			return false;
		}
		else if (d == newPar.radius + i.radius) {
			//cout << "Circle touch to each other\n";
			return false;
		}
		else {
			//cout << "Circle not touch to each other\n";
		}
	}
	return true;
}

vector<Particle> collision(vector<Particle> vPar, double dt, CImg<unsigned char> img) {
	//Initialise collision related info
	double collpartner_1;
	double collpartner_2;
	int wall;
	double colltime = numeric_limits<int>::max();
	double colltime_particle = colltime;
	bool coll_with_wall = false;
	bool coll_with_particle = false;

	// Checking collision time with particles in main domain
	for (int i = 0; i < vPar.size(); i++) {
		for (int j = i + 1; j < vPar.size(); j++) {
			int rab[2] = { vPar[i].xc - vPar[j].xc, vPar[i].yc - vPar[j].yc };
			double vab[2] = { vPar[i].vx - vPar[j].vx, vPar[i].vy - vPar[j].vy };

			double disc = pow(rab[0] * vab[0] + rab[1] * vab[1], 2) - (vab[0] * vab[0] + vab[1] * vab[1]) * ((rab[0] * rab[0] + rab[1] * rab[1]) - pow(vPar[j].radius + vPar[i].radius, 2));
			if (disc > 0) { //future collision
				colltime_particle = (-(rab[0] * vab[0] + rab[1] * vab[1]) - sqrt(disc)) / (pow(vab[0], 2) + pow(vab[1], 2));
			}
			else { //collisione passata (<0) o attuale(=0): non la considero
				colltime_particle = numeric_limits<int>::max(); // no collision
			}

			if (colltime_particle < colltime && colltime_particle >= 0) { //Collision with particle
				colltime = colltime_particle;
				collpartner_1 = i;
				collpartner_2 = j;
				coll_with_wall = false;
				coll_with_particle = true;
			}
		}

		double colltime_R_wallX = (img.width() - 1 - vPar[i].radius - vPar[i].xc) / (vPar[i].vx);
		if (colltime_R_wallX >= 0 && colltime_R_wallX <= colltime) {
			colltime = colltime_R_wallX;
			collpartner_1 = i;
			wall = 1;
			coll_with_wall = true;
			coll_with_particle = false;
		}

		double colltime_L_wallX = -(vPar[i].xc - vPar[i].radius) / (vPar[i].vx);
		if (colltime_L_wallX >= 0 && colltime_L_wallX <= colltime) {
			colltime = colltime_L_wallX;
			collpartner_1 = i;
			wall = 3;
			coll_with_wall = true;
			coll_with_particle = false;
		}

		double colltime_U_wallY = (img.height() - 1 - vPar[i].yc - vPar[i].radius) / (vPar[i].vy); //qua sistemare per moto accelerato
		if (colltime_U_wallY >= 0 && colltime_U_wallY <= colltime) {
			colltime = colltime_U_wallY;
			collpartner_1 = i;
			wall = 2;
			coll_with_wall = true;
			coll_with_particle = false;
		}

		double colltime_H_wallY = -(vPar[i].yc - vPar[i].radius) / (vPar[i].vy); //qua sistemare per moto accelerato
		if (colltime_H_wallY >= 0 && colltime_H_wallY <= colltime) {
			colltime = colltime_H_wallY;
			collpartner_1 = i;
			wall = 0;
			coll_with_wall = true;
			coll_with_particle = false;
		}
	}

	if (colltime == 0) {
		double newcolltime; //calcolo il prossimo tempo di collisione affinchè possa muovere le palline fino ad esso


		cout << colltime << endl;
		cout << "COLLTIME ORA: 0\n";
		cout << "wall:" << coll_with_wall << endl;
		cout << "parti: " << coll_with_particle << endl;
		//system("PAUSE");
	}

	// Update positions and velocities
	if (colltime < dt) {
		for (int i = 0; i < vPar.size(); i++) {
			vPar[i].move(colltime-(1e-5)); //move all particles until first collision: fix here when t collision is 0
		}

		if (coll_with_particle) { //Collision with particle
			double ra[2] = { vPar[collpartner_1].xc, vPar[collpartner_1].yc };
			double va[2] = { vPar[collpartner_1].vx, vPar[collpartner_1].vy };
			double rb[2] = { vPar[collpartner_2].xc, vPar[collpartner_2].yc };
			double vb[2] = { vPar[collpartner_2].vx, vPar[collpartner_2].vy };

			double rab[2] = { ra[0] - rb[0], ra[1] - rb[1] };
			double rab_dot_rab = rab[0] * rab[0] + rab[1] * rab[1];
			double rab_norm = sqrt(rab_dot_rab);
			double n[2] = { rab[0] / rab_norm, rab[1] / rab_norm };

			// Calculate del_v using array operations
			double del_v[2] = { n[0] * (va[0] - vb[0]) + n[1] * (va[1] - vb[1]) };

			// Update velocities
			vPar[collpartner_1].vx -= del_v[0] * 2 * vPar[collpartner_2].mass / (vPar[collpartner_1].mass + vPar[collpartner_2].mass);
			vPar[collpartner_1].vy -= del_v[1] * 2 * vPar[collpartner_2].mass / (vPar[collpartner_1].mass + vPar[collpartner_2].mass);
			vPar[collpartner_2].vx += del_v[0] * 2 * vPar[collpartner_1].mass / (vPar[collpartner_1].mass + vPar[collpartner_2].mass);
			vPar[collpartner_2].vy += del_v[1] * 2 * vPar[collpartner_1].mass / (vPar[collpartner_1].mass + vPar[collpartner_2].mass);
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
				break;
			case 3:
				vPar[collpartner_1].vx *= -1;
				break;
			}
		}
	}
	else if (colltime >= dt) { //Update positions and velocities using dt
		for (int i = 0; i < vPar.size(); i++) {
			vPar[i].move(dt);
		}
	}
	return vPar;
}
