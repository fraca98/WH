#include "Particle.h"
class Collision {

public:
	double time;
	Particle a;
	Particle b;
	int countA;
	int countB;

	Collision(double t, Particle p1, Particle p2) {
		time = t;
		a = p1;
		b = p2;
		if (a != nullptr) countA = a.count;
		else           countA = -1;
		if (b != nullptr) countB = b.count;
		else           countB = -1;
	}

	int compareTo(Collision collB) {
		if (time < collB.time) return -1;
		else if (time > collB.time) return +1;
		else                            return  0;
	}

	bool isValid() {
		if (a != null && a.count != countA) return false;
		if (b != null && b.count != countB) return false;
		return true;
	}
};

