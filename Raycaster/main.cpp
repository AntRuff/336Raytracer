#include "vec.h"
#include <iostream>
#include <list>
#include <iterator>
#include <math.h>
using namespace std;

class sphere{
    
    public:

        vec3 origin;
        float radius;
        int r, g, b;


        sphere(vec3 o, float r, int red, int green, int blue){
            origin = o;
            radius = r;
            r = red;
            g = green;
            b = blue;
        }
};

int main(int argc, char *agrv[]) {

    vec3 eye = vec3(0, 0, 0);
    vec3 imagePlane = vec3(0, 0, 1);
    double xBound = 512;
    double yBound = 384;

    list <sphere> sphereList;

    sphereList.push_front(sphere(vec3(0, 0, 1), 1.0, 255, 255, 255));

    int x, y;



    for (y = -yBound/2; y < yBound/2; y++){
        for (x = -xBound/2; x < xBound/2; x++){
            
        }
    }

}

void Raycast(vec3 rayStart, vec3 rayEnd, list<sphere> sphereList){
    for(sphere s : sphereList){
        float a = pow((rayEnd.x() - rayStart.x()), 2) 
                + pow((rayEnd.y() - rayStart.y()), 2) 
                + pow((rayEnd.z() - rayStart.z()), 2);
        float b = 2 * (((rayEnd.x() - rayStart.x())*(rayStart.x()-s.origin.x()))
                      +((rayEnd.y() - rayStart.y())*(rayStart.y()-s.origin.y()))
                      +((rayEnd.z() - rayStart.z())*(rayStart.z()-s.origin.z())));
        float c = pow(s.origin.x(), 2) + pow(s.origin.y(), 2) + pow(s.origin.z(), 2) 
                + pow(rayStart.x(), 2) + pow(rayStart.y(), 2) + pow(rayStart.z(), 2)
                - 2 * ((s.origin.x()*rayStart.x()) + (s.origin.y()*rayStart.y()) + (s.origin.z()*rayStart.z()))
                - pow(s.radius, 2);

        float intersect = (b * b) - (4 * a * c);
        if (intersect < 0.0){
        } else if (intersect == 0.0){
            // Do something with -b/2a
        } else {
            // Do something with full quadratic equation
        }

    }
}