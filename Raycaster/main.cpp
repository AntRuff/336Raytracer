#include "vec.h"
#include <iostream>
#include <list>
#include <iterator>
#include <math.h>
#include <stdio.h>
#include <stdint.h>

using namespace std;

#define R 0
#define G 1
#define B 2


class sphere{
    
    public:

        vec3 origin;
        float radius;
        vec3 color;


        sphere(vec3 o, float r, vec3 rgb){
            origin = o;
            radius = r;
            color = rgb;
        }
};

vec3 Raycast(vec3 rayStart, vec3 rayEnd, list<sphere> sphereList){

    vec3 closestPoint = rayEnd;
    float closestD = sqrt(pow(closestPoint.x() - rayStart.x(), 2) + 
                          pow(closestPoint.y() - rayStart.y(), 2) +
                          pow(closestPoint.z() - rayStart.z(), 2));
    vec3 color = vec3(0, 0, 0);


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
            float u = (-b/(2*a));
            vec3 temp = vec3(rayStart.x() + u*(rayEnd.x() - rayStart.x()),
                             rayStart.y() + u*(rayEnd.y() - rayStart.y()),
                             rayStart.z() + u*(rayEnd.z() - rayStart.z()));
            float tempD = sqrt(pow(temp.x() - rayStart.x(), 2) + 
                               pow(temp.y() - rayStart.y(), 2) +
                               pow(temp.z() - rayStart.z(), 2));
            if (tempD < closestD) {  
                closestPoint = temp;
                closestD = tempD;
                color = s.color;
            }
        } else {
            float uP = (-b * intersect)/(2*a);
            float uN = (-b * -intersect)/(2*a);
            vec3 tempP = vec3(rayStart.x() + uP*(rayEnd.x() - rayStart.x()),
                             rayStart.y() + uP*(rayEnd.y() - rayStart.y()),
                             rayStart.z() + uP*(rayEnd.z() - rayStart.z()));
            vec3 tempN = vec3(rayStart.x() + uN*(rayEnd.x() - rayStart.x()),
                             rayStart.y() + uN*(rayEnd.y() - rayStart.y()),
                             rayStart.z() + uN*(rayEnd.z() - rayStart.z()));
            float tempPD = sqrt(pow(tempP.x() - rayStart.x(), 2) + 
                                pow(tempP.y() - rayStart.y(), 2) +
                                pow(tempP.z() - rayStart.z(), 2));
            float tempND = sqrt(pow(tempN.x() - rayStart.x(), 2) + 
                                pow(tempN.y() - rayStart.y(), 2) +
                                pow(tempN.z() - rayStart.z(), 2));
            if (tempPD < closestD){
                closestPoint = tempP;
                closestD = tempPD;
                color = s.color;
            }
            if (tempND < closestD){
                closestPoint = tempN;
                closestD = tempND;
                color = s.color;
            }
            
        }

    }

    return color;
}

int main(int argc, char *agrv[]) {

    vec3 eye = vec3(0, 0, 0);
    vec3 imagePlane = vec3(0, 0, 1);
    const int xBound = 512;
    const int yBound = 384;

    int raster[yBound][xBound][3];

    list <sphere> sphereList;

    sphereList.push_front(sphere(vec3(0, 0, 1), 1.0, vec3(255, 255, 255)));

    int x, y;

    FILE *f;

    for (y = 0; y < yBound; y++){
        for (x = 0; x < xBound; x++){
            vec3 color = Raycast(eye, vec3((float) x, (float) y, imagePlane.z()), sphereList);
            raster[y][x][R] = color.x();
            raster[y][x][G] = color.y();
            raster[y][x][B] = color.z();
        }
    }

    f = fopen("output.ppm", "w");

    fprintf(f, "P6\n%d\n%d\n255\n", xBound, yBound);
    fwrite(raster, 1, sizeof(raster), f);

    fclose(f);

    return 0;

}