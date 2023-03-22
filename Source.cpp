#include <iostream>
#include "CImg.h"
#include "Particle.h"
#include <vector>

using namespace std;
using namespace cimg_library;

int main()
{
    // Background white
    unsigned int w = 500;
    unsigned int h = 500;

    CImg<unsigned char> bg(w, h, 1, 3, 255);

    // Color Particle
    unsigned char red[] = { 255, 0, 0 };

    CImgDisplay dsp(w, h, "Moto Browniano", 0);
    dsp.display(bg);

    CImg<unsigned char> img;
    img = bg; //if img == empty (as bg)

    // Vector of Particles (vector --> undefined length)
    vector<Particle> vPar;

    // Reducing particle size
    int redsize = 5;

    while (!dsp.is_closed() && !dsp.is_keyESC()) {

        if (dsp.button()) {
            std::cout << "CLICK\n";
            //std::cout << dsp.mouse_x() << std::endl;
            //std::cout << dsp.mouse_y() << std::endl;

            Particle par(dsp.mouse_x(), dsp.mouse_y()); //create a particle
            vPar.push_back(par);
            img.draw_circle(par.xc,par.yc,par.radius, red);
        }


        // Aumenta la gravità
        if (dsp.is_keyP()) {
            std::cout << "P\n";
        }
        // Diminuisce la gravità
        if (dsp.is_keyO()) {
            std::cout << "O\n";
        }
        
        // Ingrandisco la dimensione delle particelle
        if (dsp.is_keyU() && !vPar.empty()) {
            std::cout << "U\n";
            img = bg;
            for (int i = 0; i < vPar.size(); i++) {
                vPar[i].radius = vPar[i].radius + 5;
                img.draw_circle(vPar[i].xc, vPar[i].yc, vPar[i].radius, red);
            }
        }
        // Diminuisco la dimensione delle particelle
        if (dsp.is_keyI() && !vPar.empty()) {
            std::cout << "I\n";
            img = bg;
            for (int i = 0; i < vPar.size(); i++) {
                vPar[i].radius = (vPar[i].radius - redsize > 0) ? vPar[i].radius - redsize : 1; //if below 1: set 1
                img.draw_circle(vPar[i].xc, vPar[i].yc, vPar[i].radius, red);
            }
        }

        dsp.display(img);
        dsp.wait();
    };

}